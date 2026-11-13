#include "CAN/txdata.h"

#include <string.h>

#include "CAN/txrx.h"
#include "CAN/warning.h"
#include "CAN/transport_protocol.h"
#include "CAN/j1939_address_claim.h"
#include "CAN/idlist.h"

static struct can_txdata can_txdata;

static const struct can_rx_slot can_txdata_request_rx_slots[] = {
	{ .match_mask = CAN_MATCH_PDUF_MASK, .match_id = 0xea0000, .port = CAN_MATCH_ANY_PORT }, //request pgn
};

static MAKE_IDLIST_FROM_RX_SLOT_ARRAY(can_txdata_request_rx_id_list, can_txdata_request_rx_slots);

/** \brief Module init function.

Call once at start to init internal structures.*/
void can_txdata_init()
{
	/*memzero data*/
	memset(&can_txdata, 0, sizeof(can_txdata));

	can_load_rx_id_list(CAN_TXDATA_REQUEST_CHANNEL, &can_txdata_request_rx_id_list);
}

static inline int need_multi_packet(const struct can_tx_parameter *p)
{
	return (p->start_byte + p->bit_length / 8) > 8;
}

static unsigned can_txdata_build_data_for_pg(const struct can_tx_parameter_group *pg, uint8_t *data);


/** \brief \b Copies \a p to tx list.

Allocates new parameter group if not found any. Sets CAN_TXDATA_MODULE warning flag
CAN_TXDATA_WARNING_PARAMETER_OVERFLOW if total parameter count is greater than CAN_TX_PARAMETER_BUFFER_SIZE, sets warning flag
CAN_TXDATA_WARNING_PARAMETER_GROUP_OVERFLOW if total parameter group count is greater than CAN_TX_PARAMETER_GROUP_BUFFER_SIZE. */
int can_txdata_load_message_into_list(const struct can_tx_parameter *p)
{
	if (can_txdata.parameter_count >= CAN_TX_PARAMETER_BUFFER_SIZE) {
		can_set_warning_flag(CAN_TXDATA_MODULE, CAN_TXDATA_WARNING_PARAMETER_OVERFLOW);
		return 0;
	}
	/*TODO:validate*/
	memcpy(&can_txdata.parameters[can_txdata.parameter_count++], p, sizeof(struct can_tx_parameter));
	unsigned i;
	struct can_tx_parameter_group *fpg = NULL;
	for (i = 0; i < can_txdata.parameter_group_count; ++i) {
		struct can_tx_parameter_group *pg = &can_txdata.parameter_groups[i];
		if ((p->info.pgn != pg->info.pgn)
		|| (p->info.port != pg->info.port)
		|| (p->info.destination_address != pg->info.destination_address)
		|| (p->info.instance != pg->info.instance))
			continue;
		fpg = pg;
		break;
	}

	if (!fpg) {
		if (can_txdata.parameter_group_count >= CAN_TX_PARAMETER_GROUP_BUFFER_SIZE) {
			can_set_warning_flag(CAN_TXDATA_MODULE, CAN_TXDATA_WARNING_PARAMETER_GROUP_OVERFLOW);
			return 0;
		}
		fpg = &can_txdata.parameter_groups[can_txdata.parameter_group_count++];
		fpg->multi_packet = need_multi_packet(p);
		memcpy(&fpg->info, &p->info, sizeof(p->info));
		fpg->last_transmission_time = get_can_clock();
	} else if (need_multi_packet(p)) {
		fpg->multi_packet = 1;
	}

	if (fpg->info.broadcast_rate > p->info.broadcast_rate)
		fpg->info.broadcast_rate = p->info.broadcast_rate;

	if (fpg->info.priority > p->info.priority)
		fpg->info.priority = p->info.priority;

	//can_dbg("Registered parameter %p: pgn = %06x, broadcast_rate = %lu\n", p, p->info.pgn, p->info.broadcast_rate);
	//can_dbg("Total number of registered parameters %u, groups %u\n", can_txdata.parameter_count, can_txdata.parameter_group_count);
	return 1;
}

int can_txdata_remove_message_from_list(uint32_t pgn, can_port_t port)
{
	/*not implemented*/
	return 0;
}

int can_txdata_remove_message_from_list_ex(uint32_t pgn, can_port_t port, can_instance_t instance)
{
	/*not implemented*/
	return 0;
}


/**
 Returns 1 for success and 0 for failure.
 */
