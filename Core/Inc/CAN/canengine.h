#ifndef CANENGINE_H
#define CANENGINE_H

#include "canconf.h"

#include "txrx.h"
//#include "can_debug.h"
#include "platform.h"


/**
  \addtogroup CanEngine
  @{
 */

/** \brief Header structure common for can_rx_single_message and can_rx_multi_message
 */
struct can_rx_message_header {
/** value determines type of structure, 0 for can_rx_single message, 1 for can_rx_multi_message. */
	uint8_t multi_packet;
/** used internally by \ref CanEngine, holds number of channels that haven't processed this
 * message yet
 */
	uint8_t channels_needing_data;
};

/** \brief structure for single CAN frame messages
 *
 * Used by \ref CanEngine to attach internal data to received frame.
 * Channel processing code will get this message from header using can_to_rx_smsg()
 */
struct can_rx_single_message {
	struct can_rx_message_header header;
	struct can_rx_frame frame;
};

/* \addtogroup TransportProtocol
 * @{
 */

/** \brief structure for multi CAN frame messages
 *
 * Used by \ref TransportProtocol to save state of connection and data
 * Channel processing code will get this message from header using can_to_rx_mmsg()
 */
struct can_rx_multi_message {
	struct can_rx_message_header header;
	uint32_t pgn;
	can_port_t port;
	uint8_t source_address;
	uint8_t destination_address;
	uint8_t number_of_packets;
	uint16_t message_size;
	can_clock_t time_received;
	can_clock_t timeout_time;
	uint8_t data[CAN_TP_BUFFER_SIZE];
	uint8_t received_number_of_packets;
	uint8_t active;
};

/** @} */


/** Returns source address for \p msg */
static inline uint8_t can_smsg_sa(const struct can_rx_single_message *msg) { return can_id_sa(msg->frame.id); }
/** Returns destionation address for \p msg */
static inline uint8_t can_smsg_da(const struct can_rx_single_message *msg) { return can_id_da(msg->frame.id); }
/** Returns pdu format for \p msg */
static inline uint8_t can_smsg_pduf(const struct can_rx_single_message *msg) { return can_id_pduf(msg->frame.id); }

/** Checks if header of multi message and returns cast pointer or NULL otherwise*/
static inline struct can_rx_multi_message *can_to_rx_mmsg(struct can_rx_message_header *h)
{
/*	if (!h->multi_packet) {
		can_dbg("can rx message header->mmsg conversion fault\n");
		return NULL;
	}
*/
	return (struct can_rx_multi_message *)h;
}

/** Checks if header of single message and returns cast pointer or NULL otherwise*/
static inline struct can_rx_single_message *can_to_rx_smsg(struct can_rx_message_header *h)
{
/*
	if (h->multi_packet) {
		can_dbg("can rx message header->smsg conversion fault\n");
		return NULL;
	}
*/
	return (struct can_rx_single_message *)h;
}

/** Extract source address from message, single/multi compatible. */
static inline uint8_t can_rx_msg_sa(const struct can_rx_message_header *_h) {
	struct can_rx_message_header *h = (struct can_rx_message_header *)_h;
	return h->multi_packet ? can_to_rx_mmsg(h)->source_address : (can_to_rx_smsg(h)->frame.id & 0xff);
}

/** Extract source port from message, single/multi compatible. */
static inline uint8_t can_rx_msg_port(const struct can_rx_message_header *_h) {
	struct can_rx_message_header *h = (struct can_rx_message_header *)_h;
	return h->multi_packet ? can_to_rx_mmsg(h)->port : can_to_rx_smsg(h)->frame.interface;
}

extern void can_engine_init();
extern void can_engine_process_rx_queue();
struct can_rx_message_header *can_engine_get_channel_message(uint8_t channel);
int can_engine_release_channel_message(uint8_t channel);
void can_engine_purge_received_messages(uint8_t channel);

extern void can_engine_work_pending_hook(uint32_t channels_mask);

#define CAN_ENGINE_WARNING_CHANNEL_RX_QUEUE_OVERFLOW_MULTI	0
#define CAN_ENGINE_WARNING_RX_QUEUE_OVERFLOW			1
#define CAN_ENGINE_WARNING_CHANNEL_RX_QUEUE_OVERFLOW_SINGLE	2
#define CAN_ENGINE_WARNING_CHANNEL_RX_QUEUE_EMPTY_RELEASE	3
#define CAN_ENGINE_WARNING_UNUSED_MULTI_MESSAGE			4
#define CAN_ENGINE_WARNING_INVALID_CHANNEL			5
#define CAN_ENGINE_WARNING_ASSERT_FAULT				7

/** @} */

/** \addtogroup CanEngineInternal
  @{
  */

/** Internal. */
struct can_engine_channel_data {
	unsigned head;
	unsigned count;
	struct can_rx_message_header *pointer_location[CAN_ENGINE_CHANNEL_BUFFER_SIZE];
};

/** Internal. */
struct tcan_engine_data {
	unsigned head;
	unsigned count;
	struct can_rx_single_message rx_messages[CAN_ENGINE_MESSAGE_BUFFER_SIZE];
	struct can_engine_channel_data channels[CAN_MAX_CHANNELS];
};

void can_engine_set_message_as_received(struct can_rx_message_header *msg);

/** @} */



#endif /*CANENGINE_H*/
