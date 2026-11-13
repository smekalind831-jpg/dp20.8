/*
 *******************************************************************************
 * file    bsp_can.c
 * author  Rezonans
 * version v1.0.0
 * date    2013
 * brief   Драйвер CAN
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>
#include "CAN/bsp_can.h"
#include "CAN/board.h"
#include "prg_struct.h"
#include "CAN/txrx.h"
#include "CAN/apm32f10x_can.h"
#include "CAN/misc.h"
#include "CAN/j1939_address_claim.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BSP_CAN_ERR_FLAG	0x80	// признак ошибочного пакета
#define BSP_CAN_OVF_FLAG	0x40	// признак переполнения bsp-буфера


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//CanTxMsg TxMessage;
static CanRxMsg rx_msg[BSP_CAN_RX_BUF_SIZE];
static CanRxMsg *rx_start, *rx_end;
static CanTxMsg tx_msg[BSP_CAN_TX_BUF_SIZE];
static CanTxMsg *tx_start, *tx_end;
static uint8_t ovf_flag;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

extern ecu_id Ecuinfo;



//******************************************************************************
//
// bsp_can_init
//
// Инициализация модуля
//
//******************************************************************************
void bsp_can_init(void)
{
	rx_start = rx_end = rx_msg;   //- адреса буфера приёма
	tx_start = tx_end = tx_msg;   //- адреса буфера передачи
	ovf_flag = 0;

	/* CAN configuration */
	CAN_bsp_Config(ENABLE);   //- с запретом автоматической ретрансляции
	start_fl = 0;
}







