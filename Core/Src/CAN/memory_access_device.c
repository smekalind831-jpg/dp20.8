#include "CAN/memory_access_device.h"
#include "CAN/memory_access_common.h"

#include <string.h>

#include "CAN/idlist.h"
#include "CAN/warning.h"
#include "CAN/j1939_address_claim.h"
#include "CAN/txrx.h"
#include "CAN/canconf.h"

#define MEMORY_ACCESS_DEVICE_MAX_READ_SIZE 7
#define MEMORY_ACCESS_DEVICE_MAX_WRITE_SIZE 7

//- канал обработки запросов доступа к памяти
static const struct can_rx_slot can_j1939_memaccess_rx_slots[] = {
	{ .match_mask = CAN_MATCH_PDUF_MASK, .match_id = 0xd90000, .port = CAN_MATCH_ANY_PORT }, //request memory access
	{ .match_mask = CAN_MATCH_PDUF_MASK, .match_id = 0xd70000, .port = CAN_MATCH_ANY_PORT }, //binary data
};

static MAKE_IDLIST_FROM_RX_SLOT_ARRAY(can_j1939_memaccess_rx_id_list, can_j1939_memaccess_rx_slots);

struct memory_access_device_data {
	uint8_t mat_source_address;
	can_port_t current_port;
	can_instance_t current_instance;
#define MEMACCESS_INITIAL					0
#define MEMACCESS_BOOTLOAD_ACCESS				2
#define MEMACCESS_ACCESS_DENIED					3
#define MEMACCESS_WRITE_SENDING_MAS_PROCEED			4
#define MEMACCESS_READ_SENDING_MAS_PROCEED			5
#define MEMACCESS_BOOTLOAD_WAITING_FOR_MAC_OPERATION_COMPLETED	6
#define MEMACCESS_READ_SENDING_DATA				7
#define MEMACCESS_READ_SENDING_MAS_OPERATION_COMPLETED		8
#define MEMACCESS_READ_HOLDING_SESSION_BEFORE_CLOSE		9
#define MEMACCESS_WRITE_WAITING_BINARY_DATA			12
#define MEMACCESS_WRITE_SENDING_MAS_OPERATION_COMPLETED		13
#define MEMACCESS_WRITE_HOLDING_SESSION_BEFORE_CLOSE		14
	uint8_t state;
	can_clock_t memory_access_timeout_time;
	uint16_t number_requested;
	uint16_t bytes_requested;//equals number_requested incase of pointer_type == 0, calculated if == 1
	uint32_t request_pointer;
	uint8_t request_pointer_type;
	uint8_t buffer_failures;
	uint8_t tmp_write_data[MEMORY_ACCESS_DEVICE_MAX_WRITE_SIZE];
};

static struct memory_access_device_data mdata;

static void memory_access_device_set_timeout_ms(unsigned ms)
{
	can_set_timeout(mdata.memory_access_timeout_time, ms);
}

/**
 * \brief Module init function. Call at start.
 */
//------------------------------------------------------------------------------
//- инициализация канала получения данных для записи во флеш-мамять
//- установка канала в неактивное состояние MEMACCESS_INITIAL
//------------------------------------------------------------------------------
void j1939_memory_access_device_init()
{
	memset(&mdata, 0, sizeof(mdata));
	mdata.state = MEMACCESS_INITIAL;

	can_load_rx_id_list(CAN_J1939_MEMORY_ACCESS_DEVICE_CHANNEL, &can_j1939_memaccess_rx_id_list); //- присваивает список каналу
}


//------------------------------------------------------------------------------
//- устанавливает новое состояние канала
//------------------------------------------------------------------------------
static void memory_access_device_set_state(uint8_t state)
{
	mdata.state = state;
}


//------------------------------------------------------------------------------
//- проверяет, находится ли канал в активном состоянии
//------------------------------------------------------------------------------
static inline int mad_is_active() { return mdata.state != MEMACCESS_INITIAL; }


//------------------------------------------------------------------------------
//- заполняет структуру с кадром на передачу и помещает кадр в очередь порта на передачу
//------------------------------------------------------------------------------
static int send_memory_access_response(can_port_t port, can_instance_t instance, uint8_t da, uint16_t length, uint8_t status, uint32_t pointer, uint16_t seed)
{
	struct can_tx_frame f = {
		.id = can_compose_eid(6, 0xd8, da, CAN_SOURCE_ADDRESS(port, instance)),
		.dlc = 8,
		.data = {
			length & 0xff,
			((length >> 3) & 0xe0) | 0x11 | ((status & 0x07) << 1),
			0xff,/*error indicator / edcp unavailable*/
			0xff,
			0xff,
			0xff,/*edcp extension unavailable*/
			seed & 0xff,
			(seed >> 8) & 0xff
		}
	};
	return can_buffer_tx_data_ex(&f, mdata.current_port, &mdata.buffer_failures);
}

