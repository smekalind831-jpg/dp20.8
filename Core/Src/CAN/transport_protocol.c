#include "CAN/transport_protocol.h"

#include <string.h>

#include "CAN/idlist.h"
#include "CAN/warning.h"
//#include "can_debug.h"
#include "CAN/j1939_address_claim.h"

/**
 * \defgroup TransportProtocolInternals J1939 Transport Protocol Internals
 * \brief J1939 Transport Protocol inner data and functions
 *
 * Holds buffers for TP.BAM rx/tx and TP.CM rx.tx Uses \ref CanConf to determine buffer sizes.
 */

static struct can_transport_protocol_data can_transport_protocol_data;

#define J1939_TP_CM_MATCH_ID 0xec0000
#define J1939_TP_DT_MATCH_ID 0xeb0000

#define J1939_RTS_TO_DT_TIMEOUT CAN_MS2TICK(250)
#define J1939_DT_TO_DT_TIMEOUT CAN_MS2TICK(250)
#define J1939_CTS_HOLD_TIMEOUT	550
#define J1939_BAM_DT_TO_DT_TX_TIMEOUT 50
#define J1939_DT_TO_CTS_TIMEOUT 1250
#define J1939_RTS_TO_CTS_TIMEOUT 1250

static const struct can_rx_slot can_transport_protocol_rx_slots[] = {
	{ .match_mask = CAN_MATCH_PDUF_MASK, .match_id = J1939_TP_CM_MATCH_ID, .port = CAN_MATCH_ANY_PORT },
	{ .match_mask = CAN_MATCH_PDUF_MASK, .match_id = J1939_TP_DT_MATCH_ID, .port = CAN_MATCH_ANY_PORT },
};

static MAKE_IDLIST_FROM_RX_SLOT_ARRAY(can_transport_protocol_rx_id_list, can_transport_protocol_rx_slots);

/**
 * \defgroup TransportProtocol J1939 Transport Protocol
 * \brief J1939 Transport Protocol support functions
 *
 *
 */

/** \brief init function */
void can_transport_protocol_init()
{
	memset(&can_transport_protocol_data, 0, sizeof(struct can_transport_protocol_data));

	can_load_rx_id_list(CAN_TRANPORT_PROTOCOL_CHANNEL, &can_transport_protocol_rx_id_list);
}

/** \brief transport protocol rx processing
 * Обрабатывает сообщения из CAN_TRANSPORT_PROTOCOL_CHANNEL
 * Processes messages from CAN_TRANSPORT_PROTOCOL_CHANNEL.
 */
void can_transport_protocol_process()
{
	struct can_rx_message_header *header;
	for (;;) {
		header = can_engine_get_channel_message(CAN_TRANPORT_PROTOCOL_CHANNEL);
		if (!header)
			break;
		if (!header->multi_packet) {
			struct can_rx_single_message *msg = can_to_rx_smsg(header);
			//can_dbg("%s processing %08x\n", __func__, msg->frame.id);
			switch (msg->frame.id & 0xff0000) {
			case J1939_TP_CM_MATCH_ID:
				switch (msg->frame.data[0]) {
				case J1939_TP_CM_RTS:
				case J1939_TP_CM_BAM:
					can_transport_protocol_bam_rts_rx(msg);
					break;
				case J1939_TP_CM_CTS:
					can_transport_protocol_cm_cts_rx(msg);
					break;
				case J1939_TP_CM_ACK:
				case J1939_TP_CM_COMM_ABORT:
					can_transport_protocol_cm_ack_comm_abort(msg);
					break;
				default:
					/*malformed message*/
				//	can_dbg("malformed tp.cm message\n");
					break;
				}
				break;
			case J1939_TP_DT_MATCH_ID:
				can_transport_protocol_dt_rx(msg);
				break;
			default:
				//can_dbg("%s matching fault\n", __func__);
				break;
			}
		} else {
		//	can_dbg("some node fault multi packet message with tp pgn\n");
		}
		can_engine_release_channel_message(CAN_TRANPORT_PROTOCOL_CHANNEL);
	}
}

/** Internal */
static struct can_rx_multi_message *find_bam_rx_message(const struct can_rx_single_message *msg)
{
	unsigned i;
	for (i = 0; i < CAN_RX_BAM_BUFFER_SIZE; ++i) {
		struct can_rx_multi_message *mmsg = &can_transport_protocol_data.rx_bam_messages[i];
		if (!mmsg->active)
			continue;
		if (mmsg->port != msg->frame.interface)
			continue;
		if (mmsg->source_address != can_smsg_sa(msg))
			continue;
		return mmsg;
	}
	return NULL;
}

