#include "CAN/idlist.h"

//#include "can_debug.h"

static const struct can_rx_id_list *can_channel_rx_id_lists[CAN_MAX_CHANNELS];

/**
 * \defgroup IdList Channel Id matching support
 */

/** Assigns list to supplied channel. list should be valid all the time, contents are not copied.*/
//------------------------------------------------------------------------------
//-Присваивает список указанному каналу. список должен быть действительным все время, содержимое не копируется
void can_load_rx_id_list(can_channel_t channel, const struct can_rx_id_list *list)
{
	unsigned i;
	/*TODO:reset state*/
	if (channel >= CAN_MAX_CHANNELS) {
		/*set warning*/
//		can_dbg("%s: invalid channel %u\n", __func__, channel);
		return;
	}
//	can_dbg("%s: channel %u list:\n", __func__, channel);
	for (i = 0; i < list->size; ++i) {
		const struct can_rx_slot *slot = &list->data[i];
		(void)slot;
//		can_dbg("mask %08x id %08x port %u\n", slot->match_mask, slot->match_id, slot->port);
	}
	can_channel_rx_id_lists[channel] = list;
}

/** returns 1 if channel "i" wait for message "id" at port*/
int can_channel_match_message_id(can_channel_t i, can_port_t port, uint32_t id)
{
	unsigned j;

	const struct can_rx_id_list *list = can_channel_rx_id_lists[i];
	if (!list) {
//		can_dbg("%s: rxidlist not set for channel %u\n", __func__, i);
		return 0;
	}
	
	for (j = 0; j < list->size; ++j) {
		const struct can_rx_slot *slot = &list->data[j];
		if ((port != slot->port) && (slot->port != CAN_MATCH_ANY_PORT))
			continue;
		if ((slot->match_mask & id) != slot->match_id)
			continue;
		return 1;
	}
	return 0;
}