static int can_txdata_send_parameter_group(struct can_tx_parameter_group *pg, uint8_t sa, uint8_t da)
{
	struct can_tx_multi_message *mmsg;
	struct can_tx_frame f;

	if (pg->multi_packet) {
		mmsg = get_can_transport_protocol_tx_data_buffer(pg->info.port, pg->info.pgn, da);
		if (!mmsg)
			return 0;
		mmsg->pgn = pg->info.pgn;
		mmsg->port = pg->info.port;
		mmsg->source_address = sa;
		mmsg->destination_address = da;
		mmsg->message_size = can_txdata_build_data_for_pg(pg, mmsg->data);

		if (sa == 0xff) {
			can_transport_protocol_release_data_buffer(mmsg);
			return 0;
		}
		can_transport_protocol_transmit(mmsg);
	} else {
		if (pg->info.type == CAN_TX_MESSAGE_TYPE_EID) {
			f.id = CAN_ID_EID_MARKER | (pg->info.priority << 26) | (pg->info.pgn << 8) | sa;
		} else {
			f.id = pg->info.pgn;
		}
		if (pg->info.pgn <= 0xefff) {
			f.id = (f.id & ~0xff00) | (da << 8);
		}
		f.dlc = 8;
		memset(f.data, 0xff, sizeof(f.data));
		can_txdata_build_data_for_pg(pg, f.data);
		if (sa == 0xff) {
		//	can_dbg("Source address is 0xff send pgn skipped\n");
			return 0;
		}
		if (!can_buffer_tx_data(&f, pg->info.port))
			return 0;
	}
	return 1;
}

static void can_txdata_process_request_message(const struct can_rx_single_message *smsg)
{
	if (smsg->frame.dlc < 3)
		return;

	uint32_t pgn = smsg->frame.data[0] | (smsg->frame.data[1] << 8) | (smsg->frame.data[2] << 16);
	can_instance_t instance = can_find_instance(smsg->frame.interface, can_smsg_da(smsg));
	//can_dbg("txdata Got request pgn = %06x, instance is %u(da %02x)\n", pgn, instance, can_smsg_da(smsg));

	if (instance == CAN_INVALID_INSTANCE)
		return;
	if (!CAN_ADDRESS_CLAIM_SUPPORTED(smsg->frame.interface, instance))
		return;

	uint8_t sa = CAN_SOURCE_ADDRESS(smsg->frame.interface, instance);
	uint8_t da = can_smsg_sa(smsg);
//	can_dbg("txdata SA %02x DA %02x\n", sa, da);

	unsigned index = can_txdata_find_message_ex(pgn, smsg->frame.interface, instance);
	if (index == (unsigned)-1)
		return;

	//can_dbg("Found pgn match\n");
	can_txdata_send_parameter_group(&can_txdata.parameter_groups[index], sa, da);
}

int can_send_request_message(struct can_tx_message_info* info)
{
	struct can_tx_frame frame;

	uint8_t sa = CAN_SOURCE_ADDRESS(info->port, info->instance);
	if (sa == 0xff) return 0;

	if (info->type == CAN_TX_MESSAGE_TYPE_EID)
		frame.id = CAN_ID_EID_MARKER | (info->priority << 26) | (0xEA << 16) | (info->destination_address << 8) | sa;
	else
		frame.id = 0xEA00 | info->destination_address;

	frame.dlc = 3;
	memset(frame.data, 0xff, sizeof(frame.data));
	for (uint8_t i = 0; i < frame.dlc; i++)
		frame.data[i] = ((info->pgn & (0xFF << i*8)) >> (i*8));

	return can_buffer_tx_data(&frame, info->port);
}

/* \brief Checks all groups for timeouts at transmits.

Also processes PGN requests and replies if corresponding PGN is registered withing list */
void can_txdata_process()
{
	/*compose timed out messages and send'em*/
	unsigned i;
	for (i = 0; i < can_txdata.parameter_group_count; ++i) {
		struct can_tx_parameter_group *pg = &can_txdata.parameter_groups[i];
		if (pg->info.broadcast_rate == BROADCAST_RATE_INFINITE)
			continue;
		if (!can_timed_out(pg->last_transmission_time + pg->info.broadcast_rate))
			continue;

		uint8_t sa = CAN_SOURCE_ADDRESS(pg->info.port, pg->info.instance);
		if (can_txdata_send_parameter_group(pg, sa, pg->info.destination_address)) {
			pg->last_transmission_time = get_can_clock();
		}
	}

	struct can_rx_message_header *header;
	for (;;) {
		header = can_engine_get_channel_message(CAN_TXDATA_REQUEST_CHANNEL);
		if (!header)
			break;
		if (!header->multi_packet) {
			can_txdata_process_request_message(can_to_rx_smsg(header));
		}
		can_engine_release_channel_message(CAN_TXDATA_REQUEST_CHANNEL);
	}
}