static struct can_rx_multi_message *get_bam_rx_buffer()
{
	unsigned i;
	for (i = 0; i < CAN_RX_BAM_BUFFER_SIZE; ++i) {
		struct can_rx_multi_message *mmsg = &can_transport_protocol_data.rx_bam_messages[i];
		if (!mmsg->active && !mmsg->header.channels_needing_data)
			return mmsg;
	}
	return NULL;
}

static struct can_rx_multi_message *find_connect_rx_message(const struct can_rx_single_message *msg)
{
	unsigned i;
	for (i = 0; i < CAN_RX_CONNECT_BUFFER_SIZE; ++i) {
		struct can_rx_multi_message *mmsg = &can_transport_protocol_data.rx_connect_messages[i];
		if (!mmsg->active)
			continue;
		if (mmsg->port != msg->frame.interface)
			continue;
		if (mmsg->source_address != can_smsg_sa(msg))
			continue;
		if (mmsg->destination_address != can_smsg_da(msg))
			continue;
		return mmsg;
	}
	return NULL;
}

static struct can_tx_multi_message *find_connect_tx_message(const struct can_rx_single_message *msg)
{
	unsigned i;
	for (i = 0; i < CAN_TX_CONNECT_BUFFER_SIZE; ++i) {
		struct can_tx_multi_message *mmsg = &can_transport_protocol_data.tx_connect_messages[i];
		if (mmsg->state == CAN_TPTX_INACTIVE)
			continue;
		if (mmsg->port != msg->frame.interface)
			continue;
		if (mmsg->source_address != can_smsg_da(msg))
			continue;
		if (mmsg->destination_address != can_smsg_sa(msg))
			continue;
		return mmsg;
	}
	return NULL;
}

static struct can_rx_multi_message *get_connect_rx_buffer()
{
	unsigned i;
	for (i = 0; i < CAN_RX_CONNECT_BUFFER_SIZE; ++i) {
		struct can_rx_multi_message *mmsg = &can_transport_protocol_data.rx_connect_messages[i];
		if (!mmsg->active && !mmsg->header.channels_needing_data)
			return mmsg;
	}
	return NULL;
}

static void j1939_tp_cm_ack_tx(const struct can_rx_multi_message *mmsg)
{
	struct can_tx_frame f;
	f.dlc = 8;
#ifndef PU6_2
	f.id = can_compose_eid(3, 0xec, mmsg->source_address,mmsg->destination_address );
#else
	f.id = can_compose_eid(6, 0xec, mmsg->source_address,mmsg->destination_address );
#endif
	f.data[0] = J1939_TP_CM_ACK;
	f.data[1] = mmsg->message_size & 0xff;
	f.data[2] = (mmsg->message_size >> 8) & 0xff;
	f.data[3] = (mmsg->message_size + 6) / 7;
	f.data[4] = 0xff;
	f.data[5] = mmsg->pgn & 0xff;
	f.data[6] = (mmsg->pgn >> 8) & 0xff;
	f.data[7] = (mmsg->pgn >> 16) & 0xff;
	can_buffer_tx_data(&f, mmsg->port);
}


/** DT packets rx processing */
void can_transport_protocol_dt_rx(struct can_rx_single_message *msg)
{
	struct can_rx_multi_message *mmsg;
	unsigned data_size = 7;
	int bam = can_smsg_da(msg) == 0xff;
	mmsg = bam ? find_bam_rx_message(msg) : find_connect_rx_message(msg);//???
	if (!mmsg) {
		//can_dbg("tp.%s rx buffer not found for sa %02x\n", bam ? "bam" : "cm", can_smsg_sa(msg));
		return;
	}

	if (msg->frame.dlc < 8) {
		/*malformed packet*/
		//can_dbg("tp.dt malformed rx packet\n");
		return;
	}
	mmsg->time_received = get_can_clock();
	mmsg->timeout_time = mmsg->time_received + J1939_DT_TO_DT_TIMEOUT;

//	if (msg->frame.data[0] != mmsg->received_number_of_packets + 1) {
//		can_dbg("tp.dt sequence mismatch\n");
//		if (bam) {
//			mmsg->active = 0;
//			return;
//		}
//		/*send retransmission cts*/
//		return;
//	}
	/*check if buffer has enough space*/
	if (7 * msg->frame.data[0] > CAN_TP_BUFFER_SIZE) {
		data_size = 7 * (msg->frame.data[0]-1) < CAN_TP_BUFFER_SIZE ? CAN_TP_BUFFER_SIZE - 7 * (msg->frame.data[0]-1) : 0;
//	if (7 * mmsg->received_number_of_packets + data_size > CAN_TP_BUFFER_SIZE) {
//		data_size = 7 * mmsg->received_number_of_packets < CAN_TP_BUFFER_SIZE ? CAN_TP_BUFFER_SIZE - 7 * mmsg->received_number_of_packets : 0;
	}
	memcpy(mmsg->data + 7*(msg->frame.data[0]-1), msg->frame.data + 1, data_size);
	++mmsg->received_number_of_packets;
	if (mmsg->received_number_of_packets == mmsg->number_of_packets) {
		if (!bam)
			j1939_tp_cm_ack_tx(mmsg);
		//can_dbg("Closing connection as received %06x\n", (mmsg->pgn << 8) | mmsg->source_address);
		mmsg->active = 0;
		can_engine_set_message_as_received(&mmsg->header);
	}
}

