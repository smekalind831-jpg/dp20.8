//tRX_MESSAGE gstRxMessages – This is the storage queue for any received messages after the channel tags have been assigned to them.
/*
7.4.2 void fnCANEngineInit()
This function does the initialization of any variables that are global to this module.
*/

#include "CAN/canengine.h"

#include <string.h>

//#include "can_debug.h"
#include "CAN/warning.h"
#include "CAN/Defs.h"
#include "CAN/idlist.h"

CAN_OS_MUTEX_DECLARE(can_engine_mutex);

#define CAN_ENGINE_MUTEX_LOCK   CAN_OS_MUTEX_LOCK(can_engine_mutex)
#define CAN_ENGINE_MUTEX_UNLOCK CAN_OS_MUTEX_UNLOCK(can_engine_mutex)


/**
 * \defgroup CanEngineInternals CAN Channel Engine internals
 * \brief Internal functions for channel message buffers manipulation and inner data
 *
 * Only non-static function of this group is can_engine_set_message_as_received(), which is used by
 * transport_protocol module to mark multi-packet messages for processing
 * @{
 */

#define CAN_ENGINE_INVALID_MESSAGE_INDEX 0xffffffff

static struct tcan_engine_data can_engine_data;

static unsigned get_can_rx_single_message_index(struct can_rx_single_message *msg)
{
	unsigned index = (msg - can_engine_data.rx_messages) / sizeof(struct can_rx_single_message);
	if (index >= CAN_ENGINE_MESSAGE_BUFFER_SIZE)
		return CAN_ENGINE_INVALID_MESSAGE_INDEX;
	return index;
}

/* called with held mutex */
static struct can_rx_single_message *get_message_buffer()
{
	if (can_engine_data.count >= CAN_ENGINE_MESSAGE_BUFFER_SIZE)
		return NULL;

	struct can_rx_single_message *msg = &can_engine_data.rx_messages[(can_engine_data.head + can_engine_data.count) % CAN_ENGINE_MESSAGE_BUFFER_SIZE];
	++can_engine_data.count;
	msg->header.channels_needing_data = 0;
	return msg;
}

static void release_message_buffer(struct can_rx_single_message *msg)
{
	unsigned idx = get_can_rx_single_message_index(msg);

	if (idx == CAN_ENGINE_INVALID_MESSAGE_INDEX) {
		/*should not happen*/
	//	can_dbg("%s fault: invalid message pointer\n", __func__);
		return;
	}
	if (!can_engine_data.count) {
	//	can_dbg("%s fault: message queue already empty\n", __func__);
		return;
	}
	if (msg == &can_engine_data.rx_messages[(can_engine_data.head + can_engine_data.count - 1) % CAN_ENGINE_MESSAGE_BUFFER_SIZE]) {
		//can_dbg("%s: purging last message\n", __func__);
		--can_engine_data.count;
		return;
	}
	if (msg != &can_engine_data.rx_messages[can_engine_data.head]) {
		//can_dbg("%s: release in middle of queue\n", __func__);
		return;
	}
	/*purge stale messages*/
	while (!msg->header.channels_needing_data) {
		QUEUE_POP(can_engine_data.head, can_engine_data.count, CAN_ENGINE_MESSAGE_BUFFER_SIZE);
		if (!can_engine_data.count) {
			//can_engine_data.head = 0;
			//can_dbg("Queue empty after stale purge\n");
			break;
		}
		msg = &can_engine_data.rx_messages[can_engine_data.head];
	}
}

static int channel_enqueue_message(can_channel_t i, struct can_rx_message_header *msg)
{
	struct can_engine_channel_data *d = &can_engine_data.channels[i];
	if (d->count >= CAN_ENGINE_CHANNEL_BUFFER_SIZE)
		return 0;
	d->pointer_location[(d->head + d->count) % CAN_ENGINE_CHANNEL_BUFFER_SIZE] = msg;
	++d->count;
	return 1;
}

static void can_dump_taken_message_buffers()
{
	unsigned index;
	for (index = 0; index < sizeof(can_engine_data.rx_messages) / sizeof(can_engine_data.rx_messages[0]); ++index) {
		struct can_rx_single_message *msg = &can_engine_data.rx_messages[index];
		(void)msg;
	//	can_dbg("channels: %u, frame id %08x, time_received %u\n", msg->header.channels_needing_data, msg->frame.id, (unsigned)msg->frame.time_received);
	}
}

