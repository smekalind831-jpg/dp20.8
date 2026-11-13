#ifndef __CAN_CONF_H__
#define __CAN_CONF_H__


// Перечень каналов, используемых в приложении
enum can_channel_enumeration {
	CAN_TXDATA_REQUEST_CHANNEL = 0,
	CAN_TRANPORT_PROTOCOL_CHANNEL,
	CAN_J1939_ADDRESS_CLAIM_CHANNEL,
	CAN_J1939_MEMORY_ACCESS_DEVICE_CHANNEL,
//	CAN_J1939_DIAGNOSTIC_MESSAGE_CHANNEL,
//	CAN_MEMORY_ACCESS_TOOL_CHANNEL,
//	CAN_RBUS_CHANNEL,
	CAN_MEMORY_ACCESS_PROCESS_CHANNEL,
	CAN_DATAMES_PROCESS_CHANNEL,
	CAN_MAX_CHANNELS,             //- этот канал просто определяет максимальное число каналов для контроля
};

// номер канала для логгирования
//#define CAN_LOGGER_CHANNEL		CAN_RBUS_CHANNEL

// количество can-интерфейсов
#define CAN_INTERFACE_COUNT		1

// размер буфера для принимаемых пакетов из bsp-can
#define CAN_RX_QUEUE_SIZE		20

// размер буфера для передаваемых пакетов в bsp-can (свой для каждого интерфейса)
#define CAN_TX_DATA_BUFFER_SIZE		20

// размер буфера принятых фреймов, прошедших через фильтры
#define CAN_ENGINE_MESSAGE_BUFFER_SIZE	CAN_RX_QUEUE_SIZE

// размер буфера указателей на CAN_ENGINE_MESSAGE_BUFFER для каждого канала
#define CAN_ENGINE_CHANNEL_BUFFER_SIZE	CAN_ENGINE_MESSAGE_BUFFER_SIZE

// макс. кол-во байт в принимаемом или передаваемом мультипакетном сообщении
#define CAN_TP_BUFFER_SIZE		256//1785

// размер буфера мультипакетных сообщений
#define CAN_RX_BAM_BUFFER_SIZE		40
#define CAN_TX_BAM_BUFFER_SIZE		1

// размер буфера мультипакетных сообщений
#define CAN_RX_CONNECT_BUFFER_SIZE	3
#define CAN_TX_CONNECT_BUFFER_SIZE	3

// ?
#define CAN_RX_DM1_FAULTS_BUFFER_SIZE 	40
#define CAN_MAX_DM1_SESSIONS		1

// максимальное количество PGN для передачи
#define CAN_TX_PARAMETER_GROUP_BUFFER_SIZE	10

// максимальное количество SPN для передачи
#define CAN_TX_PARAMETER_BUFFER_SIZE		51

// кол-во мс в 1 тике get_can_clock
#define CAN_MS_PER_TICK			1

// Максимальное кол-во возможных адресов, которое может быть у can-интерфейса
#define CAN_MAX_SOURCE_ADDRESS_COUNT			3 
// Кол-во возможных адресов на can-шине для интерфейса port
#define CAN_SOURCE_ADDRESS_COUNT(port)			CAN_MAX_SOURCE_ADDRESS_COUNT
// Адрес с номером instance интерфейса port с учетом процедуры подтверждения на шине
#define CAN_SOURCE_ADDRESS(port, instance)		can_j1939_source_address(port, instance)
// Желаемое значение адреса с номером instance на can-шине для интерфейса port
#define CAN_PREFERRED_SOURCE_ADDRESS(port, instance)	myaddresses(port,instance)

#define CAN_ADDRESS_CLAIM_SUPPORTED(port, instance) (!instance)


// состояние can-интерфейса port после инициализации canlib (1 - передатчик вкл., 0 - выключен)
#define CAN_DEFAULT_TX_ACTIVE(port) 			1

#define CAN_ARBITRARY_ADDRESS_CAPABLE(port, instance)	0
#define CAN_INDUSTRY_GROUP(port, instance)		1
#define CAN_VEHICLE_SYSTEM_INSTANCE(port, instance)	0
#define CAN_VEHICLE_SYSTEM(port, instance)		0
#define CAN_FUNCTION(port, instance)			0
#define CAN_FUNCTION_INSTANCE(port, instance)		0
#define CAN_ECU_INSTANCE(port, instance)		0
#define CAN_MANUFACTURER_CODE(port, instance)		0
#define CAN_IDENTITY_NUMBER(port, instance)		((*(uint32_t*)0x1FFFF7E8)&0x1fffff)

#ifndef NULL
#define NULL	0
#endif
#endif /*__CAN_CONF_H__*/
