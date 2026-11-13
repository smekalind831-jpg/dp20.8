#include "CAN/j1939_address_claim.h"

#include <string.h>

#include "CAN/idlist.h"
#include "CAN/warning.h"
#include "main.h"

/**
 * \defgroup AddressClaimInternals Address claim internals
 * \brief Address claim module internal functions and data structures
 *
 * TODO
 * @{
 */

static const struct can_rx_slot can_j1939_address_claim_rx_slots[] = {
	{ .match_mask = CAN_MATCH_PDUF_MASK, .match_id = 0xea0000, .port = CAN_MATCH_ANY_PORT }, //request pgn
	{ .match_mask = CAN_MATCH_PDUF_MASK, .match_id = 0xee0000, .port = CAN_MATCH_ANY_PORT }, //address claimed
	{ .match_mask = CAN_MATCH_PDUFS_MASK, .match_id = 0xfed800, .port = CAN_MATCH_ANY_PORT }, //commanded address pgn
};

static MAKE_IDLIST_FROM_RX_SLOT_ARRAY(can_j1939_address_claim_rx_id_list, can_j1939_address_claim_rx_slots);

struct j1939_address_claim_data
{
	uint8_t can_source_addresses[CAN_INTERFACE_COUNT][CAN_MAX_SOURCE_ADDRESS_COUNT];
	can_clock_t timeouts[CAN_INTERFACE_COUNT][CAN_MAX_SOURCE_ADDRESS_COUNT];
	uint8_t states[CAN_INTERFACE_COUNT][CAN_MAX_SOURCE_ADDRESS_COUNT];
#define CLAIM_STATE_NONE		0
#define CLAIM_STATE_CLAIM_COOLDOWN_BEFORE_ACTIVATION	1
#define CLAIM_STATE_CLAIM_TRANSMISSION_ERROR_COOLDOWN	2
#define CLAIM_STATE_CLAIMED_ADDRESS		3
#define CLAIM_STATE_CANNOT_CLAIM_IDLE		4
#define CLAIM_STATE_CANNOT_CLAIM_WAITING_TRANSMISSION 5
#define CLAIM_STATE_ERROR_SAME_NAME 6
};

static struct j1939_address_claim_data mdata;

/** Internal */
static void j1939_address_claim_transmit(can_port_t port, can_instance_t instance)
{
	struct can_tx_frame frame;
	frame.id = 0x8ceeff00 | mdata.can_source_addresses[port][instance];
	frame.dlc = 8;
	j1939_address_claim_fill_name(port, instance, frame.data);
	can_buffer_tx_data(&frame, port);
}

/** Internal */
static void can_j1939_address_claim_schedule_request_response(can_port_t port, can_instance_t instance)
{
	switch (mdata.states[port][instance]) {
	case CLAIM_STATE_CANNOT_CLAIM_IDLE:
		mdata.states[port][instance] = CLAIM_STATE_CANNOT_CLAIM_WAITING_TRANSMISSION;
		mdata.timeouts[port][instance] = get_can_clock() + j1939_address_claim_random_delay();
		break;
	case CLAIM_STATE_CLAIMED_ADDRESS:
		j1939_address_claim_transmit(port, instance);
		break;
	default:
		/*response prohibited*/
		break;
	}
}

/** NAME comparison function, returns -1 for \a l < \a r, 0 for \a l == \a r and 1 for \a l > \a r */
static int compare_names(uint8_t *l, uint8_t *r)
{
	int i;
	for (i = 7; i >= 0; --i) {
		if (l[i] < r[i])
			return -1;
		if (l[i] > r[i])
			return 1;
	}
	return 0;
}

/** Internal */
static void j1939_address_lost(can_port_t port, can_instance_t instance)
{
	//uint8_t addr = mdata.can_source_addresses[port][instance];
	mdata.can_source_addresses[port][instance] = 0xfe;

	//get new address
	//j1939_address_claim_transmit(port, instance);
	mdata.states[port][instance] = CLAIM_STATE_CANNOT_CLAIM_WAITING_TRANSMISSION;

	mdata.timeouts[port][instance] = get_can_clock() + j1939_address_claim_random_delay();
}


/** @} */

/**
 * \defgroup AddressClaim Address claim
 * \brief Address claim support functions and inner state
 *
 * Takes NAME components from \ref CanConf and claims all configured addresses
 * Does not support address arbitration and commanded address but will set warning if device with the same NAME will try to claim same address
 * See SAE J1939-81 for details
 * @{
 */


/**
 * \brief Module init function. Call at start.
 */
