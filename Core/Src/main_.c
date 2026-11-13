/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#define can_mode

#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
//#include "can.h"
#include "Defs1.h"


/* Прототипы локальных функций-----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);


/* ---------- переменные------------------------------------------------------*/


   static unsigned char Stp = 0,     //\ этап чередования опросов
                        Index1,      //\ номер передабаемого байта
                        UindexR = 0, //\ номер принятого байта изменяется в прер
                        UindexW;     //\ номер передаваемого байта
                  
   uint32_t    Index2 = 0; //\========== отладка
   
   static unsigned int  delil; //\ делитель для TIM2
   
   static Con SPI1_CUR, //\ задачи SPI
              SPI2_CUR;
   
   volatile SC Encod1,  // сырой ответ с вала
               Encod2;  
   
   struct {     //\ для проверки адекватности переданногог
           uint16_t max_number_turns;      // максимальное число подсчитываемых оборотов вала датчика
           uint16_t max_number_discrete;   // максимальное кол-во дискрет = число оборотов х кол-во дискрет в одном обороте
         }Parameters_Sav;   volatile struct {                 //\ флаги из прерываний
                      unsigned char 
                      Part    :1,    //\ принят байт с паритетлом
                      CMD     :1,    //\ принят 2-й байт 
                      RESET   :1,    //\ команда сброса
                      Frame   :1,    //\ принят весь пакет
                      CanIn   :1;    //\ принят пакет CAN
                    } Flags;

    volatile unsigned char //Input1  [32] = {0},   // входной выходной SPI
                     //Output1 [32] = {0};
                     Input1  [32],
                     Output1 [32],
                     FRWR,    //\ внешний старт SPI2

                     UASRTrx, //\ длина принемамой команды - 1
                     UASRTtx, //\ длина ответа
                     UinARR [8],  //\ приём RBUS
                     UoutARR[27]; //\ ответ

 uint8_t CAN_error_tmr;
 struct Save transmit;
 struct Save Tr1;

 struct Parameters parameters;
 
 /*
 CAN_TxHeaderTypeDef Tx;
 CAN_RxHeaderTypeDef Rx;
 unsigned char RxData[8];
 unsigned char TxData[8]; 
 
 extern uint32_t TxMailbox = 0;
 */
 volatile struct Status status; 

 unsigned int S1 = 0, 
              S2 = 0;  //\ ========= отладка ==========

//======================= Const =======================
const char INQ [] = {0x19, 0x1C, 0xDB, 0x5A}; //\ inquire
const char NUM [] = {0x19, 0x1C, 0x1B, 0x1A}; //\ number

