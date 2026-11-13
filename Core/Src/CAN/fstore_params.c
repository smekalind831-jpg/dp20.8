/*
 *******************************************************************************
 * file    fstore_bi_config.c
 * author  Rezonans
 * version v1.0
 * date    2014
 * brief   Элемент fram_store с настройками блока индикации
 *******************************************************************************
 */

#include "CAN/fstore_params.h"


void default_params(void)    //\ вызывать, если нет ключа иначе брать из FRAM (CAN_SET - 4000)
{
  Ecuinfo.user_data_length              = 0x800;
  Ecuinfo.id_CAN.address                = (uint8_t)0xDC;
  Ecuinfo.id_CAN.name.identity_number   = 0;
  Ecuinfo.id_CAN.name.manufacturer_code = 0x00;
  Ecuinfo.id_CAN.name.ecu_instance      = 0;
  Ecuinfo.id_CAN.name.function_instance = 0;
  Ecuinfo.id_CAN.name.function          = 39;			//I/O Controller
  Ecuinfo.id_CAN.name.vehicle_system    = 0;
  Ecuinfo.id_CAN.name.vehicle_system_instance = 0x00;
  Ecuinfo.id_CAN.name.industry_group    = 0x00;
  Ecuinfo.id_CAN.name.arbitrary_address_capable = 0;
 
  /*
  Ecuinfo.params.EMP_0 = 250;
  Ecuinfo.params.EMP_100 = 20000;
  Ecuinfo.params.amp_factor = 1500;
  Ecuinfo.params.noise_filter = 50;
  Ecuinfo.params.amp_lowLewel = 2300;
  Ecuinfo.params.amp_highLewel = 3700;
  Ecuinfo.params.MAX_AUX_change = 5;          //- максимальное кол-во периодов без обмена по CAN / RBus (для включения габаритных огней на стреле), секунды
  Ecuinfo.params.calibration = 0;
  
  Ecuinfo.params.adt7310.errCount = 0xFFFF;   //- граница счета, указывающая что завис обмен по SPI с ADT7310
  Ecuinfo.params.adt7310.enable = 0x01;       //- 1: датчик температуры присутствует и работает, 0: датчика температуры нет или инициализировался с ошибкой
*/ 
}
       
const parameter_struct Nastr_params[]=
{
    {0x80001151, &parameters.address_group,         1},   //\ одренсная группа
    {0x80001152, &parameters.angle_offset_1,        2},   //\смещение 1
    {0x80001153, &parameters.angle_offset_2,        2},   //\ смещение2
    {0x80001154, &parameters.direction_rotation,    1},  //\ направление вращения
    {0x80001155, &parameters.max_degree_discrete,   1},   //\ число дискрет на градус
    {0x80001156, &parameters.max_number_discrete,   2},   //\ число дискрет
    {0x80001157, &parameters.max_number_turns,      2},   //\ число оборотов
    {0x80001159, &parameters.serial_number_High,    2},   //\ серийный H
    {0x80001160, &parameters.serial_number_Low,     2},    //\ --***---  
    {0x80001161, &Ecuinfo.id_CAN.name.byte,         4},   //\ млаждшие 4-байта настроек CAN
    {0x80001162, &Ecuinfo.id_CAN.name.byte[4],      4},    //\ старшие 4-байта настроек CAN
    {0x80001163, &Ecuinfo.id_CAN.address,           1},
    {0x80001164, &parameters.save,                  1}    //\ команды по CAN 
   /* {0x80001172, &Ecuinfo.id_CAN.name.manufacturer_code,                   1}, 
    {0x80001173, &Ecuinfo.id_CAN.name.ecu_instance,                   1}, 
    {0x80001174, &Ecuinfo.id_CAN.name.function_instance,                     1}, 
    {0x80001175, &Ecuinfo.id_CAN.name.function,                     2}, 
    {0x80001176, &Ecuinfo.id_CAN.name.vehicle_system,                     2}, 
    {0x80001177, &Ecuinfo.id_CAN.name.vehicle_system_instance,                      2}, 
    {0x80001178, &Ecuinfo.id_CAN.name.industry_group,    1}, 
    {0x80001178, & Ecuinfo.id_CAN.name.arbitrary_address_capable,    1}
    */
   
};

