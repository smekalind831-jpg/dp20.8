
#include "FRAM.h"

//\ ============================ Запись FRAM ===================================
void FRAM_WRITE ( unsigned int ADR, unsigned char* arr, unsigned char Size )
{ unsigned char L;
 
   SPI1->CR1 &= ~0x01;  // первое изменение (подъём)
    
 //  CSx_Port->BSRR = 0x0800;       // HOLD = 1
   MS_Port->BSRR = 0x0400 << 16; // MS = 0
   
    Delay (Tic2);  //\ под ??
     
    // L = ReadStatusRegister ( void ); //\ проверка на готовность
    // if ( !( L & WEL) ) return;   //\ ничего не получилось 
    
    W_Protection(0x00);   // разрешил запись
    MS_Port->BSRR = 0x0400 << 16; // MS = 0   
   
// L = R_Status ();
    Output1 [0] = WRITE;    // запись
    Output1 [1] = ADR >> 8; // адрес
    Output1 [2] = ADR;
    
    FRWR = 3;      // только команды
    L = SPI1->DR;   // снять лишнее прерывание
    
    SPI1->CR2 |= 0xC0; //\ запуск
    while (FRWR) asm ("NOP");

   //\ === копирование ===========
    for (L = 0; L < Size; L++) //\ один раз весь цикл пробежит
    { 
     Output1 [L] = *arr;
     arr++;
    }
   
   FRWR = Size;
   SPI1->CR2 |= 0xC0; //\ запуск
 
  while ( FRWR ) asm ("NOP");   // дождаться передачи

  Delay(Tic2);
  W_Protection(0x01);   // запретил запись
  
  MS_Port->BSRR = 0x0400;       // MS = 1
  SPI1->CR1 |= 0x01; // второе изменение (спад)
//  CSx_Port->BSRR = 0x0800 << 16; // HOLD = 0  
   
}//\ FRAM_WRITE

//\ ============================== чтение ======================================
void FRAM_READ ( unsigned int ADR, unsigned char Size )
{ unsigned char L;
 
   SPI1->CR1 &= ~0x01;  // первое изменение (подъём)
    
 //  CSx_Port->BSRR = 0x0800; // HOLD = 1
   MS_Port->BSRR = 0x0400 << 16; // MS = 0
   Delay(Tic2);
  
    
    Output1 [0] = READ;    // чтение
    Output1 [1] = ADR >> 8; // адрес
    Output1 [2] = ADR;
    FRWR = 3;      // только команды
 
    L = SPI1->DR;   // снять лишнее прерывание
    SPI1->CR2 |= 0xC0; //\ запуск

   while ( FRWR ) asm ("NOP");
    //\ === копирование ===========
    for (L = 0; L < Size; L++) //\ один раз весь цикл пробежит
    { 
     Output1 [L] = 0xA5;
    }
   
   FRWR = Size;
   SPI1->CR2 |= 0xC0; //\ запуск
   
  while ( FRWR ) asm ("NOP");   // дождаться передачи
  
  Delay(Tic2);
  // ? задержка ?  
 
  MS_Port->BSRR = 0x0400;       // MS = 1  
  SPI1->CR1 |= 0x01; // второе изменение (спад)
  
  // CSx_Port->BSRR = 0x0800 << 16; // HOLD = 0  
  
  // (SPI1->SR & 0x02);
}//\ чтение

//\ ================================ ON/OFF ====================================
void W_Protection (unsigned char en)
{  unsigned char byte;
  
  if(en) byte = WRDI;   //\ излишне, при MS _/ сам закроется
   else byte = WREN;
  
   __disable_irq();   
   SPI1->CR1 &= ~0x01; // первое изменение (подъём)   
 
 //  CSx_Port->BSRR = 0x0800; // HOLD = 1
   MS_Port->BSRR = 0x0400 << 16; // MS = 0
    // ? задержка ? 
  
  SPI1->DR = byte; // команда
  while ( !( SPI1->SR & 0x01 ) ) asm ("NOP"); //\прерывание по приёму (сдвиговый пуст)
   byte = SPI1->DR; //\ снять лишнее
   
 //  Delay(1);
   
   MS_Port->BSRR = 0x0400;       // MS = 1
//  CSx_Port->BSRR = 0x0800 << 16; // HOLD = 0  
  __enable_irq();
  return;
} //\ ON/OFF

// ================== Status R ==========================
unsigned char R_Status (void)
{ uint8_t StatusRegister;
  

  SPI1->CR1 &= ~0x01; // первое изменение (подъём)

  
 // CSx_Port->BSRR = HOLD_Pin;     // HOLD = 1 \возможно HOLD - лишнее
  MS_Port->BSRR = 0x0400 << 16; // MS = 0 
  __disable_irq();
  
  Delay(Tic2);
   // W_Protection(0x00);   // разрешил запись
 //   MS_Port->BSRR = MS_Pin << 16; // MS = 0 
  
  SPI1->DR = RDSR;   //\ команда чтение статуса
  while ( !( SPI1->SR & 0x01 ) ) asm ("NOP"); //\ пока заняться нечем
  StatusRegister = SPI1->DR;   //\ сбросить флаг
  
  SPI1->DR = 0xA5;
  while ( !( SPI1->SR & 0x01 ) ) asm ("NOP"); //\ пока заняться нечем
  StatusRegister = SPI1->DR;
 
  Delay(Tic2);
  
  MS_Port->BSRR = 0x0400;       // MS = 1
 
  SPI1->CR1 |= 0x01; // второе изменение (спад) 
//  CSx_Port->BSRR = 0x0800 << 16; // HOLD = 0  
  __enable_irq();

 return StatusRegister;
}

