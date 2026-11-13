#ifndef CAN_TXDATA_H
#define CAN_TXDATA_H

#include "txdata_base.h"

/** \defgroup TxData J1939 Paramater group transmit module
 @{ */
   
#ifdef __cplusplus
extern "C"
{
#endif


struct can_tx_message_info
{
	uint32_t pgn;
	can_port_t port;
	can_instance_t instance;
	uint8_t priority;
	uint8_t destination_address;
#define CAN_TX_MESSAGE_TYPE_EID	0
#define CAN_TX_MESSAGE_TYPE_SID	1
	uint8_t type;
	can_clock_t broadcast_rate; ///< use BROADCAST_RATE_INFINITE for manually sent messages
};

struct can_tx_parameter
{
	struct can_tx_message_info info;
	uint8_t start_byte;
	uint8_t start_bit;
	uint16_t bit_length;
	uint8_t *pdata;
};

struct can_tx_parameter_group
{
	struct can_tx_message_info info;
	//can_clock_t timeout_time;
	can_clock_t last_transmission_time;
	uint8_t multi_packet;
};

/** TODO: support for constant can_tx_parameters, not all of them need to be in RAM */
struct can_txdata {
	struct can_tx_parameter parameters[CAN_TX_PARAMETER_BUFFER_SIZE];
	struct can_tx_parameter_group parameter_groups[CAN_TX_PARAMETER_GROUP_BUFFER_SIZE];
	unsigned parameter_count;
	unsigned parameter_group_count;
};

extern void can_txdata_init();
int can_txdata_load_message_into_list(const struct can_tx_parameter *p);
/** Not implemented */
int can_txdata_remove_message_from_list(uint32_t pgn, can_port_t port);
/** Not implemented */
int can_txdata_remove_message_from_list_ex(uint32_t pgn, can_port_t port, can_instance_t instance);
extern void can_txdata_process();
extern void can_txdata_process_txmessage(unsigned index, uint8_t da, can_clock_t timeout);

int can_send_request_message(struct can_tx_message_info* info);

unsigned can_txdata_build_data(unsigned index, uint8_t *data);

unsigned can_txdata_find_message(uint32_t pgn, can_port_t port);
unsigned can_txdata_find_message_ex(uint32_t pgn, can_port_t port, can_instance_t instance);

int can_txdata_set_broadcast_rate(unsigned index, can_clock_t rate);
int can_txdata_force_send(unsigned index, can_clock_t min_rate);

can_clock_t can_txdata_get_j1939_last_tranmission_time(unsigned index);
can_clock_t can_txdata_get_j1939_broadcast_rate(unsigned index);

uint8_t *can_txdata_get_sequence_number(unsigned index);
uint8_t can_txdata_get_priority(unsigned index);
int can_txdata_set_priority(unsigned index, uint8_t priority);

/*
8.5.15 U_8 fnGetJ1939PGNAccess(u8Index)
This function returns the access level for the parameter group with the passed index.
8.5.16 tTX_MESSAGE *fnGetTxParameterList()
This function returns a pointer to the list of messages that are cyclically broadcast or broadcast upon request.
8.5.17 BOOLEAN fnSetParameterGroupTx( u32PGN, u8CANPort )
This function initiate the immediate transmission of the a message in the list of messages that are transmitted cyclically or
by request. The passed PGN is compared to the messages in the list and if a match is found the message is broadcast.
A TRUE or FALSE is returned to the calling function to indicate the success or failure finding a message with the passed
PGN.
*/

#define CAN_TXDATA_WARNING_PARAMETER_GROUP_OVERFLOW	0
#define CAN_TXDATA_WARNING_PARAMETER_OVERFLOW		1
#define CAN_TXDATA_WARNING_MULTIPACKET_OVERFLOW		2
#define CAN_TXDATA_WARNING_TRANSMIT_QUEUE_OVERFLOW	3

#ifdef __cplusplus
}
#endif

/** @} */

#endif /*CAN_TXDATA_H*/
