#ifndef CAN_TRANSPORT_PROTOCOL_H
#define CAN_TRANSPORT_PROTOCOL_H

#include "canengine.h"

#include "canconf.h"

/** \addtogroup TransportProtocol
  @{
  */

/** Multipacket message transmit slot. Used by \ref TransportProtocolInternals and \ref TxData modules */
struct can_tx_multi_message {
	uint32_t pgn;
	can_port_t port;
	uint8_t source_address;
	uint8_t destination_address;
	uint8_t number_of_packets;
	uint8_t sequence_index;
	uint8_t sequence_max;
	uint8_t state;
#define CAN_TPTX_INACTIVE	0
#define CAN_TPTX_WAITING_CTS	1
#define CAN_TPTX_ACTIVE		2
#define CAN_TPTX_ONHOLD		3
#define CAN_TPTX_WAITING_ACK	4
	uint16_t message_size;
	can_clock_t timeout_time;
	uint8_t data[CAN_TP_BUFFER_SIZE];
};


extern void can_transport_protocol_init();

extern void can_transport_protocol_process();

extern void can_transport_protocol_check_timeout();

/** @} */

/** \addtogroup TransportProtocolInternals
  @{
  */

void can_transport_protocol_dt_rx(struct can_rx_single_message *msg);
void can_transport_protocol_bam_rts_rx(struct can_rx_single_message *msg);
void can_transport_protocol_cm_cts_rx(struct can_rx_single_message *msg);
void can_transport_protocol_cm_ack_comm_abort(struct can_rx_single_message *msg);

#define J1939_TP_CM_RTS	16
#define J1939_TP_CM_CTS	17
#define J1939_TP_CM_BAM	32
#define J1939_TP_CM_ACK 19
#define J1939_TP_CM_COMM_ABORT 255

struct can_tx_multi_message *get_can_transport_protocol_tx_data_buffer(can_port_t port, uint32_t pgn, uint8_t da);

void can_transport_protocol_release_data_buffer(struct can_tx_multi_message *mmsg);

void can_transport_protocol_transmit(struct can_tx_multi_message *mmsg);

#define CAN_TRANSPORT_PROTOCOL_WARNING_RX_BAM_OVERFLOW			0
#define CAN_TRANSPORT_PROTOCOL_WARNING_RX_CONNECT_OVERFLOW		1
#define CAN_TRANSPORT_PROTOCOL_WARNING_TX_DT_OVERFLOW			2
#define CAN_TRANSPORT_PROTOCOL_WARNING_RX_DATA_OVERFLOW			3
#define CAN_TRANSPORT_PROTOCOL_WARNING_RX_DATA_MAY_OVERFLOW		4
#define CAN_TRANSPORT_PROTOCOL_WARNING_RX_BAM_TOO_SMALL_BUFFER		5
#define CAN_TRANSPORT_PROTOCOL_WARNING_RX_CONNECT_TOO_SMALL_BUFFER	6

/** Internal */
struct can_transport_protocol_data {
	struct can_rx_multi_message rx_bam_messages[CAN_RX_BAM_BUFFER_SIZE];
	struct can_rx_multi_message rx_connect_messages[CAN_RX_CONNECT_BUFFER_SIZE];
	struct can_tx_multi_message tx_bam_messages[CAN_TX_BAM_BUFFER_SIZE];
	struct can_tx_multi_message tx_connect_messages[CAN_TX_CONNECT_BUFFER_SIZE];
};

/** @} */


#endif /*CAN_TRANSPORT_PROTOCOL_H*/