void j1939_address_claim_init()
{
	unsigned i, j, acount;
	memset(&mdata, 0, sizeof(mdata));//defaults as CLAIM_STATE_NONE
	memset(mdata.can_source_addresses, 0xfe, sizeof(mdata.can_source_addresses));

	for (i = 0; i < CAN_INTERFACE_COUNT; ++i) {
		acount = CAN_SOURCE_ADDRESS_COUNT(i);
		for (j = 0; j < acount; ++j) {
			if (CAN_ADDRESS_CLAIM_SUPPORTED(i, j)) {
				mdata.timeouts[i][j] = get_can_clock() + j1939_address_claim_random_delay();
			} else {
				mdata.states[i][j] = CLAIM_STATE_CLAIMED_ADDRESS;
				mdata.can_source_addresses[i][j] = CAN_PREFERRED_SOURCE_ADDRESS(i, j);
			}
		}
	}

	can_load_rx_id_list(CAN_J1939_ADDRESS_CLAIM_CHANNEL, &can_j1939_address_claim_rx_id_list);
}

/**
 * \brief Source address for supplied port/instance pair
 *
 * Returns claimed address if available or 0xff if not claimed yet or claim failed
 */
uint8_t can_j1939_source_address(can_port_t port, can_instance_t instance)
{
	uint8_t address = mdata.states[port][instance] == CLAIM_STATE_CLAIMED_ADDRESS
		? mdata.can_source_addresses[port][instance] : 0xff;
	return address;
}

/**
 * \brief Returns instance index for supplied port and destination address, or 0xff in not found.
 *
 * Used by all modules that support specific destination address messages reception.
 */
can_instance_t can_find_instance(can_port_t port, uint8_t da)
{
	unsigned i;
	unsigned acount = CAN_SOURCE_ADDRESS_COUNT(port);
	if (da == 0xff)
		return CAN_INVALID_INSTANCE;
	for (i = 0; i < acount; ++i) {
		if (mdata.can_source_addresses[port][i] == da)
			return i;
	}
	return CAN_INVALID_INSTANCE;
}

/**
 * \brief Processes received address claim messages and schedules responses.
 *
 * For request pgn (0xea00) checks DA, if 0xff or one of this device instances schedules response (random delay).
 * Checks address claims (0xee00) and either retransmits own address claim or looses address and disables transmission from lost address.
 * TODO: Support for multipacket commanded address message
 */
void j1939_address_claim_process()
{
	unsigned i, j;
	struct can_rx_message_header *header;
	for (;;) {
		header = can_engine_get_channel_message(CAN_J1939_ADDRESS_CLAIM_CHANNEL);
		if (!header)
			break;
		if (!header->multi_packet) {
			struct can_rx_single_message *smsg = can_to_rx_smsg(header);
			switch (smsg->frame.id & CAN_MATCH_PDUF_MASK) {
			case 0xea0000: {
				if (smsg->frame.dlc < 3)
					break;
				uint32_t pgn = smsg->frame.data[0]
					| (smsg->frame.data[1] << 8);
				if (pgn != 0x00ee00)
					break;
				uint8_t da = can_smsg_da(smsg);
				if (da == 0xff) {
					unsigned acount = CAN_SOURCE_ADDRESS_COUNT(smsg->frame.interface);
					for (i = 0; i < acount; ++i) {
						if (!CAN_ADDRESS_CLAIM_SUPPORTED(smsg->frame.interface, i))
							continue;
						can_j1939_address_claim_schedule_request_response(smsg->frame.interface, i);
					}
				} else {
					can_instance_t instance = can_find_instance(smsg->frame.interface, da);
					if (instance != CAN_INVALID_INSTANCE) {
						can_j1939_address_claim_schedule_request_response(smsg->frame.interface, instance);
					}
				}
			} break;
			case 0xee0000:
				if (!j1939_address_claim_check(smsg))
					break;
				/*close connections from that address and proceed with claiming other address*/
				break;
			default:
				/*assert failure*/
				break;
			}
		} else {
			struct can_rx_multi_message *mmsg = can_to_rx_mmsg(header);
			if ((mmsg->pgn == 0xfed800)
			&& (mmsg->message_size == 9)) {
				j1939_address_claim_check_commanded_address(mmsg);
			}
		}
		can_engine_release_channel_message(CAN_J1939_ADDRESS_CLAIM_CHANNEL);
	}
//#define aclaim_dbg(fmt, args...) can_dbg("[ADDRESS_CLAIM]" fmt, ##args)
	for (i = 0; i < CAN_INTERFACE_COUNT; ++i) {
		unsigned acount = CAN_SOURCE_ADDRESS_COUNT(i);
		for (j = 0; j < acount; ++j) {
			uint8_t state = mdata.states[i][j];
			if (!can_timed_out(mdata.timeouts[i][j]))
				continue;
			/**/
			switch (state) {
			case CLAIM_STATE_NONE: {
				/*try claiming*/
				//can_dbg("[%u,%u] state %u timeout: transmitting claim\n", i, j, state);
				mdata.can_source_addresses[i][j] = CAN_PREFERRED_SOURCE_ADDRESS(i, j);
				mdata.states[i][j] = CLAIM_STATE_CLAIM_COOLDOWN_BEFORE_ACTIVATION;
				can_set_timeout(mdata.timeouts[i][j], 250);
				j1939_address_claim_transmit(i, j);
			} break;
			case CLAIM_STATE_CLAIM_COOLDOWN_BEFORE_ACTIVATION:
				//can_dbg("[%u,%u] state %u timeout: transmitting claim\n", i, j, state);
				mdata.states[i][j] = CLAIM_STATE_CLAIMED_ADDRESS;
				/*transit to finished state and activate tx*/
				break;
			case CLAIM_STATE_CANNOT_CLAIM_WAITING_TRANSMISSION:
				/**/
				j1939_address_claim_transmit(i, j);
				mdata.states[i][j] = CLAIM_STATE_CANNOT_CLAIM_IDLE;
				break;
			/*finished, error etc - no timeout action*/
			default: {
			} break;
			}
		}
	}
}

