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
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "About.h"
//#include <arm_itm.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
 
/* USER CODE END PV */

/* Прототипы локальных функций-----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
 
 

/* USER CODE END PFP */

/* ---------- переменные------------------------------------------------------*/
/* USER CODE BEGIN 0 */
   static unsigned char Stp,   //\ этап чередования опросов
                        Index1, //\номер передабаемого байта
                        Index2;
   static unsigned int  delil; //\ делитель для TIM2
   
   static Con SPI1_CUR, //\ задачи SPI
              SPI2_CUR;
   
   volatile SC Encod1,  // сырой ответ с вала
               Encod2,  
               Encod3;
   
   volatile char //Input1  [32] = {0},   // входной выходной SPI
                     //Output1 [32] = {0};
                     Input1  [32],
                     Output1 [32],
                     FRWR;  //\ внешний старт SPI2
 
  struct Parameters parameters =
  {
   .address_group = 0x01,            //- адресная группа
   .direction_rotation = COUNTER_CLOCKWISE,  //\CLOCKWISE,- направление вращения
   .max_number_turns = 32,//2, //32 //512,
   .max_number_discrete = 2048//128// 2048 //32768  // для 32-оборота->2048 
  }; 
  
 static struct Save transmit;
 unsigned int S1 = 0, 
              S2 = 0;  //\ ========= отладка ==========
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
 