//******************************************************************************
//
// CAN_Config
//
// Configures the CAN
//
//******************************************************************************
void CAN_bsp_Config(uint8_t can_artdis)
{
CAN_Config_T CANInit;
	GPIO_ConfigStruct_T pinout;

	//- настройка портов CAN
	pinout.pin = CAN1TX_pin;            //- CANTX: port A, pin 12
	pinout.speed = GPIO_SPEED_10MHz;    //- alternative, out, push-pull, 10 MHz
	pinout.mode = GPIO_MODE_AF_PP;
	GPIO_Config(CAN1_port, &pinout);

	pinout.pin = CAN1RX_pin;            //- CANRX: port A, pin 11
	pinout.speed = GPIO_SPEED_10MHz;    //- in, pull-up, 10 MHz
	pinout.mode = GPIO_MODE_IN_PU;
	GPIO_Config(CAN1_port, &pinout);
	
	pinout.pin = CAN1can_pin;           //- CAN enable: port A, pin 4
	pinout.speed = GPIO_SPEED_10MHz;    //- out, push-pull, 10 MHz
	pinout.mode = GPIO_MODE_OUT_PP;
	GPIO_Config(CAN1can_port, &pinout);
  GPIO_ResetBits(CAN1can_port, CAN1can_pin);    //- Standby disable, CAN - включить
	
  //- настройка контроллера CAN
	RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_CAN1);
	CAN_Reset(CAN1);
	CANInit.CAN_PRESCALER = 9;            //- предделитель - 1 = делитель на 10
	CANInit.CAN_MODE = CAN_MODE_NORMAL;   //- обычный режим
	CANInit.CAN_RSJW = CAN_RSJW_4tq;      //- кол-во изменяемых квантов для синхронизации
	CANInit.CAN_BS1 = CAN_BS1_10tq;       //- кол-во квантов 1 сегмента - 1 = 11 квантов
	CANInit.CAN_BS2 = CAN_BS2_5tq;        //- кол-во квантов 2 сегмента - 1 = 6 квантов
	CANInit.CAN_TTCM = DISABLE;           //- выкл режим связи по таймеру
	CANInit.CAN_ABOM = DISABLE;           //- выкл автовыход из Bus-off
	CANInit.CAN_AWUPM = DISABLE;          //- выкл авто wakeup
	CANInit.CAN_ARTDIS = can_artdis;		  //- однократные посылки, без автоматической ретрансляции
	CANInit.CAN_RFLOCK = DISABLE;         //- запрет режима Loop back
	CANInit.CAN_TX_PS = ENABLE;           //- приоритет при передаче: в хронологическом порядке
	CAN_Config(CAN1, &CANInit);
	
	CANFilterInit();    //- настройка фильтров CAN

	//настройка контроллера прерываний	
	//настройка прерывания по CAN
	CAN_EnableInterrupt(CAN1, CAN_INT_FMNP1);   //- FIFO0 - есть принятые сообщения
	CAN_EnableInterrupt(CAN1, CAN_INT_FMNP2);   //- FIFO1 - есть принятые сообщения
	CAN_EnableInterrupt(CAN1, CAN_INT_FOFI1);   //- FIFO0 - при переполнении
	CAN_EnableInterrupt(CAN1, CAN_INT_FOFI2);   //- FIFO1 - при переполнении
	CAN_EnableInterrupt(CAN1, CAN_INT_EPVI);    //- по Error Passive (ERRPFLG=1)
	CAN_EnableInterrupt(CAN1, CAN_INT_BOFI);    //- по Bus-Off (BOFFFLG=1)
	CAN_EnableInterrupt(CAN1, CAN_INT_ERRI);    //- разрешить все объявленные прерывания
	
	NVIC_EnableIRQRequest(CAN1_RX1_IRQn, 0, 2);         //- разрешить прерывание при приёме CAN1_RX1_IRQn=21
	NVIC_EnableIRQRequest(USB_LP_CAN1_RX0_IRQn, 0, 2);  //- разрешить прерывание при приёме CAN1_RX0_IRQn=20
}
//фильтры сообщений 
void CANFilterInit(void)
{
	//настройка фильтров CAN
	
	//настройка фильтра 1 
	//транспортный протокол (команда) - 1 фильтр
	filterset(0x00ec0000|((Ecuinfo.id_CAN.address)<<8),0x00ff0000,0);
	
	//настройка фильтра 2 
	//идентификатор запроса  - 2 фильтр
	filterset(0x18ea0000|((Ecuinfo.id_CAN.address)<<8),0x00ff0000,1);
	
	//настройка фильтра 3 
	//идентификатор address claimed - 3 фильтр
	filterset(0x00eeff00|(Ecuinfo.id_CAN.address),0x00ffffff,2);
	
	//настройка фильтра 4 
	//идентификатор memory access - 4 фильтр
	filterset(0x00d90000|((Ecuinfo.id_CAN.address)<<8),0x00ffff00,3);
		
	//настройка фильтра 5 
	//идентификатор binary data - 5 фильтр
	filterset(0x00d70000|((Ecuinfo.id_CAN.address)<<8),0x00ffff00,4);
		
	//настройка фильтра 6 
	//транспортный протокол (данные) - 6 фильтр
	filterset(0x00eb0000|((Ecuinfo.id_CAN.address)<<8),0x00ff0000,5);
		
	//настройка фильтра 7 
	//сообщения двигателя - 7 фильтр
	filterset(0x00feee00,0x00fffe00,6);
		
	//настройка фильтра 8 
	//сообщение electronic engine controller - 8 фильтр
	filterset(0x00f00400,0x00ffff00,7);
	
	//настройка фильтра 9 
	//сообщение DM1 - 9 фильтр
	filterset(0x00feca00,0x00ffff00,8);
		
	//настройка фильтра 10 
	//сообщение DM1 - 10 фильтр
	filterset(0x00fef500,0x00ffff00,9);
	
	//настройка фильтра 11 
	//сообщение AUXIO2 - 11 фильтр
	filterset(0x00a70000,0x00ff0000,10);
	
}