static void j1939_tp_cm_cts_tx(struct can_rx_multi_message *mmsg)
{
	struct can_tx_frame f;
	f.dlc = 8;
#ifndef PU6_2
	f.id = can_compose_eid(3, 0xec, mmsg->source_address, mmsg->destination_address);
#else
	f.id = can_compose_eid(6, 0xec, mmsg->source_address, mmsg->destination_address);
#endif
	f.data[0] = J1939_TP_CM_CTS;
	f.data[1] = mmsg->number_of_packets;
	f.data[2] = mmsg->received_number_of_packets + 1;
	f.data[3] = 0xff;
	f.data[4] = 0xff;
	f.data[5] = mmsg->pgn & 0xff;
	f.data[6] = (mmsg->pgn >> 8) & 0xff;
	f.data[7] = (mmsg->pgn >> 16) & 0xff;
	can_buffer_tx_data(&f, mmsg->port);
}

/** BAM and RTS rx processing. Gets free rx buffer, if none available sets CAN_TRANPORT_PROTOCOL_MODULE warning flag CAN_TRANSPORT_PROTOCOL_WARNING_RX_BAM_OVERFLOW or CAN_TRANSPORT_PROTOCOL_WARNING_RX_CONNECT_OVERFLOW, initializes rx buffer and schedules CTS in case of RTS */
void can_transport_protocol_bam_rts_rx(struct can_rx_single_message *msg)
{
	/*find and reuse connection*/
	struct can_rx_multi_message *mmsg;
	int bam = msg->frame.data[0] == J1939_TP_CM_BAM;
	unsigned declared_message_size = (msg->frame.data[2] << 8) | msg->frame.data[1];

	if (declared_message_size >= CAN_TP_BUFFER_SIZE) {
		can_set_warning_flag(CAN_TRANPORT_PROTOCOL_MODULE,
			bam
			? CAN_TRANSPORT_PROTOCOL_WARNING_RX_BAM_TOO_SMALL_BUFFER
			: CAN_TRANSPORT_PROTOCOL_WARNING_RX_CONNECT_TOO_SMALL_BUFFER);
		return;
	}

	mmsg = bam ? find_bam_rx_message(msg) : find_connect_rx_message(msg);
	if (!mmsg) {
		if (bam) {
			mmsg = get_bam_rx_buffer();
		} else {
			if (can_find_instance(msg->frame.interface, can_smsg_da(msg)) == 0xff)
				return;
			mmsg = get_connect_rx_buffer();
		}
	}
	if (!mmsg) {
		can_set_warning_flag(CAN_TRANPORT_PROTOCOL_MODULE,
			bam 
			? CAN_TRANSPORT_PROTOCOL_WARNING_RX_BAM_OVERFLOW
			: CAN_TRANSPORT_PROTOCOL_WARNING_RX_CONNECT_OVERFLOW);
		return;
	}
	mmsg->header.multi_packet = 1;
	mmsg->header.channels_needing_data = 0;
	mmsg->pgn = (msg->frame.data[7] << 16)
		| (msg->frame.data[6] << 8)
		| msg->frame.data[5];
	mmsg->source_address = can_smsg_sa(msg);
	mmsg->destination_address = can_smsg_da(msg);
	mmsg->number_of_packets = msg->frame.data[3];
	mmsg->message_size = declared_message_size;
	mmsg->time_received = get_can_clock();
	mmsg->timeout_time = mmsg->time_received + J1939_RTS_TO_DT_TIMEOUT;
	mmsg->received_number_of_packets = 0;
	mmsg->active = 1;
	mmsg->port = msg->frame.interface;
#if 0
	can_dbg("Established TP %s. SA=%02x, PGN=%06x, Size=%u, Packets=%u\n",
		bam ? "BAM" : "Connect", mmsg->source_address,
		mmsg->pgn, mmsg->message_size, mmsg->number_of_packets);
#endif
	if (!bam) {
		j1939_tp_cm_cts_tx(mmsg);
	}
}

