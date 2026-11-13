#ifndef __PRG_STRUCT__
#define __PRG_STRUCT__
#include "stdint.h"

//#define NOFDATS	16

#pragma pack(push, 1) 
typedef __packed struct
{
  unsigned short user_data_length;
  struct
  {
    unsigned char address;                               /* адрес для работы в CAN линии */
    union 
    {
      unsigned char byte[8];                             /* имя для работы в CAN линии */
      struct
      {
        unsigned identity_number                :21;
        unsigned manufacturer_code              :11;
        unsigned char ecu_instance              :3;
        unsigned char function_instance         :5;
        unsigned char function                  :8;
        unsigned char reserved                  :1;
        unsigned char vehicle_system            :7;
        unsigned char vehicle_system_instance   :4;
        unsigned char industry_group            :3;
        unsigned char arbitrary_address_capable :1;     //- поддержка механизма разрешения конфликта адресов: 0 - нет, 1 - в устройстве есть механизм автоматического назначения адреса
      };
    } name;
  } id_CAN;

/*  
 struct
  {
    char part_number[100];   // нуль-терминированная строка; обозначение устройства, например "СМ5.1-2-12 РИВП.453835.041-07" 
    char soft_ver[100];      // нуль-терминированная строка; версия основного ПО 
  } hard_n_soft_id;

 */
  //- структура с настроечными параметрами
 /* struct
  {
    //- калибровочные параметры
    uint16_t EMP_0;           //- значение ацп напряженности при 0% показания прибора стенда
    uint16_t EMP_100;         //- значение ацп напряженности при 100% показания прибора стенда
    uint16_t amp_factor;      //- коэффициент усиления 1-го каскада входного фильтра

   //- настроечные параметры
    uint16_t noise_filter;    //- уровень шумов входного фильтра: значение ацп, ниже которого значения отбрасываются
    uint16_t amp_lowLewel;    //- минимальный уровень сигнала с АЦП при котором включается усилитель фильтра
    uint16_t amp_highLewel;   //- максимальный уровень сигнала с АЦП при котором выключается усилитель фильтра: должен быть больше чем: amp_lowLewel
    uint8_t  calibration;     //- код калибровки: какой параметр сохранить во flash-памяти
    uint8_t  MAX_AUX_change;  //- максимальное кол-во периодов без обмена по CAN (для включения габаритных огней на стреле)
    
   
    struct
    {
      uint16_t errCount;      //- граница счета, указывающая что завис обмен по SPI с ADT7310
      uint8_t  enable;        //- 1: датчик температуры присутствует и работает, 0: датчика температуры нет или инициализировался с ошибкой
    } adt7310;       
     
  } params; */
  
} ecu_id;
#pragma pack( pop ) 
#endif    //- __PRG_STRUCT__