/*
This function is used to place a pointer to a received multipacket message into the channel pointer buffer. The calling
function passes a pointer to the data concerning the multipacket message. can_engine_set_message_as_received then looks to see
if this message is needed by any of the channels. If it is needed, the pointer is saved in the channel pointer buffer and the
tag/s of the channel/s that need the message is attached to the queue location.
If there are no channels that need the message that is passed, then a warning flag will be set by calling the function
fnSetWarningFlag and passing it the module tag and the warning flag number 4.
If there is a channel that requires this message but no space is available in the channel pointer buffer, then a warning flag
will be set by calling the function fnSetWarningFlag and passing it the module tag and the warning flag number 0.
*/
void can_engine_set_message_as_received(struct can_rx_message_header *msg)
{
	unsigned i;
	unsigned count;
	uint32_t id;
	if (!msg || !msg->multi_packet)
		return;

	struct can_rx_multi_message *mmsg = can_to_rx_mmsg(msg);
	id = 0x80000000 | (mmsg->pgn << 8) | mmsg->source_address;

	CAN_ENGINE_MUTEX_LOCK;

	count = 0;
	for (i = 0; i < CAN_MAX_CHANNELS; ++i) {
		if (!can_channel_match_message_id(i, mmsg->port, id))
			continue;
		if (channel_enqueue_message(i, msg)) {
			++count;
		} else {
			can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_CHANNEL_RX_QUEUE_OVERFLOW_MULTI);
		}
	}
	if (!count) {
	//	can_dbg("Multi message %08x not needed\n", id);
		can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_UNUSED_MULTI_MESSAGE);
	}
	mmsg->header.channels_needing_data = count;

	CAN_ENGINE_MUTEX_UNLOCK;
}

/** @} */

/**
 * \defgroup CanEngine CAN Channel Engine
 * \brief Functions for mid-level channel processing of received messages
 *
 * can_engine_process_rx_queue() will call can_engine_process_rx_hook() and will extract single
 * packet messages from rx queue one-by-one and store them in inner structure with additional data,
 * corresponding pointers will be included in matching channels. Processing will stop on either
 * empty rx queue or full inner queue.
 * Channel processing code should use can_engine_get_channel_message() and
 * can_engine_release_channel_message in pairs.
 * @{
 */


/** \brief Module init function
 *
 * Init can_engine module. Should be called at start.
 */
void can_engine_init()
{
	memset(&can_engine_data, 0, sizeof(can_engine_data));
	CAN_OS_MUTEX_INIT(can_engine_mutex);
}

/** \brief RX queue processing.
 *
 * This function processes any messages that are stored in the temporary receive queue. Messages from the temporary
 * receive queue are copied to the receive queue, structure can_engine_data.rx_messages, and assigned channel tags. Once this
 * function is called all of the messages in the temporary receive queue will be processed. If the receive queue overflows,
 * the function will stop retrieving messages from the temporary receive queue and set a warning flag by calling function
 * can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_RX_QUEUE_OVERFLOW). The receive queue size can be
 * adjusted by changing the #define CAN_ENGINE_MESSAGE_BUFFER_SIZE which is located in the module canconf.h.
 * Each channel has a limit to the number of messages that can be waiting for it. This number can be adjusted by changing
 * the #define CAN_ENGINE_CHANNEL_BUFFER_SIZE located in canconf.h. If the channel pointer buffer overflows, a warning flag will
 * be set by calling the function can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_CHANNEL_RX_QUEUE_OVERFLOW_SINGLE).
 */
void can_engine_process_rx_queue()
{
	can_engine_process_rx_hook();

	CAN_ENGINE_MUTEX_LOCK;

	unsigned i, j;
	unsigned count;
	unsigned rx_messages_in_queue = can_rx_get_count();
	struct can_rx_single_message *msg;
	uint32_t channels_enqueued = 0;


	for (i = 0; i < rx_messages_in_queue; ++i) {
		msg = get_message_buffer();
		if (!msg) {
			if (!is_can_warning_flag_set(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_RX_QUEUE_OVERFLOW))
				can_dump_taken_message_buffers();
			can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_RX_QUEUE_OVERFLOW);
			break;
		}

		if (!can_dequeue_rx_frame(&msg->frame)) { //should not happen
		//	can_dbg("Trying to dequeue rx frame that should be there failed\n");
			can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_ASSERT_FAULT);
			release_message_buffer(msg);
			break;
		}

		msg->header.multi_packet = 0;
		count = 0;
		for (j = 0; j < CAN_MAX_CHANNELS; ++j) {
			if (!can_channel_match_message_id(j, msg->frame.interface, msg->frame.id))
				continue;
			if (channel_enqueue_message(j, &msg->header)) {
				//can_dbg("Rx msg %08x enqueued in %u queue\n", msg->frame.id, j);
				++count;
				channels_enqueued |= 1 << j;
			} else {
				can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_CHANNEL_RX_QUEUE_OVERFLOW_SINGLE);
			}
		}
		//can_dbg("Message %08x needed by %u channels\n", msg->frame.id, count);
		if (!count) {
			release_message_buffer(msg);
			continue;
		}
		msg->header.channels_needing_data = count;
	}

	CAN_ENGINE_MUTEX_UNLOCK;

	if (channels_enqueued)
		can_engine_work_pending_hook(channels_enqueued);
}

