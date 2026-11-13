//******************************************************************************
// Секция include:
//******************************************************************************
#include "version.h"

//******************************************************************************
// Секция определения переменных, используемых в модуле
//******************************************************************************
//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------
const unsigned char Assembly_Version[4] = {DAY,MONTH,YEAR_HUNDREDS,YEAR_UNITS};
const char Softvere_version = 'A';

const unsigned char Device_Name[31][24] =  {
                                          {'D','P','M','2','0','.','8','-','0','6','4','.','1','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','0','6','4','.','0','2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','0','6','4','.','0','3',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','0','6','4','.','0','4',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','0','6','4','.','0','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          
                                          {'D','P','M','2','0','.','8','-','1','2','8','.','1','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','1','2','8','.','0','2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','1','2','8','.','0','3',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','1','2','8','.','0','4',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','1','2','8','.','0','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          
                                          {'D','P','M','2','0','.','8','-','2','5','6','.','1','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','2','5','6','.','0','2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','2','5','6','.','0','3',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','2','5','6','.','0','4',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','2','5','6','.','0','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          
                                          {'D','P','M','2','0','.','8','-','5','1','2','.','1','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','5','1','2','.','0','2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','5','1','2','.','0','3',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','5','1','2','.','0','4',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','5','1','2','.','0','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          
                                          {'D','P','M','2','0','.','8','-','1','0','2','4','.','1','1',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','1','0','2','4','.','0','2',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','1','0','2','4','.','0','3',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','1','0','2','4','.','0','4',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','1','0','2','4','.','0','1',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          
                                          {'D','P','M','2','0','.','8','-','0','0','3','2','.','1','1',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','0','0','3','2','.','0','2',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','0','0','3','2','.','0','3',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','0','0','3','2','.','0','4',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'D','P','M','2','0','.','8','-','0','0','3','2','.','0','1',' ',' ',' ',' ',' ',' ',' ',' ',' '},

                                          {'D','P','M','2','0','.','8','-','X','X','X','.','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
                                          };
//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------

//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************

//******************************************************************************
// Секция описания функций (сначала глобальных, потом локальных)
//******************************************************************************
uint8_t ch_name_dev(void)
{
  if(parameters.address_group == 1 && parameters.max_number_turns == 64 && parameters.max_number_discrete == 4096)
    return 0;
  else if(parameters.address_group == 2 && parameters.max_number_turns == 64 && parameters.max_number_discrete == 4096)
    return 1;
  else if(parameters.address_group == 3 && parameters.max_number_turns == 64 && parameters.max_number_discrete == 4096)
    return 2;
  else if(parameters.address_group == 4 && parameters.max_number_turns == 64 && parameters.max_number_discrete == 4096)
    return 3;
  else if(parameters.address_group == 1 && parameters.max_number_turns == 64 && parameters.max_number_discrete == 4096)
    return 4;
  
  else if(parameters.address_group == 1 && parameters.max_number_turns == 128 && parameters.max_number_discrete == 4096)
    return 5;
  else if(parameters.address_group == 2 && parameters.max_number_turns == 128 && parameters.max_number_discrete == 4096)
    return 6;
  else if(parameters.address_group == 3 && parameters.max_number_turns == 128 && parameters.max_number_discrete == 4096)
    return 7;
  else if(parameters.address_group == 4 && parameters.max_number_turns == 128 && parameters.max_number_discrete == 4096)
    return 8;
  else if(parameters.address_group == 1 && parameters.max_number_turns == 128 && parameters.max_number_discrete == 4096)
    return 9;
  
  else if(parameters.address_group == 1 && parameters.max_number_turns == 256 && parameters.max_number_discrete == 4096)
    return 10;
  else if(parameters.address_group == 2 && parameters.max_number_turns == 256 && parameters.max_number_discrete == 4096)
    return 11;
  else if(parameters.address_group == 3 && parameters.max_number_turns == 256 && parameters.max_number_discrete == 4096)
    return 12;
  else if(parameters.address_group == 4 && parameters.max_number_turns == 256 && parameters.max_number_discrete == 4096)
    return 13;
  else if(parameters.address_group == 1 && parameters.max_number_turns == 256 && parameters.max_number_discrete == 4096)
    return 14;
  
  else if(parameters.address_group == 1 && parameters.max_number_turns == 512 && parameters.max_number_discrete == 32768)
    return 15;
  else if(parameters.address_group == 2 && parameters.max_number_turns == 512 && parameters.max_number_discrete == 32768)
    return 16;
  else if(parameters.address_group == 3 && parameters.max_number_turns == 512 && parameters.max_number_discrete == 32768)
    return 17;
  else if(parameters.address_group == 4 && parameters.max_number_turns == 512 && parameters.max_number_discrete == 32768)
    return 18;
  else if(parameters.address_group == 1 && parameters.max_number_turns == 512 && parameters.max_number_discrete == 32768)
    return 19;
  
  else if(parameters.address_group == 1 && parameters.max_number_turns == 1024 && parameters.max_number_discrete == 32768)
    return 20;
  else if(parameters.address_group == 2 && parameters.max_number_turns == 1024 && parameters.max_number_discrete == 32768)
    return 21;
  else if(parameters.address_group == 3 && parameters.max_number_turns == 1024 && parameters.max_number_discrete == 32768)
    return 22;
  else if(parameters.address_group == 4 && parameters.max_number_turns == 1024 && parameters.max_number_discrete == 32768)
    return 23;
  else if(parameters.address_group == 1 && parameters.max_number_turns == 1024 && parameters.max_number_discrete == 32768)
    return 24;
 
 ////////////////////////////////////////////////////////////////////////
  else if(parameters.address_group == 1 && parameters.max_number_turns == 32 && parameters.max_number_discrete == 2048)
    return 25;
  else if(parameters.address_group == 2 && parameters.max_number_turns == 32 && parameters.max_number_discrete == 2048)
    return 26;
  else if(parameters.address_group == 3 && parameters.max_number_turns == 32 && parameters.max_number_discrete == 2048)
    return 27;
  else if(parameters.address_group == 4 && parameters.max_number_turns == 32 && parameters.max_number_discrete == 2048)
    return 28;
  else if(parameters.address_group == 1 && parameters.max_number_turns == 32 && parameters.max_number_discrete == 2048)
    return 29;
  return 30;
}
//******************************************************************************
// ENF OF FILE
//******************************************************************************

