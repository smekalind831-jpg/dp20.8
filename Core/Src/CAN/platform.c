/*
 *******************************************************************************
 * file    platform.c
 * author  Rezonans
 * version v1.0.0
 * date    2013
 * brief   Функции для работы canlib на платформе STM32
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include "can_debug.h"
#include "CAN/bsp_can.h"
#include "CAN/txrx.h"
#include "CAN/can_clock.h"
#include "CAN/global_macro.h"
#include "CAN/fstore_params.h"
#include "main.h"
#include "CAN/can_process_tsk.h"
#include "CAN/apm32f10x_bakr.h"
#include "CAN/apm32f10x_pmu.h"
#include "GlobalVar.h"

extern uint32_t Frequency;
//extern uint32_t adc_value;

//******************************************************************************
//
// can_platform_init
//
// Используется в can_txrx_init при первоначальной инициализации canlib
// (т.к. аппаратный can-модуль инициализируется в BSP, то ничего делать не надо)
//
//******************************************************************************
void can_platform_init()
{
}

//******************************************************************************
//
// can_engine_process_rx_hook
//
// С помощью этой функции canlib читает принятые пакеты из can-шины.
// (используется в can_engine_process_rx_queue сразу при заходе туда.
//  can_engine_process_rx_queue - первый метод в пользовательской функции
//  обработки состояния canlib)
//
//******************************************************************************
void can_engine_process_rx_hook()
{
	int processed_count = 0;
	Tbsp_can_msg f;
	struct can_rx_frame frame;

	// поддерживается только 1 CAN-интерфейс
	frame.interface = 0;
	
	// Цикл, пока в приемном буфере canlib есть место
	while (can_buffer_rx_full() == 0) {
		
		// если приемный bsp-буфер пуст, то выход из цикла
		if (bsp_can_read(&f) == 0)
			break;
		
//		 can_debug_print_frame(&f);
		 
		// обработка ошибочного пакета
		if (f.err) {
		//	can_dbg("Can error %08x\n", f.id);
			continue;
		}
		
		// обработка пакета с запросом данных
		if (f.rtr) {
		//	can_dbg("Not processing request frames\n");
			continue;
		}
		
		// обработка переполнения bsp-буфера
		if (f.ovf) {
		//	can_dbg("Bsp-buffer is overflow\n");
		}
		
		// преобразование формата принятого пакета
		frame.time_received = get_can_clock();
		frame.dlc = f.dlc;
		frame.id  = f.id | (f.ide ? 0x80000000 : 0);
		//assert sizeof(frame.data) == sizeof(f.data)
		memcpy(frame.data, f.data, sizeof(frame.data));
	
		// если передача пакета в canlib прошла с ошибкой, то выход
		if (can_buffer_raw_rx_data(&frame) == 0)
			break;

		// счетчик обработанных правильных пакетов
		processed_count++;
	}
}

//******************************************************************************
//
// can_set_data_for_tx
//
// С помощью этой функции canlib передает пакеты в can-шину.
// (используется в can_tx_data)
//
//******************************************************************************
int can_set_data_for_tx(can_port_t port, struct can_tx_frame *frame)
{
	int ret;
	Tbsp_can_msg f;
	
	f.id  = frame->id & 0x1FFFFFFF;
	// признак расширенного идентификатора устанавливается по состоянию
	//  старшего бита идентификатора. На данный момент из-за глюков canlib
	//  это происходит не всегда, поэтому устанавливаем его принудительно.
	f.ide = 1;//(frame->id & 0x80000000 != 0);
	f.rtr = 0;
	f.dlc = frame->dlc;
	memcpy(f.data, frame->data, sizeof(f.data));

	// помещаем пакет в bsp-буфер
	ret = bsp_can_write(&f);
	//can_dbg("Writen message %08x in port, ret %d\n", f.id, ret);
	return ret;
}

//******************************************************************************
//
// get_can_clock
//
// Получение счетчика времени can-шины.
//
//******************************************************************************
can_clock_t get_can_clock()
{
	return (can_clock_t)ms_counter;
}

//******************************************************************************
//
// j1939_address_claim_random_delay
//
// Получение случайного числа.
// (используется в связке с get_can_clock)
//
//******************************************************************************
can_clock_t j1939_address_claim_random_delay()
{
	/**/;
	return rand() % 153;
}