/// +++++++++++++++++++ ВЕРСИЯ ДЛЯ CAN ++++++++++++++++ 
int main(void)
{ unsigned int L, tmp;//debug = 2048;
    /* MCU Configuration--------------------------------------------------------*/
unsigned char tmp1, tmp2, tmp3; // для перегрузки из volatile в функции

   /*======== Сброс переферии===== */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

   LL_GPIO_AF_Remap_SWJ_NOJTAG(); // JTAG выкл
 
  // HAL_Init();

   SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init( 0 ); // 0 - для CS60218 1 - FC25
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();

  // SysTick_Config(SystemCoreClock/1000);   //- 1 ms 
 // MX_CAN_Init();
  /* Initialize interrupts */
  MX_NVIC_Init();
 
  

  
//\========== Interrupt ==========

   //\ ===== использую для задержек без прерывания========    
  // LL_TIM_EnableIT_UPDATE(TIM3);
  // LL_TIM_EnableCounter(TIM3);
  
   //\ ============== SPI =============
    SPI1->CR1 |=  0x40;   //\ (0x1UL << SPI_CR1_SPE_Pos) включение SPI 
    SPI2->CR1 |=  0x40;   //\ 
    
    L = SPI1->DR; // сброс ложных прерываний по чтению 
    L = SPI2->DR;
    
    SPI1->CR2 |= 0x40; // прерывание по приёму
    SPI2->CR2 |= 0x40; 
   
    //\ =============== USART ===========
     USART1->CR1 |= RX_I;  //\ прерывание
      
   //\ ====== Прерыпания от порта PB12(off)       
    EXTI->IMR = 0x01000;   
  
   //\ ========= Начальные значения ===========
    CSx_Port->BSRR = 0x0003 | 0x0400;    // Все CSx, MS = 1 
    CSx_Port->BSRR = 0x0800 << 16;       // HOLD = 0
    LIN_GPIO_Port->BSRR = LIN_Pin; // LIN = 1
    Stp = 0;   // начало
    delil = 0;
   
    EXTI->IMR = 0x01000;  //\ прерывание отключения питания

    __enable_irq();
   
    SPI1_CUR.Cont = 0x00;
    SPI2_CUR.Cont = 0x00;   
    status.mode = 0x00;
  //\================= восстановление после включения (возможно перенесу в case 0) ============

    

   /* запустить чтение и дождаться сырые данные с энкодоров */
  for ( L = 0; L < 5; L++)
  { 

  SPI2_CUR.CN = 1; //\ SPI2 читает 16 бит
  SPI1_CUR.CN = 2; //\ 2 байта
  Index1 = 0x00;   //\ с первой ячейки для CS2
  FRWR = 0x00;
  
  SPI1_CUR.CC = SPI2_CUR.CC = 0x00; // флаг окончания чтения энкодера
  SPI1_CUR.RC = 2;
  SPI2_CUR.RC = 1;
  

  SPI_READ (SPI1_CUR, SPI2_CUR);    // запуск прерываний CS
  /* запустить чтение и дождаться данные с энкодоров */  
  while ( !( SPI1_CUR.CC && SPI2_CUR.CC ) ) asm ("NOP");  // энкодеры прочитаны
  SPI1_CUR.CC = SPI2_CUR.CC = 0x00;
  CSx_Port->BSRR = CS1_Pin | CS2_Pin;
  
   Encod2.byte[1] = Input1[0];  //\ CS2 - первым пришёл старший
   Encod2.byte[0] = Input1[1];
   
    if ( !Encod1.ERR && !Encod2.ERR && Encod1.DF1 &&  Encod1.DF2 ) // оба ответа без ошибок
    {
     Angle[0].angle_no_correct = Encod1.ANGLE; // CS1 углы без служебных данных
     Angle[1].angle_no_correct = Encod2.ANGLE; // CS2
     L = 50; // принудительный выход из цикла
    } // проверка на ошибку
  } // 5-и кратня попытка прочитать
// задержка
CSx_Port->BSRR = CS1_Pin | CS2_Pin; //\CS1 = CS2 = 1

 if (L == 5)
  status.mode = 0x03;    //\ энкодеры не исправны
    
   /* чтение ключа включения по адресу 0x1FF*/
     
  // W_Bute (0x01FF,0);// Key); 

 tmp = R_Bute (0x01FF);
   
 if ( tmp == Key ) //\ не первый раз включили
  {
    //\ ========== параметры ======================
    
    status.mode = 0x02; //\ рабочий режим

    FRAM_READ ( FRAM_Glob, 0x0F ); // основные настройки  
    SPI1_CUR.CC = 0;

    for (L = 0; L < 0x0F; L++)
     parameters.data[L] = Input1[L]; // данные при выключении 
   
    //\ ========== оперативные значения ===========
    Index1 = 0;
    SPI1_CUR.CC = 0x00;
    FRAM_READ ( FRAM_Rop, 8 );
    
    Angle[0].angle_no_offset = Input1[1]; //\ до выключения
    Angle[0].angle_no_offset <<= 8;
    Angle[0].angle_no_offset += Input1[0];

    Angle[1].angle_no_offset = Input1[3];
    Angle[1].angle_no_offset <<= 8;
    Angle[1].angle_no_offset += Input1[2];

    Angle[0].turns = Input1[5];
    Angle[0].turns <<= 8;
    Angle[0].turns += Input1[4];

    Angle[1].turns = Input1[7];
    Angle[1].turns <<= 8;
    Angle[1].turns += Input1[6];
    
    
// ---- проверить переход через 0 после подачи питания ----
    MainCount (); // рассчёт угла
//\ ---------- CAN настройки ------------------------------
    Index1 = 0;
    SPI1_CUR.CC = 0x00; 

#ifdef can_mode      
    FRAM_READ ( CAN_SET, 9 );
    
    Ecuinfo.id_CAN.address = Input1[0];
    for (L = 1; L <9 ; L++)
    {
     Ecuinfo.id_CAN.name.byte[L - 1] = Input1[L];
    }
#endif
    
  } 
  else //\ первый вход
  {
 
    status.mode = 0x01; //\ требуется калибр.
    //\============= значения по умолчанию ==============
    parameters.address_group = 0x01;          //- адресная группа: 0x01 - азимут
    parameters.direction_rotation = CLOCKWISE;     //- направление вращения, смотреть со стороны вала № 1
    parameters.max_number_turns = 32; //64; //512;//12;////      //- максимальное число подсчитываемых оборотов вала датчика
    parameters.max_number_discrete = 2048; //4096; //32768; //2048;// - максимальное кол-во дискрет = число оборотов х кол-во дискрет в одном обороте
      //uint8_t max_degree_discrete,    //- =
    parameters.serial_number_High = 0x0000;
    parameters.serial_number_Low  = 0x0000;      
    
    transmit.angle1 = parameters.angle_offset_1 = Angle[0].angle_no_correct;        // угод между 0 энкодера1 и установленным значением
    transmit.angle2 = parameters.angle_offset_2 = Angle[1].angle_no_correct;
    
   //\ ============ исходные значения ===================
    
    Angle[0].turns           = 0x0000;
    Angle[1].turns           = 0x0000;
    transmit.turns1          = 0x0000;
    transmit.turns2          = 0x0000;    

    Angle[0].old_angle = Angle[0].angle_no_offset = CountCS ( Angle[0].angle_no_correct,\
        parameters.angle_offset_1, parameters.direction_rotation); //\ рассчёт c учётом смещ. "0"
           
    L = (parameters.direction_rotation == 1) ? 2 : 1; // идёт в противоположную сторону
           
    Angle[1].old_angle = Angle[1].angle_no_offset = CountCS ( Angle[1].angle_no_correct,\
                                    parameters.angle_offset_2, L); 
   
    W_Bute (0x01FF, Key); //\ запись ключа
    FRAM_WRITE ( FRAM_Glob, parameters.data, 0x0F ); //настройки в FRAM
    
#ifdef can_mode    
   // default_params();  //\ параметры CAN по умолчанию
    FRAM_WRITE (CAN_SET, &Ecuinfo.id_CAN.address, 9); 
#endif
    
  } //\ первое включение 
 
   // Включить таймер 
   LL_TIM_EnableIT_UPDATE(TIM2);
   LL_TIM_EnableCounter(TIM2); 
   
   

  //\ ============ ОТЛАДКА ================
 /*   GPIOA->BSRR = CanEN_Pin;
   GPIOA->BSRR = CanEN_Pin << 16;
 
    parameters.max_degree_discrete = 12;
    parameters.max_number_discrete = 1 << parameters.max_degree_discrete;
   
   for(parameters.max_degree_discrete = 0; 
       parameters.max_number_discrete >> parameters.max_degree_discrete != 1; 
       parameters.max_degree_discrete++);
*/
 /*  RCC->APB1ENR |= 0x0800;    // тактирование WDT
   WWDG->CR  = 0x80 | 0x49;    // 0x80 -WWDT_EN | зчначение (T)
   WWDG->CFR = 0x0180 | 0x0040 | 0x0200; // предделитель /8 (0x0180) | счёт до 0x0040 (W) | 0x0200 - RES_EN
     
  while (1);     
   //L = USART1->SR;
   // USART1->SR &= ~TC_I;
   //USART1->SR |= RX_I;
   // USART1->CR1 |= TX_I; // вкл прерывание
   // USART1->CR1 |= TC_I; 
*/
 /*
 Tx.StdId = 0x0378; //\ идентификатор стандартного кадра
 Tx.ExtId = 0;      //\ идентификатор расширенного кадра
 Tx.RTR = CAN_RTR_DATA; //\CAN_RTR_REMOTE отправляем кадр с данными
 Tx.IDE = CAN_ID_STD;   //\ CAN_ID_EXT отправляем стандартный кадр
 Tx.DLC = 8;            //\ число полезных байт    
 Tx.TransmitGlobalTime = 0;  //\ относится к Time Triggered
 TxData[0] = 90;
 TxData[1] = 80;
 TxData[2] = 70;
 TxData[3] = 60;
 TxData[4] = 50;

  HAL_CAN_Start(&hcan);
  HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING );//| CAN_IT_ERROR | CAN_IT_BUSOFF | CAN_IT_LAST_ERROR_CODE);
 
   while (1)
  {
      CanEN_GPIO_Port->BSRR = CanEN_Pin; //\ CanEN

      while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0);

      if( HAL_CAN_AddTxMessage(&hcan, &Tx, TxData, &TxMailbox) )
      {
        ++L;
      } else
      {
        Flags.CanIn = 0;
      }
    //  CanEN_GPIO_Port->BSRR = CanEN_Pin;
       Delay (Tic3);
  }
    */
 //\ ============ ОТЛАДКА END ============   
 //\ ====================== CAN ==========
  
 //default_params();  //\ поместить в FRAM эту ф - ю использовать только при отсутствии ключа
#ifdef can_mode   
 bsp_can_init();
 can_init();
#endif           
 
  while (1)
  {
  switch ( Stp )
   {
   case 0:   // только включили
    {
      break; 
    }
    case 1:  // CS1, CS2
    {
      if ( SPI1_CUR.RC || SPI2_CUR.RC ) // в таймере запустили измерение
        {
          //\ сколько байт
          SPI2_CUR.CN = 1; //\ SPI2 читает 16 бит
          SPI1_CUR.CN = 2; //\ 2 байта
          
         //SPI1_CUR.CR = 1; //\ 1 -  CS2, 3 - CS3
          Index1 = 0x00;  //\ с первой ячейки
          FRWR = 0x00;
          
          //\ запустить SPI_READ
          SPI_READ (SPI1_CUR, SPI2_CUR); // запуск прерываний переклю CS
          
           SPI1_CUR.RC = SPI2_CUR.RC = 0x00; // процесс запущен, окончание появится в CC
         
        } // в таймере запустили измерение 
      
      if ( SPI1_CUR.CC && SPI2_CUR.CC )  // если 2 энкодера
        {
         //\ ============ обрааботка ================
          SPI1_CUR.CC = SPI2_CUR.CC = 0x00; // ответ анализируем
        //     CSx_Port->BSRR = CS1_Pin | CS2_Pin; //\
          
          Encod2.byte[0] = Input1[1];  //\ CS2 - первым пришёл старший
          Encod2.byte[1] = Input1[0];
          
     /*     L = Encod1.byte[0]; //\ CS1         
          Encod1.byte[0] = Encod1.byte[ 1];
          Encod1.byte[1] = L;  */
          
          if ( ( !( Encod1.ERR) ) && Encod1.DF1 &&  Encod1.DF2 ) Angle[0].angle_no_correct = Encod1.ANGLE; // CS1 углы без служебных данных
          if ( ( !( Encod2.ERR) ) && Encod2.DF1 &&  Encod2.DF2 ) Angle[1].angle_no_correct = Encod2.ANGLE; // CS2
      
          //=== ОТЛАДКА ==========    
       //   if ((!Encod1.ANGLE) || (!Encod2.ANGLE))
       //    Angle[1].angle_no_correct = Encod2.ANGLE; 
         //=== ОТЛАДКА END ==========            
          
          SPI1_CUR.CC = SPI2_CUR.CC  = 0x00;  //\ флаги
         // Delay ( Tic1 );  //\ пока отладка
         
        //  SPI1_CUR.Cont = 1; // нужен перерасчёт
        //  SPI2_CUR.Cont = 1;
          MainCount (); // рассчёт угла
          
          Delay (Tic1);  //\ пока отладка
          GPIOB->BSRR |= CS1_Pin | CS2_Pin;  //\ отладка все 1
          Tr1.angle1 = Angle[0].angle;       // для CAN
          Tr1.angle2 = Angle[1].angle;
        }
      break; 
    }
    case 2: // CS1, FRAM
    {
    
     if ( SPI1_CUR.RC /*|| SPI2_CUR.RC */) // в таймере запустили чтение/запись
     {
       
        SPI1_CUR.RC = 0x00;
   //\=============== FRAM =======================
    
      Index1 = 0;
      SPI1_CUR.CC = 0x00;  //\ SPI1 в процессе   
      
      
      CanEN_GPIO_Port->BSRR = CanEN_Pin; // 
        FRAM_WRITE ( FRAM_Rop, transmit.Bute, 0x08 );
      CanEN_GPIO_Port->BSRR = CanEN_Pin << 16; // 

     }// в таймере запустили чтение/запись  
      
     if ( SPI1_CUR.CC )  //\ записали
     {  
       SPI1_CUR.CC = 0;
     }
     break; 
    }
    case 3: //выключили питание выход только по сбросу
    { 
      if ( SPI1_CUR.RC || SPI2_CUR.RC )
      {
        SPI1_CUR.RC = SPI2_CUR.RC = 0;
      
        Index1 = 0;
  
        SPI1_CUR.CC = 0x00;  //\ SPI1 в процессе
        
        FRAM_WRITE ( FRAM_Rop, (unsigned char*) transmit.Bute, 8 );
      } // первый вход
     
      if (SPI1_CUR.CC) //\ записали
      {

        LL_TIM_DisableIT_UPDATE(TIM2);
        LL_TIM_DisableCounter(TIM2); 
        
        SPI1_CUR.CC = 0;
        SPI1->CR2 &= ~0xC0;     //\ откл. прерывания
      } //3
      break;
    }
   
   } //\switch

    //\=================== CAN работа ===============================
#ifdef can_mode  
      if(CAN_error_tmr < 16) CAN_error_tmr++;
      if(CAN_ReadFlagStatus(CAN1, 0x10F00002) == SET) CAN_error_tmr=0;    //- Error Passive Flag
      if(CAN_ReadFlagStatus(CAN1, 0x10F00004) == SET) CAN_error_tmr=0;		//- Bus-Off Flag
      if(CAN_error_tmr == 0) bsp_can_init();
      
      CAN_process_tsk();
      
     //\=================== CAN работа END ===========================              
      if (parameters.save == 1)
      {
        //\ =========== сохранение параметров в FRAM ===============
        parameters.save = 0x00;
        FRAM_WRITE ( CAN_SET, Ecuinfo.id_CAN.name.byte, 8 );
        FRAM_WRITE ( FRAM_Glob, parameters.data, 0x0F );      
      }
#endif
      
 
  //\============= по флагу окончания очередного этапа ============
         
/*
  if ( SPI2_CUR.Cont )   
  {  
    
        SPI2_CUR.Cont = 0; //\==== сбрасывать по окончательному рассчёту ========
*/       
        //\==================== отладка ================
 /*        S1 += 512;
         S1 = S1 > 0x1000 ? 0x0000 + 512: S1;
         Angle[0].angle_no_correct = (S1);
          
         S2 += 16;
         S2 = S2 > 0x1000 ? 0x0000 + 16: S2; 
         Angle[1].angle_no_correct = (0x1000 -  S2 );   */
  /*
  //\==================== отладка END ============ 
               
        Angle[0].old_angle = Angle[0].angle_no_offset; //\ для подсчёта оборотов
       //\ сохранённое angle_no_offset в transmit /FRAM/ должно быть уже подгружени
         
       //\ Учесть, что для SPI1 направление вращения в обратную сторону
        transmit.angle1 = Angle[0].angle_no_offset = CountCS ( Angle[0].angle_no_correct,\
                  parameters.angle_offset_1, parameters.direction_rotation); //\ рассчёт c учётом смещ. "0"
           
           //\ =========== проверка изменения оборотов
        transmit.turns1 = Angle[0].turns = CountTURN ( Angle[0].angle_no_offset, Angle[0].old_angle, //\ старыйновый угол
                                         Angle[0].turns,                    //\ текущие обороты
                                         parameters.max_number_turns);        //\ всего оборотов

  */       
         /* если было два измерения потом заменится */
         /*Angle[1].angle = Angle[0].turns; //\ более свежее значение
         */
         
         //\ ============ пересчёт CS2 ========                                       
    /* if(SPI1_CUR.Cont)  */ 
      //{ //\ ========== пересчёт с учётом смещения 0 SPI1 (SC2)
               
 /*
      Angle[1].old_angle = Angle[1].angle_no_offset; //\ для подсчёта оборотов
        //\ сохранённое angle_no_offset в transmit /FRAM/ должно быть уже подгружени
         
       //ЕСЛИ ВАЛЫ ИДУТ В РАЗНЫЕ СТОРОНЫ!!! 
        L = (parameters.direction_rotation == 1) ? 2 : 1; // идёт в противоположную сторону
           
        transmit.angle2 = Angle[1].angle_no_offset = CountCS ( Angle[1].angle_no_correct,\
                                    parameters.angle_offset_2, L); //\ рассчёт c учётом смещ. "0"
           
        //\ =========== проверка изменения оборотов 
       transmit.turns2 = Angle[1].turns = CountTURN ( Angle[1].angle_no_offset, Angle[1].old_angle, //\ старыйновый угол
                                    Angle[1].turns,                               //\ текущие обороты
                                    parameters.max_number_turns / 32);          //\ если 0 - не ограничено
*/
                                                
        
 /*        //\ ============= здесь число оборотов для CS1 - c учётом оборотов SC2
        Angle[1].angle = CountDEG (     32,                           //\ max число дискрет ()
                                        1,                            //\ max оборотов
                                        Angle[1].turns,               //\ обороты
                                        Angle[1].angle_no_offset );   //\ угол без смещения
 */     
        //\ ======== ОТЛАДКА проверка для синхронизации с CS1  =======
                                                   
    /*   if ( !( Angle[1].angle_no_offset % 128 ) )    //\ CS1 перешол в ту или другую сторону
         transmit.turns1 = Angle[1].angle = Angle[0].turns;      
     */ 
       
    //\============= ограничение угла вращения 
    //  Angle[1].angle = Angle[1].angle % parameters.max_number_turns; //\ ограничение оборотов  
       
  /*
  //\ ============= здесь число оборотов для CS1 - c учётом оборотов SC2
        Angle[1].angle = CountDEG (     32,                           //\ max число дискрет ()
                                        1,                            //\ max оборотов
                                        Angle[1].turns,               //\ обороты
                                        Angle[1].angle_no_offset );   //\ угол без смещения

      //========= корректировка оборотов =========
       Angle[1].angle = Angle[1].angle % ( parameters.max_number_turns );
       
       if (parameters.max_number_turns <= 32)
        Angle[0].turns = Angle[1].angle;
       
        //\ ============= абсолютный угол по энкодерам
        Angle[0].angle = CountDEG  ( parameters.max_number_discrete,           //\ max число дискрет
                                         parameters.max_number_turns,       //\ max оборотов
                                         (parameters.max_number_turns <= 32)? Angle[1].angle:
                                         Angle[0].turns,    // Angle[0].turns,  // Angle[1].angle, \ обороты (выходной угол CS2 с учётом его оборотов!!)
                                         Angle[0].angle_no_offset );        //\ обороты без смещения
        */
     //\======== ОТЛАДКА ==========
  /*
   if ( Angle[0].angle == debug) // 32760 )//  2040 )
      {
       L = S1/S2;
     //  UinARR[0] = 0x19;
     //  Flags.Part = 1;
      }
     //\======== ОТЛАДКА END ==========
  }  //  if(SPI2_CUR.Cont) 
 */
  
  //\============= анализ флагов RBUS ============================= 
  if ( Flags.Part == 1 ) //\принят байт с 9
  {
   Flags.Part = 0;
   
   //\ ========== рабочий режим ===========
   if ( UinARR[0] == INQ[parameters.address_group - 1] ) //\ с 9-м пришол inquire
   {
    UoutARR[0] = NUM[parameters.address_group - 1]; //\ первый байт ответа
    UindexW = 0; // остановили счёт
    Work_reqest ( Angle[0].angle );
   }//\рабочий режим
  }//\9-й 

  //\ ========== сервисный ===============
  if ( Flags.CMD == 1 ) //\ пришла команда
  {
   Flags.CMD = 0; 

   if ( UinARR[1] == INQ[parameters.address_group - 1] &&\
        UinARR[0] == 0x3F ) //\определение длины пакета
   {
    if ( UinARR[2] == 0x53 )
      UASRTrx = 0x05;
    else if (UinARR[2] == 0x54)
      UASRTrx = 0x06; 
    else
      UASRTrx = 0x03;
   }//\адрес свой
  }//\ команда 

  //\ ========== обработка принятого ======
  //\ ============= ОТЛАДКА ===============

  /*
    parameters.max_degree_discrete = 12;
    parameters.max_degree_discrete = 1 << parameters.max_degree_discrete;
   
   for(parameters.max_degree_discrete = 0; 
       parameters.max_number_discrete >> parameters.max_degree_discrete != 1; 
       parameters.max_degree_discrete++);
   */
  //Flags.Frame = 1;
 /*
  UinARR[0] = 0x3F;
  UinARR[1] = 0x19;
  UinARR[2] = 0x54; //команда
  UinARR[3] = 0x03; // Tk/Nt
  UinARR[4] = 0x0C; // Np/Vn (lo)
  UinARR[5] = 0x00; // CRC/Vn (Hig)
  UinARR[6] = 0x7D; // CRC
 */ 
  //\ ============= ОТЛАДКА END ===========
  if ( Flags.Frame == 1 )
  {
   Flags.Frame = 0;

   if ( UinARR[1] == INQ[parameters.address_group - 1] &&\
        UinARR[0] == 0x3F )
   {
     
     L = 0x00;
    if ( UinARR[2] != 0x53 && UinARR[2] != 0x54 )
    {
    for(int i = 0; i < 3; i++)
     L ^= UinARR[i];

    if ( L == UinARR[3] )
     L = 1; //\ CRC - true 
    else
     L = 0;
    }//\ 4-х байтная команда

    if ( UinARR[2] == 0x53 )
    {  
     for(int i = 0; i < 5; i++)
      L ^= UinARR[i];
  
     if ( L == UinARR[5] )
      L = 1; //\ CRC - true
     else
      L = 0;
    }//\ 6-и байтная команда

    if ( UinARR[2] == 0x54 )
    {  
     for(int i = 0; i < 6; i++)
      L ^= UinARR[i];

     if ( L == UinARR[6] )
      L = 1; //\ CRC - true
     else
      L = 0;
    }//\ 7-и байтная команда
    
    if ( L == 0x01 ) //\ adres, CRC совпали
    {
      UoutARR[1] = NUM[parameters.address_group - 1]; // 0x23 NUMBER
      //\ -------- серийный номер -------------
      if ( UinARR[2] == REQUEST_SERIAL_NUMBER )
       Sereal( parameters.serial_number_High, parameters.serial_number_Low );
      //\ ------------ название --------------
      if ( UinARR[2] == REQUEST_DEVICE_NAME )  // 0x4E
       Name ();
       //\------------ версия дата -----------
      if ( UinARR[2] == REQUEST_PROGRAMM_VERSION )  //0x56
       Version (); 
      //\------------ статус ошибки ---------
      if ( UinARR[2] == ERR_STATE )  // 0x45 !!! в протоколах нет описания !!!
       ERR_stat (); 
      //\----- запрос режима настройки ------
      if ( UinARR[2] == REQUEST_SETTING_MODE )  // 0x53
       {
         L = status.mode;
         
         tmp1 = UinARR[3];
         tmp2 = UinARR[4];
         Settings ( tmp1 /*UinARR[3]*/, tmp2 /*UinARR[4]*/, Angle ); 

         if ( ( status.mode != L ) && ( status.mode == 2 ) ) //\включили рабочий после калибр.
          FRAM_WRITE ( FRAM_Glob, parameters.data, 0x0F );
       }
       //\---------- установка параметров ----
      if ( UinARR[2] == REQUEST_SET_PARAMETERS )  // 0x54
       {
        tmp1 = UinARR[3];
        tmp2 = UinARR[4];
        tmp3 = UinARR[5];
        SettingSet ( tmp1 /*UinARR[3]*/,tmp2 /*UinARR[4]*/, tmp3 /*UinARR[5]*/ );
       }

    }//\ adres, CRC совпали
   }//\ адрес, сервис 0x3F
  }//\ обработка

 }//\ while (1)
}//\ main