/** CTS rx processing */
void can_transport_protocol_cm_cts_rx(struct can_rx_single_message *msg)
{
	if (can_find_instance(msg->frame.interface, can_smsg_da(msg)) == 0xff)
		return;

	//tx related;
	struct can_tx_multi_message *mmsg = find_connect_tx_message(msg);
	if (!mmsg)
		return;

	uint8_t number_of_packets_to_send = msg->frame.data[1];
	/**/
	if (!number_of_packets_to_send) {
		can_set_timeout(mmsg->timeout_time, J1939_CTS_HOLD_TIMEOUT);
		mmsg->state = CAN_TPTX_ONHOLD;
		return;
	}

	mmsg->timeout_time = get_can_clock();
	mmsg->state = CAN_TPTX_ACTIVE;
	mmsg->sequence_index = msg->frame.data[2];
	mmsg->sequence_max = mmsg->sequence_index + number_of_packets_to_send;
}

void can_transport_protocol_cm_ack_comm_abort(struct can_rx_single_message *msg)
{
	if (can_find_instance(msg->frame.interface, can_smsg_da(msg)) == 0xff)
		return;

	struct can_tx_multi_message *mmsg = find_connect_tx_message(msg);
	if (!mmsg)
		return;

	can_transport_protocol_release_data_buffer(mmsg);
}


/** CAN transport protocol timeouts processing. Checks rx timeouts to disable slots and tx timeouts to send new data */
void can_transport_protocol_check_timeout()
{
	unsigned i;
	can_clock_t clock = get_can_clock();

	for (i = 0; i < CAN_RX_BAM_BUFFER_SIZE; ++i) {
		struct can_rx_multi_message *mmsg = &can_transport_protocol_data.rx_bam_messages[i];
		if (!mmsg->active)
			continue;
		if (mmsg->header.channels_needing_data)
			continue;
		if (clock - mmsg->timeout_time < 0x80000000) {
			mmsg->active = 0;
			continue;
		}
	}
	for (i = 0; i < CAN_RX_CONNECT_BUFFER_SIZE; ++i) {
		struct can_rx_multi_message *mmsg = &can_transport_protocol_data.rx_connect_messages[i];
		if (!mmsg->active)
			continue;
		if (mmsg->header.channels_needing_data)
			continue;
		if (clock - mmsg->timeout_time < 0x80000000) {
			mmsg->active = 0;
			continue;
		}
	}
	for (i = 0; i < CAN_TX_BAM_BUFFER_SIZE; ++i) {
		struct can_tx_multi_message *mmsg = &can_transport_protocol_data.tx_bam_messages[i];
		if (mmsg->state == CAN_TPTX_INACTIVE)
			continue;
		/*state should be == CAN_TPTX_ACTIVE for bam*/
		if (!can_timed_out(mmsg->timeout_time))
			continue;
		struct can_tx_frame f;
		f.id = can_compose_eid(3, 0xeb, 0xff, mmsg->source_address);
		f.dlc = 8;
		f.data[0] = mmsg->sequence_index;
		unsigned size = mmsg->sequence_index == mmsg->number_of_packets
			? mmsg->message_size - (mmsg->sequence_index - 1)* 7 : 7;
		memcpy(f.data + 1, mmsg->data + (mmsg->sequence_index - 1) * 7, size);
		++mmsg->sequence_index;
		while (size < 7) {
			f.data[++size] = 0xff;
		}
		can_buffer_tx_data(&f, mmsg->port);
		if (mmsg->sequence_index <= mmsg->sequence_max) {
			can_set_timeout(mmsg->timeout_time, J1939_BAM_DT_TO_DT_TX_TIMEOUT);
		} else {
			can_transport_protocol_release_data_buffer(mmsg);
		}
	}
	for (i = 0; i < CAN_TX_CONNECT_BUFFER_SIZE; ++i) {
		struct can_tx_multi_message *mmsg = &can_transport_protocol_data.tx_connect_messages[i];
		if (mmsg->state == CAN_TPTX_INACTIVE)
			continue;
		if (!can_timed_out(mmsg->timeout_time))
			continue;

		switch (mmsg->state) {
		case CAN_TPTX_ACTIVE: {
			struct can_tx_frame f;
			f.id = can_compose_eid(3, 0xeb, mmsg->destination_address, mmsg->source_address);
			f.dlc = 8;
			while (mmsg->sequence_index < mmsg->sequence_max) {
				f.data[0] = mmsg->sequence_index;
				unsigned size = mmsg->sequence_index == mmsg->number_of_packets
					? mmsg->message_size - (mmsg->sequence_index - 1)* 7 : 7;
				memcpy(f.data + 1, mmsg->data + (mmsg->sequence_index - 1) * 7, size);
				++mmsg->sequence_index;
				while (size < 7) {
					f.data[++size] = 0xff;
				}
				if (!can_buffer_tx_data(&f, mmsg->port)) {
					can_set_timeout(mmsg->timeout_time, 50);//this happens on tx queue overflow
					return;
				}
			}
			mmsg->state = CAN_TPTX_WAITING_CTS;
			can_set_timeout(mmsg->timeout_time, J1939_DT_TO_CTS_TIMEOUT);
			/*transmit*/
		} break;
		default:
			can_transport_protocol_release_data_buffer(mmsg);
		}
	}
}

