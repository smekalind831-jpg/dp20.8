
#include "ReadSC.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_tim.h"
#include "Defs1.h"

//\=============== запуск чтения ТОЛЬКО инкодеров==============
void SPI_READ (Con SP1, Con SP2)
{
 char I; 
  
  //\ =============  SPI2 читать всегда ===============
  CSx_Port->BSRR = CS1_Pin << 16; // CS1 = 0 запускается всегда
 

 //\ ============ SPI1 читать энкодер ==================
  if ( SP1.RC )
  {  
  SPI1->CR1 &= ~0x40;
  SPI1->CR1 |= 0x01; //   
  SPI1->CR1 |= 0x40;
//   SPI1->CR1 |= 0x01;  // второе изменение (спад)    
  
   for (I = 0; I <= 2; I++)
     Output1 [I] = 0xA5;
     
   CSx_Port->BSRR = CS2_Pin << 16; // CS2/CS3 = 0 
  } //\ ( SP1.RC )
  
 //  Delay (Tic1);  //\ пока отладка 
  
   SPI2->CR2 |= 0xC0;   //\ прерывание передатчик (0x1UL << SPI_CR2_TXEIE_Pos) (0x08)
                        //\ приёмник полон (0x1UL << SPI_CR2_RXNEIE_Pos) (0x04)
   if ( SP1.RC ) SPI1->CR2 |= 0xC0; //\ этапы CS1 + CS2, CS1 + CS3  
  
  return;
}
//\======================= Задержка 0,1 мс ========================
void Delay (unsigned int Tim)
{
  TIM3->CNT = 0x0000;    // обнулил таймер
  TIM3->CR1 |= TIM_CR1_CEN;  // запустил таймер3
  
   while ( TIM3->CNT <= Tim )           
     asm ("NOP");
   
  TIM3->CR1 &= ~(TIM_CR1_CEN);  // остановил таймер3
}