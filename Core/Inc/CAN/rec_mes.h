#ifndef RECMES_H
#define RECMES_H

#include "types.h"
#include "canengine.h"



typedef struct
{
	uint8_t data[8];
	uint32_t fault_tmr;
}CANMES_struct;

#include "auxio1_mes.h"
#include "auxio2_mes.h"
//#include "auxio5_mes.h"
//#include "auxio6_mes.h"
//#include "auxio7_mes.h"
//#include "bjm_mes.h"
//#include "blocks_mes.h"
//#include "curparam_mes.h"
//#include "dats_mes.h"
//#include "time_mes.h"
//#include "sound_mes.h"
//#include "danfoss_joy_mes.h"

char Rec_canmes(CANMES_struct *pmes, struct can_rx_single_message *smsg, uint8_t address, uint16_t pgn, uint32_t timeout);
char recmes_timeout(CANMES_struct *pmes);
#endif //RECMES