#ifndef __GLOBALVAR_H__
#define __GLOBALVAR_H__

#include "prg_struct.h"
#include "board.h"

//------------------------------------------------------------------------------
//- дата версии прошивки, сохраняется как uint32_t soft_data;
//- __DATE__ - внутренняя переменная IAR: строка формата "Mmm dd yyyy"
//------------------------------------------------------------------------------
#define YEAR_HUNDREDS   ((__DATE__ [7] - '0') * 10 + \
                          (__DATE__ [8] - '0'))

#define YEAR_UNITS      ((__DATE__ [9] - '0') * 10 + \
                          (__DATE__ [10] - '0'))

#define MONTH   (__DATE__ [2] == 'n' ? 1 \
                  : __DATE__ [2] == 'b' ? 2 \
                    : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
                      : __DATE__ [2] == 'y' ? 5 \
                        : __DATE__ [2] == 'n' ? 6 \
                          : __DATE__ [2] == 'l' ? 7 \
                            : __DATE__ [2] == 'g' ? 8 \
                              : __DATE__ [2] == 'p' ? 9 \
                                : __DATE__ [2] == 't' ? 10 \
                                  : __DATE__ [2] == 'v' ? 11 : 12)

#define DAY     ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 + (__DATE__ [5] - '0'))

#define SOFT_DATE     (YEAR_UNITS<<24 | YEAR_HUNDREDS<<16 | MONTH<<8 | DAY)     //- ДДММГГГГ, хранится как uint32_t


//------------------------------------------------------------------------------
#define DEFAULT_ADDR      (uint8_t)0xDC     //- адрес в сети CAN

#define STR(y) #y
#define SENSOR_NAME "DPM20.8"
#define PARTNUM	"DPM20.8**107-6936*"                                        //- hard_id для CAN
#define SOFT_VER(x,y) "|||*POWER_LINE_CONVERGENCE_SENSOR|" STR(x) "." STR(y) "|" DATE "|" TIME    //- soft_id для CAN

#define Bootveraddress			0x08001f80      //- адрес в памяти версии загрузчика
#define UserDataAddress     0x08002000      //- адрес начала пользовательских (калибровочных и настроечных) данных
#define EndOfFlash			    0x0801ffff
#define UserDataLength      0x800           //- размер области пользовательских (калибровочных и настроечных) данных
#define SectorSize			    0x400		        //- размер страницы flash-памяти



//- ошибки датчика температуры ADT7301
/*
#define ERR_7310_IniTX        0x0100    //- ошибка TX при инициализации
#define ERR_7310_IniRX        0x0200    //- ошибка RX при инициализации
#define ERR_7310_RegTX        0x0400    //- ошибка TX при чтении-записи регистра
#define ERR_7310_RegRX        0x0800    //- ошибка RX при чтении-записи регистра
#define ERR_7310_TempTX       0x1000    //- ошибка TX при чтении температуры
#define ERR_7310_TempRX       0x2000    //- ошибка RX при чтении температуры
*/

//------------------------------------------------------------------------------
extern ecu_id Ecuinfo;                //- структура с настроечными и калибровочными данными
extern uint32_t ms_counter;           //- системный тик: 1 мс

extern uint32_t globalError;          //- флаг ошибок

extern uint8_t flagAMP;               //- признак включения усилителя фильтра: после сброса = 0 (отключен)
extern uint32_t exchangeCount;        //- счетчик периодов без обмена по CAN / RBus (для включения габаритных огней на стреле)
/*
extern uint32_t el_field_strength;    //- напряжённость эл. поля по среднему арифметическому
extern uint32_t adc1_average;         //- усреднённое значение напряжённости ЭМП
extern uint16_t emp_calibr;           //- значение ацп напряжённости ЭМП для калибровки и визуального контроля
extern uint16_t el_field_CAN;         //- напряжённость эл. поля, выводимая по CAN

extern uint8_t out1_state;            //- состояние 1-го силового выхода: 0 - Z-состояние, 1 - притянут к земле (габаритные фонари)
extern uint8_t out2_state;            //- состояние 2-го силового выхода: 0 - Z-состояние, 1 - притянут к земле
extern uint8_t in1;                   //- первый вход:
extern uint8_t in2;                   //- второй вход:
extern uint8_t in_can;                //- состояние входов и выходов (подтверждение состояния) для передачи по CAN

extern uint8_t temp_ADT;          //- температура с датчика м/с ADT 7310
extern uint8_t V24;               //- напряжение бортовой сети автомобиля

extern uint8_t  ADT7310_status;   //- регистр состояния (статуса) ADT7310 (Address 0x00)
extern uint8_t  ADT7310_config;   //- регистр конфигурации ADT7310 (Address 0x01)
extern uint8_t  ADT7310_id;       //- идентификатор ADT7310 (Address 0x03)
extern uint16_t ADT7310_crit;     //- критическая температура ADT7310 (Address 0x04)
extern uint16_t ADT7310_high;     //- верхняя граница температуры ADT7310 (Address 0x06)
extern uint16_t ADT7310_low;      //- нижняя граница температуры ADT7310 (Address 0x07)
extern uint8_t  ADT7310_hyst;     //- гистерезис для критической, верхней и нижней границ температур ADT7310 (Address 0x05)
*/
#endif