/** Returns tx slot matching supplied parameters, first inactive buffer if not found, null if no slots available */
struct can_tx_multi_message *get_can_transport_protocol_tx_data_buffer(can_port_t port, uint32_t pgn, uint8_t da)
{
	unsigned i;
	struct can_tx_multi_message *inactive_buffer = NULL;
	if (da == 0xff) {
	/*search in bam buffers*/
		for (i = 0; i < CAN_TX_BAM_BUFFER_SIZE; ++i) {
			struct can_tx_multi_message *mmsg = &can_transport_protocol_data.tx_bam_messages[i];
			if (mmsg->state == CAN_TPTX_INACTIVE)
				inactive_buffer = mmsg;
			if (mmsg->pgn != pgn)
				continue;
			if (mmsg->port != port)
				continue;
			if (mmsg->state != CAN_TPTX_INACTIVE) {
			/*request to send before previous tx complete*/
				return NULL;
			}
			return mmsg;
		}
	} else {
	/*search in connect buffers*/
		for (i = 0; i < CAN_TX_CONNECT_BUFFER_SIZE; ++i) {
			struct can_tx_multi_message *mmsg = &can_transport_protocol_data.tx_connect_messages[i];
			if (mmsg->state == CAN_TPTX_INACTIVE)
				inactive_buffer = mmsg;
			if (mmsg->pgn != pgn)
				continue;
			if (mmsg->port != port)
				continue;
			if (mmsg->destination_address != da)
				continue;
			if (mmsg->state != CAN_TPTX_INACTIVE) {
			/*request to send before previous tx complete*/
				return NULL;
			}
			return mmsg;
		}
	}
	return inactive_buffer;
}

/** Releases tx slot by setting state to inactive */
void can_transport_protocol_release_data_buffer(struct can_tx_multi_message *mmsg)
{
	mmsg->state = CAN_TPTX_INACTIVE;
}

/** Starts transmission of message: BAM for destination address 0xff, TP.CM otherwise */
void can_transport_protocol_transmit(struct can_tx_multi_message *mmsg)
{
	struct can_tx_frame f;
	int is_bam = mmsg->destination_address == 0xff;
	mmsg->number_of_packets = (mmsg->message_size + 6) / 7;
	mmsg->sequence_index = 1;
	mmsg->sequence_max = mmsg->number_of_packets;

	//can_dbg("Sending TP message pgn %06x, sa %02x, da %02x\n", mmsg->pgn, mmsg->source_address, mmsg->destination_address);

	f.dlc = 8;
	f.id = can_compose_eid(3, 0xec, mmsg->destination_address, mmsg->source_address);
	f.data[0] = is_bam ? J1939_TP_CM_BAM : J1939_TP_CM_RTS;
	f.data[1] = mmsg->message_size & 0xff;
	f.data[2] = (mmsg->message_size >> 8) & 0xff;
	f.data[3] = (mmsg->message_size + 6) / 7;
	f.data[4] = 0xff;
	f.data[5] = mmsg->pgn & 0xff;
	f.data[6] = (mmsg->pgn >> 8) & 0xff;
	f.data[7] = (mmsg->pgn >> 16) & 0xff;
	can_buffer_tx_data(&f, mmsg->port);
	can_set_timeout(mmsg->timeout_time, is_bam ? J1939_BAM_DT_TO_DT_TX_TIMEOUT : J1939_RTS_TO_CTS_TIMEOUT);
	mmsg->state = CAN_TPTX_ACTIVE;
}
