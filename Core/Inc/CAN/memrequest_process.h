#ifndef MEMREQ_PROCESS_H
#define MEMREQ_PROCESS_H

#include "types.h"
#include "canengine.h"
#include "main.h"

typedef struct 
{
	uint8_t can_source_addresses[CAN_INTERFACE_COUNT][CAN_MAX_SOURCE_ADDRESS_COUNT];	//адрес запрашивающего ECU
	uint32_t start_SPN;	//для запросов параметров - первый параметр
	uint16_t numSPNs;	//число запрашиваемых параметров
	uint8_t flmemaccess;	//флаг активной сессии доступа к памяти, запрещает другие доступы
	uint8_t states[CAN_INTERFACE_COUNT][CAN_MAX_SOURCE_ADDRESS_COUNT];
	uint8_t fl_access_enable[CAN_INTERFACE_COUNT][CAN_MAX_SOURCE_ADDRESS_COUNT];
	can_clock_t memory_access_timeout_time[CAN_INTERFACE_COUNT][CAN_MAX_SOURCE_ADDRESS_COUNT];
#define MEMACCESS_INITIAL		0
#define MEMACCESS_WORK	1
#define MEMACCESS_BOOTLOAD_ACCESS		2
#define MEMACCESS_ACCESS_DENIED		3
#define MEMACCESS_WRITE0	4
#define MEMACCESS_READ0	5
#define MEMACCESS_BOOTLOAD1	6
#define MEMACCESS_READ1	7
#define MEMACCESS_READ2	8
#define MEMACCESS_READ3	9
#define MEMACCESS_READ4	10
#define MEMACCESS_READ5	11
#define MEMACCESS_WRITE1	12
#define MEMACCESS_WRITE2	13
#define MEMACCESS_WRITE3	14
#define MEMACCESS_WRITE4	15
#define MEMACCESS_WRITE5	16
#define MEMACCESS_WRITE6	17

}j1939_memaccess_data_struct;

void j1939_memaccess_init(void);
void memaccess_process(void);

//extern uint8_t membuffer[userdatalen];

#endif