//------------------------------------------------------------------------------
static int send_memory_access_response_current(uint16_t length, uint8_t status, uint32_t pointer, uint16_t seed)
{
	return send_memory_access_response(mdata.current_port, mdata.current_instance, mdata.mat_source_address, length, status, pointer, seed);
}

static int send_ok_simple_memory_access_response(uint8_t status)
{
	return send_memory_access_response_current(0, status, 0xffffffff, 0xffff);
}


//------------------------------------------------------------------------------
//- первый ответ на запрос: чтение из памяти микроконтроллера
//-   продолжить (Proceed) или ошибка (MAE_INVALID_VALUE)
//------------------------------------------------------------------------------
static void process_mac_read(struct can_rx_single_message *smsg)
{
	unsigned number_requested = smsg->frame.data[0] | ((smsg->frame.data[1] & 0xe0) << 3);  //- длина входящего в микроконтроллер сообщения
	unsigned bytes_requested = 0;                                                           //- длина ответа микроконтроллера
	uint32_t pointer = smsg->frame.data[2]                                                  //- принятые данные (4 байта):
		| (smsg->frame.data[3] << 8)                                                          //-   SPN настроечного параметра
		| (smsg->frame.data[4] << 16)
		| (smsg->frame.data[5] << 24);
	uint8_t pointer_type = smsg->frame.data[1] & 0x10 ? 1 : 0;                      //- бит 4 байта[1] - 1: запрос настроечного параметра, 0:
	//uint16_t key = smsg->frame.data[6] | (smsg->frame.data[7] << 8);              //- ключ, здесь не нужен

	if (pointer_type) {
		unsigned i;                                                                   //- чтение значения настроечного параметра
		for (i = pointer; i < pointer + number_requested; ++i) {
			uint8_t stub[8];                                                            //- сюда помещается значение настроечного параметра
			int b = j1939_memory_access_device_read_get_parameter_hook(i, stub);
			if (b <= 0) {
				bytes_requested = 0;
				break;
			} else {
				bytes_requested += b;
			}
		}
	} else {                                                                        //- чтение содержимого памяти микроконтроллера (hook в реализации отключен, не читаем)
		bytes_requested = j1939_memory_access_device_read_get_memory_hook(pointer, number_requested) ? number_requested : 0;
	}

	if (!bytes_requested || (bytes_requested > MEMORY_ACCESS_DEVICE_MAX_READ_SIZE)) {
		send_memory_access_response_current(number_requested, MAS_OPERATION_FAILED, MAE_INVALID_VALUE, 0xffff);
		memory_access_device_set_state(MEMACCESS_INITIAL);
	} else {
		mdata.number_requested = number_requested;
		mdata.bytes_requested = bytes_requested;
		mdata.request_pointer = pointer;
		mdata.request_pointer_type = pointer_type;
		memory_access_device_set_state(MEMACCESS_READ_SENDING_MAS_PROCEED);           //- ответ микроконтроллера: продолжить
		memory_access_device_set_timeout_ms(-1);
	}
}

