/*
 *******************************************************************************
 * file    bsp_can.h
 * author  Rezonans
 * version v1.0.0
 * date    2013
 * brief   Драйвер CAN, apm32f10x
 *******************************************************************************
 */
  
#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"	
//#include "apm32f10x.h"

#define BSP_CAN_RX_BUF_SIZE	16
#define BSP_CAN_TX_BUF_SIZE	16

#define BSP_CAN_ERR_FLAG	0x80	    //- признак ошибочного пакета
#define BSP_CAN_OVF_FLAG	0x40	    //- признак переполнения bsp-буфера


#define CAN1_port		GPIOA
#define CAN1TX_pin		GPIO_PIN_12
#define CAN1RX_pin		GPIO_PIN_11
#define CAN1can_port	        GPIOA
#define CAN1can_pin		GPIO_PIN_1      
	 	   
  
typedef struct
{
	uint32_t id;		// идентификатор пакета

	uint8_t ide : 1;	// признак расширенного идентификатора
	uint8_t rtr : 1;	// признак пакета-запроса
	uint8_t err : 1;	// признак ошибочного пакета
	uint8_t ovf : 1;	// признак переполнения bsp-буфера
	uint8_t dlc : 4;	// кол-во байт данных в пакете

	uint8_t data[8];	// данные пакета
		
} Tbsp_can_msg;


void CANFilterInit(void);
void filterset(uint32_t EID,uint32_t mask,uint8_t fnumber);
void bsp_can_init(void);
uint8_t bsp_can_read(Tbsp_can_msg *buf);
uint8_t bsp_can_write(Tbsp_can_msg *buf);
extern void CAN_bsp_Config(uint8_t can_artdis);
void rbus_to_can(char len, char *buf);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_CAN_H__ */

/*********************************END OF FILE**********************************/