//========================= инициализация CAN ==================================
void can_process(void)
{
	//- обработка принятых CAN-пакетов и расфасовка их по каналам
	can_engine_process_rx_queue();
	
  //- Обрабатывает сообщения из CAN_TRANSPORT_PROTOCOL_CHANNEL
	can_transport_protocol_process();
  
	//can_j1939_diagnostic_message_rx();
	//can_logger_process();
	
  //- обработка сообщений канала доступа к памяти оконечного устройства (датчика)
  j1939_memory_access_device_process();
	
  // обработка PGN (передача по таймауту и по запросу из can)
	can_txdata_process();
	can_transport_protocol_check_timeout();
	
  //can_j1939_diagnostic_message_process_timeouts();
	
  j1939_address_claim_process();
	//can_memory_access_tool_process();
	//can_rbus_process();
	
	// физическая передача буферизированных данных в CAN
	can_tx_data();
}
//\=======================================
void can_init()
{	
	can_txrx_init();
	can_txdata_init();
	can_engine_init();
	can_transport_protocol_init();

  //- инициализация канала получения данных для записи во флеш-память
	j1939_memory_access_device_init();

  //- инициализация канала запроса адреса датчика в сети
	j1939_address_claim_init();

  //- канал сообщений: AUXIO
	j1939_datames_init();

	//can_memory_access_tool_init();
	//can_j1939_diagnostic_message_init();
	//can_rbus_init();
	//can_logger_init();
}
//\===========================================

