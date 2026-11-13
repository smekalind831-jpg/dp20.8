/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include "stm32f1xx_hal.h"
/**
 * @brief CAN Interrupts Flage
 */
 
  /*
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
//#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "About.h"   // дата версия
#include "stm32f1xx_ll_wwdg.h"  

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif 
   
 */ 
#include "stdint.h"
#include "CAN/GlobalVar.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

  // #include "ReadSC.h"

/* USER CODE END Includes */

/* Глобальные типы -----------------------------------------------------------*/
/* USER CODE BEGIN ET */
 //\ ========================== Типы ====================================
typedef struct {       // ответ энкодера
  union
  {
    uint8_t byte[2];
    uint16_t data;
    struct
    { 
      uint16_t PARC:1,  // чётность
      DF2:1,            // всегда 1
      DF1:1,            //
      ERR:1,            // если 1 - ошибка
      ANGLE:12;
    };
  };	
} SC;
//\-------------- Текущее состояние -------------------
//- режимы работы датчика и программы
struct Status{
  uint8_t mode;  // 0 - режим работы: не определён,
                 // 1 - требуется  калибровка (0x53 RBus)
                 // 2 - нормальный режим
                 // 3 - неисправность энкодера
  uint8_t flag_zero_position_set;             //- 0 - нет команды; 1 - установить положение датчика соответствует 0 градусам (R-Bus, запись в FRAM), 
  uint16_t discrete_in_turns;                 //- кол-во дискрет в одном обороте
};

//\-------------- для рассчётов текущих углов ---------

typedef struct
 {
  uint16_t angle_no_correct;    // "сырой" угол 1 без служебных бит
  uint16_t angle_no_offset;     // c выстановкой нуля
  uint16_t angle;               // с учётом оборотов CS1 - резльтат
                                // CS2 - обороты CS1
  uint16_t old_angle;           // для определения оборота
  uint16_t turns;               // полных оборотов
  uint32_t W_FORM;              // переменная для рассчёта скорости
 } work;

static work Angle[2]; //----- пока 2 инкодера

 //\------------ оперативная информация для/из FRAM ---

struct Save                    
{
  union 
  {
    unsigned char Bute [0x08];
    struct
    {
      uint16_t angle1;          //\ сохраняется текущий угол no_offsen
      uint16_t angle2;          //\ при чтении станет Old
      uint16_t turns1;          //\ текущие обороты
      uint16_t turns2;
      uint32_t W_WORD;          //\ для скорости непрерывный угол 
     }; 
  };
};    

struct SaveS                    
{
  union 
  {
    unsigned char Bute [0x08];
    struct
    {
      uint16_t angle1;          //\ сохраняется текущий угол no_offsen
      uint16_t angle2;          //\ при чтении станет Old
      int16_t turns1;          //\ текущие обороты
      int16_t turns2;
      uint32_t W_WORD;          //\ для скорости непрерывный угол 
     }; 
  };
}; 

//\------------

  
//\-------------
//\ пользовательские данные записанные в защ. FRAM 
#pragma pack(push, 1) 
struct Parameters{
  union
  {
    uint8_t data[0x10];
    struct 
    {
      uint8_t address_group;          //-[0]  адресная группа: 0x01 - азимут
      uint8_t direction_rotation;     //-[1] направление вращения, смотреть со стороны вала № 1
      uint16_t max_number_turns;      //-[3,2] максимальное число подсчитываемых оборотов вала датчика
      uint16_t max_number_discrete;   //-[5,4] максимальное кол-во дискрет = число оборотов х кол-во дискрет в одном обороте
      uint8_t max_degree_discrete;    //-[6] ?? число дискрет на градус ???
      uint16_t serial_number_High;    //-[8,7]
      uint16_t serial_number_Low;     //-[10,9]
      uint16_t angle_offset_1;        //-[12,11] угод между 0 энкодера1 и установленным значением
      uint16_t angle_offset_2;        //-[14,13]
      uint16_t save;                  //-[15] признак сохранения  Nastr_params и default_params во флеш
    };
  };
};
#pragma pack(pop) 
//\-------------- внутреннее состояние SPI для CS

typedef struct      
 { 
    //\SPI/RAM
   unsigned int 
    RC: 2, // нужно прочитать CS1/(2/3), 0x00 - всё спокойно 
    /* 2-bit в принципе излишне по Stp опренделяем что читать*/
    RF: 1, // нужно обновить данные FRAM
    CN: 5, // число передаваемых байт/слов (для FRAM)
    CC: 1, // Complite CS 1/(2/3)
    CF: 1, // Complite FRAM
    Cont: 1, // необходим пересчёт угла данного энкодера
    reserv: 5;
 }  Con;
//---------------------- CAN ---------------------------------
//CAN_HandleTypeDef hcan;
void can_init(void);
void can_process(void);
void default_params(void);
void CAN_bsp_Config(uint8_t can_artdis);
void CAN_process_tsk(void);

uint16_t Speed();
//\---------------- статус ошибок
 /*
 struct Error{  
  union
  {
    uint16_t e;
    struct{      
      uint8_t reserv1                   : 8;      
      uint8_t calibration               : 1;
      uint8_t parameters                : 1;
      uint8_t bounce_battery            : 1;
      uint8_t bounce_signal_inductor    : 1;
      uint8_t low_battery               : 1;
      uint8_t no_battery                : 1;
      uint8_t reserv2                   : 2;      
    };
  };
};
*/
/* USER CODE END ET */

