//------------------------------------------------------------------------------
//- CAN_process_tsk.c
//------------------------------------------------------------------------------

#include "CAN/can_process_tsk.h"
#include "main.h"

const uint8_t numsoftIDs = 1;   //- отправляется первым байтом  в SoftID: Number of Software Identification Fields, spn 965
uint8_t flag_init_tx = 1;       //- при старте инициализируются пакеты на передачу
uint8_t start_fl = 0;
uint8_t empty_data = 0xFF;      //- байты с отсутствующими данными в сообщении

uint8_t ini_ADT7310;  //- данные чипа ADT7310

//------------------------------------------------------------------------------
//- инициализация сообщений для отправки по CAN
//------------------------------------------------------------------------------
void CAN_init_tx()
{

//  static unsigned int I;
  //настройка сообщений hardware ID и Software ID
	struct can_tx_parameter param;

  //----------------------------------------------------------------------------
  //- сообщение для отправки по запросу: PartNumber изделия
  //----------------------------------------------------------------------------
/*
	param.info.pgn = 0xfdc5,
	param.info.destination_address = 0xff;
	param.info.type = 0;
	param.info.port = 0;
	param.info.instance=0;
	param.info.broadcast_rate = BROADCAST_RATE_INFINITE;
	param.info.priority = 6;
	
	param.start_byte = 0;
	param.start_bit = 0;
	param.bit_length = strlen(Ecuinfo.hard_n_soft_id.part_number)*8;
	param.pdata = (unsigned char *)&Ecuinfo.hard_n_soft_id.part_number;
	can_txdata_load_message_into_list(&param);
*/	
  //----------------------------------------------------------------------------
  //- сообщения для отправки по запросу:
  //- идентификатор программы микроконтроллера
  //----------------------------------------------------------------------------
/*
	param.info.pgn = 0xfeda,
	param.info.destination_address = 0xff;
	param.info.type = 0;
	param.info.port = 0;
	param.info.broadcast_rate = BROADCAST_RATE_INFINITE;
	param.info.priority = 6;
	
	//- Number of Software Identification Fields - первый байт сообщения SoftID
  param.start_byte = 0;
	param.start_bit = 0;
	param.bit_length =8;
	param.pdata=(unsigned char *)&numsoftIDs;
	can_txdata_load_message_into_list(&param);

  //- версия программы микроконтроллера (до 200 байт)
	param.start_byte = 1;
	param.start_bit = 0;
	param.bit_length = strlen(Ecuinfo.hard_n_soft_id.soft_ver)*8;
	param.pdata = (unsigned char *)&Ecuinfo.hard_n_soft_id.soft_ver;
	can_txdata_load_message_into_list(&param);
        */
  //----------------------------------------------------------------------------
  //- сообщение для отправки по таймеру с показаниями 
  //----------------------------------------------------------------------------

  //- общие настройки сообщения
  param.info.pgn = 0xFFC6,                  //- PGN заменить!!!
	param.info.destination_address = 0xff;
	param.info.type = 0;
	param.info.port = 0;
	param.info.instance=0;
	param.info.broadcast_rate = CAN_MS2TICK(CAN_spd);//(500);
//	param.info.broadcast_rate = BROADCAST_RATE_INFINITE;
	param.info.priority = 0x6;
        
  //- нулевой, первый байт - основной угол
  param.bit_length = 16;
  param.start_byte = 0;
  param.start_bit=0;
  

  param.pdata = (unsigned char *)&Tr1.angle1; //el_field_CAN;
  can_txdata_load_message_into_list(&param);

  //- второй, третий
  param.bit_length = 16;
  param.start_byte = 2;
  param.start_bit=0;
       
  // скорость
       
  param.pdata = (unsigned char *)&Tr1.turns1; //
  can_txdata_load_message_into_list(&param);

  //- четвёртый б пятый
  param.bit_length = 16;
  param.start_byte = 4;
  param.start_bit=0;
  
  // усред скорость
  
  param.pdata = (unsigned char *)&Tr1.turns2;
  can_txdata_load_message_into_list(&param);
        
  
  //- шестой байт 
  param.bit_length = 8;
  param.start_byte = 6;
  param.start_bit=0;
  
  param.pdata = (unsigned char *)&empty_data;//in_can;
  can_txdata_load_message_into_list(&param);

  //- седьмой байт - не используется 
  param.bit_length = 8;
  param.start_byte = 7;
  param.start_bit=0;
  
  param.pdata = (unsigned char *)&empty_data;
  can_txdata_load_message_into_list(&param);
}

//------------------------------------------------------------------------------
//- 
//------------------------------------------------------------------------------
void CAN_process_tsk(void)
{
  
  static uint32_t time_can_process;
        
	//Инициализируем пакеты на передачу
	if(flag_init_tx)
	{
		CAN_init_tx();
		flag_init_tx = 0;
	}
	
  //\ CAN_process_tsk(void) выполняется не чаще 1 мс
  
   if(time_can_process <= get_can_clock() || time_can_process > get_can_clock()+1)
    {
    
    time_can_process = get_can_clock() + 1;
     
    //- период в 1 с -----------------------------------------------------------
    if(time_can_process % 1000 == 0)
    {
      //- проверяем обмен по CAN: если нет входных данных по AUXIO2 - включаем габаритные огни на Out1
     //    if(++exchangeCount > Ecuinfo.params.MAX_AUX_change)
     //    {
     //   out1_state = 1;
     //   OUT_1_on;
     //   }

     // if(Ecuinfo.params.adt7310.enable)
    //  {
        //- датчик температуры присутствует и инициализирован
       // uint32_t err = ADT7310_ReadTemp();
      //  if(0)
     //   {
    //      temp_ADT = 0xFE;          //- ошибка чтения температуры
   //       globalError |= err;       //- код ошибки в глобальную переменную
    //    }
    //  }
    //  else
    //  {
        //- датчика температуры нет или инициализирован с ошибкой
     //   temp_ADT = 0xFF;          //- датчика нет или не инициализирован
     // }
      
//      ADT7310_ReadReg(0x03, &ADT7310_id); - сделал обновление в platform.c: чтение из регистров ADT7310 только при запросе параметра
      
    } //- завершение периода 1 с


    
		if(start_fl==0 && CAN_SOURCE_ADDRESS(0, 0)<0xfe)
		{			
			CAN_bsp_Config(DISABLE);
			start_fl=1;
		}

		datames_process();
		can_process();
    } //- завершение цикла 1 мс
}