//------------------------------------------------------------------------------
//- обновление пользовательских данных
//------------------------------------------------------------------------------
void UserDataupdate(void* buffer, uint16_t bufsize)  //\сделать чтение из FRAM
{
  /*
  uint32_t *pdata;
  uint32_t tmp,crcsum;
  
  __disable_irq(); */ 
/*  pdata=(uint32_t*)UserDataAddress;
  
  FLASH_Unlock();
  for(tmp = UserDataAddress; tmp < UserDataAddress + UserDataLength; tmp += SectorSize)
  {
    FLASH_ErasePage(tmp);
  }
  for(tmp=0;tmp<(bufsize+sizeof(uint32_t)-1)/sizeof(uint32_t);tmp++)
  {
    FLASH_ProgramWord(tmp*sizeof(uint32_t)+UserDataAddress, ((uint32_t*)buffer)[tmp]);
  }
  CRC_ResetDATA();
  crcsum=CRC_CalcBlockCRC(pdata, (UserDataLength-4)/sizeof(uint32_t));
  FLASH_ProgramWord(UserDataAddress+UserDataLength-4, crcsum);
  
  FLASH_Lock();*/
//  __enable_irq();
}

//\======================== РАСCCCСЧЁТ УГЛОВ =========================
void MainCount (void)
{  unsigned int L;
      
        SPI2_CUR.Cont = 0; //\==== сбрасывать по окончательному рассчёту ========
       
        //\==================== отладка ================
 /*        S1 += 512;
         S1 = S1 > 0x1000 ? 0x0000 + 512: S1;
         Angle[0].angle_no_correct = (S1);
          
         S2 += 16;
         S2 = S2 > 0x1000 ? 0x0000 + 16: S2; 
         Angle[1].angle_no_correct = (0x1000 -  S2 );   */
        //\==================== отладка END ============ 
               
        Angle[0].old_angle = Angle[0].angle_no_offset; //\ для подсчёта оборотов
       //\ сохранённое angle_no_offset в transmit /FRAM/ должно быть уже подгружени
         
       //\ Учесть, что для SPI1 направление вращения в обратную сторону
        transmit.angle1 = Angle[0].angle_no_offset = CountCS ( Angle[0].angle_no_correct,\
                  parameters.angle_offset_1, parameters.direction_rotation); //\ рассчёт c учётом смещ. "0"
           
           //\ =========== проверка изменения оборотов
        transmit.turns1 = Angle[0].turns = CountTURN ( Angle[0].angle_no_offset, Angle[0].old_angle, //\ старыйновый угол
                                         Angle[0].turns,                    //\ текущие обороты
                                         parameters.max_number_turns);        //\ всего оборотов

         
         /* если было два измерения потом заменится */
         /*Angle[1].angle = Angle[0].turns; //\ более свежее значение
         */
         
         //\ ============ пересчёт CS2 ========                                       
    /* if(SPI1_CUR.Cont)  */ 
      //{ //\ ========== пересчёт с учётом смещения 0 SPI1 (SC2)
               
       Angle[1].old_angle = Angle[1].angle_no_offset; //\ для подсчёта оборотов
        //\ сохранённое angle_no_offset в transmit /FRAM/ должно быть уже подгружени
         
       //ЕСЛИ ВАЛЫ ИДУТ В РАЗНЫЕ СТОРОНЫ!!! 
        L = (parameters.direction_rotation == 1) ? 2 : 1; // идёт в противоположную сторону
           
        transmit.angle2 = Angle[1].angle_no_offset = CountCS ( Angle[1].angle_no_correct,\
                                    parameters.angle_offset_2, L); //\ рассчёт c учётом смещ. "0"
           
        //\ =========== проверка изменения оборотов 
       transmit.turns2 = Angle[1].turns = CountTURN ( Angle[1].angle_no_offset, Angle[1].old_angle, //\ старыйновый угол
                                    Angle[1].turns,                               //\ текущие обороты
                                    parameters.max_number_turns / 32);          //\ если 0 - не ограничено
                                                
        
 /*        //\ ============= здесь число оборотов для CS1 - c учётом оборотов SC2
        Angle[1].angle = CountDEG (     32,                           //\ max число дискрет ()
                                        1,                            //\ max оборотов
                                        Angle[1].turns,               //\ обороты
                                        Angle[1].angle_no_offset );   //\ угол без смещения
 */     
        //\ ======== ОТЛАДКА проверка для синхронизации с CS1  =======
                                                   
    /*   if ( !( Angle[1].angle_no_offset % 128 ) )    //\ CS1 перешол в ту или другую сторону
         transmit.turns1 = Angle[1].angle = Angle[0].turns;      
     */ 
       
    //\============= ограничение угла вращения 
    //  Angle[1].angle = Angle[1].angle % parameters.max_number_turns; //\ ограничение оборотов  
       
              //\ ============= здесь число оборотов для CS1 - c учётом оборотов SC2
        Angle[1].angle = CountDEG (    /* 32, */  parameters.max_number_discrete,                          //\ max число дискрет ()
                                       /* 1,  */  parameters.max_number_turns / 32,                        //\ max оборотов
                                        Angle[1].turns,               //\ обороты
                                        Angle[1].angle_no_offset );   //\ угол без смещения

      //========= корректировка оборотов =========
    //   Angle[1].angle = Angle[1].angle % ( parameters.max_number_turns );
    
     /*
       if (parameters.max_number_turns <= 32)
        Angle[0].turns = Angle[1].angle;
    */
       
        //\ ============= абсолютный угол по энкодерам
        Angle[0].angle = CountDEG  ( parameters.max_number_discrete,           //\ max число дискрет
                                         parameters.max_number_turns,       //\ max оборотов
                                       //  (parameters.max_number_turns <= 32)? Angle[1].angle:
                                         Angle[0].turns,    // Angle[0].turns,  /* Angle[1].angle, *//\ обороты (выходной угол CS2 с учётом его оборотов!!)
                                         Angle[0].angle_no_offset );        //\ обороты без смещения
        
     //   Angle[0].angle += 1; // чтобы получать значения до 0x80...
 if (/* ( parameters.max_number_turns <= 32 ) &&\ */
     (Angle[0].turns < (parameters.max_number_turns - 1 ) ) ) 
    {
      L = 0;
        if( Angle[0].angle >= Angle[1].angle )
        {
          L = Angle[0].angle - Angle[1].angle;   // определяем размер люфта
        }
        else if (Angle[0].angle < Angle[1].angle)
        {
           L = Angle[1].angle - Angle[0].angle;
        }
        
        if ( ( L > 20 )  && ( L < 1600 ) ) //\ не люфт!!! && не переход через 0 (максимально 25 оборотов)
        {
          if( Angle[0].angle > Angle[1].angle ) 
          {
            Angle[0].turns = ( Angle[0].turns == 0 ) ? ( parameters.max_number_turns - 1 ) : ( ( Angle[0].turns - 1 ) % parameters.max_number_turns );
            Angle[0].angle -= ( parameters.max_number_discrete/parameters.max_number_turns );
          } else
          {
            Angle[0].turns = (Angle[0].turns + 1 ) % ( parameters.max_number_turns );
            Angle[0].angle += ( parameters.max_number_discrete/parameters.max_number_turns );
          }  
          
        }
    }          
     //\======== ОТЛАДКА ==========
   /*  if ( Angle[0].angle == debug) // 32760 )//  2040 )
      {
    //   L = S1/S2;
     //  UinARR[0] = 0x19;
     //  Flags.Part = 1;
      }  */
}
//\============================= CAN ============================== 
uint8_t myaddresses(uint8_t port, uint8_t instance)
 {
    if(!instance) return DEFAULT_ADDR;
    else return DEFAULT_ADDR;
 }
