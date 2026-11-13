#include "RBUS8.h"

//\============= ответ рабочего режима ==========
void Work_reqest (unsigned int Ang)
{ char i;
 
  UoutARR[1] = Ang;   // угол
  UoutARR[2] = Ang >> 8;
  UoutARR[3] = 0;  
  for(int i = 0; i < 3; i++)     // CRC
    UoutARR[3] ^= UoutARR[i];

  UASRTtx = 3;         // число байт

// LIN_GPIO_Port->BSRR = LIN_Pin;  // LIN = 1
  USART1->SR  &= ~TC_I; // почистить
  USART1->CR1 &= ~TC_I; // отключить TС
  USART1->CR1 |= TX_I;  // вкл пустого TX
  (void)i;
}//\Work
//\ ============== запрос серийного № =========
void Sereal( unsigned int HIG, unsigned int LO )
{ char i;
 
 UoutARR[0] = ADDR_SERV; //\ 0x3F
 UoutARR[2] = LO & 0xFF;
 UoutARR[3] = LO >> 0x08; 
 UoutARR[4] = HIG & 0xFF;
 UoutARR[5] = HIG >> 0x08;
 UoutARR[6] = 0x00;
 for(int i = 0; i < 6; i++)
   UoutARR[6] ^= UoutARR[i]; //\ CRC

  UASRTtx = 6;         // число байт

  //LIN_GPIO_Port->BSRR = LIN_Pin;  // LIN = 1
  USART1->SR  &= ~TC_I; // почистить
  USART1->CR1 &= ~TC_I; // отключить TС
  USART1->CR1 |= TX_I;  // вкл пустого TX

  (void)i;
}//\ sereal № 0x23

//\============ Имя =======================
void Name (void) //char* name)
{ char i;

  UoutARR[0] = ADDR_SERV; //\ 0x3F
  for(int i=2; i<30; i++) UoutARR[i] = 0x20; //\ пробелы

  UoutARR[26] = 0x00;
  UoutARR[26] = UoutARR[0] ^ UoutARR[1];
  
   for (uint8_t i = 2; i != 26; i++)
   {
     UoutARR[i] = Device_Name[ch_name_dev()][i-2];
     UoutARR[26] ^= UoutARR[i];
    }   
    
  UASRTtx = 26;         // число байт

 // LIN_GPIO_Port->BSRR = LIN_Pin;  // LIN = 1
  USART1->SR  &= ~TC_I; // почистить
  USART1->CR1 &= ~TC_I; // отключить TС
  USART1->CR1 |= TX_I;  // вкл пустого TX
   (void)i;
} //\ Name ACII 0x4E

//\=============== Запрос версии ============
void Version (void)
{ char i;

  UoutARR[0] = ADDR_SERV; //\ 0x3F
  UoutARR[2] = Softvere_version;
  UoutARR[3] = Assembly_Version[0];
  UoutARR[4] = Assembly_Version[1];
  UoutARR[5] = Assembly_Version[2];
  UoutARR[6] = Assembly_Version[3];
  UoutARR[7] = 0x00;

  for(int i = 0; i < 7; i++)
   UoutARR[7] ^= UoutARR[i];
        
  UASRTtx = 7;         // число байт

 // LIN_GPIO_Port->BSRR = LIN_Pin;  // LIN = 1
  USART1->SR  &= ~TC_I; // почистить
  USART1->CR1 &= ~TC_I; // отключить TС
  USART1->CR1 |= TX_I;  // вкл пустого TX
  (void)i;
}//\ Version 0x56

//\=============== !! запрос ошибки нет описания !! ===============
void ERR_stat (void)
{ char i;
  
    (void)i;
}//\ 0x45