#define MASK_FROM_BIT_LENGTH(len) ((1 << len) - 1)

/* \brief Buids data for parameter group \c pg, in supplied buffer \c data.
 \return Resulting message size.*/
static unsigned can_txdata_build_data_for_pg(const struct can_tx_parameter_group *pg, uint8_t *data)
{
	unsigned i;
	unsigned msgsize = 0;
	
	for (i = 0; i < can_txdata.parameter_count; ++i) {
		struct can_tx_parameter *p = &can_txdata.parameters[i];
		if ((p->info.pgn != pg->info.pgn)
		|| (p->info.port != pg->info.port)
		|| (p->info.instance != pg->info.instance)
		|| (p->info.destination_address != pg->info.destination_address))
			continue;
		if ((p->start_bit > 0) && (p->bit_length > 8 - p->start_bit)) {
			//not implemented
			can_set_warning_flag(CAN_TXDATA_MODULE, 7);
			continue;
		}
		if (p->bit_length % 8) {
			uint8_t andmask = MASK_FROM_BIT_LENGTH(p->bit_length) << p->start_bit;
			uint8_t ormask = (p->pdata[0] & MASK_FROM_BIT_LENGTH(p->bit_length)) << p->start_bit;
			data[p->start_byte] = (data[p->start_byte] & ~andmask) | ormask;
		} else {
			memcpy(data + p->start_byte, p->pdata, p->bit_length / 8);
		}
		unsigned newsize = p->start_byte + (p->bit_length + 7) / 8;
		if (msgsize < newsize)
			msgsize = newsize;
	}
	return msgsize;
}


/* \brief Buids data for parameter group number \c index, in supplied buffer \c data.
 \return Resulting message size.*/
unsigned can_txdata_build_data(unsigned index, uint8_t *data)
{
	return can_txdata_build_data_for_pg(&can_txdata.parameter_groups[index], data);
}

unsigned can_txdata_find_message_ex(uint32_t pgn, can_port_t port, can_instance_t instance)
{
	unsigned i;
	for (i = 0; i < can_txdata.parameter_group_count; ++i) {
		struct can_tx_parameter_group *p = &can_txdata.parameter_groups[i];
		if ((p->info.port == port)
		&& (p->info.pgn == pgn)
		&& (p->info.instance == instance))
			return i;
	}
	return -1;
}

/** Searches through parameter groups list for \c pgn.
 \return index of found parameter group or (unsigned)-1 if not found
*/
unsigned can_txdata_find_message(uint32_t pgn, can_port_t port)
{
	unsigned i;
	for (i = 0; i < can_txdata.parameter_group_count; ++i) {
		struct can_tx_parameter_group *p = &can_txdata.parameter_groups[i];
		if ((p->info.port == port)
		&& (p->info.pgn == pgn))
			return i;
	}
	return -1;
}

int can_txdata_set_broadcast_rate(unsigned index, can_clock_t rate)
{
	struct can_tx_parameter_group *p = &can_txdata.parameter_groups[index];
	p->info.broadcast_rate = rate;
	
	return 1;
}

/** Forces parameter group \c index to be sent if last transmission was at least \c min_rate can_clocks_t in the past.
 Primary intended use for BJM resend on updates.
 \return 1 if message was sent (to let caller drop updated state)
 and 0 if message could not be sent due to timing or lower level CAN problems.
*/
int can_txdata_force_send(unsigned index, can_clock_t min_rate)
{
	struct can_tx_parameter_group *pg = &can_txdata.parameter_groups[index];
	can_clock_t current_time = get_can_clock();
	if (can_time_compare(pg->last_transmission_time + min_rate, current_time) <= 0) {
		if (can_txdata_send_parameter_group(pg, CAN_SOURCE_ADDRESS(pg->info.port, pg->info.instance), pg->info.destination_address)) {
			pg->last_transmission_time = current_time;
			return 1;
		}
		return 0;
	}
	//TODO: Force transmission in near future? We'd need some flag
	return 0;
}

can_clock_t can_txdata_get_j1939_last_tranmission_time(unsigned index)
{
	return can_txdata.parameter_groups[index].last_transmission_time;
}

can_clock_t can_txdata_get_j1939_broadcast_rate(unsigned index)
{
	return can_txdata.parameter_groups[index].info.broadcast_rate;
}

uint8_t can_txdata_get_priority(unsigned index)
{
	return can_txdata.parameter_groups[index].info.priority;
}
