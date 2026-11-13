#ifndef J1939_ADDRESS_CLAIM_H
#define J1939_ADDRESS_CLAIM_H

#ifdef __cplusplus
extern "C"
{
#endif



#include "types.h"
#include "canengine.h"

/** \addtogroup AddressClaim
  @{
 */

extern void j1939_address_claim_init();
extern void j1939_address_claim_process();
uint8_t can_j1939_source_address(can_port_t port, can_instance_t instance);

can_instance_t can_find_instance(can_port_t port, uint8_t da);

#define CAN_J1939_ADDRESS_CLAIM_WARNING_SAME_NAME	0

/** @} */

/** \addtogroup AddressClaimInternals
 @{
 */


/** return 1 if source address changed, 0 if not changed*/
int j1939_address_claim_check(struct can_rx_single_message *msg);
void j1939_address_claim_fill_name(can_port_t port, can_instance_t instance, uint8_t *data);
void j1939_address_claim_set_timeout(can_port_t port, can_clock_t timeout);
//uint8_t *j1939_address_claim_get_name(uint8_t port);
int j1939_address_claim_check_commanded_address(struct can_rx_multi_message *mmsg);

/** @} */

/** \addtogroup PlatformSupport
 @{
 */

can_clock_t j1939_address_claim_random_delay();

/** @} */

#ifdef __cplusplus
}
#endif

#endif /*J1939_ADDRESS_CLAIM_H*/