void filterset(uint32_t EID,uint32_t mask,uint8_t fnumber)
{
	CAN_FilterInitTypeDef CANfilter;
	CANfilter.CAN_FilterIdHigh=EID>>13;
	CANfilter.CAN_FilterIdLow=((EID<<3)&0xffff)|4;
	CANfilter.CAN_FilterMaskIdHigh=mask>>13;
	CANfilter.CAN_FilterMaskIdLow=((mask<<3)&0xffff)|4;
	CANfilter.CAN_FilterFIFOAssignment=0;
	CANfilter.CAN_FilterNumber=fnumber;
	CANfilter.CAN_FilterMode=CAN_FilterMode_IdMask;
	CANfilter.CAN_FilterScale=CAN_FilterScale_32bit;
	CANfilter.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CANfilter);
}


//******************************************************************************
//
// bsp_can_read
//
// Чтение одного пакета из входного кольцевого буфера
//
//******************************************************************************
uint8_t bsp_can_read(Tbsp_can_msg *buf)
{
	uint8_t ret = 0;
	
	// Проверка на наличие пакетов во входном кольцевом буфере
	if (rx_start != rx_end) {

		// преобразование формата принятого пакета
		buf->ide = (rx_start->IDE == CAN_Id_Extended);
		buf->id  = (buf->ide) ? rx_start->ExtId : rx_start->StdId;
		buf->rtr = (rx_start->RTR == CAN_RTR_REMOTE);
		buf->dlc = (rx_start->DLC & 0x0F);
		buf->err = ((rx_start->DLC & BSP_CAN_ERR_FLAG) != 0);
		buf->ovf = ovf_flag;
		memcpy(buf->data, rx_start->Data, sizeof(buf->data));
		
		// перемещаем указатель кольцевого буфера вперед
		if (rx_start < &rx_msg[BSP_CAN_RX_BUF_SIZE - 1])
			rx_start++;
		else rx_start = rx_msg;
		
		// чтение из приемного буфера прошло успешно
		ret = 1;
		
		if (ovf_flag != 0) {
			rx_start = rx_end;
			ovf_flag = 0;
		}
	}
	
	return ret;
}


//******************************************************************************
//
// bsp_can_buf2trn
//
// Вывод пакетов из кольцевого буфера в аппаратный при наличии там
//  свободных ячеек. (используется в bsp_can_write и прерывании)
//
//******************************************************************************
static void bsp_can_buf2trn(void)
{
	// Пока есть свободные ячейки в аппаратном выходном буфере
	//  и кольцевой буфер не пуст
	while ((CAN1->TSR & CAN_TSR_TME) && tx_start != tx_end) {
		
		// помещаем пакет в аппаратный буфер
		if (CAN_Transmit(CAN1, tx_start) != CAN_TxStatus_NoMailBox) {
		
        		// перемещаем указатель кольцевого буфера вперед
			if (tx_start < &tx_msg[BSP_CAN_TX_BUF_SIZE - 1])
				tx_start++;
			else tx_start = tx_msg;
		}
	}
}

//******************************************************************************
//
// bsp_can_write
//
// Запись одного пакета в выходной кольцевой буфер. Вывод пакетов из кольцевого
// буфера в аппаратный при наличии там свободных ячеек.
//
//******************************************************************************
uint8_t bsp_can_write(Tbsp_can_msg *buf)
{
	uint8_t ret = 0;
	CanTxMsg *new_tx_end = tx_end;
	
	// Узнаем новое положение конца буфера
	if (new_tx_end < &tx_msg[BSP_CAN_TX_BUF_SIZE - 1])
		new_tx_end++;
	else new_tx_end = tx_msg;
		
	// Проверка на наличие места в буфере
	if (new_tx_end != tx_start) {

		// преобразование формата пакета
		if (buf->ide) {
			tx_end->IDE   = CAN_ID_EXT;
			tx_end->ExtId = buf->id;
		} else {
			tx_end->IDE   = CAN_ID_STD;
			tx_end->StdId = buf->id;
		}
		
		tx_end->RTR = buf->rtr ? CAN_RTR_REMOTE : CAN_RTR_DATA;
		tx_end->DLC = buf->dlc;
		memcpy(tx_end->Data, buf->data, sizeof(tx_end->Data));

		// Помещение пакета в выходной кольцевой буфер
		tx_end = new_tx_end;

		// т.к. bsp_can_buf2trn используется также в прерывании, то чтобы исключить совместный доступ
		// к данным, запрещаем прерывание.
		CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);
		bsp_can_buf2trn();

		// В случае, если кольцевой буфер не пуст, то его остаток будет передан через прерывание по опустошению
		// ячеек аппаратного буфера.
		if (tx_start != tx_end) {
			CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
			CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
			
		}
		ret = 1;
	}else 
	{
		__disable_irq();
		bsp_can_buf2trn();
		__enable_irq();
	}
	
	return ret;
}