int main(void)
{ unsigned int L;
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

   /*======== Сброс переферии===== */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

   LL_GPIO_AF_Remap_SWJ_NOJTAG(); // JTAG выкл

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

    SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init( 0 ); // 0 - для CS60218 1 - FC25
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  

  /* Initialize interrupts */
  
  MX_NVIC_Init();
 
  
 /* USER CODE BEGIN 2 */
  
//\========== Interrupt ==========
   
  //\ ==== основной конвеер ====
   LL_TIM_EnableIT_UPDATE(TIM2);
   LL_TIM_EnableCounter(TIM2);   
  
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
      
    EXTI->IMR = 0x01000;  //\ ====== Прерыпания от порта PB12(off)  
  
  //\ ========= Начальные значения ===========
    CSx_Port->BSRR = 0x0003 | 0x0400;  // Все CSx, MS, 
    CSx_Port->BSRR = 0x0800 << 16;     //HOLD = 0
    Stp = 0;   // начало
    delil = 0;
   
   EXTI->IMR = 0x01000;  //\ прерывание отключения питания

   __enable_irq();
   
   SPI1_CUR.Cont = 0x00;
   SPI2_CUR.Cont = 0x00;   
 
  //\================= восстановление после включения =================
   
   /* чтение */
   
  //\========== Отладка ============   
  // NVIC_EnableIRQ(TIM2_IRQn);
  //CanEN_GPIO_Port->ODR |= CanEN_Pin;  //\ CanEN = 1
    
    //__disable_irq(); // проверка 
   
  // LL_TIM_EnableIT_UPDATE(TIM2);
  // LL_TIM_EnableCounter(TIM2);
  //  W_Status (0x86);
   
  // __enable_irq();
 //============ Отладка END ==========  
  
   
   
   
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */

  //\ ================== прочитать FRAM params, transmit ======================= 
  
  while (1)
  {
    
  switch (Stp)
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
           
          Encod2.byte[1] = Input1[0];  //\ CS2 - первым пришёл старший
          Encod2.byte[0] = Input1[1];
          
          L = Encod1.byte[0]; //\ CS1         
          Encod1.byte[0] = Encod1.byte[1];
          Encod1.byte[1] = L;
          
          transmit.angle1 = Angle[0].angle_no_correct = Encod1.ANGLE; // CS1 углы без служебных данных
          transmit.angle2 = Angle[1].angle_no_correct = Encod2.ANGLE; // CS2
          
          SPI1_CUR.CC = SPI2_CUR.CC  = 0x00;  //\ флаги
          Delay ( Tic1 );  //\ пока отладка
         
          SPI1_CUR.Cont = 1; // нужен перерасчёт
          SPI2_CUR.Cont = 1;
          
          GPIOB->BSRR |= CS1_Pin | CS2_Pin | CS3_Pin;  //\ отладка все 1
           //\ запуск рассчёта
        }
      break; 
    }
    case 2: // CS1, FRAM
    {
    
     if ( SPI1_CUR.RC || SPI2_CUR.RC ) // в таймере запустили чтение/запись
     {
       SPI2_CUR.CN = 1; //\ SPI2 читает 16 бит.
       SPI2_CUR.RC = 0;
       SPI1_CUR.RC = SPI1_CUR.CC = SPI1_CUR.CN= 0; //\ CS2 НЕ включать, SPI1 не занимать.       
       FRWR = 0x00;
       
       //\ запустить SPI_READ
       SPI_READ (SPI1_CUR, SPI2_CUR);
      
   //\=============== FRAM =======================
      
   //\================ отладка ==============      
       for (L = 0; L < 8; L++)
       {
        transmit.Bute[L] = L;
       }
   //\================ отладка END ==============
 
      Index1 = 0;
      
      Index2 = 0; //\ ========== отладка 

      SPI1_CUR.CC = 0x00;  //\ SPI1 в процессе      
      FRAM_WRITE ( 0x0000, transmit.Bute, 8 );
     
      SPI1_CUR.RC = SPI2_CUR.RC = 0;
      
    }// в таймере запустили чтение/запись  
      
     if ( SPI1_CUR.CC )  //\ зисали
     { 
        SPI1_CUR.CC = 0;
        SPI1->CR2 &= ~0xC0;     //\ откл. прерывания
        WriteProtection(1);     //\ на всякий случай
        //\ ? задержка ?
     //  SPI1_CUR.Cont = 1;
        
       CSx_Port->BSRR = MS_Pin;  // MS = 1
       // ? задержка ?
       CSx_Port->BSRR = HOLD_Pin << 16;  // HOLD = 0
        
        SPI1->CR1 |= 0x01; //\ по второму фронту (спад)
        // MS, HS -> 1
     }
      
      if ( SPI2_CUR.CC )        //\ прочитали
      {
        SPI2_CUR.CC = 0;
        SPI2_CUR.Cont = 1;      //\ пересчёт
        
        L = Encod1.byte[0];           
        Encod1.byte[0] = Encod1.byte[1];
        Encod1.byte[1] = L;
        
        
        SPI2->CR2 &= ~0xC0;     //\ откл. прерывания
        // ? задержка ?
        GPIOB->BSRR = CS1_Pin; //\ снять выбор
         //\ запуск рассчёта
      }  //\ чтение CS1
    break; 
    }
    case 3: // CS1, CS3
    {
       break; 
    }
  }//\switch
  
 
  //\============= по флагу окончания очередного этапа ============
         
  //\ ============ пересчёт CS1 ========
  if(SPI2_CUR.Cont)   
  { //\ ========== пересчёт с учётом смещения 0 SPI2 (SC2)
    //     printf("Text\n\r");      
      //\==================== отладка ================
       
       
           S1 += 512;
           S1 = S1 > 0x01000 ? 0x0000: S1;
          
           Angle[0].angle_no_correct = ( 0x01000 - S1 );
          
           S2 += 16;
           S2 = S2 > 0x01000 ? 0x0000: S2; 
           
           Angle[1].angle_no_correct = ( S2 );   
        
           
         L = S1/S2;

        //\==================== отладка END ============ 
         
               
     Angle[0].old_angle = Angle[0].angle_no_offset; //\ для подсчёта оборотов
       //\ сохранённое angle_no_offset в transmit /FRAM/ должно быть уже подгружени
         
       //\ Учесть, что для SPI1 направление вращения в обратную сторону
        Angle[0].angle_no_offset = CountCS ( Angle[0].angle_no_correct,\
        parameters.angle_offset_1, parameters.direction_rotation); //\ рассчёт c учётом смещ. "0"
           
           //\ =========== проверка изменения оборотов
         Angle[0].turns = CountTURN ( Angle[0].angle_no_offset, Angle[0].old_angle, //\ старыйновый угол
                                         Angle[0].turns,                    //\ текущие обороты
                                         parameters.max_number_turns);        //\ всего оборотов
         SPI2_CUR.Cont = 0; //\==== сбрасывать по окончательному рассчёту ========
         
         /* если было два измерения потом заменится */
         Angle[1].angle = Angle[0].turns; //\ более свежее значение
         
         //\ ============ пересчёт CS2 ========                                       
     if(SPI1_CUR.Cont)   
      { //\ ========== пересчёт с учётом смещения 0 SPI1 (SC2)
               
       Angle[1].old_angle = Angle[1].angle_no_offset; //\ для подсчёта оборотов
        //\ сохранённое angle_no_offset в transmit /FRAM/ должно быть уже подгружени
           
       L = (parameters.direction_rotation == 1) ? 2 : 1; // идёт в противоположную сторону
           
           
        Angle[1].angle_no_offset = CountCS ( Angle[1].angle_no_correct,\
        parameters.angle_offset_2, L); //\ рассчёт c учётом смещ. "0"
           
        //\ =========== проверка изменения оборотов 
        Angle[1].turns = CountTURN ( Angle[1].angle_no_offset, Angle[1].old_angle, //\ старыйновый угол
                                    Angle[1].turns,                               //\ текущие обороты
                                    parameters.max_number_turns / 32);          //\ если 0 - не ограничено
                                                
        
        //\ ============= здесь число оборотов для CS1 - c учётом оборотов SC2
        Angle[1].angle = CountDEG (     32,                           //\ max число дискрет ()
                                        1,                            //\ max оборотов
                                        Angle[1].turns,               //\ обороты
                                        Angle[1].angle_no_offset );   //\ угол без смещения
      
        //\ ======== ОТЛАДКА проверка для синхронизации с CS1  =======
          
        if ( Angle[1].angle_no_offset % 128 )    //\ CS1 перешол в ту или другую сторону
           Angle[1].angle = Angle[0].turns;      
        
        //\ ======== ОТЛАДКА проверка для синхронизации с CS1  END ===
        
          SPI1_CUR.Cont = 0; //\==== сбрасывать по окончательному рассчёту ========
      } //\ if(SPI1_CUR.Cont)   
            
        
    //\============= ограничение угла вращения 
     Angle[1].angle = Angle[1].angle % parameters.max_number_turns; //\ ограничение оборотов   
 /*       if (Angle[1].angle > parameters.max_number_turns) //\ переход через 0 за придел
        {
          Angle[1].turns = Angle[1].angle = Angle[0].turns = 0; //\ оставляем только энкодеры
        }
 */
        //\ ============= абсолютный угол по энкодерам
        Angle[0].angle = CountDEG  ( parameters.max_number_discrete,        //\ max число дискрет
                                         parameters.max_number_turns,       //\ max оборотов
                                         Angle[1].angle,  /* Angle[0].turns, */ //\ обороты (выходной угол CS2 с учётом его оборотов!!)
                                         Angle[0].angle_no_offset );        //\ обороты без смещения
     //\======== ОТЛАДКА ==========
     if ( Angle[0].angle == 0 )
        SPI2_CUR.Cont = 0;
     //\======== ОТЛАДКА END ==========
  } //  if(SPI2_CUR.Cont) 
  
  
 //\ ===== рассчёт угла 2 энкодера + обороты 
   /* Вариант №2 ДПМ20.8-032.01 число оборотов CS1 определяем по 
   углу CS2/  В этом варианте используем:
   */
  
    /* USER CODE END WHILE */
     
    /* USER CODE BEGIN 3 */
 }// while (1)
  /* USER CODE END 3 */
}