//\================================= INTERRUPN ====================


/*
 void CAN_IRQ  (void)
 {
 // HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &Rx, RxData);
  Flags.CanIn = 0x01;
 } 
*/

 //=========== TIM2 ==============  
 void TIM2_IRQ (void)
 {
          
  /*  if (delil >= Del)
    {
      delil = 0x00;
             
//      ITM_EVENT32_WITH_PC ( 2, delil);
     // __ITM_EVENT16 (1, delil)
 //     printf("Text\n\r");
      
      Stp++;  // 1 - SC1, SC2; 2 - SC1, CFRAM; 3 - SC1, SC3
      if ( Stp == 3 ) Stp = 1;
      
      SPI1_CUR.RC = Stp;  //\ Этап чтения 
      SPI2_CUR.RC = Stp;
      
      //---------- отладка =============
  
     //---------- отладка END=============
     } */
   
    
   if (delil == Del2) //\ запись
   {
     Stp = 2;
     SPI1_CUR.RC = Stp;  //\ Этап записи
   } //\ запись 
    
   if (delil >= Del) //\ включение измерений
   {
    Stp = 1;
    SPI1_CUR.RC = Stp;  //\ Этап чтения 
    SPI2_CUR.RC = Stp;
    delil = 0x00;
   } //\ включение измерений
    
    delil++; //\ делитель таймера
    TIM2->SR &= ~TIM_SR_UIF;   //\ флаг прерывания
 }// TIM2
   

