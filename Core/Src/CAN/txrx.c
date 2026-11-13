#include "CAN/txrx.h"

#include <string.h>

#include "CAN/types.h"
#include "CAN/warning.h"
#include "defs.h"

#include "CAN/canconf.h"
#include "CAN/can_clock.h"
//#include "CAN/can_debug.h"
#include "CAN/platform.h"

/**
 * \defgroup CanLowTxRx CAN Low level messaging API
 * \brief Functions for low-level tx/rx and structures representing single frames
 *
 * User should use can_txrx_init() at application start to initialize inner structures.
 *
 * Channel API will use can_dequeue_rx_frame() in processing function,
 * users should not call it directly.
 *
 * can_buffer_raw_rx_data should be used in platform CAN frame rx implementation.
 *
 * can_buffer_tx_data should be used for low-level CAN frames transmission when normal
 * logic is not enough.
 *
 * can_id_* and can_compose_eid are just convenience functions for id part extraction
 */

/**
 * \defgroup CanLowTxRxInternals CAN Low level messaging internals
 * \ingroup CanStackInternals
 * \brief Low level tx/rx stack internal functions
 *
 * Channel API will use can_dequeue_rx_frame() in processing function,
 * users should not call it directly.
 *
 * can_buffer_raw_rx_data should be used in platform CAN frame rx implementation.
 * @{
 */

//- CAN очередь одиночных принятых кадров, ожидающая назначения канала
/** CAN single received frames queue awaiting for channel assignment */
struct can_rx_message_queue {
	unsigned head;
	unsigned count;
	struct can_rx_frame data[CAN_RX_QUEUE_SIZE];
};

//- CAN очередь отдельных кадров, ожидающая свободного аппаратного слота для передачи, по одной очереди на порт CAN (интерфейс)
/** CAN single frames queue awaiting for free hardware transmission slot,
 * one queue per CAN port(interface)
 */
struct can_tx_message_queue {
	unsigned head;
	unsigned count;
	struct can_tx_frame data[CAN_TX_DATA_BUFFER_SIZE];
};

static struct can_rx_message_queue can_rx_message_queue;
static struct can_tx_message_queue can_tx_message_queues[CAN_INTERFACE_COUNT];

static uint8_t can_tx_active[CAN_INTERFACE_COUNT];

/** Helper function for can_dequeue_rx_frame() */
static int can_enqueue_rx_frame(const struct can_rx_frame *f)
{
	struct can_rx_message_queue *q = &can_rx_message_queue;

	if (q->count >= CAN_RX_QUEUE_SIZE)
		return 0;

	memcpy(&q->data[(q->head + q->count) % CAN_RX_QUEUE_SIZE], f, sizeof(struct can_rx_frame));
	++q->count;
	return 1;
}

/** Moves head message from rx queue to parameter f, returns 1 for success, 0 for empty queue. Used only by can_engine.*/
int can_dequeue_rx_frame(struct can_rx_frame *f)
{
	struct can_rx_message_queue *q = &can_rx_message_queue;

	if (!q->count)
		return 0;

	--q->count;
	memcpy(f, &q->data[q->head], sizeof(struct can_rx_frame));
	q->head = (q->head + 1) % CAN_RX_QUEUE_SIZE;
	return 1;
}

/** Returns rx queue message count */
unsigned can_rx_get_count()
{
	return can_rx_message_queue.count;
}

/** @} */

/** \addtogroup CanLowTxRFcan_platfx
  * @{
  */

void can_txrx_purge_rx_queue()
{
	memset(&can_rx_message_queue, 0, sizeof(struct can_rx_message_queue));
}

/** can_txrx_purge_tx_queue. Called in can_txrx_init(). Should be called after port HW reset due to errors to delete stale messages */
void can_txrx_purge_tx_queue(can_port_t port)
{
	memset(&can_tx_message_queues[port], 0, sizeof(struct can_tx_message_queue));
}