//\================================= INTERRUPN =================================
  /* USER CODE BEGIN Int 8 */
 //=========== TIM2 ==============  
 void TIM2_IRQ (void)
   {
                                              
    if (delil >= Del)
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
    //  TIM2->DIER &= ~TIM_DIER_UIE; 
     //---------- отладка END=============
    }
    
    delil++; //\ делительтаймера
    
  //  LL_TIM_ClearFlag_UPDATE(TIM2);
    TIM2->SR &= ~TIM_SR_UIF;   //\ флаг прерывания
  
   /* USER CODE END Int 8 */ 
   /* USER CODE BEGIN Int 81 */   
   }// TIM2
   /* USER CODE END Int 81 */

//========== TIM3 ===============
/* USER CODE BEGIN Int 9 */
  void TIM3_IRQ (void)       //$PROJ_DIR$\config\cm5.1-18-8_apm.icf    
  {
  
    TIM3->SR &= ~TIM_SR_UIF;   //\ флаг прерывания 
/* USER CODE END Int 9 */  
    
/* USER CODE BEGIN Int 91 */   
  } // TIM3

/* USER CODE END Int 91 */  

//============ Off ================
 void Off_IRQ  (void)
 {

   /*
    Записать FRAM
   */
   /* USER CODE BEGIN Int 91 */
   

   EXTI->PR = Off_In;     //\ флаг
   
   /* USER CODE BEGIN Int 91 */
 } // Off_IRQ
  

  /* USER CODE BEGIN SPI1 */
 // =========== SPI1 ===============