//========== TIM3 ===============
 void TIM3_IRQ (void)   
 {
    TIM3->SR &= ~TIM_SR_UIF;   //\ флаг прерывания 
 } // TIM3



//============ Off ================
 void Off_IRQ  (void)
 {
   if ( !( Off_Port->IDR & Off_In ) )
   {
    Stp = 3;
    SPI1_CUR.RC = 1;
    SPI2_CUR.RC = 1;
   }
    EXTI->PR = Off_In;     //\ флаг
 } // Off_IRQ
  
 // =========== SPI1 ===============
 void SPI1_IRQ (void)
 {// unsigned int H;
  //\=============== приёмник =================   
   if   ( SPI1->SR & 0x01 ) //\ приёмник (READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))
   {  
    if ( ( SPI1_CUR.CN ) || ( FRWR ) ) //\ есть что передавать 
      /* переделать только на FRWR*/
     {      
      if ( SPI1_CUR.CN ) SPI1_CUR.CN--; //\ не уменьншать не нужное
      if ( FRWR ) FRWR--;
      
      Input1 [Index1] =  SPI1->DR;  //\ пришедшее 
      Index1++;
      
      SPI1->CR2 |= 0x80;   //\ Включаем передачу
     }  //\ приинято не всё
     
     if ( ( !SPI1_CUR.CN ) && ( !FRWR ) ) //\ прочитан нужный объём При записе FRAM, CN == 0
      {
       Index1 = 0;         //\ очередной пакет завершон
       SPI1_CUR.CC = 1;    //\ Обмен закончен
       SPI1->CR2 &= ~0x40; //\ откл. прерывание приёмника
       SPI1->CR2 &= ~0x80; //\ откл. прерывание передатчика
      } //\ принял всё
     return;
    } //\ приёмник    

//\=============== передатчик ===============
   if ( (SPI1->SR & 0x02) && (SPI1->CR2 & 0x80) )  //\ передатчик пуст и есть что передать 
   {
    if ( ( SPI1_CUR.CN ) || ( FRWR ) ) 
    { 
       SPI1->DR = Output1 [ Index1 ];    //\ следующий байт

       SPI1_CUR.CC = 0x00;   //\ если пришёл без обнуления после команд FRAM
       SPI1->CR2 &= ~0x80;   //\ выкл по передаче
    }
    return;
   } //\ передатчик пуст
 } // SPI1_IRQ (void)
 // =========== SPI1 END ===========
   
   
 // =========== SPI2 ===============
 void SPI2_IRQ (void)
 {  
//\=============== приёмник =================   
   if   ( SPI2->SR & 0x01 ) //\ приёмник (READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))
   {  
    if ( SPI2_CUR.CN )
     {      
      SPI2_CUR.CN--;
      Encod1.data =  (unsigned int) SPI2->DR ;  //\ пришедшее ( только для SPI2 )
     
      SPI2->CR2 |= 0x80;   //\ Включаем передачу
     }  //\ приинято не всё
     
     if ( !SPI2_CUR.CN )   //\ прочитан нужный объём
      {
       SPI2_CUR.CC = 1;    //\ угол прочитан
       SPI2->CR2 &= ~0x40; //\ откл. прерывание приёмника
       SPI2->CR2 &= ~0x80; //\ отладка выключил прерывание
      } //\ принял всё
     return;
    } //\ приёмник    

//\=============== передатчик ===============
   if ( (SPI2->SR & 0x02) && (SPI2->CR2 & 0x80) )  //\ передатчик пуст и есть что передать 
   {
    if ( SPI2_CUR.CN ) //\ так только для SPI2 ( передаётся только одно слово )
    {
       SPI2->DR = 0x0000;    //\ передатчик
       SPI2->CR2 &= ~0x80;   //\ выкл по передаче
    }
    return;
   } //\ передатчик пуст
 }  // =========== SPI2 END ============
 