/** \brief Gets first message from channel queue
 *
 * This function returns a pointer too the requesting channel for the next message in the receive queue. If there are no
 * messages in the receive queue, a NULL pointer is returned. This function is meant to be used in conjunction with function
 * can_engine_release_channel_message(). If this function is called twice without calling function
 * can_engine_release_channel_message() in between, the pointer returned will be the same as it was for the previous call.
 * Calls can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_INVALID_CHANNEL) if invalid channel supplied.
 *
 */
struct can_rx_message_header  *can_engine_get_channel_message(uint8_t channel)
{
	if (channel >= CAN_MAX_CHANNELS) {
		can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_INVALID_CHANNEL);
		return NULL;
	}
	struct can_engine_channel_data *d = &can_engine_data.channels[channel];

	CAN_ENGINE_MUTEX_LOCK;

	struct can_rx_message_header *ret = d->count ? d->pointer_location[d->head] : NULL;

	CAN_ENGINE_MUTEX_UNLOCK;

	return ret;
}

/** \brief Erases first message from channel queue
 *
 * This function uses the passed channel number to erases the channel tag connected to the next message in the receive
 * queue that is for the passed channel. This function is meant to be used in conjunction with function
 * can_engine_get_channel_message. If this function is not called between calls to the function can_engine_get_channel_message,
 * then the pointer returned to the calling function for can_engine_get_channel_message will be the same for both calls.
 * If after erasing the channel tag connected to the message, there are no other tags attached to the message, the receive
 * queue location will be available to receive a new message. If the channel number passed to the function has not been defined
 * or there is no tag attached to a message for the passed channel tag, the function will call
 * can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_CHANNEL_RX_QUEUE_EMPTY_RELEASE)
 * and return 0. If a tag was found for the passed channel then the function will return 1.
 * If the channel tag passed to the function has not been defined a warning flag will be set by calling the function
 * can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_INVALID_CHANNEL).
 *
 * Note: It is very important that each channel attends to its received messages in a timely fashion or the buffer will overflow
 * and messages will be lost.
 */
int can_engine_release_channel_message(uint8_t channel)
{
	if (channel >= CAN_MAX_CHANNELS) {
		can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_INVALID_CHANNEL);
		return 0;
	}
	struct can_engine_channel_data *d = &can_engine_data.channels[channel];

	CAN_ENGINE_MUTEX_LOCK;

	int ret;
	if (d->count) {
		struct can_rx_message_header *msg = d->pointer_location[d->head];
		QUEUE_POP(d->head, d->count, CAN_ENGINE_CHANNEL_BUFFER_SIZE);
		if (!msg->channels_needing_data) {
			/*memory corruption*/
			//can_dbg("%s: channels_needing_data already zero\n", __func__);
			can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_ASSERT_FAULT);
		} else if (!--msg->channels_needing_data) {
			if (!msg->multi_packet) {
				release_message_buffer(can_to_rx_smsg(msg));
			} else {
				can_to_rx_mmsg(msg)->active = 0;
			}
		}
		ret = 1;
	} else {
		can_set_warning_flag(CAN_ENGINE_MODULE, CAN_ENGINE_WARNING_CHANNEL_RX_QUEUE_EMPTY_RELEASE);
		ret = 0;
	}

	CAN_ENGINE_MUTEX_UNLOCK;

	return ret;
}

void can_engine_purge_received_messages(uint8_t channel)
{
	while (can_engine_get_channel_message(channel))
		can_engine_release_channel_message(channel);
}

/** @} */

