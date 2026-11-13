#ifndef CAN_IDLIST_H
#define CAN_IDLIST_H

#include "types.h"
#include "canconf.h"

#define CAN_MATCH_ANY_PORT 0xff

#define CAN_MATCH_PDUF_MASK 0x1ff0000
#define CAN_MATCH_PDUFS_MASK 0x1ffff00
#define CAN_MATCH_ID_FROM_PDUF

/* CAN receive virtual lot match definition*/
struct can_rx_slot {
	uint32_t match_mask; ///< Note use of 0x80000000 bit to match extended id
	uint32_t match_id; ///< Note use of 0x80000000 bit to match extended id
	unsigned port;
};

struct can_rx_id_list {
	const struct can_rx_slot *data;
	unsigned size;
};

#define MAKE_IDLIST_FROM_RX_SLOT_ARRAY(name, slot_array_name)		\
const struct can_rx_id_list name = {					\
	.data = slot_array_name,					\
	.size = sizeof(slot_array_name) / sizeof(struct can_rx_slot),	\
}									\


void can_load_rx_id_list(can_channel_t channel, const struct can_rx_id_list *list);
int can_channel_match_message_id(can_channel_t i, can_port_t port, uint32_t id);

#endif /*CAN_IDLIST_H*/
