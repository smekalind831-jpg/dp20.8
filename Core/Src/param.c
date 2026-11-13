//******************************************************************************
// Секция include:
//******************************************************************************
#include "param.h"
//******************************************************************************
// Секция определения переменных, используемых в модуле
//******************************************************************************
//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------

//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************

//******************************************************************************
// Секция описания функций (сначала глобальных, потом локальных)
//******************************************************************************
void Sensor_Number_Set(uint8_t Sensor_Number)
{
  switch (Sensor_Number)
  {
  case DPM20_8_64_01:
    parameters.address_group = ADDR_GROUP1;
    parameters.max_number_turns = 64;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_64_02:
    parameters.address_group = ADDR_GROUP2;
    parameters.max_number_turns = 64;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_64_03:
    parameters.address_group = ADDR_GROUP3;
    parameters.max_number_turns = 64;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_64_04:
    parameters.address_group = ADDR_GROUP4;
    parameters.max_number_turns = 64;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_64_11:
    parameters.address_group = ADDR_GROUP1;
    parameters.max_number_turns = 64;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_128_01:
    parameters.address_group = ADDR_GROUP1;
    parameters.max_number_turns = 128;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_128_02:
    parameters.address_group = ADDR_GROUP2;
    parameters.max_number_turns = 128;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_128_03:
    parameters.address_group = ADDR_GROUP3;
    parameters.max_number_turns = 128;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_128_04:
    parameters.address_group = ADDR_GROUP4;
    parameters.max_number_turns = 128;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_128_11:
    parameters.address_group = ADDR_GROUP1;
    parameters.max_number_turns = 128;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_256_01:
    parameters.address_group = ADDR_GROUP1;
    parameters.max_number_turns = 256;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_256_02:
    parameters.address_group = ADDR_GROUP2;
    parameters.max_number_turns = 256;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_256_03:
    parameters.address_group = ADDR_GROUP3;
    parameters.max_number_turns = 256;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_256_04:
    parameters.address_group = ADDR_GROUP4;
    parameters.max_number_turns = 256;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_256_11:
    parameters.address_group = ADDR_GROUP1;
    parameters.max_number_turns = 256;
    parameters.max_number_discrete = 4096;
    break;
    
  case DPM20_8_512_01:
    parameters.address_group = ADDR_GROUP1;
    parameters.max_number_turns = 512;
    parameters.max_number_discrete = 32768;
    break;
    
  case DPM20_8_512_02:
    parameters.address_group = ADDR_GROUP2;
    parameters.max_number_turns = 512;
    parameters.max_number_discrete = 32768;
    break;
    
  case DPM20_8_512_03:
    parameters.address_group = ADDR_GROUP3;
    parameters.max_number_turns = 512;
    parameters.max_number_discrete = 32768;
    break;
    
  case DPM20_8_512_04:
    parameters.address_group = ADDR_GROUP4;
    parameters.max_number_turns = 512;
    parameters.max_number_discrete = 32768;
    break;
    
  case DPM20_8_512_11:
    parameters.address_group = ADDR_GROUP1;
    parameters.max_number_turns = 512;
    parameters.max_number_discrete = 32768;
    break;
    
  case DPM20_8_1024_01:
    parameters.address_group = ADDR_GROUP1;
    parameters.max_number_turns = 1024;
    parameters.max_number_discrete = 32768;
    break;
    
  case DPM20_8_1024_02:
    parameters.address_group = ADDR_GROUP2;
    parameters.max_number_turns = 1024;
    parameters.max_number_discrete = 32768;
    break;
    
  case DPM20_8_1024_03:
    parameters.address_group = ADDR_GROUP3;
    parameters.max_number_turns = 1024;
    parameters.max_number_discrete = 32768;
    break;
    
  case DPM20_8_1024_04:
    parameters.address_group = ADDR_GROUP4;
    parameters.max_number_turns = 1024;
    parameters.max_number_discrete = 32768;
    break;
    
  case DPM20_8_1024_11:
    parameters.address_group = ADDR_GROUP1;
    parameters.max_number_turns = 1024;
    parameters.max_number_discrete = 32768;
    break;
    /////////////////////////////////
     case DPM20_8_32_01:
    parameters.address_group = ADDR_GROUP1;
    parameters.max_number_turns = 32;
    parameters.max_number_discrete = 2048;
    break;
    
  case DPM20_8_32_02:
    parameters.address_group = ADDR_GROUP2;
    parameters.max_number_turns = 32;
    parameters.max_number_discrete = 2048;
    break;
    
  case DPM20_8_32_03:
    parameters.address_group = ADDR_GROUP3;
    parameters.max_number_turns = 32;
    parameters.max_number_discrete = 2048;
    break;
    
  case DPM20_8_32_04:
    parameters.address_group = ADDR_GROUP4;
    parameters.max_number_turns = 32;
    parameters.max_number_discrete = 2048;
    break;
    
  case DPM20_8_32_11:
    parameters.address_group = ADDR_GROUP1;
    parameters.max_number_turns = 32;
    parameters.max_number_discrete = 2048;
    break; 
  default:
    Sensor_Number = 0;
    break;
  }
}	//Sensor_Number_Set
//------------------------------------------------------------------------------

//******************************************************************************
// ENF OF FILE
//******************************************************************************