void can_engine_work_pending_hook(uint32_t channels)
{
}

//------------------------------------------------------------------------------
//- возвращает размер настроечного параметра по SPN
/**return size or -1 if not found 0 value undefined*/
//------------------------------------------------------------------------------
int j1939_memory_access_device_write_get_parameter_size_hook(uint32_t spn)
{
	uint8_t ret=-1;
  uint8_t cnt;
	//ищем в fstore
	for(cnt=0; cnt<ITEMS_OF(Nastr_params); cnt++)
    if (Nastr_params[cnt].SPN==spn)  ret=Nastr_params[cnt].size;   //\ищет размер запрошенного параметра
	return ret;
}


//------------------------------------------------------------------------------
//- записывает значение настроечного параметра в outbuffer. 
//- возвращает размер настроечного параметра или 0 если не найден
//------------------------------------------------------------------------------
int j1939_memory_access_device_read_get_parameter_hook(uint32_t spn, uint8_t *outbuffer)
{
	uint8_t ret=0;
  uint8_t cnt;
	//ищем в fstore
	for(cnt=0; cnt<ITEMS_OF(Nastr_params); cnt++)     //\ spn номер параметра, в масиве перечисляются возможные параметры
		if (Nastr_params[cnt].SPN == spn) break;
	if(cnt < ITEMS_OF(Nastr_params)) 
	{   //\ в зависимости от того, какую функцию запросили в spn возвращает регистр параметк
       
          //- считывание регистров ADT7310 только при запросе
  /*   if(Nastr_params[cnt].SPN == 0x80001171)         //- идентификатор ADT7310 (Address 0x03)
        ADT7310_ReadReg(0x03, &ADT7310_id);
   
    else if(Nastr_params[cnt].SPN == 0x80001172)    //- регистр состояния (статуса) ADT7310 (Address 0x00)
        ADT7310_ReadReg(0x00, &ADT7310_status);
    else if(Nastr_params[cnt].SPN == 0x80001173)    //- регистр управления ADT7310 (Address 0x01)
        ADT7310_ReadReg(0x01, &ADT7310_config);
    else if(Nastr_params[cnt].SPN == 0x80001174)    //- гистерезис для критической, верхней и нижней границ температур ADT7310 (Address 0x05)
        ADT7310_ReadReg(0x05, &ADT7310_hyst);
    else if(Nastr_params[cnt].SPN == 0x80001175)    //- критическая температура ADT7310 (Address 0x04)
        ADT7310_ReadReg16(0x04, &ADT7310_crit);
    else if(Nastr_params[cnt].SPN == 0x80001176)    //- верхняя граница температуры ADT7310 (Address 0x06)
        ADT7310_ReadReg16(0x06, &ADT7310_high);
    else if(Nastr_params[cnt].SPN == 0x80001177)    //- нижняя граница температуры ADT7310 (Address 0x07)
        ADT7310_ReadReg16(0x07, &ADT7310_low);
           */   
    memcpy(outbuffer, Nastr_params[cnt].pvalue, Nastr_params[cnt].size);
		ret = Nastr_params[cnt].size;
	} 
 
	return ret;
}


//------------------------------------------------------------------------------
int j1939_memory_access_device_write_is_memory_writable_hook(uint32_t pointer, unsigned size)
{
	return 0;
}

//------------------------------------------------------------------------------
/**return NULL for error, pointer for ok*/
const uint8_t *j1939_memory_access_device_read_get_memory_hook(uint32_t pointer, unsigned size)
{
	return NULL;
}

//------------------------------------------------------------------------------
/**lock reads for parameters*/
void j1939_memory_access_device_parameter_write_start_hook(uint32_t spn_start, unsigned size)
{
}


//uint8_t membuffer[userdatalen];
// extern void UserDataupdate(void* buffer, uint16_t bufsize);


