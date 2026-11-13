#ifndef CAN_PROCESS_TSK_H
#define CAN_PROCESS_TSK_H

#include <string.h>
#include <stdint.h>
#include <math.h>

#include "memrequest_process.h"
#include "datames_process.h"
#include "j1939_address_claim.h"
#include "main.h"
#include "txdata.h"
//#include "bsp_ADT7310.h"
#include "GlobalVar.h"
#include "bma456.h"


typedef struct
{
	uint16_t minperiod;
	uint16_t period;
	uint8_t size;
	uint8_t priority;
	uint16_t tmr;
	uint8_t *Data;
	uint16_t PGN;
	uint8_t status;
}  CANmes_struct;


void CAN_init_tx(void);
extern void CAN_process_tsk(void);

#endif