//\ =============== USART ====================
 void USART_IRQ(void)
 { 
   unsigned int L1; 
    //\ ============== приёмник ================
  if ( USART1->SR &  RX_I ) //\ приём
  {                     
   L1 = USART1->DR;
   if ( L1 & 0x0100 ) //\ принят 9-й бит
   {
    UindexR = 0x01;  //\ разрешить счёт
    UinARR[0] = L1 & 0xFF; 
    Flags.Part = 1;
    return;
   }

   if ( UindexR ) UinARR[UindexR] = L1 & 0xFF; //\ любой следующий байт
     
   if ( UindexR == 2 ) //\ принята команда
   {
    Flags.CMD = 1; 
   }
   
   if ( UindexR == UASRTrx ) //\ принят весь пакет
   {
    UindexR = 0x00; //счёт остановить
    Flags.Frame = 1;
   } //\ принято всё

   if ( UindexR ) UindexR++; // счёт только после приёма 9-й
   return;
 }//\ приём
       
   //\ ============== сдвиговый пуст =========
 /* последовательность для очистки: 
 чтение и з SR и запись в DR или записьт 0 в 
 6 - й бит (TC) регистра SR */
 if ( ( USART1->SR & TC_I ) && ( USART1->CR1 & TC_I ) ) 
 {

   USART1->SR &= ~TC_I;   //\ снять флаг
   USART1->CR1 &= ~TC_I;  //\ выкл сдвиг
   L1 = USART1->SR;       //\ для флага переполнения
   L1 = USART1->DR;       //\ почистить флаг, передаваемое возвращается 
   UindexW = 0x00;

   (void)L1;
   //LIN_GPIO_Port->BSRR = LIN_Pin << 16;  // LIN = 0
   return;
 }//\ сдвиговый
    
//\ ============== передача ===============
 if ( ( USART1->SR & TX_I ) && ( USART1->CR1 & TX_I ) )// буфер пуст
 {
   USART1->DR = UoutARR[UindexW];
   L1 = USART1->DR;        //\ почистить флаг
   //    WWDG->CR  = 0x7F; //\ перезапист WDT включактся сброс только по RBUS
    
   if ( UindexW == UASRTtx ) //\ передал всё
   {
     USART1->CR1 &= ~TX_I; //\ выкл передачу
     USART1->SR &= ~TC_I;  //\ снять флаг
     USART1->CR1 |= TC_I;  //\ вкл сдвиговый регистр пуст
     return;
   } //\ передал всё
  
   UindexW++;
  } //\ буфер пуст
 }//\ USART


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_4);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(64000000);
  LL_SetSystemCoreClock(64000000);
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM2_IRQn interrupt configuration */
  NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(TIM2_IRQn);
  /* TIM3_IRQn interrupt configuration */
  NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(TIM3_IRQn);
  /* SPI1_IRQn interrupt configuration */
  NVIC_SetPriority(SPI1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(SPI1_IRQn);
  /* SPI2_IRQn interrupt configuration */
  NVIC_SetPriority(SPI2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(SPI2_IRQn);
  /* USART1_IRQn interrupt configuration */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART1_IRQn);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

