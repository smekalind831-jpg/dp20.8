#ifndef __BSP_ADT7310__
#define __BSP_ADT7310__

#include "main.h"
#include "board.h"
#include "GlobalVar.h"

//- адреса регистров ADT7310
#define REG_STATUS  0x00    //- регистр статуса
#define REG_ID      0x03    //- идентификатор

uint32_t ADT7310_init(void);
uint32_t ADT7310_ReadReg(uint8_t, uint8_t*);
uint32_t ADT7310_ReadReg16(uint8_t, uint16_t*);
uint32_t ADT7310_WriteReg(uint8_t, uint8_t);
uint32_t ADT7310_ReadTemp(void);




#endif    //- __BSP_ADT7310__