//\=============== запрос настроек ===========
void Settings ( unsigned char Tk, unsigned char  Np, work* angl ) //\ ============ 0x53
{ char i;
  int16_t L;
  
  UoutARR[0] = ADDR_SERV; //\ 0x3F 
  UoutARR[2] = REQUEST_SETTING_MODE; //\ 0x53
 

 switch (Tk)
   {
    case 0x01: //\ Tk == 1
     {
      if ( Np == 0x00) //\ запрос режима работы
      { 
       
       UoutARR[3] = status.mode; //\ калабр/ребота/не исправен
       UoutARR[4] = 0x00;
     } else if ( Np == 0x01 ) //\ включение калибровки 
     {
      status.mode = 0x01; //\ режим калибровки
          //Режим калибровки угла. В этом режиме калибровочные данные и параметры не 
          //сохраняются в энергонезависимой памяти микроконтроллера, а 
          //хранятся до выключения питания датчика. Чтобы сохранить данные в энергонезависимой 
          //памяти нужно перейти в рабочий режим. Данные сохранятся 
          //в момент перехода из режима калибровки в рабочий режим.      
      UoutARR[3] = status.mode;
      UoutARR[4] = 0x00;
     } else if ( Np == 0x02 )
     {
      status.mode = 0x02; //\ режим работы
      
      i = ( ( parameters.max_number_discrete/parameters.max_number_turns <= 128) &&
            ( parameters.max_number_turns >= 32))? 1 : 0;
      if (i)
      {
       UoutARR[3] = status.mode;
       UoutARR[4] = 0x00;
      }
      else
      {
        UoutARR[3] = 0xAA;
        UoutARR[4] = 0xAA;
      }
     } else if ( Np == 0x03 ) //\ установка 0 в текущем месте
     {
      parameters.angle_offset_1 = angl[0].angle_no_correct;        // угод между 0 энкодера и установленным значением
      parameters.angle_offset_2 = angl[1].angle_no_correct;
      angl[0].turns = 0;
      angl[1].turns = 0;

      angl[0].old_angle = angl[0].angle_no_offset = 0x00;
      angl[1].old_angle = angl[1].angle_no_offset = 0x00;
      FRAM_WRITE ( FRAM_Glob, parameters.data, 0x0F );
      
      UoutARR[3] = 0xA5;
      UoutARR[4] = 0xA5;
     } else if ( Np == 0x04 ) //\ обнуление смещения
     {
       parameters.angle_offset_1 = 0x00;
       parameters.angle_offset_2 = 0x00;
    
       UoutARR[3] = 0x00;
       UoutARR[4] = 0x00;
     } else if ( Np == 0xFE ) //\ Reset
     {
       UoutARR[3] = 'R';
       UoutARR[4] = 'E';
      
       Delay (10000);
       RCC->APB1ENR |= 0x0800;    // тактирование WDT
       WWDG->CR  = 0x80 | 0x7F | 0x40; //0x3f   // вкл | зчначение (T) | 6-й бит!!!!!
       WWDG->CFR = 0x0180 | 0x0040 | 0x0200; // предделитель /8 (0x0180) | счёт до 0x0040 (W) | 0x0200 - EN
     } else return; //\ НЕ запуская обмен 
      break;
    } // Tk == 0x01:
    case 0x11:  // Tk == 0x11
    {
      if ( Np == 0x08 ) //\ OffSet CS1
      {
       UoutARR[3] = parameters.angle_offset_1;
       UoutARR[4] = parameters.angle_offset_1 >> 8;
      } else if ( Np == 0x11 ) //\ OffSet CS2
      {
       UoutARR[3] = parameters.angle_offset_1;
       UoutARR[4] = parameters.angle_offset_1 >> 8;
      } else if ( Np == 0x18 ) //\ статус калибровки
      {
        if (status.mode == 0x02) //\откалиброван
        {
         UoutARR[3] = 0xA5;
         UoutARR[4] = 0xA5;
        }
        else
        {
         UoutARR[3] = 0x00;
         UoutARR[4] = 0x00;
        }
      } else return; //\ НЕ запуская обмен 
     break;
    }// Tk == 0x11
    case 0xAE:
    {
      if ( Np == 0x01 ) //\ обороты CS1
      {
//#define debug
#ifndef debug
       UoutARR[3] = angl[0].turns;
       UoutARR[4] = angl[0].turns >> 8;
#endif
         
#ifdef  debug
        UoutARR[3] = angl[0].W_FORM;
        UoutARR[4] = angl[0].W_FORM >> 8;
#endif       
      } else if ( Np == 0x02 ) //\ обороты CS1
      {
#ifdef  debug
        UoutARR[3] = angl[1].W_FORM;
        UoutARR[4] = angl[1].W_FORM >> 8;
#endif 
#ifndef debug        
       UoutARR[3] = angl[1].turns;
       UoutARR[4] = angl[1].turns >> 8; //\ CS2
#endif
      } else if ( Np == 3)
      {
       UoutARR[3] = angl[0].angle_no_offset;////parameters.angle_offset_1;//
       UoutARR[4] = angl[0].angle_no_offset >> 8;//parameters.angle_offset_1>>8;//////\исходный код
      } else if ( Np == 4)
      {
       UoutARR[3] = angl[1].angle_no_offset;//parameters.angle_offset_2;//.angle_no_offset;//
       UoutARR[4] = angl[1].angle_no_offset >> 8;//parameters.angle_offset_2>>8;//.angle_no_offset >> 8;//
      } else if ( Np == 5)
      {
#ifndef debug  
       UoutARR[3] = angl[0].angle;
       UoutARR[4] = angl[0].angle >> 8;
#endif
#ifdef debug       
       UoutARR[3] = angl[0].angle;
       UoutARR[4] = angl[0].angle >> 8;
#endif       
      } else if ( Np == 6)
      {
#ifndef debug         
       UoutARR[3] = angl[1].angle;
       UoutARR[4] = angl[1].angle >> 8;
#endif
#ifdef debug       
       UoutARR[3] = angl[1].angle;
       UoutARR[4] = angl[1].angle >> 8;
#endif       
      }else if ( Np == 7)
      {
       UoutARR[3] = angl[0].turns;
       UoutARR[4] = angl[0].turns >> 8; //\ как в рабочем режиме
      } else if ( Np == 8) // число дискрет на оборот
      {
        L = parameters.max_number_discrete/parameters.max_number_turns;
       UoutARR[3] = L;
       UoutARR[4] = L >> 8; //\ 
      } else return; //\ НЕ запуская обмен
     break;
    } // Tk == 0xAE
 }; //\ switch Tk
       
  UoutARR[5] = 0x00;
   
  for(int i = 0; i < 5; i++)
   UoutARR[5] ^= UoutARR[i];
  
 // UindexW = 0x00; // в 0 - й 
  UASRTtx = 0x05; // 6 байт
 // LIN_GPIO_Port->BSRR = LIN_Pin;  // LIN = 1
  USART1->SR  &= ~TC_I; // почистить
  USART1->CR1 &= ~TC_I; // отключить TС
  USART1->CR1 |= TX_I;  // вкл TX 
   (void) i;   
}//\ 0x53