// ========================= Status W ==================
void W_Status (uint8_t Status)
{ char tmp;	


  SPI1->CR1 &= ~0x01; // первое изменение (подъём)

  
 // CSx_Port->BSRR = HOLD_Pin; // HOLD = 1 \возможно HOLD - лишнее
  MS_Port->BSRR = 0x0400 << 16; // MS = 0
  __disable_irq();
  
  Delay (Tic2);     //\ пока отладка 
        
  W_Protection (0); // разрешил запись
 
  SPI1->DR = WRSR;  // запись статуса
  while ( !( SPI1->SR & 0x01 ) ) asm ("NOP"); //\ пока заняться нечем
  tmp = SPI1->DR;   // лишнее прерывание
        
  SPI1->DR = Status; // регистр
  while ( !( SPI1->SR & 0x01 ) ) asm ("NOP"); //\ пока заняться нечем
  tmp = SPI1->DR;   // лишнее прерывание

  W_Protection (1); // запретил запись (возможно логику придётся менять)
        



  
  Delay (Tic2);     //\ пока отладка 
        
  MS_Port->BSRR = 0x0400;  // MS = 1 
  SPI1->CR1 |= 0x01; // второе изменение (спад)

  // ? задержка ?
 // CSx_Port->BSRR = HOLD_Pin << 16;  // HOLD = 0
    
    (void)tmp;
    __enable_irq();
}

// ========================= Bute R ===================
unsigned char R_Bute (uint16_t ADR)
{  uint8_t Ans;
 

  SPI1->CR1 &= ~0x01; // первое изменение (подъём)

  
 // CSx_Port->BSRR = HOLD_Pin; // HOLD = 1 \возможно HOLD - лишнее  (uint32_t)
  MS_Port->BSRR = 0x0400 << 16; // MS = 0 
  
  __disable_irq();
  Delay(Tic2);
    
  SPI1->DR = READ;   // команда чтения
  while (!( SPI1->SR & 0x01 ) ) asm ("NOP"); //\ пока заняться нечем
  Ans = SPI1->DR;   //\ сбросить флаг
  
  SPI1->DR = ADR >> 8; // адрес H
  while ( !( SPI1->SR & 0x01 ) ) asm ("NOP");
  Ans = SPI1->DR;
  
  SPI1->DR = ADR; // адрес L
  while ( !( SPI1->SR & 0x01 ) ) asm ("NOP");
  Ans = SPI1->DR;

//  for (i=0; i<100; i++)
  {
  SPI1->DR = 0xA5; // чтение
  while ( !( SPI1->SR & 0x01 ) ) asm ("NOP");
  Ans = SPI1->DR;
  }
  
  Delay(Tic2);
  //\ ? задержка ?
  MS_Port->BSRR = 0x0400; // MS = 1 
 
  SPI1->CR1 |= 0x01; // второе изменение (спад)

  
  __enable_irq();
 return Ans;
}

//\ ========================= Bute W ===================
void W_Bute (uint16_t ADR, unsigned char Bute)
{
 uint8_t Ans;
 

  SPI1->CR1 &= ~0x01; // первое изменение (подъём)
 
 // CSx_Port->BSRR = HOLD_Pin; // HOLD = 1 \возможно HOLD - лишнее
 // MS_Port->BSRR = MS_Pin << 16; // MS = 0 
   __disable_irq();
    //\ ? задержка 
  
  W_Protection(0x00);   // разрешил запись
  MS_Port->BSRR = 0x0400 << 16; // MS = 0   
  Delay(Tic2);
  
  SPI1->DR = WRITE;     // команда записи
  while ( !( SPI1->SR & 0x01 ) ) asm ("NOP"); //\ пока заняться нечем
  Ans = SPI1->DR;   //\ сбросить флаг

  SPI1->DR = ADR >> 8; // адрес H
  while ( !( SPI1->SR & 0x01 ) ) asm ("NOP");
  Ans = SPI1->DR;
  
  SPI1->DR = ADR; // адрес L
  while ( !( SPI1->SR & 0x01 ) ) asm ("NOP");
  Ans = SPI1->DR;

  SPI1->DR = Bute; // запись
  while ( !( SPI1->SR & 0x01 ) ) asm ("NOP");
  Ans = SPI1->DR;

  W_Protection(0x01);   // запретил запись
 
  Delay(Tic2);
  //\ ? задержка ?
  MS_Port->BSRR = 0x0400; // MS = 1 
   //\ ? задержка ?
 // CSx_Port->BSRR = (HOLD_Pin << 16); // HOLD = 0
 

  SPI1->CR1 |= 0x01; // второе изменение (спад)

  
  (void) Ans;
  __enable_irq();
}//\ W_Bute