//------------------------------------------------------------------------------
//- запись принятых настроечных параметров во flash-память
/** return 1 for ok, 0 for fault*/
//------------------------------------------------------------------------------
int j1939_memory_access_device_parameter_write_hook(uint32_t spn, uint8_t *data)
{
  uint8_t ret = 0;
  uint8_t cnt;
  uint32_t tmp;
  
	//ищем в fstore
    for(cnt=0; cnt<ITEMS_OF(Nastr_params); cnt++)
      if (Nastr_params[cnt].SPN == spn)   break;
	
  if(cnt < ITEMS_OF(Nastr_params)) 
  {
		ret = Nastr_params[cnt].size;
    if(Nastr_params[cnt].SPN == 0x80001180)   //- SPN команда 0x80001180 в fstore_params.c
    {
       //- получена команда на 
       // 1 - сохранения в FRAM из parametrs
       // 2 - установка 0
   //   ret = 1;
      /*
      switch(*data)
      {
      case 0:     //- нет команды на калибровку
        break;
      case 1:     //- ацп напряжённости при 0% показаниях стенда
        Ecuinfo.params.EMP_0 = adc1_average;
        UserDataupdate(&Ecuinfo,sizeof(Ecuinfo));     //- комментировать при настройке
        break;
      case 2:     //- ацп напряжённости при 100% показаниях стенда
        Ecuinfo.params.EMP_100 = adc1_average;
        UserDataupdate(&Ecuinfo,sizeof(Ecuinfo));     //- комментировать при настройке
        break;
      case 3:     //- сохранить значение АЦП термодатчика
//        UserDataupdate(&Ecuinfo,sizeof(Ecuinfo));
        break;
    
      } */
    } else
    {
      //- перезаписываем принятые параметры
   //  *(uint16_t *)Nastr_params[cnt].pvalue = *(uint16_t *)data; 
   //   ret = 1;
        
      switch(Nastr_params[cnt].size)
      {
      case 1: 
          tmp = (uint8_t)*data;
      //    if(spn == 0x80001164)
      //    A = 56;
        *(uint8_t *)Nastr_params[cnt].pvalue = (uint8_t)tmp;//*(uint8_t *)data;
       // UserDataupdate(&Ecuinfo,sizeof(Ecuinfo));     //- комментировать при настройке
        break;
      case 2:
         tmp = (uint16_t)*data;
        *(uint16_t *)Nastr_params[cnt].pvalue = *(uint16_t *)data; //(uint16_t)tmp; //
       // UserDataupdate(&Ecuinfo,sizeof(Ecuinfo));     //- комментировать при настройке
        break;
      case 4:
         tmp = (uint32_t)*data;
        *(uint32_t *)Nastr_params[cnt].pvalue = (uint32_t)tmp; //*(uint32_t *)data;
      //  UserDataupdate(&Ecuinfo,sizeof(Ecuinfo));     //- комментировать при настройке
        break;
      }
    }
  }  
 return ret;
}


//------------------------------------------------------------------------------
/**commit to nonvolatile memory if neccessary and unlock access*/
void j1939_memory_access_device_parameter_write_complete_hook(uint32_t spn_start, unsigned size)
{
}

void j1939_memory_access_device_raw_write_start_hook(uint32_t pointer, unsigned size)
{
}
uint8_t *j1939_memory_access_device_write_get_memory_hook(uint32_t pointer, unsigned size)
{
	return NULL;
}
void j1939_memory_access_device_raw_write_complete_hook(uint32_t pointer, unsigned size)
{
}

void j1939_memory_access_device_process_bootload_request_hook(can_port_t port, can_instance_t instance, uint8_t address)
{
	//приняли подтверждение окончания операции доступа к памяти
#ifdef STM32F10X_CL
	PWR_BackupAccessCmd(ENABLE);
	BKP_WriteBackupRegister(BKP_DR1, 0xaa55);
	BKP_WriteBackupRegister(BKP_DR2, address);
#else
	PMU_EnableBackupAccess();
	BAKE_SetBackupRegister(BAKR_DATA1, 0xaa55);
	BAKE_SetBackupRegister(BAKR_DATA2, address);
	NVIC_SystemReset();
#endif
}
/*********************************END OF FILE**********************************/