static void process_mac_write(struct can_rx_single_message *smsg)
{
 // char A;
	unsigned number_requested = smsg->frame.data[0] | ((smsg->frame.data[1] & 0xe0) << 3);//\в 0 и 1-м байте размер запроса 8 младших и 3 старших бита
	unsigned bytes_requested = 0;
	uint32_t pointer = smsg->frame.data[2]             //\ SPN настраеваемого параметра
		| (smsg->frame.data[3] << 8)
		| (smsg->frame.data[4] << 16)
		| (smsg->frame.data[5] << 24);
	uint8_t pointer_type = smsg->frame.data[1] & 0x10 ? 1 : 0; //\ 1- адресация объекта 0 - по байтно
	//uint16_t key = smsg->frame.data[6] | (smsg->frame.data[7] << 8);
     //   if(pointer == 0x80001164)
    //   A = 56;
        
	if (pointer_type) {
		unsigned i;
		for (i = pointer; i < pointer + number_requested; ++i) {
			int b = j1939_memory_access_device_write_get_parameter_size_hook(i);  //\ находит нужный объект и возвращает размер
			if (b <= 0) {
				bytes_requested = 0;
				break;
			} else {
				bytes_requested += b;
			}
		}
	} else {
		bytes_requested = j1939_memory_access_device_write_is_memory_writable_hook(pointer, number_requested) ? number_requested : 0;
	}
	if (!bytes_requested || (bytes_requested > MEMORY_ACCESS_DEVICE_MAX_WRITE_SIZE)) {
		send_memory_access_response_current(number_requested, MAS_OPERATION_FAILED, MAE_INVALID_VALUE, 0xffff);
		memory_access_device_set_state(MEMACCESS_INITIAL);
	} else {
		mdata.number_requested = number_requested;     //\ число запрошенных объектов
		mdata.bytes_requested = bytes_requested;       //\ сколько байт ? или объектов
		mdata.request_pointer = pointer;               //\ откуда
		mdata.request_pointer_type = pointer_type;     //\ 0 - побайтно физ. память 1 - объект
		memory_access_device_set_state(MEMACCESS_WRITE_SENDING_MAS_PROCEED);
		memory_access_device_set_timeout_ms(-1);       //\ для отсрочки момента отправки угла
	}
}

static void complete_memwrite_request()
{
	if (mdata.request_pointer_type) {
		j1939_memory_access_device_parameter_write_start_hook(mdata.request_pointer, mdata.number_requested);

		unsigned dataoffset = 0;
		uint32_t i;
		for (i = mdata.request_pointer; i < mdata.request_pointer + mdata.number_requested; ++i) {
			int b = j1939_memory_access_device_parameter_write_hook(i, mdata.tmp_write_data + dataoffset);
			if ((b <= 0) || (dataoffset + b) > sizeof(mdata.tmp_write_data)) {
				//shouldn't happen
				break;
			}
			dataoffset += b;
		}

		j1939_memory_access_device_parameter_write_complete_hook(mdata.request_pointer, mdata.number_requested);
	} else {
		j1939_memory_access_device_raw_write_start_hook(mdata.request_pointer, mdata.number_requested);

		uint8_t *ptr = j1939_memory_access_device_write_get_memory_hook(mdata.request_pointer, mdata.number_requested);
		if (ptr)
			memcpy(ptr, mdata.tmp_write_data, mdata.number_requested);

		j1939_memory_access_device_raw_write_complete_hook(mdata.request_pointer, mdata.number_requested);
	}
}


//------------------------------------------------------------------------------
static void process_memrequest_message(struct can_rx_single_message *smsg)
{
	if (smsg->frame.dlc < 8) {
		memory_access_device_set_state(MEMACCESS_INITIAL);
		return;
	}
	unsigned number_requested = smsg->frame.data[0] | ((smsg->frame.data[1] & 0xe0) << 3);
	unsigned command = (smsg->frame.data[1] >> 1) & 0x07;
	switch (mdata.state) {
	case MEMACCESS_INITIAL:
		switch (command) {
		case MAC_READ:
			process_mac_read(smsg);
			break;
		case MAC_WRITE:
			process_mac_write(smsg);
			break;
		case MAC_BOOTLOAD:
			memory_access_device_set_state(MEMACCESS_BOOTLOAD_ACCESS);
			memory_access_device_set_timeout_ms(-1);
			break;
		//case MAC_STATUS_REQUEST://status request
		//case MAC_ERASE://erase
		default:
			//reply nack
			send_memory_access_response_current(number_requested, MAS_OPERATION_FAILED, MAE_NOT_SUPPORTED_COMMAND, 0xffff);
		}
		break;
	case MEMACCESS_BOOTLOAD_WAITING_FOR_MAC_OPERATION_COMPLETED:
		switch (command) {
		case MAC_OPERATION_COMPLETED:
			j1939_memory_access_device_process_bootload_request_hook(mdata.current_port, mdata.current_instance, can_smsg_sa(smsg));
			//should not return, but do it anyway
			memory_access_device_set_state(MEMACCESS_INITIAL);
			break;
		default: break;
		}
		break;
	case MEMACCESS_WRITE_HOLDING_SESSION_BEFORE_CLOSE:
		 complete_memwrite_request();
		 //fall-through
	case MEMACCESS_READ_HOLDING_SESSION_BEFORE_CLOSE:
		//waiting for another request after completing current request
		//this allows chain requests without session hijack
		switch (command) {
		case MAC_OPERATION_COMPLETED:
			memory_access_device_set_state(MEMACCESS_INITIAL);
			break;
		case MAC_READ:
			process_mac_read(smsg);
			break;
		case MAC_WRITE:
			process_mac_write(smsg);
			break;
		default:
			send_memory_access_response_current(number_requested, MAS_OPERATION_FAILED, MAE_NOT_SUPPORTED_COMMAND, 0xffff);
			memory_access_device_set_state(MEMACCESS_INITIAL);
			break;
		}
		break;
	default:
		//mad or mat implementation error
		memory_access_device_set_state(MEMACCESS_INITIAL);
		break;
	}
}