/** can txrx module init. Should be called at start, calls can_platform_init()*/
void can_txrx_init()
{
	can_port_t i;
	can_platform_init();
	/*TODO: memzero state*/
	can_txrx_purge_rx_queue();
	for (i = 0; i < CAN_INTERFACE_COUNT; ++i) {
		can_txrx_purge_tx_queue(i);
		can_tx_active[i] = CAN_DEFAULT_TX_ACTIVE(i);  //- вкл передатчик после инициализации
	}
}

/**
 * Buffers frame in rx queue to be dispatched via can_engine_process_rx_queue call.
 * can_set_warning_flag(CAN_RX_MODULE, 0) upon overflow
 */
int can_buffer_raw_rx_data(struct can_rx_frame *f)
{
	if (!can_enqueue_rx_frame(f)) {
		can_set_warning_flag(CAN_RX_MODULE, 0);
		return 0;
	}
	return 1;
}

/**
 * Check rx queue for full
 */
int can_buffer_rx_full(void)
{
	return (can_rx_message_queue.count >= CAN_RX_QUEUE_SIZE) ? 1 : 0;
}

//------------------------------------------------------------------------------
//- помещает кадр в очередь порта на передачу. Данные будут переданы при вызове функции can_tx_data()
/** Enqueues frame into port tx queue. Data will be transmitted upon can_tx_data() call. returns 0 for failure (invalid port, overflow, tx inactive, dlc invalid), 1 for success.*/
//------------------------------------------------------------------------------
int can_buffer_tx_data(struct can_tx_frame *f, can_port_t port)
{
	if (port >= CAN_INTERFACE_COUNT) {
	//	can_dbg("%s fail: invalid port %u\n", __func__, port);
		return 0;
	}

	struct can_tx_message_queue *q = &can_tx_message_queues[port];

	if (q->count >= CAN_TX_DATA_BUFFER_SIZE) {
	//	can_dbg("%s fail: queue overflow\n", __func__);
		return 0;
	}

	if (!can_tx_active[port]) {
	//	can_dbg("%s fail: tx inactive\n", __func__);
		return 0;
	}

	if (!f->dlc || (f->dlc > 8)) {
	//	can_dbg("%s fail: Invalid frame dlc %u\n", __func__, f->dlc);
	}

	memcpy(&q->data[(q->head + q->count) % CAN_TX_DATA_BUFFER_SIZE], f, sizeof(struct can_tx_frame));
	++q->count;
	return 1;
}


//------------------------------------------------------------------------------
//- помещает кадр в очередь порта на передачу
//- возвращает 0 - в случае ошибки, 1 - кадр помещён в очередь
//------------------------------------------------------------------------------
int can_buffer_tx_data_ex(struct can_tx_frame *f, can_port_t port, uint8_t *fail_counter)
{
	if (can_buffer_tx_data(f, port)) {
		*fail_counter = 0;
		return 1;
	}

	if (*fail_counter < 255)
		++(*fail_counter);
	return 0;
}


//------------------------------------------------------------------------------
//- Обрабатывает очередь сообщений can_tx для каждого интерфейса. 
//- Останавливается либо при пустой очереди, либо при сбое функции can_set_data_for_tx
/** Processes can tx message queue for every interface. Stop either on empty queue or on failure of can_set_data_for_tx function*/
//------------------------------------------------------------------------------
void can_tx_data()
{
	unsigned i;
	for (i = 0; i < CAN_INTERFACE_COUNT; ++i) {
		struct can_tx_message_queue *q = &can_tx_message_queues[i];
		while (q->count) {
			struct can_tx_frame *f = &q->data[q->head];
			if (!can_set_data_for_tx(i, f)) {
		//		can_dbg("Write in port %u failed\n", i);
				break;
			}
			QUEUE_POP(q->head, q->count, CAN_TX_DATA_BUFFER_SIZE);
		}
	}
}

/** @} */
