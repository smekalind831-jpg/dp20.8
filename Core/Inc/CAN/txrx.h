#ifndef CAN_TXRX_H
#define CAN_TXRX_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "can_clock.h"
#include "types.h"

/**
  \addtogroup CanLowTxRx
  @{
*/


/** Basic structure for received CAN frames, initialized by platform module*/
struct can_rx_frame {
	can_clock_t time_received;///< based upon hardware timestamps or can_clock
	can_port_t interface; ///< can hardware interface index
	uint8_t dlc;
	uint32_t id;
	uint8_t data[8];
};

/** Extract CAN J1939 source address from id*/
static inline uint8_t can_id_sa(uint32_t id) { return id & 0xff; }
/** Extract CAN J1939 destination address from id*/
static inline uint8_t can_id_da(uint32_t id) { return (id & 0xff00) >> 8; }
/** Extract CAN J1939 PDU format from id*/
static inline uint8_t can_id_pduf(uint32_t id) { return (id & 0xff0000) >> 16; }

#define CAN_ID_EID_MARKER 0x80000000

/** Composes valid Extended ID (EID) from CAN J1939 components*/
static inline uint32_t can_compose_eid(uint8_t priority, uint8_t pduf, uint8_t pdus, uint8_t sa) {
	return CAN_ID_EID_MARKER
		| (priority << 26)
		| (pduf << 16)
		| (pdus << 8)
		| sa;
}

/** Basic structure for CAN tx frames, used in low-level tx */
struct can_tx_frame {
	uint32_t id;
	uint8_t dlc;
	uint8_t data[8];
};

extern void can_txrx_init();

int can_buffer_tx_data(struct can_tx_frame *f, can_port_t port);
int can_buffer_tx_data_ex(struct can_tx_frame *f, can_port_t port, uint8_t *fail_counter);
int can_buffer_raw_rx_data(struct can_rx_frame *f);
int can_buffer_rx_full(void);
extern void can_tx_data();

extern void can_txrx_purge_rx_queue();
extern void can_txrx_purge_tx_queue(can_port_t port);


/** @} */

/** \addtogroup CanLowTxRxInternals
 * @{
 */

int can_dequeue_rx_frame(struct can_rx_frame *f);
unsigned can_rx_get_count();

/** @} */

#ifdef __cplusplus
}
#endif

#endif /*CAN_TXRX_H*/