/* Глобальные переменные --------------------------------------------------------*/
/* USER CODE BEGIN EC */
//====================== Переменные глобальные =========================
/*
extern unsigned char RxData[8];
extern unsigned char TxData[8];
extern CAN_TxHeaderTypeDef Tx;
extern CAN_RxHeaderTypeDef Rx;     
*/


extern volatile SC Encod1,  // сырой ответ с вала
                   Encod2;
//extern struct Error error;

extern struct Parameters parameters;              
extern volatile unsigned char Input1  [32],
                              Output1 [32],
                              FRWR,        //\ Для запуска SPI1 извне
                              UASRTtx,     //\ длина ответа USART
                              UinARR [8],  //\ приём RBUS
                              UoutARR[27]; //\ ответ
extern volatile struct Status status;                              
extern struct Save transmit;
extern struct SaveS Tr1;


/*                
volatile  Con SPI1_CUR, //\ задачи SPI
              SPI2_CUR;
*/
/*
extern   Con SPI1_CUR, //\ задачи SPI
             SPI2_CUR; 
*/
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

//\================= Прототипы экспортируемых функций ===================
void Error_Handler(void);
  
/* USER CODE BEGIN EFP */
//\=== чтение энкодеров
  void SPI_READ (Con SP1, Con SP2);
  void Delay (unsigned int Tim);

//\=== функции FRAM  
  void FRAM_WRITE ( unsigned int ADR, unsigned char* arr, unsigned char Size );
  void FRAM_READ ( unsigned int ADR, unsigned char Size );
  void W_Protection ( unsigned char en );

//\=== расчёт углов, оборотов....  
  unsigned int CountCS   ( unsigned int CS, unsigned int Offset, char RT );
  unsigned int CountTURN ( unsigned int no_off,     // угол без смещения
                           unsigned int Old,        // предидущее -//-
                           unsigned int tur,        // число оборотов
                           unsigned int max);       // максимальное число оборотов
  
  unsigned int CountDEG  ( unsigned int numDisc,    // максимально дискрет,
                           unsigned int max,        // максимально оборотов,
                           unsigned int tur,        // число оборотов,
                           unsigned int no_off,   // угол без смещения
                           unsigned int* wave);
  void MainCount (void);                            // основной расчёт угла
 
 //\=== FRAM 
  unsigned char R_Bute   (uint16_t ADR);            // чтение из FRAM
  void W_Bute (uint16_t ADR, unsigned char Bute);   // запись байта
  void W_Status (uint8_t Status);                   // запись статуса 
  unsigned char R_Status(void);

 //\=== RBUS
  void Work_reqest ( unsigned int Ang ); // рабочий реж.
  void Sereal( unsigned int HIG, unsigned int LO); //S№
  void Name (void);//char* name);
  void Version (void);   //\ версия + дата
  void ERR_stat (void);  //\ Error
  void Settings ( unsigned char Tk, unsigned char  Np, work* angl ); //\режима настройки 
  void SettingSet ( unsigned char Nt, unsigned char Vn, unsigned char  Vn1 );
  
/* USER CODE END EFP */

/* USER CODE BEGIN Private defines */
//\======================= CAN  =======================
uint8_t myaddresses(uint8_t port, uint8_t instance);  

//\===================== const ========================
#define Del2  2  //\ делитель таймера 
#define Del1  1
#define Tic1 0x01   //\ отладка время НЕ-> работа SPI 
#define Tic2 0x01
#define Tic3 500
#define Tic4 1000
#define Key  0x67  //\ пишется в FRAM - 0x01FF признак первого вкл.
#define cnV  0
#define reloadV  10                        
#define minV (uint32_t)20000
#define CAN_spd 100

//\================= Служебные ========================
#define CLOCKWISE		((uint8_t)0x01)   //- по часовой стрелке
#define COUNTER_CLOCKWISE	((uint8_t)0x02)   //- против часовой стрелке 
#define FRAM_Rop                0x0000      // для оперативной инфо
#define FRAM_Glob               0x0600      // обл. защмщённая 0x600.... 0x7FFh
#define AdrKey                  0x01FF      // адрес ключа первого включения
//#define AdrFlag                 0x01FE      // адрес признака выключения ???
#define CAN_SET                 0x0400      // адрес параметров CAN

#define ADDR_SERV               0x3F
#define REQUEST_SERIAL_NUMBER   0x23
#define REQUEST_DEVICE_NAME     0x4E
#define REQUEST_PROGRAMM_VERSION 0x56
#define ERR_STATE               0x45
#define REQUEST_SETTING_MODE    0x53
#define REQUEST_SET_PARAMETERS  0x54
//#define alrm                    300       // зона от 0 после OFF искать переход
//\====================== USART =======================
#define RX_I 0x20  //\ маски разрешения прер. по приёму
#define TX_I 0x80  //\ буфер пуст
#define TC_I 0x40  //\ сдвиговый пуст.
//\ ======== status FRQAM ===============
#define Lock        0x04  // lock 600h to 7FFh (upper 1/4)
#define UNLock      0x00  // Unlock ALL
//\ =================== speed =========================
#define miniV 4          // интервал между рассчётами при высокой скрости
#define maxV 10          // периоды- // -- низкой
#define Vmin 20         // грница низкой скорости
#define Vmax 100         // верхней
  
//\===================== const END ====================

#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
