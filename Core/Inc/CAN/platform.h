#ifndef CAN_PLATFORM_H
#define CAN_PLATFORM_H

#include "txrx.h"

/** \defgroup PlatformSupport Platform support module 
 @{ */

/** Platform init function.

 Implemented by user. Called by \fn can_txrx_init*/
void can_platform_init();

void can_engine_process_rx_hook();
int can_set_data_for_tx(can_port_t port, struct can_tx_frame *f);
void can_engine_work_pending_hook(uint32_t channels);

/** @} */

#endif /*CAN_PLATFORM_H*/