void SPI1_IRQ (void)
{
  //\=============== приёмник =================   
   if   ( SPI1->SR & 0x01 ) //\ приёмник (READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))
   {  
    if ( ( SPI1_CUR.CN ) || ( FRWR ) ) //\ есть что передавать 
      /* переделать только на FRWR*/
     {      
      if ( SPI1_CUR.CN ) SPI1_CUR.CN--; //\ не уменьншать не нужное
      if ( FRWR ) FRWR--;
      
      Input1 [Index1] =  (unsigned char) SPI2->DR;  //\ пришедшее 
      Index1++;
      
      Index2++;  //========= отладка
      
      SPI1->CR2 |= 0x80;   //\ Включаем передачу
     }  //\ приинято не всё
     
     if ( ( !SPI1_CUR.CN ) && ( !FRWR ) ) //\ прочитан нужный объём При записе FRAM, CN == 0
      {
       Index1 = 0;         //\ очередной пакет завершон
   
       Index2 = 0; //========= отладка
       
       SPI1_CUR.CC = 1;    //\ Обмен закончен
       SPI1->CR2 &= ~0x40; //\ откл. прерывание приёмника
       SPI1->CR2 &= ~0x80; //\ откл. прерывание передатчика
      } //\ принял всё
     return;
    } //\ приёмник    

//\=============== передатчик ===============
   if ( (SPI1->SR & 0x02) && (SPI1->CR2 & 0x80) )  //\ передатчик пуст и есть что передать 
   {
    if ( ( SPI1_CUR.CN ) || ( FRWR ) ) //\ передаётся два слова
    { 
       SPI1->DR = Output1 [ Index1 ];    //\ следующий байт
           
       //Index2++;  //\========== отладка 
       
       SPI1_CUR.CC = 0x00;   //\ если пришёл без обнуления после команд FRAM
       SPI1->CR2 &= ~0x80;   //\ отладка выкл по передаче
    }
    return;
   } //\ передатчик пуст
} // SPI1_IRQ (void)
 // =========== SPI1 END ===========
   /* USER CODE END SPI1 */

   /* USER CODE BEGIN SPI2 */
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
     
     if ( !SPI2_CUR.CN )  // прочитан нужный объём
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
       SPI2->DR = 0x0000;    //\ отладка передатчик
       SPI2->CR2 &= ~0x80;   //\ отладка выкл по передаче
    }
    return;
   } //\ передатчик пуст
 } // SPI2_IRQ
  // =========== SPI2 END ============
 /* USER CODE END SPI2 */

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