/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
	uint8_t err_state;
	if(rx_end==NULL) return;
	// получаем код последней ошибки
	err_state = CAN_GetFlagStatus(CAN1, CAN_FLAG_LEC);
	
	// получаем пакет из FIFO приемника
	CAN_Receive(CAN1, CAN_FIFO0, rx_end);
	
	if (ovf_flag == 0) {

        	// в поле DLC помещаем флаг ошибочного пакета
		if (err_state) rx_end->DLC |= BSP_CAN_ERR_FLAG;
	
		CanRxMsg *rx_end_new;
			
		// узнаем новое положение указателя конца кольцевого буфера
		if (rx_end < &rx_msg[BSP_CAN_RX_BUF_SIZE-1]) rx_end_new = rx_end + 1;
		else rx_end_new = rx_msg;
	
		// если указатели конца и начала буфера равны, то устанавливаем
		//  признак переполнения bsp-буфера
		if (rx_end_new == rx_start) ovf_flag = 1;
		else {
		
			// перемещаем указатель конца буфера
			rx_end = rx_end_new;
		}
	}
}

void CAN1_RX1_IRQHandler(void)
{
	uint8_t err_state;
	
	// получаем код последней ошибки
	err_state = CAN_GetFlagStatus(CAN1, CAN_FLAG_LEC);
	
	// получаем пакет из FIFO приемника
	CAN_Receive(CAN1, CAN_FIFO1, rx_end);
	
	if (ovf_flag == 0) {

        	// в поле DLC помещаем флаг ошибочного пакета
		if (err_state) rx_end->DLC |= BSP_CAN_ERR_FLAG;
	
		CanRxMsg *rx_end_new;
			
		// узнаем новое положение указателя конца кольцевого буфера
		if (rx_end < &rx_msg[BSP_CAN_RX_BUF_SIZE-1]) rx_end_new = rx_end + 1;
		else rx_end_new = rx_msg;
	
		// если указатели конца и начала буфера равны, то устанавливаем
		//  признак переполнения bsp-буфера
		if (rx_end_new == rx_start) ovf_flag = 1;
		else {
		
			// перемещаем указатель конца буфера
			rx_end = rx_end_new;
		}
	}
}

/**
  * @brief  This function handles CAN1 TX request.
  * @param  None
  * @retval None
  */
void CAN1_TX_IRQHandler(void)
{
	bsp_can_buf2trn();

	// Если кольцевой буфер пуст, то запрещаем данное прерывание
	if (tx_start == tx_end) {
		CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);
	}
}

void rbus_to_can(char len, char *buf)
{
	struct can_tx_frame f;
	uint8_t cnt;
	f.id = can_compose_eid(6, 0xff, 0x34, CAN_SOURCE_ADDRESS(0, 0));
	f.dlc = 8;
	for (cnt=0;cnt<8;cnt++)
	{
		if(cnt<len)		f.data[cnt]=buf[cnt];
		else f.data[cnt]=0xff;
	}
	if(CAN_SOURCE_ADDRESS(0, 0)<0xfe)
				can_buffer_tx_data(&f, 0);
}

/*********************************END OF FILE**********************************/