/** return 1 if source address changed, 0 if not changed*/
int j1939_address_claim_check(struct can_rx_single_message *msg)
{
	uint8_t my_name[8];
	can_port_t port = msg->frame.interface;
	/*check if sa == our and check name against our name*/;
	can_instance_t instance = can_find_instance(port, can_smsg_sa(msg));
	if (instance == CAN_INVALID_INSTANCE)
		return 0;
	if (!CAN_ADDRESS_CLAIM_SUPPORTED(port, instance))
		return 0;
	//claiming our address
	//compare names and return 1 if changed
	j1939_address_claim_fill_name(port, instance, my_name);
	int ret = compare_names(my_name, msg->frame.data);
	if (!ret) {
		can_set_warning_flag(CAN_J1939_ADDRESS_CLAIM_MODULE, CAN_J1939_ADDRESS_CLAIM_WARNING_SAME_NAME);
		//j1939_address_lost(port, instance);
		//mdata.states[port][instance] = CLAIM_STATE_ERROR_SAME_NAME;
		/*same address?*/
		return 0;
	}

	if (ret < 0) {
		j1939_address_claim_transmit(port, instance);
		return 0;
	}
	/*enter address claiming mode*/
	j1939_address_lost(port, instance);
	return 1;
}

/** fills supplied buffer with NAME data as transmitted in address claim */
void j1939_address_claim_fill_name(can_port_t port, can_instance_t instance, uint8_t *data)
{
	uint32_t identity = CAN_IDENTITY_NUMBER(port, instance);
	uint16_t manufacturer = CAN_MANUFACTURER_CODE(port, instance);
	/*fill with our NAME*/;
	data[0] = identity & 0xff;
	data[1] = (identity >> 8) & 0xff;
	data[2] = ((identity >> 16) & 0x1f)
		| ((manufacturer & 0x07) << 5);
	data[3] = (manufacturer >> 3) & 0xff;
	data[4] = (CAN_ECU_INSTANCE(port, instance) & 0x07)
		| ((CAN_FUNCTION_INSTANCE(port, instance) & 0x1f) << 3);
	data[5] = CAN_FUNCTION(port, instance);
	data[6] = (CAN_VEHICLE_SYSTEM(port, instance) & 0x7f) << 1;
	data[7] = (CAN_VEHICLE_SYSTEM_INSTANCE(port, instance) & 0x0f)
		| ((CAN_INDUSTRY_GROUP(port, instance) & 0x07) << 4)
		| (CAN_ARBITRARY_ADDRESS_CAPABLE(port, instance) ? 0x80 : 0);
}

/** Does nothing? */
void j1939_address_claim_set_timeout(uint8_t port, can_clock_t timeout)
{
	/**/;
}

/** Not implemented */
int j1939_address_claim_check_commanded_address(struct can_rx_multi_message *mmsg)
{
	/*check if we can move to other address*/;
	return 0;
}

/** @} */
