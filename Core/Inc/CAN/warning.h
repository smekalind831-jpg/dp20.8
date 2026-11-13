#ifndef CAN_WARNING_H
#define CAN_WARNING_H

#include <stdint.h>

/**
 * \addtogroup Warnings
 * @{
 */

#define CAN_RX_MODULE				0
#define CAN_ENGINE_MODULE			1
#define CAN_TRANPORT_PROTOCOL_MODULE		2
#define CAN_J1939_DIAGNOSTIC_MESSAGE_MODULE	3
#define CAN_TXDATA_MODULE			4
#define CAN_J1939_ADDRESS_CLAIM_MODULE		5

#define CAN_MAX_MODULES			5

void can_set_warning_flag(uint8_t module, uint8_t flag);
void can_reset_warning_flag(uint8_t module, uint8_t flag);

int is_can_warning_flag_set(uint8_t module, uint8_t flag);

int can_warning_get_and_reset_next_flag(uint8_t *module, uint8_t *flag);

/** @} */

#endif /*CAN_WARNING_H*/