static void process_binary_data_message(struct can_rx_single_message *smsg)
{
	switch (mdata.state) {
	case MEMACCESS_WRITE_WAITING_BINARY_DATA:
		//check dlc etc
		memcpy(mdata.tmp_write_data, smsg->frame.data + 1, 7);/*0 byte length*/
		memory_access_device_set_state(MEMACCESS_WRITE_SENDING_MAS_OPERATION_COMPLETED);
		memory_access_device_set_timeout_ms(-1);
		break;
	}
}

static void process_message(struct can_rx_message_header *header)
{
	if (header->multi_packet)
		return;
	struct can_rx_single_message *smsg = can_to_rx_smsg(header);
	can_port_t port = smsg->frame.interface;
	can_instance_t instance = can_find_instance(port, can_smsg_da(smsg));

	if (instance == CAN_INVALID_INSTANCE) //message for other device
		return;

	if (mad_is_active()
	&& ((can_smsg_sa(smsg) != mdata.mat_source_address)
	|| (mdata.current_port != port)
	|| (mdata.current_instance != instance))) { //active session with other address
		//send busy incase of memrequest and none incase of binary data
		//or schedule sending cause it maybe not last message?
		if (can_smsg_pduf(smsg) == 0xd9) {
			send_memory_access_response(port, instance, can_smsg_sa(smsg), 0, MAS_BUSY, 0xffffffff, 0xffff);
		}
		return;
	}
	mdata.mat_source_address = can_smsg_sa(smsg);
	mdata.current_port = port;
	mdata.current_instance = instance;

	switch (smsg->frame.id & CAN_MATCH_PDUF_MASK) {
	case 0xd90000: //memrequest
		process_memrequest_message(smsg);
		break;
	case 0xd70000: //binary data
		process_binary_data_message(smsg);
		break;
	default:
		break;
	}
}

//------------------------------------------------------------------------------
//- отправка двоичных данных при состоянии канала: MEMACCESS_READ_SENDING_DATA
//------------------------------------------------------------------------------
static int send_read_binary_data()
{
	struct can_tx_frame f = {
		.id = can_compose_eid(6, 0xd7, mdata.mat_source_address, CAN_SOURCE_ADDRESS(mdata.current_port, mdata.current_instance)),
		.dlc = 8,
	};
	if (mdata.request_pointer_type) {
		//implement get parameters - отправляет параметры
		unsigned datasize = 0;
		uint32_t i;
		for (i = mdata.request_pointer; i < mdata.request_pointer + mdata.number_requested; ++i) {
			int b = j1939_memory_access_device_read_get_parameter_hook(i, f.data + 1 + datasize);
			if ((b <= 0) || (datasize + b) > 8) {
				//shouldn't happen
				memory_access_device_set_state(MEMACCESS_INITIAL);
				return 0;
			}
			datasize += b;
		}
		f.data[0] = datasize; //Number of Occurrences of Raw Binary Data
	} else {
    //- отправляет считанные из памяти данные (не реализовано)
		const uint8_t *ptr = j1939_memory_access_device_read_get_memory_hook(mdata.request_pointer, mdata.number_requested);
		if (mdata.number_requested > MEMORY_ACCESS_DEVICE_MAX_READ_SIZE) { //shouldn't happen
			memory_access_device_set_state(MEMACCESS_INITIAL);
			return 0;
		}
		memcpy(f.data, ptr, mdata.number_requested);
		memset(f.data + mdata.number_requested, 0xff, 8 - mdata.number_requested);
	}
	
  return can_buffer_tx_data_ex(&f, mdata.current_port, &mdata.buffer_failures);
}


//------------------------------------------------------------------------------
#define mad_try_retransmit_later() memory_access_device_set_timeout_ms(10)