void SettingSet ( unsigned char Nt, unsigned char Vn/*4*/, unsigned char  Vn1/*5*/ )  //-----------0x54
{ unsigned int L;

  UoutARR[0] = ADDR_SERV; //\ 0x3F 
  UoutARR[2] = REQUEST_SET_PARAMETERS; //\ 0x54
  UoutARR[3] = UinARR[3];
  
  L = Vn1 << 8 | Vn;  //\ устанваливаемое значение

 if ( Nt == 0x01 ) // адресная группа
 {
  if ( ( L == 1)    || 
       ( L == 2)    || 
       ( L == 3)    ||
       ( L == 4)    
     )
   {
    parameters.address_group = L;            
   }

  UoutARR[4] = parameters.address_group;
  UoutARR[5] = 0x00;
 } else if ( Nt == 0x02 ) //\ число оборотов
 {
   if ( L )
   {
    parameters.max_number_turns = L;
   }

    UoutARR[4] = parameters.max_number_turns;
    UoutARR[5] = parameters.max_number_turns >> 8;
 
 } else if ( Nt == 0x03 ) //\ число дискрет
 {
 /*  if ( L && L <= 0x0F ) 
   {
    parameters.max_degree_discrete = L;
    parameters.max_number_discrete = 1 << parameters.max_degree_discrete;
   }
   for(parameters.max_degree_discrete = 0; 
       parameters.max_number_discrete >> parameters.max_degree_discrete != 1; 
       parameters.max_degree_discrete++); */
   if ( L )
     parameters.max_number_discrete = L;
   
   UoutARR[4] = parameters.max_number_discrete;
   UoutARR[5] = parameters.max_number_discrete >> 8;
 } else if ( Nt == 0x04 ) //\ направление вращения
 {
  if( (L == CLOCKWISE) || 
      (L == COUNTER_CLOCKWISE ) )
   {
    parameters.direction_rotation = L;
   }     
  
  UoutARR[4] = parameters.direction_rotation;
  UoutARR[5] = parameters.direction_rotation >> 8;
 } else if ( Nt == 0x05 )  //\ предустановленные
 {
  if ( L && L <= 0x24 )
  {
   Sensor_Number_Set( L ); //\ утановка 
  }
   UoutARR[4] = 0x00;
   UoutARR[5] = 0x00;
 } else if ( Nt == 0x30 ) //\ серийны номер
 {
  parameters.serial_number_Low = L;
  
  UoutARR[4] = parameters.serial_number_Low;
  UoutARR[5] = parameters.serial_number_Low >> 8;
 } else if ( Nt == 0x31 ) //\ серийны номер
 {
  parameters.serial_number_High = L;
  
  UoutARR[4] = parameters.serial_number_High;
  UoutARR[5] = parameters.serial_number_High >> 8;
 } else if ( Nt == 0x00 ) //\ коэффициент редукции
 {
   UoutARR[4] = 32;
   UoutARR[5] = 0x00;
 }else return;
 

  UoutARR[6] = 0x00;
  
  for(int i = 0; i < 6; i++)
  UoutARR[6] ^= UoutARR[i];
  
 // UindexW = 0x00; // в 0 - й 
  UASRTtx = 0x06; // 7 байт
  
  // LIN_GPIO_Port->BSRR = LIN_Pin;  // LIN = 1
  USART1->SR  &= ~TC_I; // почистить
  USART1->CR1 &= ~TC_I; // отключить TС
  USART1->CR1 |= TX_I;  // вкл TX  
} //\ 0x54 

