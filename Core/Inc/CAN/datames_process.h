#ifndef DATAMES_PROCESS_H
#define DATAMES_PROCESS_H

#include "types.h"
#include "canengine.h"
#include "idlist.h"
#include "warning.h"
#include "j1939_address_claim.h"
#include "txrx.h"
#include "rec_mes.h"
#include "auxio2_mes.h"
#include "main.h"

//extern uint32_t aux1_temp;    //- временная, удалить

extern void j1939_datames_init(void);
void datames_process(void);

#endif