//------------------------------------------------------------------------------
//- периодически запускаемый процесс
//------------------------------------------------------------------------------
static void process_timeout()
{
	int ok;
	switch (mdata.state) {
	case MEMACCESS_BOOTLOAD_ACCESS:
		ok = send_ok_simple_memory_access_response(MAS_PROCEED);
		ok &= send_ok_simple_memory_access_response(MAS_OPERATION_COMPLETED);
		if (ok) {
			memory_access_device_set_state(MEMACCESS_BOOTLOAD_WAITING_FOR_MAC_OPERATION_COMPLETED);
			memory_access_device_set_timeout_ms(250);
		} else {
			mad_try_retransmit_later();
		}
		break;
	case MEMACCESS_BOOTLOAD_WAITING_FOR_MAC_OPERATION_COMPLETED:
		mdata.state = MEMACCESS_INITIAL;
		break;
	case MEMACCESS_READ_SENDING_MAS_PROCEED:      //- ответ: продолжить отправку прочитанных в микроконтроллере данных
		ok = send_memory_access_response_current(mdata.number_requested, MAS_PROCEED, mdata.request_pointer, 0xffff);
		if (!ok) {
			mad_try_retransmit_later();
			break;
		}
		memory_access_device_set_state(MEMACCESS_READ_SENDING_DATA);    //- на следующем цикле отправить сами данные
		//fall-through
	case MEMACCESS_READ_SENDING_DATA: { //prepare and send data
		if (!send_read_binary_data()) {
			mad_try_retransmit_later();
			break;
		}
		memory_access_device_set_state(MEMACCESS_READ_SENDING_MAS_OPERATION_COMPLETED);
		//fall-through
	}
	case MEMACCESS_READ_SENDING_MAS_OPERATION_COMPLETED: //send op complete and wait 250ms
		ok = send_memory_access_response_current(mdata.number_requested, MAS_OPERATION_COMPLETED, mdata.request_pointer, 0xffff);
		if (!ok) {
			mad_try_retransmit_later();
			break;
		}
		memory_access_device_set_state(MEMACCESS_READ_HOLDING_SESSION_BEFORE_CLOSE);
		memory_access_device_set_timeout_ms(250);
		break;
	case MEMACCESS_READ_HOLDING_SESSION_BEFORE_CLOSE: //close session
		memory_access_device_set_state(MEMACCESS_INITIAL);
		break;
	case MEMACCESS_WRITE_SENDING_MAS_OPERATION_COMPLETED: //send complete
		ok = send_memory_access_response_current(mdata.number_requested, MAS_OPERATION_COMPLETED, mdata.request_pointer, 0xffff);
		if (!ok) {
			mad_try_retransmit_later();
			break;
		}
		memory_access_device_set_state(MEMACCESS_WRITE_HOLDING_SESSION_BEFORE_CLOSE);
		memory_access_device_set_timeout_ms(250);
		break;
	case MEMACCESS_WRITE_HOLDING_SESSION_BEFORE_CLOSE:
		complete_memwrite_request();
		memory_access_device_set_state(MEMACCESS_INITIAL);
		break;
	case MEMACCESS_WRITE_SENDING_MAS_PROCEED:
		ok = send_memory_access_response_current(mdata.number_requested, MAS_PROCEED, mdata.request_pointer, 0xffff);
		if (!ok) {
			mad_try_retransmit_later();
			break;
		}
		memory_access_device_set_state(MEMACCESS_WRITE_WAITING_BINARY_DATA);
		memory_access_device_set_timeout_ms(250);
		break;
	case MEMACCESS_WRITE_WAITING_BINARY_DATA:
		memory_access_device_set_state(MEMACCESS_INITIAL);
		break;
	default:
		//should not happen
		break;
	}
}


//------------------------------------------------------------------------------
//- отсюда запускается процесс обработки канала доступа к памяти оконечного устройства (датчика)
//- вызывается периодически в основном цикле
//------------------------------------------------------------------------------
void j1939_memory_access_device_process()
{
	struct can_rx_message_header *header;
	for (;;) {
		header = can_engine_get_channel_message(CAN_J1939_MEMORY_ACCESS_DEVICE_CHANNEL);
		if (!header)
			break;
		process_message(header);
		can_engine_release_channel_message(CAN_J1939_MEMORY_ACCESS_DEVICE_CHANNEL);
		if (mad_is_active() && can_timed_out(mdata.memory_access_timeout_time)) {
			//process timeout
			process_timeout();
		}
	}
	if (mad_is_active() && can_timed_out(mdata.memory_access_timeout_time)) {
		//process timeout
		process_timeout();
	}
}
