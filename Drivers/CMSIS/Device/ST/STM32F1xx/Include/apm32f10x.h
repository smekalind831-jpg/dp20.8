/*!
 * @file       apm32f10x.h
 *
 * @brief      CMSIS Cortex-M3 Device Peripheral Access Layer Header File.
 *
 * @details    This file contains all the peripheral register's definitions, bits definitions and memory mapping
 *
 * @version    V1.0.0
 *
 * @date       2019-8-5
 *
 */

#ifndef __APM32F10x_H
#define __APM32F10x_H

#ifdef __cplusplus
 extern "C" {
#endif 

/*!
 *  APM32F10X_LD: APM32 Low density devices
 *  APM32F10X_MD: APM32 Medium density devices
 *  APM32F10X_HD: APM32 High density devices
 */
 /*
#if !defined (APM32F10X_LD)  && !defined (APM32F10X_MD) && !defined (APM32F10X_HD)
#error "Please select a the target APM32F10x device used in your application (in apm32f10x.h file)"
#endif
 */
/**
 * @brief Define Value of the External oscillator in Hz
  */           
#ifndef  HXT_VALUE 
 #define HXT_VALUE                  ((uint32_t)16000000)
//- #define HXT_VALUE                  ((uint32_t)8000000)
#endif

/** Time out for HXT start up */
#define HXT_STARTUP_TIMEOUT         ((uint16_t)0x0500)

/** Value of the Internal oscillator in Hz */
#define HIRC_VALUE                  ((uint32_t)8000000)

/**
 * @brief APM32F10x Standard Peripheral Library version number
   */
#define __APM32F10X_STDPERIPH_VERSION_MAIN   (0x01) /*!< [31:24] main version */                                  
#define __APM32F10X_STDPERIPH_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __APM32F10X_STDPERIPH_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __APM32F10X_STDPERIPH_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __APM32F10X_STDPERIPH_VERSION       ( (__APM32F10X_STDPERIPH_VERSION_MAIN << 24)\
                                             |(__APM32F10X_STDPERIPH_VERSION_SUB1 << 16)\
                                             |(__APM32F10X_STDPERIPH_VERSION_SUB2 << 8)\
                                             |(__APM32F10X_STDPERIPH_VERSION_RC))


/** APM32 devices does not provide an MPU */
 #define __MPU_PRESENT                  0
/** APM32 uses 4 Bits for the Priority Levels  */
#define __NVIC_PRIO_BITS                4
/** Set to 1 if different SysTick Config is used */
#define __Vendor_SysTickConfig          0

/**
 * @brief APM32F10x Interrupt Number Definition, according to the selected device 
 *        in @ref Library_configuration_section 
 */
typedef enum
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
  HardFault_IRQn              = -13,    /*!< 3 Cortex-M3 Hard Fault Interrupt                     */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt              */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                      */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                    */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                       */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                   */

/******  STM32 specific Interrupt Numbers *********************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                            */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt            */
  TAMPER_IRQn                 = 2,      /*!< Tamper Interrupt                                     */
  RTC_IRQn                    = 3,      /*!< RTC global Interrupt                                 */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                               */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                 */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                 */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                 */
  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                 */
  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                 */
  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                 */
  DMA1_Channel1_IRQn          = 11,     /*!< DMA1 Channel 1 global Interrupt                      */
  DMA1_Channel2_IRQn          = 12,     /*!< DMA1 Channel 2 global Interrupt                      */
  DMA1_Channel3_IRQn          = 13,     /*!< DMA1 Channel 3 global Interrupt                      */
  DMA1_Channel4_IRQn          = 14,     /*!< DMA1 Channel 4 global Interrupt                      */
  DMA1_Channel5_IRQn          = 15,     /*!< DMA1 Channel 5 global Interrupt                      */
  DMA1_Channel6_IRQn          = 16,     /*!< DMA1 Channel 6 global Interrupt                      */
  DMA1_Channel7_IRQn          = 17,     /*!< DMA1 Channel 7 global Interrupt                      */
  ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                       */
  USB_HP_CAN1_TX_IRQn         = 19,     /*!< USB Device High Priority or CAN1 TX Interrupts       */
  USB_LP_CAN1_RX0_IRQn        = 20,     /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
  CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
  CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */
  EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
  TIM1_BRK_IRQn               = 24,     /*!< TIM1 Break Interrupt                                 */
  TIM1_UP_IRQn                = 25,     /*!< TIM1 Update Interrupt                                */
  TIM1_TRG_COM_IRQn           = 26,     /*!< TIM1 Trigger and Commutation Interrupt               */
  TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
  TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
  TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                */
  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
  I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
  I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
  SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
  USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
  EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
  RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
  USBWakeUp_IRQn              = 42,     /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
} IRQn_Type;

/**
  * @}
  */

#include "core_cm3.h"
#include "system_apm32f10x.h"
#include <stdint.h>


typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;
typedef const int16_t sc16;
typedef const int8_t sc8;

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;
typedef __I int16_t vsc16;
typedef __I int8_t vsc8;

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;
typedef const uint16_t uc16;
typedef const uint8_t uc8; 

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;
typedef __I uint16_t vuc16;
typedef __I uint8_t vuc8;

#ifndef __IM
  #define __IM   __I
#endif
#ifndef __OM  
  #define __OM   __O
#endif
#ifndef __IOM 
  #define __IOM  __IO
#endif

enum {BIT_RESET, BIT_SET};
enum {RESET, SET};
enum {DISABLE, ENABLE};
enum {ERROR, SUCCESS};

#ifndef NULL 
#define NULL   ((void *)0)
#endif

#if defined (__CC_ARM )
#pragma anon_unions
#endif

/**
  * @brief Reset and clock management unit (RCM)
  */
typedef struct 
{                               
   /** Clock control register */
   union 
   {
      __IOM uint32_t CTRL;                         

      struct 
      {
           __IOM uint32_t HIRCEN        : 1;           
           __IM  uint32_t HIRCRDYF      : 1;           
           __IM  uint32_t RESERVED1     : 1;
           __IOM uint32_t HIRCTRIM      : 5;           
           __IM  uint32_t HIRCCAL       : 8;           
           __IOM uint32_t HXTEN         : 1;           
           __IM  uint32_t HXTRDYF       : 1;           
           __IOM uint32_t HXTBYP        : 1;           
           __IOM uint32_t CSSEN         : 1;           
           __IM  uint32_t RESERVED2     : 4;
           __IOM uint32_t PLLEN         : 1;           
           __IM  uint32_t PLLRDYF       : 1; 
           __IM  uint32_t RESERVED3     : 6;
      } CTRL_B;
    } ;
        
   /** Clock configuration register */
   union 
   {
      __IOM uint32_t CFG;                       
   
      struct 
      {
         __IOM uint32_t SCSEL           : 2;           
         __IM  uint32_t SCS             : 2;           
         __IOM uint32_t AHBDIV          : 4;           
         __IOM uint32_t APB1DIV         : 3;           
         __IOM uint32_t APB2DIV         : 3;           
         __IOM uint32_t ADCDIV          : 2;           
         __IOM uint32_t PLLSEL          : 1;           
         __IOM uint32_t PLLXTDIV        : 1;           
         __IOM uint32_t PLLMF           : 4;           
         __IOM uint32_t USBDIV          : 1;           
         __IM  uint32_t RESERVED1       : 1;
         __IOM uint32_t COC             : 3;           
         __IOM uint32_t FPUDIV          : 1;   
         __IM  uint32_t RESERVED2       : 4;
      } CFG_B;
   } ;
     
   /** Clock interrupt control register */
   union 
   {
      __IOM uint32_t INT;                        

      struct 
      {
         __IM  uint32_t LIRCRDYIF       : 1;           
         __IM  uint32_t LXTRDYIF        : 1;           
         __IM  uint32_t HIRCRDYIF       : 1;           
         __IM  uint32_t HXTRDYIF        : 1;           
         __IM  uint32_t PLLRDYIF        : 1;           
         __IM  uint32_t RESERVED1       : 2;
         __IM  uint32_t CSSIF           : 1;           
         __IOM uint32_t LIRCRDYIE       : 1;           
         __IOM uint32_t LXTRDYIE        : 1;           
         __IOM uint32_t HIRCRDYIE       : 1;           
         __IOM uint32_t HXTRDYIE        : 1;           
         __IOM uint32_t PLLRDYIE        : 1;           
         __IM  uint32_t RESERVED2       : 3;
         __OM  uint32_t LIRCRDYC        : 1;           
         __OM  uint32_t LXTRDYC         : 1;           
         __OM  uint32_t HIRCRDYC        : 1;           
         __OM  uint32_t HXTRDYC         : 1;           
         __OM  uint32_t PLLRDYC         : 1;           
         __IM  uint32_t RESERVED3       : 2;
         __OM  uint32_t CSSIFC          : 1; 
         __IM  uint32_t RESERVED4       : 8;
      } INT_B;
   } ;
  
   /** APB2 peripheral reset register */
   union 
   {
      __IOM uint32_t APB2RST;                   

      struct 
      {
         __IOM uint32_t AFIORST         : 1;           
         __IM  uint32_t RESERVED1       : 1;
         __IOM uint32_t PARST           : 1;           
         __IOM uint32_t PBRST           : 1;           
         __IOM uint32_t PCRST           : 1;           
         __IOM uint32_t PDRST           : 1;           
         __IOM uint32_t PERST           : 1;           
         __IOM uint32_t PFRST           : 1;           
         __IOM uint32_t PGRST           : 1;           
         __IOM uint32_t ADC1RST         : 1;           
         __IOM uint32_t ADC2RST         : 1;           
         __IOM uint32_t TMR1RST         : 1;           
         __IOM uint32_t SPI1RST         : 1;           
         __IOM uint32_t TMR8RST         : 1;           
         __IOM uint32_t USART1RST       : 1;           
         __IOM uint32_t ADC3RST         : 1;           
         __IM  uint32_t RESERVED2       : 16;          
      } APB2RST_B;
   } ;
  
   /** APB1 peripheral reset register */
   union {
      __IOM uint32_t APB1RST;                   

      struct 
      {
         __IOM uint32_t TMR2RST         : 1;           
         __IOM uint32_t TMR3RST         : 1;           
         __IOM uint32_t TMR4RST         : 1;           
         __IOM uint32_t TMR5RST         : 1;           
         __IOM uint32_t TMR6RST         : 1;           
         __IOM uint32_t TMR7RST         : 1;           
         __IOM uint32_t RESERVED1       : 5;           
         __IOM uint32_t WWDTRST         : 1;           
         __IM  uint32_t RESERVED2       : 2;
         __IOM uint32_t SPI2RST         : 1;           
         __IOM uint32_t SPI3RST         : 1;           
         __IM  uint32_t RESERVED3       : 1;
         __IOM uint32_t USART2RST       : 1;           
         __IOM uint32_t USART3RST       : 1;           
         __IOM uint32_t UART4RST        : 1;           
         __IOM uint32_t UART5RST        : 1;           
         __IOM uint32_t I2C1RST         : 1;           
         __IOM uint32_t I2C2RST         : 1;           
         __IOM uint32_t USBRST          : 1;           
         __IM  uint32_t RESERVED4       : 1;
         __IOM uint32_t CANRST          : 1;           
         __IM  uint32_t RESERVED5       : 1;
         __IOM uint32_t BAKRRST         : 1;           
         __IOM uint32_t PMURST          : 1;           
         __IOM uint32_t DACRST          : 1;
         __IM  uint32_t RESERVED6       : 2;
      } APB1RST_B;
   } ;

   /** AHB clock enable register */
   union 
   {
      __IOM uint32_t AHBCLKEN;                     

      struct 
      {
         __IOM uint32_t DMA1CLKE        : 1;           
         __IOM uint32_t DMA2CLKE        : 1;           
         __IOM uint32_t SRAMCLKE        : 1;           
         __IOM uint32_t FPUCLKE         : 1;         
         __IOM uint32_t FMCCLKE         : 1;           
         __IOM uint32_t QSPICLKE        : 1;         
         __IOM uint32_t CRCCLKE         : 1;           
         __IM  uint32_t RESERVED1       : 1;
         __IOM uint32_t EMMCCLKEN       : 1;           
         __IM  uint32_t RESERVED2       : 1;
         __IOM uint32_t SDIOCLKE        : 1; 
         __IM  uint32_t RESERVED3       : 21;
      } AHBCLKEN_B;
   } ;

   /** APB2 clock enable register */
   union 
   {
      __IOM uint32_t APB2CLKEN;                    

      struct 
      {
         __IOM uint32_t AFIOCLKE        : 1;           
         __IM  uint32_t RESERVED1       : 1;
         __IOM uint32_t PACLKE          : 1;           
         __IOM uint32_t PBCLKE          : 1;           
         __IOM uint32_t PCCLKE          : 1;           
         __IOM uint32_t PDCLKE          : 1;           
         __IOM uint32_t PECLKE          : 1;           
         __IOM uint32_t PFCLKE          : 1;           
         __IOM uint32_t PGCLKE          : 1;           
         __IOM uint32_t ADC1CLKE        : 1;           
         __IOM uint32_t ADC2CLKE        : 1;           
         __IOM uint32_t TMR1CLKE        : 1;           
         __IOM uint32_t SPI1CLKE        : 1;           
         __IOM uint32_t TMR8CLKE        : 1;           
         __IOM uint32_t USART1CLKE      : 1;           
         __IOM uint32_t ADC3CLKE        : 1;           
         __IM  uint32_t RESERVED2       : 16;        
      } APB2CLKEN_B;
   };
  
   /** APB1 clock enable register */
   union 
   {
      __IOM uint32_t APB1CLKEN;                    

      struct 
      {
         __IOM uint32_t TMR2CLKE        : 1;           
         __IOM uint32_t TMR3CLKE        : 1;           
         __IOM uint32_t TMR4CLKE        : 1;           
         __IOM uint32_t TMR5CLKE        : 1;           
         __IOM uint32_t TMR6CLKE        : 1;           
         __IOM uint32_t TMR7CLKE        : 1;                      
         __IM  uint32_t RESERVED1       : 5;
         __IOM uint32_t WWDTCLKE        : 1;           
         __IM  uint32_t RESERVED2       : 2;
         __IOM uint32_t SPI2CLKE        : 1;           
         __IOM uint32_t SPI3CLKE        : 1;           
         __IM  uint32_t RESERVED3       : 1;
         __IOM uint32_t USART2CLKE      : 1;           
         __IOM uint32_t USART3CLKE      : 1;           
         __IOM uint32_t UART4CLKE       : 1;           
         __IOM uint32_t UART5CLKE       : 1;           
         __IOM uint32_t I2C1CLKE        : 1;           
         __IOM uint32_t I2C2CLKE        : 1;           
         __IOM uint32_t USBCLKE         : 1;           
         __IM  uint32_t RESERVED4       : 1;
         __IOM uint32_t CANCLKE         : 1;           
         __IM  uint32_t RESERVED5       : 1;
         __IOM uint32_t BAKRCLKE        : 1;           
         __IOM uint32_t PMUCLKE         : 1;           
         __IOM uint32_t DACCLKE         : 1;    
         __IM  uint32_t RESERVED6       : 2;
      } APB1CLKEN_B;
   } ;
  
   /** Backup domain control register */
   union 
   {
      __IOM uint32_t BD;                       

      struct 
      {
         __IOM uint32_t LXTEN           : 1;           
         __IM  uint32_t LXTRDYF         : 1;           
         __IOM uint32_t LXTBYP          : 1;           
         __IM  uint32_t RESERVED1       : 5;
         __IOM uint32_t RTCSEL          : 2;           
         __IM  uint32_t RESERVED2       : 5;
         __IOM uint32_t RTCEN           : 1;           
         __IOM uint32_t BDRST           : 1;  
         __IM  uint32_t RESERVED3       : 15;
      } BD_B;
   } ;
  
   /** Control/status register */
   union 
   {
      __IOM uint32_t CSTS;                        

      struct 
      {
         __IOM uint32_t LIRCEN          : 1;           
         __IM  uint32_t LIRCRDYF        : 1;           
         __IM  uint32_t RESERVED1       : 22;
         __IOM uint32_t CLRRSTF         : 1;           
         __IM  uint32_t RESERVED2       : 1;
         __IOM uint32_t PINRSTF         : 1;           
         __IOM uint32_t PMURSTF         : 1;           
         __IOM uint32_t SWRSTF          : 1;           
         __IOM uint32_t IWDTRSTF        : 1;           
         __IOM uint32_t WWDTRSTF        : 1;           
         __IOM uint32_t LPRRSTF         : 1;           
      } CSTS_B;
   } ;
} RCM_T;                                        


/**
  * @brief General purpose I/O (GPIO)
  */
typedef struct 
{                               
  
   /** Port control register0 */
   union
   {
      __IOM uint32_t CTRL0;                        

      struct
      {
         __IOM uint32_t MOD0            : 2;           
         __IOM uint32_t CFG0            : 2;           
         __IOM uint32_t MOD1            : 2;           
         __IOM uint32_t CFG1            : 2;           
         __IOM uint32_t MOD2            : 2;           
         __IOM uint32_t CFG2            : 2;           
         __IOM uint32_t MOD3            : 2;           
         __IOM uint32_t CFG3            : 2;           
         __IOM uint32_t MOD4            : 2;           
         __IOM uint32_t CFG4            : 2;           
         __IOM uint32_t MOD5            : 2;           
         __IOM uint32_t CFG5            : 2;           
         __IOM uint32_t MOD6            : 2;           
         __IOM uint32_t CFG6            : 2;           
         __IOM uint32_t MOD7            : 2;           
         __IOM uint32_t CFG7            : 2;           
      } CTRL0_B;
   } ;

   /** Port control register1 */
   union 
   {
      __IOM uint32_t CTRL1;                        

      struct 
      {
         __IOM uint32_t MOD8            : 2;           
         __IOM uint32_t CFG8            : 2;           
         __IOM uint32_t MOD9            : 2;           
         __IOM uint32_t CFG9            : 2;           
         __IOM uint32_t MOD10           : 2;           
         __IOM uint32_t CFG10           : 2;           
         __IOM uint32_t MOD11           : 2;           
         __IOM uint32_t CFG11           : 2;           
         __IOM uint32_t MOD12           : 2;           
         __IOM uint32_t CFG12           : 2;           
         __IOM uint32_t MOD13           : 2;           
         __IOM uint32_t CFG13           : 2;           
         __IOM uint32_t MOD14           : 2;           
         __IOM uint32_t CFG14           : 2;           
         __IOM uint32_t MOD15           : 2;           
         __IOM uint32_t CFG15           : 2;           
      } CTRL1_B;
   } ;

     /** Port data in register */
   union 
   {
      __IM  uint32_t DIN;                        

      struct 
      {
         __IM  uint32_t DIN0            : 1;           
         __IM  uint32_t DIN1            : 1;           
         __IM  uint32_t DIN2            : 1;           
         __IM  uint32_t DIN3            : 1;           
         __IM  uint32_t DIN4            : 1;           
         __IM  uint32_t DIN5            : 1;           
         __IM  uint32_t DIN6            : 1;           
         __IM  uint32_t DIN7            : 1;           
         __IM  uint32_t DIN8            : 1;           
         __IM  uint32_t DIN9            : 1;           
         __IM  uint32_t DIN10           : 1;           
         __IM  uint32_t DIN11           : 1;           
         __IM  uint32_t DIN12           : 1;           
         __IM  uint32_t DIN13           : 1;           
         __IM  uint32_t DIN14           : 1;           
         __IM  uint32_t DIN15           : 1;   
         __IM  uint32_t RESERVED        : 16;
      } DIN_B;
   } ;
     
   /** Port data output register */
   union 
   {
      __IOM uint32_t DOUT;                        

      struct 
      {
         __IOM uint32_t DOUT0           : 1;           
         __IOM uint32_t DOUT1           : 1;           
         __IOM uint32_t DOUT2           : 1;           
         __IOM uint32_t DOUT3           : 1;           
         __IOM uint32_t DOUT4           : 1;           
         __IOM uint32_t DOUT5           : 1;           
         __IOM uint32_t DOUT6           : 1;           
         __IOM uint32_t DOUT7           : 1;           
         __IOM uint32_t DOUT8           : 1;           
         __IOM uint32_t DOUT9           : 1;           
         __IOM uint32_t DOUT10          : 1;           
         __IOM uint32_t DOUT11          : 1;           
         __IOM uint32_t DOUT12          : 1;           
         __IOM uint32_t DOUT13          : 1;           
         __IOM uint32_t DOUT14          : 1;           
         __IOM uint32_t DOUT15          : 1;    
         __IM  uint32_t RESERVED        : 16;
      } DOUT_B;
   } ;
  
   /** Port bit set/clear register */
   union 
   {
      __OM  uint32_t BSC;                       
         
      struct 
      {
         __OM  uint32_t BS0             : 1;           
         __OM  uint32_t BS1             : 1;           
         __OM  uint32_t BS2             : 1;           
         __OM  uint32_t BS3             : 1;           
         __OM  uint32_t BS4             : 1;           
         __OM  uint32_t BS5             : 1;           
         __OM  uint32_t BS6             : 1;           
         __OM  uint32_t BS7             : 1;           
         __OM  uint32_t BS8             : 1;           
         __OM  uint32_t BS9             : 1;           
         __OM  uint32_t BS10            : 1;           
         __OM  uint32_t BS11            : 1;           
         __OM  uint32_t BS12            : 1;           
         __OM  uint32_t BS13            : 1;           
         __OM  uint32_t BS14            : 1;           
         __OM  uint32_t BS15            : 1;           
         __OM  uint32_t BC0             : 1;           
         __OM  uint32_t BC1             : 1;           
         __OM  uint32_t BC2             : 1;           
         __OM  uint32_t BC3             : 1;           
         __OM  uint32_t BC4             : 1;           
         __OM  uint32_t BC5             : 1;           
         __OM  uint32_t BC6             : 1;           
         __OM  uint32_t BC7             : 1;           
         __OM  uint32_t BC8             : 1;           
         __OM  uint32_t BC9             : 1;           
         __OM  uint32_t BC10            : 1;           
         __OM  uint32_t BC11            : 1;           
         __OM  uint32_t BC12            : 1;           
         __OM  uint32_t BC13            : 1;           
         __OM  uint32_t BC14            : 1;           
         __OM  uint32_t BC15            : 1;           
      } BSC_B;
   } ;
  
   /** Port bit clear register */
   union 
   {
      __OM  uint32_t BC;                        

      struct 
      {
         __OM  uint32_t BC0             : 1;           
         __OM  uint32_t BC1             : 1;           
         __OM  uint32_t BC2             : 1;           
         __OM  uint32_t BC3             : 1;           
         __OM  uint32_t BC4             : 1;           
         __OM  uint32_t BC5             : 1;           
         __OM  uint32_t BC6             : 1;           
         __OM  uint32_t BC7             : 1;           
         __OM  uint32_t BC8             : 1;           
         __OM  uint32_t BC9             : 1;           
         __OM  uint32_t BC10            : 1;           
         __OM  uint32_t BC11            : 1;           
         __OM  uint32_t BC12            : 1;           
         __OM  uint32_t BC13            : 1;           
         __OM  uint32_t BC14            : 1;           
         __OM  uint32_t BC15            : 1;   
         __IM  uint32_t RESERVED        : 16;
      } BC_B;
   } ;
  
   /** Port configuration lock register */
   union {
      __IOM uint32_t LOCK;                        

      struct 
      {
         __IOM uint32_t LCK0            : 1;            
         __IOM uint32_t LCK1            : 1;            
         __IOM uint32_t LCK2            : 1;            
         __IOM uint32_t LCK3            : 1;            
         __IOM uint32_t LCK4            : 1;            
         __IOM uint32_t LCK5            : 1;            
         __IOM uint32_t LCK6            : 1;            
         __IOM uint32_t LCK7            : 1;            
         __IOM uint32_t LCK8            : 1;            
         __IOM uint32_t LCK9            : 1;            
         __IOM uint32_t LCK10           : 1;            
         __IOM uint32_t LCK11           : 1;            
         __IOM uint32_t LCK12           : 1;            
         __IOM uint32_t LCK13           : 1;            
         __IOM uint32_t LCK14           : 1;            
         __IOM uint32_t LCK15           : 1;            
         __IOM uint32_t LCKK            : 1;      
         __IM  uint32_t RESERVED        : 16;
   } LOCK_B;
  } ;
} GPIO_T;                                      

/**
  * @brief Alternate function I/O (AFIO)
  */
typedef struct 
{                                
   /** Event control register */
   union 
   {
      __IOM uint32_t EVCTRL;                        

      struct 
      {
         __IOM uint32_t PINSEL          : 4;            
         __IOM uint32_t PORTSEL         : 3;            
         __IOM uint32_t EVOEN           : 1; 
         __IM  uint32_t RESERVED        : 24;
      } EVCTRL_B;
   } ;

     /** Alternate function IO remap and Serial wire JTAG configuration register */
   union {
      __IOM uint32_t REMAP;                        
                                              
      struct 
      {
         __IOM uint32_t SPI1_REMAP      : 1;            
         __IOM uint32_t I2C1_REMAP      : 1;            
         __IOM uint32_t USART1_REMAP    : 1;          
         __IOM uint32_t USART2_REMAP    : 1;          
         __IOM uint32_t USART3_REMAP    : 2;          
         __IOM uint32_t TMR1_REMAP      : 2;            
         __IOM uint32_t TMR2_REMAP      : 2;            
         __IOM uint32_t TMR3_REMAP      : 2;            
         __IOM uint32_t TMR4_REMAP      : 1;            
         __IOM uint32_t CAN_REMAP       : 2;            
         __IOM uint32_t PD01_REMAP      : 1;            
         __IOM uint32_t TMR5_CH4_REMAP  : 1;        
         __IOM uint32_t ADC1_ETI_REMAP  : 1;    
         __IOM uint32_t ADC1_ETR_REMAP  : 1;    
         __IOM uint32_t ADC2_ETI_REMAP  : 1;    
         __IOM uint32_t ADC2_ETR_REMAP  : 1;    
         __IM  uint32_t RESERVED1       : 3;
         __OM  uint32_t SWJ_CFG         : 3; 
         __IM  uint32_t RESERVED2       : 5;
      } REMAP_B;
   } ;

   /** External interrupt configuration register1 */   
   union {
      __IOM uint32_t EINTCFG1;                     

      struct 
      {
         __IOM uint32_t EINT0           : 4;            
         __IOM uint32_t EINT1           : 4;            
         __IOM uint32_t EINT2           : 4;            
         __IOM uint32_t EINT3           : 4; 
         __IM  uint32_t RESERVED        : 16;
      } EINTCFG1_B;
   } ;

   /** External interrupt configuration register2 */   
   union {
      __IOM uint32_t EINTCFG2;                     

      struct 
      {
         __IOM uint32_t EINT4           : 4;            
         __IOM uint32_t EINT5           : 4;            
         __IOM uint32_t EINT6           : 4;            
         __IOM uint32_t EINT7           : 4;  
         __IM  uint32_t RESERVED        : 16;
      } EINTCFG2_B;
   } ;

   /** External interrupt configuration register3 */      
   union 
   {
      __IOM uint32_t EINTCFG3;                     

      struct 
      {
         __IOM uint32_t EINT8           : 4;            
         __IOM uint32_t EINT9           : 4;            
         __IOM uint32_t EINT10          : 4;            
         __IOM uint32_t EINT11          : 4;  
         __IM  uint32_t RESERVED        : 16;
      } EINTCFG3_b;
   } ;
   
   /** External interrupt configuration register4 */  
   union 
   {
      __IOM uint32_t EINTCFG4;                     

      struct 
      {
         __IOM uint32_t EINT12          : 4;            
         __IOM uint32_t EINT13          : 4;            
         __IOM uint32_t EINT14          : 4;            
         __IOM uint32_t EINT15          : 4;   
         __IM  uint32_t RESERVED        : 16;
      } EINTCFG4_b;
   } ;
   __IM  uint32_t  RESERVED;
  
   /** Alternate function IO remap register2 */
   union 
   {
      __IOM uint32_t REMAP2;                       

      struct 
      {
         __IM  uint32_t RESERVED1       : 5;
         __IOM uint32_t TMR9_REMAP      : 1;            
         __IOM uint32_t TMR10_REMAP     : 1;           
         __IOM uint32_t TMR11_REMAP     : 1;           
         __IOM uint32_t TMR13_REMAP     : 1;           
         __IOM uint32_t TMR14_REMAP     : 1;           
         __IOM uint32_t EMMC_NADV       : 1; 
         __IM  uint32_t RESERVED2       : 21;
      } REMAP2_B;
   } ;
} AFIO_T;                                       

/**
  * @brief Universal synchronous asynchronous receiver  transmitter (USART)
  */
typedef struct 
{                               
  
   /** Status register */
   union 
   {
      __IOM uint32_t STS;                         

      struct 
      {
         __IM  uint32_t PEF             : 1;           
         __IM  uint32_t FEF             : 1;           
         __IM  uint32_t NEF             : 1;           
         __IM  uint32_t OVREF           : 1;           
         __IM  uint32_t IDLEF           : 1;           
         __IOM uint32_t RXBNEF          : 1;           
         __IOM uint32_t TXCF            : 1;           
         __IM  uint32_t TXBEF           : 1;           
         __IOM uint32_t LBDF            : 1;           
         __IOM uint32_t CTSF            : 1;  
         __IM  uint32_t RESERVED        : 22;
      } STS_B;
   } ;
  
   /** TX Buffer Data Register */
   union 
   {
      __IOM uint32_t DATA;                         

      struct 
      {
         __IOM uint32_t DATA            : 9;   
         __IM  uint32_t RESERVED        : 23;
      } DATA_B;
   } ;
  
   /** Baud rate register */
   union 
   {
      __IOM uint32_t BR;                        

      struct 
      {
         __IOM uint32_t FRACTION        : 4;      
         __IOM uint32_t MANTISSA        : 12;     
         __IM  uint32_t RESERVED        : 16;
      } BR_B;
   } ;
  
   /** Control register 1 */
   union 
   {
   
      __IOM uint32_t CTRL1;                        

      struct 
      {
         __IOM uint32_t TXBK            : 1;           
         __IOM uint32_t RXWUP           : 1;           
         __IOM uint32_t RXEN            : 1;           
         __IOM uint32_t TXEN            : 1;           
         __IOM uint32_t IDLEIE          : 1;           
         __IOM uint32_t RXBNEIE         : 1;           
         __IOM uint32_t TXCIE           : 1;           
         __IOM uint32_t TXBEIE          : 1;           
         __IOM uint32_t PEIE            : 1;           
         __IOM uint32_t PMSEL           : 1;           
         __IOM uint32_t PCEN            : 1;           
         __IOM uint32_t WUPM            : 1;           
         __IOM uint32_t WLS             : 1;           
         __IOM uint32_t UEN             : 1;    
         __IM  uint32_t RESERVED        : 18;
      } CTRL1_B;
   } ;

     /** Control register 2 */
   union 
   {
      __IOM uint32_t CTRL2;                        

      struct 
      {
         __IOM uint32_t ADDR            : 4;           
         __IM  uint32_t RESERVED1       : 1;
         __IOM uint32_t LBDL            : 1;           
         __IOM uint32_t LBDIE           : 1;           
         __IM  uint32_t RESERVED2       : 1;
         __IOM uint32_t LBCEN           : 1;           
         __IOM uint32_t CLKPHA          : 1;           
         __IOM uint32_t CLKPOL          : 1;           
         __IOM uint32_t CKPEN           : 1;           
         __IOM uint32_t STOPB           : 2;           
         __IOM uint32_t LINEN           : 1;  
         __IM  uint32_t RESERVED3       : 17;
      } CTRL2_B;
   } ;
  
   /** Control register 3 */
   union 
   {
      __IOM uint32_t CTRL3;                        

      struct 
      {
         __IOM uint32_t ERRIE           : 1;           
         __IOM uint32_t IRDAEN          : 1;           
         __IOM uint32_t IRDALP          : 1;           
         __IOM uint32_t HDEN            : 1;           
         __IOM uint32_t NACKEN          : 1;           
         __IOM uint32_t SCEN            : 1;           
         __IOM uint32_t RXDMAEN         : 1;           
         __IOM uint32_t TXDMAEN         : 1;           
         __IOM uint32_t RTSEN           : 1;           
         __IOM uint32_t CTSEN           : 1;           
         __IOM uint32_t CTSIE           : 1; 
         __IM  uint32_t RESERVED        : 21;
      } CTRL3_B;
   } ;
  
   /** Guard TMRe and divider number register */
   union 
   {
      __IOM uint32_t GTDIV;                       

      struct 
      {
         __IOM uint32_t DIV             : 8;           
         __IOM uint32_t GT              : 8; 
         __IM  uint32_t RESERVED        : 16;
      } GTDIV_B;
   } ;
} USART_T;                                     

/**
  * @brief FMC memory controller(FMC)
  */
typedef struct 
{                               
   /** FMC access control register */
   union 
   {
      __IOM uint32_t CTRL1;                        

      struct 
      {
         __IOM uint32_t LATENCY         : 3;           
         __IOM uint32_t HCAEN           : 1;           
         __IOM uint32_t PBEN            : 1;           
         __IM  uint32_t PBSF            : 1;    
         __IM  uint32_t RESERVED        : 26;
      } CTRL1_B;
   } ;

   /** key register */
   union 
   {
      __OM  uint32_t KEY;                       

      struct 
      {
         __OM  uint32_t KEY             : 32;          
      } KEY_B;
   } ;

   /** option byte key register */  
   union 
   {
      __OM  uint32_t OBKEY;                    

      struct 
      {
         __OM  uint32_t KEY             : 32;          
      } OBKEY_B;
   };
  
   /** status register */
   union 
   {
      __IOM uint32_t STS;                         

      struct 
      {
         __IM  uint32_t BUSYF           : 1;           
         __IM  uint32_t RESERVED1       : 1;
         __IOM uint32_t PEF             : 1;           
         __IM  uint32_t RESERVED2       : 1;
         __IOM uint32_t WPEF            : 1;           
         __IOM uint32_t OCF             : 1;   
         __IM  uint32_t RESERVED3       : 26;
      } STS_B;
   };

   /** status register */  
   union 
   {
      __IOM uint32_t CTRL2;                         

      struct 
      {
         __IOM uint32_t PG              : 1;           
         __IOM uint32_t PAGEERA         : 1;           
         __IOM uint32_t MASSERA         : 1;           
         __IM  uint32_t RESERVED1       : 1;
         __IOM uint32_t OBP             : 1;           
         __IOM uint32_t OBE             : 1;           
         __IOM uint32_t STA             : 1;           
         __IOM uint32_t LOCK            : 1;           
         __IM  uint32_t RESERVED2       : 1;
         __IOM uint32_t OBWEN           : 1;           
         __IOM uint32_t ERRIE           : 1;           
         __IM  uint32_t RESERVED3       : 1;
         __IOM uint32_t OCIE            : 1;     
         __IM  uint32_t RESERVED4       : 19;
      } CTRL2_B;
   } ;
  
   /** address register */
   union 
   {
      __OM  uint32_t ADD;                         

      struct 
      {
         __OM  uint32_t ADD             : 32;          
      } ADD_B;
   };
   
   __IM  uint32_t  RESERVED;
  
   /** Option byte register */
   union 
   {
      __IOM  uint32_t OBCS;                        

      struct 
      {
         __IM  uint32_t OBE             : 1;           
         __IM  uint32_t READPORT        : 1;           
         __IOM  uint32_t WDT            : 1;           
         __IM  uint32_t STOPRST         : 1;           
         __IM  uint32_t STDBYRST        : 1;           
         __IM  uint32_t NOTUSED         : 5;
         __IM  uint32_t DATA0           : 8;           
         __IM  uint32_t DATA1           : 8; 
         __IM  uint32_t RESERVED        : 6;
      } OBCS_B;
   };
  
   /** Write protection register */
   union 
   {
      __IM  uint32_t WRTPORT;                       

      struct
      {
         __IM  uint32_t WRTPORT         : 32;          
      } WRTPORT_B;
   };
} FMC_T;                                      

/**
 * @brief  CRC register
 */
typedef struct
{
/**
 * @brief  DATA register
         */
union
{
    __IOM uint32_t DATA;

    struct
    {
        __IOM  uint32_t DATA            : 32;
    } DATA_B;
} ;

/**
 * @brief independent DATA register
                 */
union
{
    __IOM  uint32_t DB;

    struct
    {
        __IOM uint32_t DB               : 8;
        __IM  uint32_t RESERVED         : 24;
    } DB_B;
};

/**
 * @brief Countrol register
 */
union
{
    __IOM uint32_t CTRL;

    struct
    {
        __IOM uint32_t RST              : 1;
        __IM  uint32_t RESERVED         : 31;
    } CTRL_B;
};

} CRC_T;

/**
 * @brief RTC register
 */
typedef struct
{
    /**
     * @brief Control register
     */
    union
    {
        __IOM uint32_t CTRL;
        
        struct
        {
            __IOM  uint32_t SECIEN      : 1;
            __IOM  uint32_t ALMIEN      : 1;
            __IOM  uint32_t OWIEN       : 1;
            __IM  uint32_t RESERVED     : 29;

        } CTRL_B;
    } ;

    /**
     * @brief Control and State register
     */
    union
    {
        __IOM uint32_t CSTS;

        struct
        {
            __IOM  uint32_t SECIF       : 1;
            __IOM  uint32_t ALMIF       : 1;
            __IOM  uint32_t OWIF        : 1;
            __IOM  uint32_t SYNCF       : 1;
            __IOM  uint32_t CFGEN       : 1;
            __IM   uint32_t RTOPC       : 1;
            __IM  uint32_t RESERVED     : 26;

        } CSTS_B;
    } ;
    /**
     * @brief RTC predivision loading register High Bit
     */
    union
    {
        __IOM uint32_t RDIVH;

        struct
        {
            __OM uint32_t DIV           : 4;
            __IM uint32_t RESERVED      : 28;
        } RDIVH_B;
    };
    /**
     * @brief  RTC predivision loading register Low Bit
     */
    union
    {
        __IOM uint32_t RDIVL;

        struct
        {
            __OM uint32_t DIV           : 16;
            __IM  uint32_t RESERVED     : 16;
        } RDIVL_B;
    };
    /**
     * @brief RTC predivider remainder register High Bit
     */
    union
    {
        __IM uint32_t DIVH;

        struct
        {
            __IM uint32_t DIV           : 4;
            __IM  uint32_t RESERVED     : 28;
        } DIVH_B;
    };
    /**
     * @brief RTC predivider remainder register Low Bit
     */
    union
    {
        __IM uint32_t DIVL;

        struct
        {
            __IM uint32_t DIV           : 16;
            __IM  uint32_t RESERVED     : 16;
        } DIVL_B;
    };
    /**
     * @brief RTC count register High Bit
     */
    union
    {
        __IOM uint32_t CNTH;

        struct
        {
            __IOM uint32_t CNT          : 16;
            __IM  uint32_t RESERVED     : 16;
        } CNTH_B;
    };
    /**
     * @brief RTC count register Low Bit
     */
    union
    {
        __IOM uint32_t CNTL;

        struct
        {
            __IOM uint32_t CNT          : 16;
            __IM  uint32_t RESERVED     : 16;
        } CNTL_B;
    };
    /**
     * @brief RTC alarm clock register High Bit
     */
    union
    {
        __OM uint32_t ALMH;

        struct
        {
            __OM uint32_t ALM           : 16;
            __IM  uint32_t RESERVED     : 16;
        } ALMH_B;
    };
    /**
     * @brief RTC alarm clock register Low Bit
     */
    union
    {
        __OM uint32_t ALML;

        struct
        {
            __OM uint32_t ALM           : 16;
            __IM  uint32_t RESERVED     : 16;
        } ALML_B;
    };

} RTC_T;


/**
 * @brief PMU register
 */
typedef struct
{

  /**
    * @brief Control register
   */
  union
  {
    __IOM uint32_t CTRL;

    struct
    {
      __IOM  uint32_t LPSM              : 1;
      __IOM  uint32_t SBMDS             : 1;
      __IOM  uint32_t RWUPF             : 1;
      __IOM  uint32_t RSBF              : 1;
      __IOM  uint32_t PVMEN             : 1;
      __IOM  uint32_t PLMS              : 3;
      __IOM  uint32_t BAKRWEN           : 1;
      __IM  uint32_t RESERVED           : 23;

    } CTRL_B;
  } ;

  /**
    * @brief PMU Status register
    */
  union
  {
    __IOM uint32_t CTRLF;

    struct
    {
      __IM uint32_t WUPF                : 1;
      __IM uint32_t SBMF                : 1;
      __IM uint32_t PVMF                : 1;
      __IM uint32_t RESERVED1           : 5;
      __IOM uint32_t WUPEN              : 1;
      __IM uint32_t RESERVED2           : 23;

    } CTRLF_B;
  };

} PMU_T;


/**
 * @brief BAKR register
 */
typedef struct
{
  /**
   * @brief BAKR reserve1 register
   */
  union
  {
    __IM uint32_t RESERVED1;

    struct
    {
      __IM uint32_t RESERVED            : 32;
    } RESERVED1_B;

  };

  /**
   * @brief BAKR DATA1 register
   */
  union
  {
    __IOM uint32_t DATA1;

    struct
    {
      __IOM uint32_t DATA               : 16;
      __IM uint32_t RESERVED            : 16;
    } DATA1_B;
  };

  /**
   * @brief BAKR DATA2 register
   */
  union
  {
    __IOM uint32_t DATA2;

    struct
    {
      __IOM uint32_t DATA               : 16;
      __IM uint32_t RESERVED            : 16;
    } DATA2_B;
  };

  /**
   * @brief BAKR DATA3 register
   */
  union
  {
    __IOM uint32_t DATA3;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA3_B;
  };

  /**
   * @brief BAKR DATA4 register
   */
  union
  {
    __IOM uint32_t DATA4;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA4_B;
  };

  /**
   * @brief BAKR DATA5 register
   */
  union
  {
    __IOM uint32_t DATA5;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA5_B;
  };

  /**
   * @brief BAKR DATA6 register
   */
  union
  {
    __IOM uint32_t DATA6;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA6_B;
  };

  /**
   * @brief BAKR DATA7 register
   */
  union
  {
    __IOM uint32_t DATA7;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA7_B;
  };

  /**
   * @brief BAKR DATA8 register
  */
  union
  {
    __IOM uint32_t DATA8;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA8_B;
  };

  /**
   * @brief BAKR DATA9 register
   */
  union
  {
    __IOM uint32_t DATA9;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA9_B;
  };

  /**
  * @brief        BAKR DATA10 register
  */
  union
  {
    __IOM uint32_t DATA10;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA10_B;
  };

  /**
     * @brief BAKR Clck Calibration register
     */
  union
  {
    __IOM uint32_t RTCCTRL;

    struct
    {
      __IOM  uint32_t CALP          : 7;
      __IOM  uint32_t RTCO          : 1;
      __IOM  uint32_t ASOEN         : 1;
      __IOM  uint32_t ASOC          :1;
      __IM  uint32_t RESERVED       : 22;
    } RTCCTRL_B;
  } ;

  /**
  * @brief BAKR Control register
   */
  union
  {
    __IOM uint32_t CTRL;

    struct
    {
      __IOM uint32_t TPEN           : 1;
      __IOM uint32_t TPTM           : 1;
      __IM  uint32_t RESERVED       : 30;
    } CTRL_B;
  };

  /**
   * @brief BAKR Control register
   */
  union
  {
    __IOM uint32_t CTRLF;

    struct
    {
      __IOM uint32_t RTPEF          : 1;
      __IOM uint32_t RTPIF          : 1;
      __IOM uint32_t TPIEN          : 1;
      __IM  uint32_t RESERVED1      : 5;
      __IOM uint32_t TPEF           : 1;
      __IOM uint32_t TPIF           : 1;
      __IM  uint32_t RESERVED2      : 22;
    } CTRLF_B;
  };

  /**
   * @brief BAKR reserve2 register
   */
  union
  {
    __IM uint32_t RESERVED2;

    struct
    {
      __IM uint32_t RESERVED        : 32;
    } RESERVED2_B;

  };

  /**
   * @brief BAKR reserve3 register
   */
  union
  {
    __IM uint32_t RESERVED3;

    struct
    {
      __IM uint32_t RESERVED        : 32;
    } RESERVED3_B;

  };

  /**
   * @briefBAKR DATA11 register
   */
  union
  {
    __IOM uint32_t DATA11;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA11_B;
  };

  /**
   * @brief BAKR DATA12 register
   */
  union
  {
    __IOM uint32_t DATA12;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA12_B;
  };

  /**
   * @brief BAKR DATA13 register
   */
  union
  {
    __IOM uint32_t DATA13;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA13_B;
  };

  /**
   * @brief BAKR DATA14 register
  */
  union
  {
    __IOM uint32_t DATA14;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA14_B;
  };

  /**
   * @brief BAKR DATA15 register
   */
  union
  {
    __IOM uint32_t DATA15;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA15_B;
  };

  /**
   * @brief BAKR DATA16 register
   */
  union
  {
    __IOM uint32_t DATA16;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA16_B;
  };

  /**
   * @brief BAKR DATA17 register
   */
  union
  {
    __IOM uint32_t DATA17;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA17_B;
  };

  /**
   * @brief BAKR DATA18 register
   */
  union
  {
    __IOM uint32_t DATA18;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA18_B;
  };

  /**
   * @brief BAKR DATA19 register
   */
  union
  {
    __IOM uint32_t DATA19;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA19_B;
  };

  /**
   * @brief BAKR DATA20 register
   */
  union
  {
    __IOM uint32_t DATA20;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA20_B;
  };

  /**
   * @brief BAKR DATA21 register
   */
  union
  {
    __IOM uint32_t DATA21;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA21_B;
  };

  /**
   * @brief BAKR DATA22 register
   */
  union
  {
    __IOM uint32_t DATA22;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA22_B;
  };

  /**
   * @brief BAKR DATA23 register
   */
  union
  {
    __IOM uint32_t DATA23;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA23_B;
  };

  /**
   * @brief BAKR DATA24 register
   */
  union
  {
    __IOM uint32_t DATA24;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA24_B;
  };

  /**
   * @brief BAKR DATA25 register
   */
  union
  {
    __IOM uint32_t DATA25;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA25_B;
  };

  /**
   * @brief BAKR DATA26 register
   */
  union
  {
    __IOM uint32_t DATA26;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA26_B;
  };

  /**
   * @brief BAKR DATA27 register
   */
  union
  {
    __IOM uint32_t DATA27;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA27_B;
  };

  /**
   * @brief BAKR DATA28 register
   */
  union
  {
    __IOM uint32_t DATA28;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA28_B;
  };

  /**
   * @brief BAKR DATA29 register
   */
  union
  {
    __IOM uint32_t DATA29;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA29_B;
  };

  /**
   * @brief BAKR DATA30 register
   */
  union
  {
    __IOM uint32_t DATA30;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA30_B;
  };

  /**
   * @brief BAKR DATA31 register
   */
  union
  {
    __IOM uint32_t DATA31;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA31_B;
  };

  /**
   * @brief BAKR DATA32 register
   */
  union
  {
    __IOM uint32_t DATA32;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA32_B;
  };

  /**
   * @brief BAKR DATA33 register
   */
  union
  {
    __IOM uint32_t DATA33;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA33_B;
  };

  /**
   * @brief BAKR DATA34 register
   */
  union
  {
    __IOM uint32_t DATA34;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA34_B;
  };

  /**
   * @brief BAKR DATA25 register
   */
  union
  {
    __IOM uint32_t DATA35;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA35_B;
  };

  /**
   * @brief BAKR DATA36 register
   */
  union
  {
    __IOM uint32_t DATA36;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA36_B;
  };

  /**
   * @brief BAKR DATA37 register
   */
  union
  {
    __IOM uint32_t DATA37;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA37_B;
  };

  /**
   * @brief BAKR DATA38 register
   */
  union
  {
    __IOM uint32_t DATA38;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA38_B;
  };

  /**
   * @brief BAKR DATA39 register
   */
  union
  {
    __IOM uint32_t DATA39;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA39_B;
  };

  /**
   * @brief BAKR DATA40 register
   */
  union
  {
    __IOM uint32_t DATA40;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA40_B;
  };

  /**
   * @brief BAKR DATA41 register
   */
  union
  {
    __IOM uint32_t DATA41;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA41_B;
  };

  /**
   * @brief BAKR DATA42 register
   */
  union
  {
    __IOM uint32_t DATA42;

    struct
    {
      __IOM uint32_t DATA           : 16;
      __IM uint32_t RESERVED        : 16;
    } DATA42_B;
  };

} BAKR_T;

/**
 * @brief TMR register
 */
typedef struct
{
    /**
     * @brief Countrol register 1
     */
    union
    {
        __IOM uint32_t CTRL1;

        struct
        {
            __IOM  uint32_t CNTEN       : 1;
            __IOM  uint32_t NGUE        : 1;
            __IOM  uint32_t UES         : 1;
            __IOM  uint32_t SPMEN       : 1;
            __IOM  uint32_t CNTDIR      : 1;
            __IOM  uint32_t CNTMODE     : 2;
            __IOM  uint32_t ARBEN       : 1;
            __IOM  uint32_t CKDR        : 2;
            __IM  uint32_t RESERVED     : 22;
        } CTRL1_B;
    } ;

    /**
     * @brief Countrol register 2
     */
    union
    {
        __IOM uint32_t CTRL2;

        struct
        {
            __IOM  uint32_t CCBEN       : 1;
            __IM  uint32_t RESERVED1    : 1;
            __IOM  uint32_t CCUS        : 1;
            __IOM  uint32_t CCDS        : 1;
            __IOM  uint32_t MMFC        : 3;
            __IOM  uint32_t TI1IS       : 1;
            __IOM  uint32_t CH1ISO      : 1;
            __IOM  uint32_t CH1NISO     : 1;
            __IOM  uint32_t CH2ISO      : 1;
            __IOM  uint32_t CH2NISO     : 1;
            __IOM  uint32_t CH3ISO      : 1;
            __IOM  uint32_t CH3NISO     : 1;
            __IOM  uint32_t CH4ISO      : 1;
            __IM   uint32_t RESERVED2   : 17;
        } CTRL2_B;
    } ;

    /**
     * @brief Control register from mode
     */
    union
    {
        __IOM uint32_t SMCTRL;

        struct
        {
            __IOM  uint32_t SMFC        : 3;
            __IM  uint32_t RESERVED1    : 1;
            __IOM  uint32_t ITC         : 3;
            __IOM  uint32_t MSMEN       : 1;
            __IOM  uint32_t ETFC        : 4;
            __IOM  uint32_t ETDC        : 2;
            __IOM  uint32_t ECM2EN      : 1;
            __IOM  uint32_t ETPC        : 1;
            __IM  uint32_t RESERVED2    : 16;
        } SMCTRL_B;
    } ;

    /**
     * @brief DMA and Interrupt enable register
     */
    union
    {
        __IOM  uint32_t DIEN;

        struct
        {
            __IOM  uint32_t UDIEN       : 1;
            __IOM  uint32_t CH1CCIEN    : 1;
            __IOM  uint32_t CH2CCIEN    : 1;
            __IOM  uint32_t CH3CCIEN    : 1;
            __IOM  uint32_t CH4CCIEN    : 1;
            __IOM  uint32_t CCUIEN      : 1;
            __IOM  uint32_t TRGIEN      : 1;
            __IOM  uint32_t BRKIEN      : 1;
            __IOM  uint32_t UDEN        : 1;
            __IOM  uint32_t CH1CCDEN    : 1;
            __IOM  uint32_t CH2CCDEN    : 1;
            __IOM  uint32_t CH3CCDEN    : 1;
            __IOM  uint32_t CH4CCDEN    : 1;
            __IOM  uint32_t CMDEN       : 1;
            __IOM  uint32_t TDEN        : 1;
            __IM  uint32_t RESERVED     : 17;
        } DIEN_B;
    } ;

    /**
     * @brief Status register
     */
    union
    {
        __IOM  uint32_t STS;

        struct
        {
            __IOM  uint32_t UDIF        : 1;
            __IOM  uint32_t CH1CCIF     : 1;
            __IOM  uint32_t CH2CCIF     : 1;
            __IOM  uint32_t CH3CCIF     : 1;
            __IOM  uint32_t CH4CCIF     : 1;
            __IOM  uint32_t CCUIF       : 1;
            __IOM  uint32_t TRGIF       : 1;
            __IOM  uint32_t BRKIF       : 1;
            __IM  uint32_t RESERVED1    : 1;
            __IOM  uint32_t CH1RCF      : 1;
            __IOM  uint32_t CH2RCF      : 1;
            __IOM  uint32_t CH3RCF      : 1;
            __IOM  uint32_t CH4RCF      : 1;
            __IM  uint32_t RESERVED2    : 19;
        } STS_B;
    };

    /**
     * @brief Software controls event generation registers
     */
    union
    {
        __OM  uint32_t SCEG;

        struct
        {
            __OM uint32_t  UEG          : 1;
            __OM uint32_t  CH1CCG       : 1;
            __OM uint32_t  CH2CCG       : 1;
            __OM uint32_t  CH3CCG       : 1;
            __OM uint32_t  CH4CCG       : 1;
            __OM uint32_t  CCUEG        : 1;
            __OM uint32_t  TEG          : 1;
            __OM uint32_t  BEG          : 1;
            __OM uint32_t  RESERVED     : 24;
        } SCEG_B;
    };

    /**
     * @brief Capture the compare mode register 1
     */
    union
    {
        __IOM uint32_t CCM1;

        /**
         * @brief Compare mode
         */
        struct
        {
            __IOM uint32_t CC1MS        : 2;
            __IOM uint32_t OC1FEN       : 1;
            __IOM uint32_t OC1BEN       : 1;
            __IOM uint32_t OC1MS        : 3;
            __IOM uint32_t OC1CEN       : 1;
            __IOM uint32_t CC2MS        : 2;
            __IOM uint32_t OC2FEN       : 1;
            __IOM uint32_t OC2BEN       : 1;
            __IOM uint32_t OC2MS        : 3;
            __IOM uint32_t OC2CEN       : 1;
            __IM uint32_t RESERVED      : 16;
        } CCM1_COMPARE_B;

        /**
         * @brief Capture mode
         */
        struct
        {
            __IOM uint32_t CC1MS        : 2;
            __IOM uint32_t IC1D         : 2;
            __IOM uint32_t IC1FC        : 3;
            __IOM uint32_t CC2MS        : 2;
            __IOM uint32_t IC2D         : 2;
            __IOM uint32_t IC2FC        : 3;
            __IM uint32_t RESERVED      : 16;
        } CCM1_CAPTURE_B;
    };

    /**
     * @brief Capture the compare mode register 2
     */
    union
    {
        __IOM uint32_t CCM2;

        /**
         * @brief Compare mode
         */
        struct
        {
            __IOM uint32_t CC3MS        : 2;
            __IOM uint32_t OC3FEN       : 1;
            __IOM uint32_t OC3BEN       : 1;
            __IOM uint32_t OC3MS        : 3;
            __IOM uint32_t OC3CEN       : 1;
            __IOM uint32_t CC4MS        : 2;
            __IOM uint32_t OC4FEN       : 1;
            __IOM uint32_t OC4BEN       : 1;
            __IOM uint32_t OC4MS        : 3;
            __IOM uint32_t OC4CEN       : 1;
            __IM uint32_t RESERVED      : 16;
        } CCM2_COMPARE_B;

        /**
         * @brief Capture mode
         */
        struct
        {
            __IOM uint32_t CC3MS        : 2;
            __IOM uint32_t IC3D         : 2;
            __IOM uint32_t IC3FC        : 3;
            __IOM uint32_t CC4MS        : 2;
            __IOM uint32_t IC4D         : 2;
            __IOM uint32_t IC4FC        : 3;
            __IM uint32_t RESERVED      : 16;
        } CCM2_CAPTURE_B;
    };

    /**
     * @brief Channel control register
     */
    union
    {
        __IOM uint32_t CHCTRL;

        struct
        {
            __IOM uint32_t CH1CCEN      : 1;
            __IOM uint32_t CH1CCP       : 1;
            __IOM uint32_t CH1OCNEN     : 1;
            __IOM uint32_t CH1OCNP      : 1;
            __IOM uint32_t CH2CCEN      : 1;
            __IOM uint32_t CH2CCP       : 1;
            __IOM uint32_t CH2OCNEN     : 1;
            __IOM uint32_t CH2OCNP      : 1;
            __IOM uint32_t CH3CCEN      : 1;
            __IOM uint32_t CH3CCP       : 1;
            __IOM uint32_t CH3OCNEN     : 1;
            __IOM uint32_t CH3OCNP      : 1;
            __IOM uint32_t CH4CCEN      : 1;
            __IOM uint32_t CH4CCP       : 1;
            __IM uint32_t RESERVED      : 18;
        } CHCTRL_B;
    };

    /**
     * @brief Counting register
     */
    union
    {
        __IOM uint32_t CNT;

        struct
        {
            __IOM uint32_t CNT          : 16;
            __IM  uint32_t RESERVED     : 16;
        } CNT_B;
    };

    /**
     * @brief Division register
     */
    union
    {
        __IOM uint32_t DIV;

        struct
        {
            __IOM uint32_t DIV          : 16;
            __IM  uint32_t RESERVED     : 16;
        } DIV_B;
    };

    /**
     * @brief Automatic reload  register
     */
    union
    {
        __IOM uint32_t AOUTORLD;

        struct
        {
            __IOM uint32_t AOUTORLD     : 16;
            __IM  uint32_t RESERVED     : 16;
        } AOUTORLD_B;
    };

    /**
     * @brief Repeat count register
     */
    union
    {
        __IOM uint32_t REPCNT;

        struct
        {
            __IOM uint32_t REPCNT       : 8;
            __IM  uint32_t RESERVED     : 24;
        } REPCNT_B;
    };

    /**
     * @brief Capture comparison register channel 1
     */
    union
    {
        __IOM uint32_t CH1CC;

        struct
        {
            __IOM uint32_t CH1CC        : 16;
            __IM  uint32_t RESERVED     : 16;
        } CH1CC_B;
    };

    /**
     * @brief Capture comparison register channel 2
     */
    union
    {
        __IOM uint32_t CH2CC;

        struct
        {
            __IOM uint32_t CH2CC        : 16;
            __IM  uint32_t RESERVED     : 16;
        } CH2CC_B;
    };

    /**
     * @brief Capture comparison register channel 3
     */
    union
    {
        __IOM uint32_t CH3CC;

        struct
        {
            __IOM uint32_t CH3CC        : 16;
            __IM  uint32_t RESERVED     : 16;
        } CH3CC_B;
    };

    /**
     * @brief Capture comparison register channel 4
     */
    union
    {
        __IOM uint32_t CH4CC;

        struct
        {
            __IOM uint32_t CH4CC        : 16;
            __IM  uint32_t RESERVED     : 16;
        } CH4CC_B;
    };

    /**
     * @brief Brake and dead zone registers
     */
    union
    {
        __IOM uint32_t BDT;

        struct
        {
            __IOM uint32_t DTS          : 8;
            __IOM uint32_t PROTCFG      : 2;
            __IOM uint32_t IMOS         : 1;
            __IOM uint32_t RMOS         : 1;
            __IOM uint32_t BRKEN        : 1;
            __IOM uint32_t BRKPOL       : 1;
            __IOM uint32_t AOEN         : 1;
            __IOM uint32_t WOEN         : 1;
            __IM  uint32_t RESERVED     : 16;
        } BDT_B;
    };

    /**
     * @brief DMA control register
     */
    union
    {
        __IOM uint32_t DCTRL;

        struct
        {
            __IOM uint32_t DBA          : 5;
            __IM uint32_t RESERVED1     : 3;
            __IOM uint32_t DBL          : 5;
            __IM  uint32_t RESERVED2    : 19;
        } DCTRL_B;
    };

    /**
     * @brief Consecutive DMA addresses
     */
    union
    {
        __IOM uint32_t DMAB;
        struct
        {
            __IOM uint32_t DMAB         : 16;
            __IM  uint32_t RESERVED2    : 16;
        } DMAB_B;
    };
} TMR_T;


/**
 * @brief    DMA Channel register
 */
typedef struct
{
    /**
     * @brief    Interrupt status register
       */
    union
    {
        __IOM uint32_t IF;

        struct
        {
            __IOM  uint32_t GIFC1           : 1;
            __IOM  uint32_t EOTTFC1         : 1;
            __IOM  uint32_t MOTIFC1         : 1;
            __IOM  uint32_t FOTIFC1         : 1;
            __IOM  uint32_t GIFC2           : 1;
            __IOM  uint32_t EOTTFC2         : 1;
            __IOM  uint32_t MOTIFC2         : 1;
            __IOM  uint32_t FOTIFC2         : 1;
            __IOM  uint32_t GIFC3           : 1;
            __IOM  uint32_t EOTTFC3         : 1;
            __IOM  uint32_t MOTIFC3         : 1;
            __IOM  uint32_t FOTIFC3         : 1;
            __IOM  uint32_t GIFC4           : 1;
            __IOM  uint32_t EOTTFC4         : 1;
            __IOM  uint32_t MOTIFC4         : 1;
            __IOM  uint32_t FOTIFC4         : 1;
            __IOM  uint32_t GIFC5           : 1;
            __IOM  uint32_t EOTTFC5         : 1;
            __IOM  uint32_t MOTIFC5         : 1;
            __IOM  uint32_t FOTIFC5         : 1;
            __IOM  uint32_t GIFC6           : 1;
            __IOM  uint32_t EOTTFC6         : 1;
            __IOM  uint32_t MOTIFC6         : 1;
            __IOM  uint32_t FOTIFC6         : 1;
            __IOM  uint32_t GIFC7           : 1;
            __IOM  uint32_t EOTTFC7         : 1;
            __IOM  uint32_t MOTIFC7         : 1;
            __IOM  uint32_t FOTIFC7         : 1;
            __IM  uint32_t RESERVED         : 4;
        } IF_B;
    } ;

    /**
    * @brief    Interrupt reset  register
    */
    union
    {
        __IOM uint32_t IFR;

        struct
        {
            __IOM  uint32_t RGIFC1          : 1;
            __IOM  uint32_t REOTIFC1        : 1;
            __IOM  uint32_t RMOTIFC1        : 1;
            __IOM  uint32_t RFOTIFC1        : 1;
            __IOM  uint32_t RGIFC2          : 1;
            __IOM  uint32_t REOTIFC2        : 1;
            __IOM  uint32_t RMOTIFC2        : 1;
            __IOM  uint32_t RFOTIFC2        : 1;
            __IOM  uint32_t RGIFC3          : 1;
            __IOM  uint32_t REOTIFC3        : 1;
            __IOM  uint32_t RMOTIFC3        : 1;
            __IOM  uint32_t RFOTIFC3        : 1;
            __IOM  uint32_t RGIFC4          : 1;
            __IOM  uint32_t REOTIFC4        : 1;
            __IOM  uint32_t RMOTIFC4        : 1;
            __IOM  uint32_t RFOTIFC4        : 1;
            __IOM  uint32_t RGIFC5          : 1;
            __IOM  uint32_t REOTIFC5        : 1;
            __IOM  uint32_t RMOTIFC5        : 1;
            __IOM  uint32_t RFOTIFC5        : 1;
            __IOM  uint32_t RGIFC6          : 1;
            __IOM  uint32_t REOTIFC6        : 1;
            __IOM  uint32_t RMOTIFC6        : 1;
            __IOM  uint32_t RFOTIFC6        : 1;
            __IOM  uint32_t RGIFC7          : 1;
            __IOM  uint32_t REOTIFC7        : 1;
            __IOM  uint32_t RMOTIFC7        : 1;
            __IOM  uint32_t RFOTIFC7        : 1;
            __IM  uint32_t RESERVED         : 4;
        } IFR_B;
    };
} DMA_T;


/**
 * @brief    DMA Channel register
 */
typedef struct
{
 /**
  * @brief    DMA Channel setup register
   */
 union
 {
   __IOM uint32_t SRC;

   struct
   {
     __IOM  uint32_t EN                     : 1;
     __IOM  uint32_t EOTIEN                 : 1;
     __IOM  uint32_t MOTIEN                 : 1;
     __IOM  uint32_t FOTIEN                 : 1;
     __IOM  uint32_t DOT                    : 1;
     __IOM  uint32_t LOOP                   : 1;
     __IOM  uint32_t PLOOP                  : 1;
     __IOM  uint32_t MLOOP                  : 1;
     __IOM  uint32_t PWID                   : 2;
     __IOM  uint32_t MWID                   : 2;
     __IOM  uint32_t PL                     : 2;
     __IOM  uint32_t M2MEN                  : 1;
     __IM  uint32_t RESERVED                : 17;
   } SRC_B;
 } ;

 /**
  * @brief    DMA Channel transfer number register
   */
 union
 {
   __IOM uint32_t CNTTC;

   struct
   {
     __IOM uint32_t CNTTC                   :16;
     __IM  uint32_t RESERVED                : 16;
   } CNTTC_B;
 };

 /**
  * @brief    DMA Channel peripheral address register
   */
 union
 {
   __IOM uint32_t PAC;

   struct
   {
     __IOM uint32_t PAC                     :32;
   } PAC_B;
 };

 /**
  * @brief    DMA Channel memory address register
   */
 union
 {
   __IOM uint32_t MAC;

   struct
   {
     __IOM uint32_t MAC                     :32;
   } MAC_B;
 };

} DMA_Channel_T;


typedef struct
{
  /**
   * @brief     CAN Each mailbox contains the sending mailbox identifier register
    */
  union
  {
    __IOM uint32_t TXI;

    struct
    {
      __IOM uint32_t TXRQ               : 1;
      __IOM uint32_t RTR                : 1;
      __IOM uint32_t EXIDEN             : 1;
      __IOM uint32_t EXID               : 18;
      __IOM uint32_t STID               : 11;
    } TXI_B;
  };

  /**
   * @brief     CAN Send the mailbox data length and timestamp register
    */
  union
  {
    __IOM uint32_t TXDLT;

    struct
    {
      __IOM uint32_t TXDL               : 4;
      __IM  uint32_t RESERVED1          : 4;
      __IOM uint32_t TXTS               : 1;
      __IM  uint32_t RESERVED2          : 7;
      __IOM uint32_t MTS                : 16;
    } TXDLT_B;
  };

  /**
   * @brief     CAN Send mailbox low byte data register
    */
  union
  {
    __IOM uint32_t TXDL;

    struct
    {
      __IOM uint32_t DATA1              : 8;
      __IOM uint32_t DATA2              : 8;
      __IOM uint32_t DATA3              : 8;
      __IOM uint32_t DATA4              : 8;
    } TXDL_B;
  };

  /**
   * @brief     CAN Send mailbox High byte data register
    */
  union
  {
    __IOM uint32_t TXDH;

    struct
    {
      __IOM uint32_t DATA5              : 8;
      __IOM uint32_t DATA6              : 8;
      __IOM uint32_t DATA7              : 8;
      __IOM uint32_t DATA8              : 8;
    } TXDH_B;
  };
} CAN_TxMailBox_T;

typedef struct
{
  /**
   * @brief     CAN Each mailbox contains the receive mailbox identifier register
    */
  union
  {
    __IOM uint32_t RXI;

    struct
    {
      __IM  uint32_t RESERVED1          : 1;
      __IOM uint32_t RTR                : 1;
      __IOM uint32_t EXIDEN             : 1;
      __IOM uint32_t EXID               : 18;
      __IOM uint32_t STID               : 11;
    } RXI_B;
  };

  /**
   * @brief     CAN receive the mailbox data length and timestamp register
    */
  union
  {
    __IOM uint32_t RXDLT;

    struct
    {
      __IOM uint32_t RXDL               : 4;
      __IM  uint32_t RESERVED1          : 4;
      __IOM uint32_t FMI                : 8;
      __IOM uint32_t MTS                : 16;
    } RXDLT_B;
  };

  /**
   * @brief     CAN receive mailbox low byte data register
    */
  union
  {
    __IOM uint32_t RXDL;

    struct
    {
      __IOM uint32_t DATA1              : 8;
      __IOM uint32_t DATA2              : 8;
      __IOM uint32_t DATA3              : 8;
      __IOM uint32_t DATA4              : 8;
    } RXDL_B;
  };

  /**
   * @brief     CAN receive mailbox High byte data register
    */
  union
  {
    __IOM uint32_t RXDH;

    struct
    {
      __IOM uint32_t DATA5              : 8;
      __IOM uint32_t DATA6              : 8;
      __IOM uint32_t DATA7              : 8;
      __IOM uint32_t DATA8              : 8;
    } RXDH_B;
  };
} CAN_RxMailBox_T;

typedef struct
{
  /**
  * @brief     CAN Filter1 register
  */
  union
  {
    __IOM uint32_t  F1;

    struct
    {
      __IOM uint32_t FB1            :1;
      __IOM uint32_t FB2            :1;
      __IOM uint32_t FB3            :1;
      __IOM uint32_t FB4            :1;
      __IOM uint32_t FB5            :1;
      __IOM uint32_t FB6            :1;
      __IOM uint32_t FB7            :1;
      __IOM uint32_t FB8            :1;
      __IOM uint32_t FB9            :1;
      __IOM uint32_t FB10           :1;
      __IOM uint32_t FB11           :1;
      __IOM uint32_t FB12           :1;
      __IOM uint32_t FB13           :1;
      __IOM uint32_t FB14           :1;
      __IOM uint32_t FB15           :1;
      __IOM uint32_t FB16           :1;
      __IOM uint32_t FB17           :1;
      __IOM uint32_t FB18           :1;
      __IOM uint32_t FB19           :1;
      __IOM uint32_t FB20           :1;
      __IOM uint32_t FB21           :1;
      __IOM uint32_t FB22           :1;
      __IOM uint32_t FB23           :1;
      __IOM uint32_t FB24           :1;
      __IOM uint32_t FB25           :1;
      __IOM uint32_t FB26           :1;
      __IOM uint32_t FB27           :1;
      __IOM uint32_t FB28           :1;
      __IOM uint32_t FB29           :1;
      __IOM uint32_t FB30           :1;
      __IOM uint32_t FB31           :1;
      __IOM uint32_t FB32           :1;
    } F1_B;
  };

  /**
  * @brief     CAN Filter2 register
  */
  union
  {
    __IOM uint32_t  F2;

    struct
    {
      __IOM uint32_t FB1            :1;
      __IOM uint32_t FB2            :1;
      __IOM uint32_t FB3            :1;
      __IOM uint32_t FB4            :1;
      __IOM uint32_t FB5            :1;
      __IOM uint32_t FB6            :1;
      __IOM uint32_t FB7            :1;
      __IOM uint32_t FB8            :1;
      __IOM uint32_t FB9            :1;
      __IOM uint32_t FB10           :1;
      __IOM uint32_t FB11           :1;
      __IOM uint32_t FB12           :1;
      __IOM uint32_t FB13           :1;
      __IOM uint32_t FB14           :1;
      __IOM uint32_t FB15           :1;
      __IOM uint32_t FB16           :1;
      __IOM uint32_t FB17           :1;
      __IOM uint32_t FB18           :1;
      __IOM uint32_t FB19           :1;
      __IOM uint32_t FB20           :1;
      __IOM uint32_t FB21           :1;
      __IOM uint32_t FB22           :1;
      __IOM uint32_t FB23           :1;
      __IOM uint32_t FB24           :1;
      __IOM uint32_t FB25           :1;
      __IOM uint32_t FB26           :1;
      __IOM uint32_t FB27           :1;
      __IOM uint32_t FB28           :1;
      __IOM uint32_t FB29           :1;
      __IOM uint32_t FB30           :1;
      __IOM uint32_t FB31           :1;
      __IOM uint32_t FB32           :1;
    } F2_B;
  };
} CAN_FilterRegister_T;


/**
 * @brief     CAN register
 */
typedef struct
{
  /**
   * @brief     CAN Master control register
    */
  union
  {
    __IOM uint32_t MCTRL;

    struct
    {
      __IOM uint32_t INIT           : 1;
      __IOM uint32_t SLEEP          : 1;
      __IOM uint32_t TX_PS          : 1;
      __IOM uint32_t RFLOCK         : 1;
      __IOM uint32_t ARTDIS         : 1;
      __IOM uint32_t AWUPM          : 1;
      __IOM uint32_t ABOM           : 1;
      __IOM uint32_t TTCM           : 1;
      __IM  uint32_t RESERVED1      : 7;
      __IOM uint32_t RESET          : 1;
      __IOM uint32_t DBF            : 1;
      __IM  uint32_t RESERVED2      : 15;
    } MCTRL_B;
  };

  /**
   * @brief     CAN Master States register
    */
  union
  {
    __IOM uint32_t MSTS;

    struct
    {
      __IOM uint32_t INITF          : 1;
      __IOM uint32_t SLAK           : 1;
      __IOM uint32_t ERRIF          : 1;
      __IOM uint32_t WUPIF          : 1;
      __IOM uint32_t SLPIF          : 1;
      __IM uint32_t RESERVED1       : 3;
      __IOM uint32_t TX             : 1;
      __IOM uint32_t RX             : 1;
      __IOM uint32_t LST_SAMP       : 1;
      __IOM uint32_t RX_L           : 1;
      __IM uint32_t RESERVED2       : 20;
    } MSTS_B;
  };

  /**
   * @brief     CAN Send States register
    */
  union
  {
    __IOM uint32_t TXSTS;

    struct
    {
      __IOM uint32_t EORM1          : 1;
      __IOM uint32_t TCM1           : 1;
      __IOM uint32_t ALSTM1         : 1;
      __IOM uint32_t FOTM1          : 1;
      __IM uint32_t RESERVED1       : 3;
      __IOM uint32_t ATRM1          : 1;
      __IOM uint32_t EORM2          : 1;
      __IOM uint32_t TCM2           : 1;
      __IOM uint32_t ALSTM2         : 1;
      __IOM uint32_t FOTM2          : 1;
      __IM uint32_t RESERVED2       : 3;
      __IOM uint32_t ATRM2          : 1;
      __IOM uint32_t EORM3          : 1;
      __IOM uint32_t TCM3           : 1;
      __IOM uint32_t ALSTM3         : 1;
      __IOM uint32_t FOTM3          : 1;
      __IM uint32_t RESERVED3       : 3;
      __IOM uint32_t ATRM3          : 1;
      __IOM uint32_t EMN            : 2;
      __IOM uint32_t TXME1          : 1;
      __IOM uint32_t TXME2          : 1;
      __IOM uint32_t TXME3          : 1;
      __IOM uint32_t LOWMF1         : 1;
      __IOM uint32_t LOWMF2         : 1;
      __IOM uint32_t LOWMF3         : 1;
    } TXSTS_B;
  };

  /**
   * @brief     CAN Receive FIFO1 register
    */
  union
  {
    __IOM uint32_t RF1;

    struct
    {
      __IOM uint32_t FMNP1          : 2;
      __IM uint32_t RESERVED1       : 1;
      __IOM uint32_t FULL1          : 1;
      __IOM uint32_t FOF1           : 1;
      __IOM uint32_t RFOM1          : 1;
      __IM uint32_t RESERVED2       : 26;
    } RF1_B;
  };

  /**
   * @brief     CAN Receive FIFO2 register
    */
  union
  {
    __IOM uint32_t RF2;

    struct
    {
      __IOM uint32_t FMNP2          : 2;
      __IM uint32_t RESERVED1       : 1;
      __IOM uint32_t FULL2          : 1;
      __IOM uint32_t FOF2           : 1;
      __IOM uint32_t RFOM2          : 1;
      __IM uint32_t RESERVED2       : 26;
    } RF2_B;
  };

  /**
   * @brief     CAN Interrupts register
    */
  union
  {
    __IOM uint32_t IEN;

    struct
    {
      __IOM uint32_t EORMIEN        : 1;
      __IOM uint32_t FMNPIEN1       : 1;
      __IOM uint32_t FFIEN1         : 1;
      __IOM uint32_t FOFIEN1        : 1;
      __IOM uint32_t FMNPIEN2       : 1;
      __IOM uint32_t FFIEN2         : 1;
      __IOM uint32_t FOFIEN2        : 1;
      __IM uint32_t RESERVED1       : 1;
      __IOM uint32_t EWGIEN         : 1;
      __IOM uint32_t EPVIEN         : 1;
      __IOM uint32_t BOFIEN         : 1;
      __IOM uint32_t LECIEN         : 1;
      __IM uint32_t RESERVED2       : 3;
      __IOM uint32_t ERRIEN         : 1;
      __IOM uint32_t WUPIEN         : 1;
      __IOM uint32_t SLPIEN         : 1;
      __IM uint32_t RESERVED3       : 14;
    } IEN_B;
  };

  /**
   * @brief     CAN Error States register
    */
  union
  {
    __IOM uint32_t ESTS;

    struct
    {
      __IM uint32_t EWF             : 1;
      __IM uint32_t EPF             : 1;
      __IM uint32_t BOF             : 1;
      __IM uint32_t RESERVED1       : 1;
      __IOM uint32_t LEC            : 3;
      __IM uint32_t RESERVED2       : 9;
      __IM uint32_t TXEC            : 8;
      __IM uint32_t RXEC            : 8;
    } ESTS_B;
  };

  /**
   * @brief     CAN Bit Time register
    */
  union
  {
    __IOM uint32_t BT;

    struct
    {
      __IOM uint32_t BRD            : 10;
      __IM uint32_t RESERVED1       : 6;
      __IOM uint32_t TS1            : 4;
      __IOM uint32_t TS2            : 3;
      __IM uint32_t RESERVED2       : 1;
      __IOM uint32_t RSJW           : 2;
      __IM uint32_t RESERVED3       : 4;
      __IOM uint32_t LBM            : 1;
      __IOM uint32_t SILM           : 1;
    } BT_B;
  };

  __IM uint32_t RESERVED0[88];

  CAN_TxMailBox_T sTxMailBox[3];
  CAN_RxMailBox_T sRxMailBox[2];

  __IM uint32_t RESERVED1[12];

  /**
   * @brief     CAN Filter the master control register
    */
  union
  {
    __IOM uint32_t FMCTRL;

    struct
    {
      __IOM uint32_t FINIT      : 1;
      __IM uint32_t RESERVED    : 31;
    } FMCTRL_B;
  };

  /**
   * @brief     CAN Filter register
    */
  union
  {
    __IOM uint32_t FM;

    struct
    {
      __IOM uint32_t FBM1       :1;
      __IOM uint32_t FBM2       :1;
      __IOM uint32_t FBM3       :1;
      __IOM uint32_t FBM4       :1;
      __IOM uint32_t FBM5       :1;
      __IOM uint32_t FBM6       :1;
      __IOM uint32_t FBM7       :1;
      __IOM uint32_t FBM8       :1;
      __IOM uint32_t FBM9       :1;
      __IOM uint32_t FBM10      :1;
      __IOM uint32_t FBM11      :1;
      __IOM uint32_t FBM12      :1;
      __IOM uint32_t FBM13      :1;
      __IOM uint32_t FBM14      :1;
      __IOM uint32_t FBM15      :1;
      __IOM uint32_t FBM16      :1;
      __IOM uint32_t FBM17      :1;
      __IOM uint32_t FBM18      :1;
      __IOM uint32_t FBM19      :1;
      __IOM uint32_t FBM20      :1;
      __IOM uint32_t FBM21      :1;
      __IOM uint32_t FBM22      :1;
      __IOM uint32_t FBM23      :1;
      __IOM uint32_t FBM24      :1;
      __IOM uint32_t FBM25      :1;
      __IOM uint32_t FBM26      :1;
      __IOM uint32_t FBM27      :1;
      __IOM uint32_t FBM28      :1;
      __IM uint32_t RESERVED    : 4;
    } FM_B;
  };

  __IM uint32_t RESERVED2;


  /**
   * @brief     CAN Filter bit width register
    */
  union
  {
    __IOM uint32_t FBW;

    struct
    {
      __IOM uint32_t BW1        :1;
      __IOM uint32_t BW2        :1;
      __IOM uint32_t BW3        :1;
      __IOM uint32_t BW4        :1;
      __IOM uint32_t BW5        :1;
      __IOM uint32_t BW6        :1;
      __IOM uint32_t BW7        :1;
      __IOM uint32_t BW8        :1;
      __IOM uint32_t BW9        :1;
      __IOM uint32_t BW10       :1;
      __IOM uint32_t BW11       :1;
      __IOM uint32_t BW12       :1;
      __IOM uint32_t BW13       :1;
      __IOM uint32_t BW14       :1;
      __IOM uint32_t BW15       :1;
      __IOM uint32_t BW16       :1;
      __IOM uint32_t BW17       :1;
      __IOM uint32_t BW18       :1;
      __IOM uint32_t BW19       :1;
      __IOM uint32_t BW20       :1;
      __IOM uint32_t BW21       :1;
      __IOM uint32_t BW22       :1;
      __IOM uint32_t BW23       :1;
      __IOM uint32_t BW24       :1;
      __IOM uint32_t BW25       :1;
      __IOM uint32_t BW26       :1;
      __IOM uint32_t BW27       :1;
      __IOM uint32_t BW28       :1;
      __IM uint32_t RESERVED    : 4;
    } FBW_B;
  };

  __IM uint32_t RESERVED3;

  /**
   * @brief     CAN Filter FIFO associated registers
    */
  union
  {
    __IOM uint32_t FFA;

    struct
    {
      __IOM uint32_t FFA1       :1;
      __IOM uint32_t FFA2       :1;
      __IOM uint32_t FFA3       :1;
      __IOM uint32_t FFA4       :1;
      __IOM uint32_t FFA5       :1;
      __IOM uint32_t FFA6       :1;
      __IOM uint32_t FFA7       :1;
      __IOM uint32_t FFA8       :1;
      __IOM uint32_t FFA9       :1;
      __IOM uint32_t FFA10      :1;
      __IOM uint32_t FFA11      :1;
      __IOM uint32_t FFA12      :1;
      __IOM uint32_t FFA13      :1;
      __IOM uint32_t FFA14      :1;
      __IOM uint32_t FFA15      :1;
      __IOM uint32_t FFA16      :1;
      __IOM uint32_t FFA17      :1;
      __IOM uint32_t FFA18      :1;
      __IOM uint32_t FFA19      :1;
      __IOM uint32_t FFA20      :1;
      __IOM uint32_t FFA21      :1;
      __IOM uint32_t FFA22      :1;
      __IOM uint32_t FFA23      :1;
      __IOM uint32_t FFA24      :1;
      __IOM uint32_t FFA25      :1;
      __IOM uint32_t FFA26      :1;
      __IOM uint32_t FFA27      :1;
      __IOM uint32_t FFA28      :1;
      __IM uint32_t RESERVED    : 4;
    } FFA_B;
  };

  __IM uint32_t RESERVED4;
  /**
   * @brief     CAN Filter activation register
    */
  union
  {
    __IOM uint32_t FEN;

    struct
    {
      __IOM uint32_t FEN1       :1;
      __IOM uint32_t FEN2       :1;
      __IOM uint32_t FEN3       :1;
      __IOM uint32_t FEN4       :1;
      __IOM uint32_t FEN5       :1;
      __IOM uint32_t FEN6       :1;
      __IOM uint32_t FEN7       :1;
      __IOM uint32_t FEN8       :1;
      __IOM uint32_t FEN9       :1;
      __IOM uint32_t FEN10      :1;
      __IOM uint32_t FEN11      :1;
      __IOM uint32_t FEN12      :1;
      __IOM uint32_t FEN13      :1;
      __IOM uint32_t FEN14      :1;
      __IOM uint32_t FEN15      :1;
      __IOM uint32_t FEN16      :1;
      __IOM uint32_t FEN17      :1;
      __IOM uint32_t FEN18      :1;
      __IOM uint32_t FEN19      :1;
      __IOM uint32_t FEN20      :1;
      __IOM uint32_t FEN21      :1;
      __IOM uint32_t FEN22      :1;
      __IOM uint32_t FEN23      :1;
      __IOM uint32_t FEN24      :1;
      __IOM uint32_t FEN25      :1;
      __IOM uint32_t FEN26      :1;
      __IOM uint32_t FEN27      :1;
      __IOM uint32_t FEN28      :1;
      __IM uint32_t RESERVED    : 4;
    } FEN_B;
  };

  __IM uint32_t RESERVED5[8];

  CAN_FilterRegister_T sFilterRegister[14];

} CAN_T;


/**
 * @brief    I2C register
 */
typedef struct
{
  /**
   * @brief Control register 1
   */
  union
  {
    __IOM uint32_t CTRL1;

    struct
    {
      __IOM uint32_t I2CEN      : 1;
      __IOM uint32_t SMB        : 1;
      __IM uint32_t RESERVED1   : 1;
      __IOM uint32_t SMBT       : 1;
      __IOM uint32_t ARPEN      : 1;
      __IOM uint32_t PECEN      : 1;
      __IOM uint32_t BCEN       : 1;
      __IOM uint32_t STRDIS     : 1;
      __IOM uint32_t STA        : 1;
      __IOM uint32_t STOP       : 1;
      __IOM uint32_t ACKEN      : 1;
      __IOM uint32_t ACKPOS     : 1;
      __IOM uint32_t TPECEN     : 1;
      __IOM uint32_t SMB_ALT    : 1;
      __IM uint32_t RESERVED2   : 1;
      __IOM uint32_t SWRST      : 1;
      __IM uint32_t RESERVED3   : 16;
    } CTRL1_B;
  } ;

  /**
   * @brief Control register 2
   */
  union
  {
    __IOM uint32_t CTRL2;

    struct
    {
      __IOM uint32_t FREQ       : 6;
      __IM uint32_t RESERVED1   : 2;
      __IOM uint32_t ERRIE      : 1;
      __IOM uint32_t EVTIE      : 1;
      __IOM uint32_t BUFIE      : 1;
      __IOM uint32_t DMAEN      : 1;
      __IOM uint32_t LAST       : 1;
      __IM uint32_t RESERVED2   : 19;
    } CTRL2_B;
  } ;

  /**
  * @brief Slave machine address register 1
  */
  union
  {
    __IOM uint32_t ADD1;

    struct
    {
      __IOM uint32_t ADD0       : 1;
      __IOM uint32_t ADD1_7     : 7;
      __IOM uint32_t ADD8_9     : 2;
      __IM uint32_t RESERVED1   : 4;
      __IOM uint32_t ADDCFG     : 1;
      __IOM uint32_t ADDMODE    : 1;
      __IM uint32_t RESERVED2   : 16;
    } ADD1_B;
  };

  /**
  * @brief Slave machine address register 2
  */
  union
  {
    __IOM uint32_t ADD2;

    struct
    {
      __IOM uint32_t DUALEN     : 1;
      __IOM uint32_t ADD2       : 7;
      __IM uint32_t RESERVED    : 24;
    } ADD2_B;
  };

  /**
  * @brief Cache data register
  */
  union
  {
    __IOM uint32_t DATA;

    struct
    {
      __IOM uint32_t DATA       : 8;
      __IM uint32_t RESERVED    : 24;
    } DATA_B;
  };

  /**
  * @brief Status register 1
  */
  union
  {
    __IOM uint32_t STS1;

    struct
    {
      __IOM uint32_t SBTCF      : 1;
      __IOM uint32_t ADDRF      : 1;
      __IOM uint32_t BTCF       : 1;
      __IOM uint32_t ADDR10F    : 1;
      __IOM uint32_t SBDF       : 1;
      __IM uint32_t RESERVED1   : 1;
      __IOM uint32_t RXBENF     : 1;
      __IOM uint32_t TXBENF     : 1;
      __IOM uint32_t BEF        : 1;
      __IOM uint32_t ALF        : 1;
      __IOM uint32_t AEF        : 1;
      __IOM uint32_t OUF        : 1;
      __IOM uint32_t PECEF      : 1;
      __IM uint32_t RESERVED2   : 1;
      __IOM uint32_t TIMEOUT    : 1;
      __IOM uint32_t SMB_ALTF   : 1;
      __IM uint32_t RESERVED3   : 16;
    } STS1_B;
  };

  /**
  * @brief Status register 2
  */
  union
  {
    __IOM uint32_t STS2;

    struct
    {
      __IOM uint32_t MMF        : 1;
      __IOM uint32_t BUSYF      : 1;
      __IOM uint32_t RWMF       : 1;
      __IM uint32_t RESERVED1   : 1;
      __IOM uint32_t RBF        : 1;
      __IOM uint32_t SMB_DAF    : 1;
      __IOM uint32_t SMB_HHF    : 1;
      __IOM uint32_t DMAF       : 1;
      __IOM uint16_t PEC_DATA   : 8;
      __IM uint32_t RESERVED2   : 16;
    } STS2_B;
  };

  /**
  * @brief Clock control register
  */
  union
  {
    __IOM uint32_t CLKCTRL;

    struct
    {
      __IOM uint32_t CDR        : 12;
      __IM uint32_t RESERVED1   : 2;
      __IOM uint32_t FMDC       : 1;
      __IOM uint32_t FASTMODE   : 1;
      __IM uint32_t RESERVED2   : 16;
    } CLKCTRL_B;
  };

  /**
  * @brief TRISE
  */
  union
  {
    __IOM uint32_t TRISE;

    struct
    {
      __IOM uint32_t TRISE      : 6;
      __IM uint32_t RESERVED    : 26;
    } TRISE_B;
  };

  __IM uint32_t RESERVED[55];

  /**
  * @brief I2C Switching register
  */
  union
  {
    __IOM uint32_t SWITCH;

    struct
    {
      __IOM uint32_t SWITCH     : 1;
      __IM uint32_t RESERVED1   : 31;
    } SEITCH_B;
  };
} I2C_T;


typedef struct
{
    __IOM uint16_t RDP;
    __IOM uint16_t USER;
    __IOM uint16_t Data0;
    __IOM uint16_t Data1;
    __IOM uint16_t WRP0;
    __IOM uint16_t WRP1;
    __IOM uint16_t WRP2;
    __IOM uint16_t WRP3;
} OB_T;

/** 
  * @brief Analog to Digital Converter
  */
typedef struct
{
    
    /** Status register */
    union 
    {
        __IOM uint32_t STS;                         

        struct 
        {
            __IOM  uint32_t AWDF        : 1;
            __IOM  uint32_t CCF         : 1;
            __IOM  uint32_t IJEOCF      : 1;
            __IOM  uint32_t IJSTRF      : 1;
            __IOM  uint32_t STRT        : 1;
            __IM  uint32_t RESERVED     : 27;
        } STS_B;
    } ;
    
    /** Control register1*/
    union 
    {
        __IOM uint32_t CTRL1;                         

        struct 
        {
            __IOM  uint32_t AWDCS       : 5;
            __IOM  uint32_t EOCIEN      : 1;
            __IOM  uint32_t AWDIEN      : 1;
            __IOM  uint32_t IJEOCIEN    : 1;
            __IOM  uint32_t SMEN        : 1;
            __IOM  uint32_t AWDSC       : 1;
            __IOM  uint32_t IJAEN       : 1;
            __IOM  uint32_t RGDMEN      : 1;
            __IOM  uint32_t IJDMEN      : 1;
            __IOM  uint32_t DMCC        : 3;
            __IOM  uint32_t DMS         : 4;
            __IM   uint32_t RESERVED    : 2;
            __IOM  uint32_t IJAWDEN     : 1;
            __IOM  uint32_t RGAWDEN     : 1;
            __IM   uint32_t RESERVED1   : 8;
        } CTRL1_B;
    } ;
    
    /** Control register2*/
    union 
    {
        __IOM uint32_t CTRL2;                         

        struct 
        {
            __IOM  uint32_t ADCON               : 1;
            __IOM  uint32_t CCM                 : 1;
            __IOM  uint32_t CALSTR              : 1;
            __IOM  uint32_t CALRST              : 1;
            __IM   uint32_t RESERVED            : 4;
            __IOM  uint32_t DMAEN               : 1;
            __IM   uint32_t RESERVED1           : 2;
            __IOM  uint32_t DAM                 : 1;
            __IOM  uint32_t IJEXTSEL            : 3;
            __IOM  uint32_t IJEXTGEN            : 1;
            __IM   uint32_t RESERVED2           : 1;
            __IOM  uint32_t RGEXTSEL            : 3;
            __IOM  uint32_t RGEXTGEN            : 1;
            __IOM  uint32_t IJSWSTR             : 1;
            __IOM  uint32_t RGSWSTR             : 1;
            __IOM  uint32_t TVEN                : 1;
            __IM   uint32_t RESERVED3           : 8;
        } CTRL2_B;
    } ;
    
    /** Sample time register1*/
    union 
    {
        __IOM uint32_t SMPT1;                         

        struct 
        {
            __IOM  uint32_t SMPT10          : 3;
            __IOM  uint32_t SMPT11          : 3;
            __IOM  uint32_t SMPT12          : 3;
            __IOM  uint32_t SMPT13          : 3;
            __IOM  uint32_t SMPT14          : 3;
            __IOM  uint32_t SMPT15          : 3;
            __IOM  uint32_t SMPT16          : 3;
            __IOM  uint32_t SMPT17          : 3;
            __IM   uint32_t RESERVED        : 8;
        } SMPT1_B;
    } ;
    
    /** Sample time register2*/
    union 
    {
        __IOM uint32_t SMPT2;                         

        struct 
        {
            __IOM  uint32_t SMPT0       : 3;
            __IOM  uint32_t SMPT1       : 3;
            __IOM  uint32_t SMPT2       : 3;
            __IOM  uint32_t SMPT3       : 3;
            __IOM  uint32_t SMPT4       : 3;
            __IOM  uint32_t SMPT5       : 3;
            __IOM  uint32_t SMPT6       : 3;
            __IOM  uint32_t SMPT7       : 3;
            __IOM  uint32_t SMPT8       : 3;
            __IOM  uint32_t SMPT9       : 3;
            __IM   uint32_t RESERVED    : 2;
        } SMPT2_B;
    } ;
    
    /** Injected channel Data offset register1*/
    union 
    {
        __IOM uint32_t IJOF1;                         

        struct 
        {
            __IOM  uint32_t OFFSET1         : 12;
            __IM   uint32_t RESERVED        : 20;
        } IJOF1_B;
    } ;

    /** Injected channel Data offset register2*/
    union 
    {
        __IOM uint32_t IJOF2;                         

        struct 
        {
            __IOM  uint32_t OFFSET2         : 12;
            __IM   uint32_t RESERVED        : 20;
        } IJOF2_B;
    } ;
    
    /** Injected channel Data offset register3*/
    union 
    {
        __IOM uint32_t IJOF3;                         

        struct 
        {
            __IOM  uint32_t OFFSET3         : 12;
            __IM   uint32_t RESERVED        : 20;
        } IJOF3_B;
    } ;
    
    /** Injected channel Data offset register4*/
    union 
    {
        __IOM uint32_t IJOF4;                         

        struct 
        {
            __IOM  uint32_t OFFSET4         : 12;
            __IM   uint32_t RESERVED        : 20;
        } IJOF4_B;
    } ;
    
    /** Analog watchdog high threshold register*/
    union 
    {
        __IOM uint32_t AWDHT;                         

        struct 
        {
            __IOM  uint32_t AWDHT          : 12;
            __IM   uint32_t RESERVED       : 20;
        } AWDHT_B;
    } ;
    
    /** Analog watchdog low threshold register*/
    union 
    {
        __IOM uint32_t AWDLT;                         

        struct 
        {
            __IOM  uint32_t AWDLT          : 12;
            __IM   uint32_t RESERVED       : 20;
        } AWDLT_B;
    } ;
    
    /** Regular channel sequence register1*/
    union 
    {
        __IOM uint32_t RGSQ1;                         

        struct 
        {
            __IOM  uint32_t RGSQ13          : 5;
            __IOM  uint32_t RGSQ14          : 5;
            __IOM  uint32_t RGSQ15          : 5;
            __IOM  uint32_t RGSQ16          : 5;
            __IOM  uint32_t RGSL            : 4;
            __IM   uint32_t RESERVED        : 8;
        } RGSQ1_B;
    } ;
    
    /** Regular channel sequence register2*/
    union 
    {
        __IOM uint32_t RGSQ2;                         

        struct 
        {
            __IOM  uint32_t RGSQ7           : 5;
            __IOM  uint32_t RGSQ8           : 5;
            __IOM  uint32_t RGSQ9           : 5;
            __IOM  uint32_t RGSQ10          : 5;
            __IOM  uint32_t RGSQ11          : 5;
            __IOM  uint32_t RGSQ12          : 5;
            __IM   uint32_t RESERVED        : 2;
        } RGSQ2_B;
    } ;
    
    /** Regular channel sequence register3*/
    union 
    {
        __IOM uint32_t RGSQ3;                         

        struct 
        {
            __IOM  uint32_t RGSQ1          : 5;
            __IOM  uint32_t RGSQ2          : 5;
            __IOM  uint32_t RGSQ3          : 5;
            __IOM  uint32_t RGSQ4          : 5;
            __IOM  uint32_t RGSQ5          : 5;
            __IOM  uint32_t RGSQ6          : 5;
            __IM   uint32_t RESERVED       : 2;
        } RGSQ3_B;
    } ;
    
    /** Injected sequence register*/
    union 
    {
        __IOM uint32_t IJSQ;                         

        struct 
        {
            __IOM  uint32_t IJSQ1           : 5;
            __IOM  uint32_t IJSQ2           : 5;
            __IOM  uint32_t IJSQ3           : 5;
            __IOM  uint32_t IJSQ4           : 5;
            __IOM  uint32_t IJSL            : 2;
            __IM   uint32_t RESERVED        : 10;
        } IJSQ_B;
    } ;
    
    /** Injected Data register1*/
    union 
    {
        __IOM uint32_t IJD1;                         

        struct 
        {
            __IM   uint32_t IJD1            : 16;
            __IM   uint32_t RESERVED        : 16;
        } IJD1_B;
    } ;
    
    /** Injected Data register2*/
    union 
    {
        __IOM uint32_t IJD2;                         

        struct 
        {
            __IM   uint32_t IJD2            : 16;
            __IM   uint32_t RESERVED        : 16;
        } IJD2_B;
    } ;

    /** Injected Data register3*/
    union 
    {
        __IOM uint32_t IJD3;                         

        struct 
        {
            __IM   uint32_t IJD3            : 16;
            __IM   uint32_t RESERVED        : 16;
        } IJD3_B;
    } ;    

    /** Injected Data register4*/
    union 
    {
        __IOM uint32_t IJD4;                         

        struct 
        {
            __IM   uint32_t IJD4            : 16;
            __IM   uint32_t RESERVED        : 16;
        } IJD4_B;
    } ;
    
    /** Regular Data register*/
    union 
    {
        __IOM uint32_t RDG;                         

        struct 
        {
            __IM  uint32_t RDATA            : 16;
            __IM  uint32_t ADC2DATA         : 16;
        } RDG_B;
    } ;
    
}ADC_T;

/** 
  * @brief External Interrupt
  */
typedef struct
{
    
    /** Interrupt Enable register */
    union 
    {
        __IOM uint32_t IEN;                         

        struct 
        {
            __IOM  uint32_t IEN0      : 1;
            __IOM  uint32_t IEN1      : 1;
            __IOM  uint32_t IEN2      : 1;
            __IOM  uint32_t IEN3      : 1;
            __IOM  uint32_t IEN4      : 1;
            __IOM  uint32_t IEN5      : 1;
            __IOM  uint32_t IEN6      : 1;
            __IOM  uint32_t IEN7      : 1;
            __IOM  uint32_t IEN8      : 1;
            __IOM  uint32_t IEN9      : 1;
            __IOM  uint32_t IEN10     : 1;
            __IOM  uint32_t IEN11     : 1;
            __IOM  uint32_t IEN12     : 1;
            __IOM  uint32_t IEN13     : 1;
            __IOM  uint32_t IEN14     : 1;
            __IOM  uint32_t IEN15     : 1;
            __IOM  uint32_t IEN16     : 1;
            __IOM  uint32_t IEN17     : 1;
            __IOM  uint32_t IEN18     : 1;
            __IOM  uint32_t IEN19     : 1;
            __IM  uint32_t RESERVED   : 12;
        } IEN_B;
    } ;
    
    /** Interrupt Event Enable register */
    union 
    {
        __IOM uint32_t EEN;                         

        struct 
        {
            __IOM  uint32_t EEN0      : 1;
            __IOM  uint32_t EEN1      : 1;
            __IOM  uint32_t EEN2      : 1;
            __IOM  uint32_t EEN3      : 1;
            __IOM  uint32_t EEN4      : 1;
            __IOM  uint32_t EEN5      : 1;
            __IOM  uint32_t EEN6      : 1;
            __IOM  uint32_t EEN7      : 1;
            __IOM  uint32_t EEN8      : 1;
            __IOM  uint32_t EEN9      : 1;
            __IOM  uint32_t EEN10     : 1;
            __IOM  uint32_t EEN11     : 1;
            __IOM  uint32_t EEN12     : 1;
            __IOM  uint32_t EEN13     : 1;
            __IOM  uint32_t EEN14     : 1;
            __IOM  uint32_t EEN15     : 1;
            __IOM  uint32_t EEN16     : 1;
            __IOM  uint32_t EEN17     : 1;
            __IOM  uint32_t EEN18     : 1;
            __IOM  uint32_t EEN19     : 1;
            __IM  uint32_t RESERVED   : 12;
        } EEN_B;
    } ;
    
    /** Rising Trigger Event Enable register */
    union 
    {
        __IOM uint32_t RTEN;                         

        struct 
        {
            __IOM  uint32_t RT0      : 1;
            __IOM  uint32_t RT1      : 1;
            __IOM  uint32_t RT2      : 1;
            __IOM  uint32_t RT3      : 1;
            __IOM  uint32_t RT4      : 1;
            __IOM  uint32_t RT5      : 1;
            __IOM  uint32_t RT6      : 1;
            __IOM  uint32_t RT7      : 1;
            __IOM  uint32_t RT8      : 1;
            __IOM  uint32_t RT9      : 1;
            __IOM  uint32_t RT10     : 1;
            __IOM  uint32_t RT11     : 1;
            __IOM  uint32_t RT12     : 1;
            __IOM  uint32_t RT13     : 1;
            __IOM  uint32_t RT14     : 1;
            __IOM  uint32_t RT15     : 1;
            __IOM  uint32_t RT16     : 1;
            __IOM  uint32_t RT17     : 1;
            __IOM  uint32_t RT18     : 1;
            __IOM  uint32_t RT19     : 1;
            __IM  uint32_t RESERVED  : 12;
        } RTEN_B;
    } ;
    
    /** Falling Trigger Event Enable register */
    union 
    {
        __IOM uint32_t FTEN;                         

        struct 
        {
            __IOM  uint32_t FT0      : 1;
            __IOM  uint32_t FT1      : 1;
            __IOM  uint32_t FT2      : 1;
            __IOM  uint32_t FT3      : 1;
            __IOM  uint32_t FT4      : 1;
            __IOM  uint32_t FT5      : 1;
            __IOM  uint32_t FT6      : 1;
            __IOM  uint32_t FT7      : 1;
            __IOM  uint32_t FT8      : 1;
            __IOM  uint32_t FT9      : 1;
            __IOM  uint32_t FT10     : 1;
            __IOM  uint32_t FT11     : 1;
            __IOM  uint32_t FT12     : 1;
            __IOM  uint32_t FT13     : 1;
            __IOM  uint32_t FT14     : 1;
            __IOM  uint32_t FT15     : 1;
            __IOM  uint32_t FT16     : 1;
            __IOM  uint32_t FT17     : 1;
            __IOM  uint32_t FT18     : 1;
            __IOM  uint32_t FT19     : 1;
            __IM  uint32_t RESERVED  : 12;
        } FTEN_B;
    } ;
    
    /** Software Interrupt Enable register */
    union 
    {
        __IOM uint32_t SWIEN;                         

        struct 
        {
            __IOM  uint32_t SWIEN0      : 1;
            __IOM  uint32_t SWIEN1      : 1;
            __IOM  uint32_t SWIEN2      : 1;
            __IOM  uint32_t SWIEN3      : 1;
            __IOM  uint32_t SWIEN4      : 1;
            __IOM  uint32_t SWIEN5      : 1;
            __IOM  uint32_t SWIEN6      : 1;
            __IOM  uint32_t SWIEN7      : 1;
            __IOM  uint32_t SWIEN8      : 1;
            __IOM  uint32_t SWIEN9      : 1;
            __IOM  uint32_t SWIEN10     : 1;
            __IOM  uint32_t SWIEN11     : 1;
            __IOM  uint32_t SWIEN12     : 1;
            __IOM  uint32_t SWIEN13     : 1;
            __IOM  uint32_t SWIEN14     : 1;
            __IOM  uint32_t SWIEN15     : 1;
            __IOM  uint32_t SWIEN16     : 1;
            __IOM  uint32_t SWIEN17     : 1;
            __IOM  uint32_t SWIEN18     : 1;
            __IOM  uint32_t SWIEN19     : 1;
            __IM  uint32_t RESERVED        : 12;
        } SWIEN_B;
    } ;
    
    /** Interrupt Flag Enable register */
    union 
    {
        __IOM uint32_t IF;                         

        struct 
        {
            __IOM  uint32_t IF0      : 1;
            __IOM  uint32_t IF1      : 1;
            __IOM  uint32_t IF2      : 1;
            __IOM  uint32_t IF3      : 1;
            __IOM  uint32_t IF4      : 1;
            __IOM  uint32_t IF5      : 1;
            __IOM  uint32_t IF6      : 1;
            __IOM  uint32_t IF7      : 1;
            __IOM  uint32_t IF8      : 1;
            __IOM  uint32_t IF9      : 1;
            __IOM  uint32_t IF10     : 1;
            __IOM  uint32_t IF11     : 1;
            __IOM  uint32_t IF12     : 1;
            __IOM  uint32_t IF13     : 1;
            __IOM  uint32_t IF14     : 1;
            __IOM  uint32_t IF15     : 1;
            __IOM  uint32_t IF16     : 1;
            __IOM  uint32_t IF17     : 1;
            __IOM  uint32_t IF18     : 1;
            __IOM  uint32_t IF19     : 1;
            __IM  uint32_t RESERVED     : 12;
        } IF_B;
    } ;
    
}EINT_T;

/** 
  * @brief Independent WATCHDOG
  */
typedef struct
{
    
    /** Keyword register */
    union 
    {
        __OM uint32_t KEYWORD;                         

        struct 
        {
            __OM  uint32_t KEYWORD          : 16;
            __IM  uint32_t RESERVED         : 16;
        } KEYWORD_B;
    } ;
    
    /** Frequency Divider register */
    
    union 
    {
        __IOM uint32_t DIV;                         

        struct 
        {
            __IOM  uint32_t DIV             : 3;
            __IM  uint32_t RESERVED         : 29;
        } DIV_B;
    } ;

    /** Reload values register */
    
    union 
    {
        __IOM uint32_t CNTRLD;                         

        struct 
        {
            __IOM  uint32_t CNTRLD          : 12;
            __IM  uint32_t RESERVED         : 20;
        } CNTRLD_B;
    } ;
    
    /** Status register */
    
    union 
    {
        __IM uint32_t STS;                         

        struct 
        {
            __IM  uint32_t DVU              : 1;
            __IM  uint32_t CNTRU            : 1;
            __IM  uint32_t RESERVED         : 30;
        } STS_B;
    } ;
    
}IWDT_T;

/**
 * @brief SPI register
 */
typedef struct
{
    /** Control register 1 */
    union
    {
        __IOM uint32_t CTRL1;
        
        struct
        {
            __IOM  uint32_t CLKPHA          : 1;
            __IOM  uint32_t CLKPOL          : 1;
            __IOM  uint32_t MSTMODE         : 1;
            __IOM  uint32_t BRC             : 3;
            __IOM  uint32_t SPIEN           : 1;
            __IOM  uint32_t LSBF            : 1;
            __IOM  uint32_t ISS             : 1;
            __IOM  uint32_t SSC             : 1;
            __IOM  uint32_t UMRXO           : 1;
            __IOM  uint32_t DFL             : 1;
            __IOM  uint32_t CRCNXT          : 1;
            __IOM  uint32_t CRCEN           : 1;
            __IOM  uint32_t BMTX            : 1;
            __IOM  uint32_t BMEN            : 1;
            __IM   uint32_t RESERVED        : 16;
        } CTRL1_B;
    };
    
    /** Control register 2 */
    union
    {
        __IOM uint32_t CTRL2;
        
        struct
        {
            __IOM  uint32_t RX_DMAEN        : 1;
            __IOM  uint32_t TX_DMAEN        : 1;
            __IOM  uint32_t SSOEN           : 1;
            __IM   uint32_t RESERVED1       : 2;
            __IOM  uint32_t ERRIE           : 1;
            __IOM  uint32_t RXBNEIE         : 1;
            __IOM  uint32_t TXBEIE          : 1;
            __IM   uint32_t RESERVED2       : 24;
        } CTRL2_B;
    };
    
    /** Status register */
    union
    {
        __IOM uint32_t STS;
        
        struct
        {
            __IM  uint32_t RXBNEF           : 1;
            __IM  uint32_t TXBEF            : 1;
            __IM  uint32_t ST               : 1;
            __IM  uint32_t UDF              : 1;
            __IOM uint32_t CRCEF            : 1;
            __IM  uint32_t MEF              : 1;
            __IM  uint32_t RXOF             : 1;
            __IM  uint32_t BUSYF            : 1;
            __IM  uint32_t RESERVED         : 24;
        } STS_B;
    };
    
    /** Data register */
    union
    {
        __IOM uint32_t DATA;
        
        struct
        {
            __IOM  uint32_t DATA           : 16;
            __IM  uint32_t RESERVED        : 16;
        } DATA_B;
    };
    
    /** CRC polynomial register */
    union
    {
        __IOM uint32_t CRCPOLY;
        
        struct
        {
            __IOM  uint32_t CRCPOLY         : 16;
            __IM  uint32_t RESERVED         : 16;
        } CRCPOLY_B;
    };
    
    /** Receive CRC register */
    union
    {
        __IOM uint32_t RXCRC;
        
        struct
        {
            __IOM  uint32_t RXCRC           : 16;
            __IM  uint32_t RESERVED         : 16;
        }RXCRC_B;
    };
    
    /** Transmit CRC register */
    union
    {
        __IOM uint32_t TXCRC;
        
        struct
        {
            __IOM  uint32_t TXCRC           : 16;
            __IM  uint32_t RESERVED         : 16;
        }TXCRC_B;
    };

    /** Transmit I2S CTRL register */
    union 
    {
        __IOM uint32_t I2SCTRL;

        struct 
        {
            __IOM uint32_t CHLEN     : 1;
            __IOM uint32_t DATALEN   : 2;
            __IOM uint32_t CLKPOL    : 1;
            __IOM uint32_t I2SSTD    : 2;
            __IM uint32_t RESERVED1  : 1;
            __IOM uint32_t PCMSYNC   : 1;
            __IOM uint32_t I2SCFMODE : 2;
            __IOM uint32_t I2SEN     : 1;
            __IOM uint32_t I2SMODE   : 1;
            __IM uint32_t RESERVED2  : 20;
        }I2SCTRL_B;
    };

    /** Transmit I2S DIV register */
    union 
    {
        __IOM uint32_t I2SDIV;

        struct 
        {
            __IOM uint32_t I2SDIV    : 8;
            __IOM uint32_t ODD       : 1;
            __IOM uint32_t MCKOEN    : 1;
            __IM uint32_t RESERVED1  : 22;
        }I2SDIV_B;
    };
}SPI_T;


/** 
  * @brief Window WATCHDOG
  */
typedef struct
{
    
    /** Control register */
    union 
    {
        __IOM uint32_t CTRL;                         

        struct 
        {
            __IOM  uint32_t CNT             : 7;
            __IOM  uint32_t WWDTEN          : 1;
            __IM  uint32_t RESERVED         : 24;
        } CTRL_B;
    } ;

    /** Configure register */
    union 
    {
        __IOM uint32_t CFG;                         

        struct 
        {
            __IOM  uint32_t WDDATA          : 7;
            __IOM  uint32_t WDTTB           : 2;
            __IOM  uint32_t EWIEN           : 1;
            __IM  uint32_t RESERVED         : 22;
        } CFG_B;
    } ;
    
    /** Status register */
    union 
    {
        __IOM uint32_t STS;                         

        struct 
        {
            __IOM  uint32_t EWIF            : 1;
            __IM  uint32_t RESERVED         : 31;
        } STS_B;
    } ;
    
}WWDT_T;

/** 
  * @brief SD host Interface
  */
typedef struct
{
    /** Power control register */
    union 
    {
        __IOM uint32_t PWR;                         

        struct 
        {
            __IOM  uint32_t PWRCTRL     : 2;
            __IM  uint32_t RESERVED     : 30;
        } PWR_B;
    } ;

    /** Clock control register */
    union 
    {
        __IOM uint32_t CLKCTRL;                         

        struct 
        {
            __IOM  uint32_t CLKDIV      : 8;
            __IOM  uint32_t CLKEN       : 1;
            __IOM  uint32_t PWRSAV      : 1;
            __IOM  uint32_t BYPEN       : 1;
            __IOM  uint32_t WIDBUSEN    : 2;
            __IOM  uint32_t DEPHSEL     : 1;
            __IOM  uint32_t HWFCEN      : 1;
            __IM  uint32_t RESERVED     : 17;
        } CLKCTRL_B;
    } ;

    /** Argument register */
    union 
    {
        __IOM uint32_t ARG;                         

        struct 
        {
            __IOM  uint32_t CMDARG      : 32;
        } ARG_B;
    } ;

    /** Command register */
    union 
    {
        __IOM uint32_t CMD;                         

        struct 
        {
            __IOM  uint32_t CMDINDEX    : 6;
            __IOM  uint32_t WAITRES     : 2;
            __IOM  uint32_t WAITINT     : 1;
            __IOM  uint32_t WAITCPEND   : 1;
            __IOM  uint32_t CPSMEN      : 1;
            __IOM  uint32_t SDIOPAUSE   : 1;
            __IOM  uint32_t ENCMDCOMPF  : 1;
            __IOM  uint32_t NINTEN      : 1;
            __IOM  uint32_t ATACMD      : 1;
            __IM  uint32_t RESERVED     : 17;
        } CMD_B;
    } ;

    /** Command  response register */
    union 
    {
        __IOM uint32_t CMDRES;                         

        struct 
        {
            __IOM  uint32_t CMDRES      : 6;
            __IM  uint32_t RESERVED     : 26;
        } CMDRES_B;
    } ;

    /** SDIO  response register1 */
    union 
    {
        __IM uint32_t RES1;                         

        struct 
        {
            __IM  uint32_t CARDSTS      : 32;
        } RES1_B;
    } ;
    
    /** SDIO  response register2 */
    union 
    {
        __IM uint32_t RES2;                         

        struct 
        {
            __IM  uint32_t CARDSTS      : 32;
        } RES2_B;
    } ;

    /** SDIO  response register3 */
    union 
    {
        __IM uint32_t RES3;                         

        struct 
        {
            __IM  uint32_t CARDSTS      : 32;
        } RES3_B;
    } ;

    /** SDIO  response register4 */
    union 
    {
        __IM uint32_t RES4;                         
    
        struct 
        {
            __IM  uint32_t CARDSTS      : 32;
        } RES4_B;
    } ;

    /** Data timer register */
    union 
    {
        __IOM uint32_t DTMR;                         

        struct 
        {
            __IOM  uint32_t DATATIME      : 32;
        } DTMR_B;
    } ;

    /** Data length register */
    union 
    {
        __IOM uint32_t DLEN;                         

        struct 
        {
            __IOM  uint32_t DATALEN     : 25;
            __IM  uint32_t RESERVED     : 7;
        } DLEN_B;
    } ;

    /** Data control register */
    union 
    {
        __IOM uint32_t DCTRL;                         

        struct 
        {
            __IOM  uint32_t DTEN        : 1;
            __IOM  uint32_t DTDOT       : 1;
            __IOM  uint32_t DTMODE      : 1;
            __IOM  uint32_t DMAEN       : 1;
            __IOM  uint32_t DBLOCKSIZE  : 4;
            __IOM  uint32_t RWSTR       : 1;
            __IOM  uint32_t RWSTOP      : 1;
            __IOM  uint32_t RWMODE      : 1;
            __IOM  uint32_t SDIOEN      : 1;
            __IM  uint32_t RESERVED     : 20;
        } DCTRL_B;
    } ;

    /** Data count register */
    union 
    {
        __IM uint32_t DCNT;                         

        struct 
        {
            __IM  uint32_t DATACNT      : 25;
            __IM  uint32_t RESERVED     : 7;
        } DCNT_B;
    } ;

    /** SDIO status register */
    union 
    {
        __IM uint32_t STS;                         

        struct 
        {
            __IM  uint32_t CCRCRAIL     : 1;
            __IM  uint32_t DCRCFAIL     : 1;
            __IM  uint32_t CTIMEOUT     : 1;
            __IM  uint32_t DTIMEOUT     : 1;
            __IM  uint32_t TXUNDERR     : 1;
            __IM  uint32_t RXOVERR      : 1;
            __IM  uint32_t CMDRES       : 1;
            __IM  uint32_t CMDSENT      : 1;
            __IM  uint32_t DEND         : 1;
            __IM  uint32_t STRBITERR    : 1;
            __IM  uint32_t DBCKEND      : 1;
            __IM  uint32_t CMDACT       : 1;
            __IM  uint32_t TXACT        : 1;
            __IM  uint32_t RXACT        : 1;
            __IM  uint32_t TXFIFOHE     : 1;
            __IM  uint32_t RXFIFOHE     : 1;
            __IM  uint32_t TXFIFOF      : 1;
            __IM  uint32_t RXFIFOF      : 1;
            __IM  uint32_t TXFIFOE      : 1;
            __IM  uint32_t RXFIFOE      : 1;
            __IM  uint32_t TXDAVL       : 1;
            __IM  uint32_t RXDAVL       : 1;
            __IM  uint32_t SDIOINT      : 1;
            __IM  uint32_t CEATAEND     : 1;
            __IM  uint32_t RESERVED     : 8;
        } STS_B;
    } ;	

    /** SDIO interrupt clear register */
    union 
    {
        __IOM uint32_t INTC;                         

        struct 
        {
            __IOM  uint32_t CCRCRAILC       : 1;
            __IOM  uint32_t DCRCFAILC       : 1;
            __IOM  uint32_t CTIMEOUTC       : 1;
            __IOM  uint32_t DTIMEOUTC       : 1;
            __IOM  uint32_t TXUNDERRC       : 1;
            __IOM  uint32_t RXOVERRC        : 1;
            __IOM  uint32_t CMDRESC         : 1;
            __IOM  uint32_t CMDSENTC        : 1;
            __IOM  uint32_t DENDC           : 1;
            __IOM  uint32_t STRBITERRC      : 1;
            __IOM  uint32_t DBCKENDC        : 1;
            __IM  uint32_t RESERVED1        : 11;
            __IOM  uint32_t SDIOINTC        : 1;
            __IOM  uint32_t CEATAENDC       : 1;
            __IM  uint32_t RESERVED2        : 8;
        } INTC_B;
    } ;	

    /** SDIO interrupt mask register */
    union 
    {
        __IOM uint32_t MASK;                         

        struct 
        {
            __IOM  uint32_t CCRCRAILIE      : 1;
            __IOM  uint32_t DCRCFAILIE      : 1;
            __IOM  uint32_t CTIMEOUTIE      : 1;
            __IOM  uint32_t DTIMEOUTIE      : 1;
            __IOM  uint32_t TXUNDERRIE      : 1;
            __IOM  uint32_t RXOVERRIE       : 1;
            __IOM  uint32_t CMDRESIE        : 1;
            __IOM  uint32_t CMDSENTIE       : 1;
            __IOM  uint32_t DENDIE          : 1;
            __IOM  uint32_t STRBITERRIE     : 1;
            __IOM  uint32_t DBCKENDIE       : 1;
            __IOM  uint32_t CMDACTIE        : 1;
            __IOM  uint32_t TXACTIE         : 1;
            __IOM  uint32_t RXACTIE         : 1;
            __IOM  uint32_t TXFIFOHEIE      : 1;
            __IOM  uint32_t RXFIFOHEIE      : 1;
            __IOM  uint32_t TXFIFOFIE       : 1;
            __IOM  uint32_t RXFIFOFIE       : 1;
            __IOM  uint32_t TXFIFOEIE       : 1;
            __IOM  uint32_t RXFIFOEIE       : 1;
            __IOM  uint32_t TXDAVLIE        : 1;
            __IOM  uint32_t RXDAVLIE        : 1;
            __IOM  uint32_t SDIOINTIE       : 1;
            __IOM  uint32_t CEATAENDIE      : 1;
            __IM  uint32_t RESERVEDIE       : 8;
        } MASK_B;
    } ;

    /** SDIO FIFO count register */
    union 
    {
        __IM uint32_t FIFOCNT;                         

        struct 
        {
            __IM  uint32_t FIFOCNT      : 24;
            __IM  uint32_t RESERVED     : 8;
        } FIFOCNT_B;
    } ;

    /** SDIO data FIFO register */
    union 
    {
        __IOM uint32_t FIFO;                         

        struct 
        {
            __IOM  uint32_t FIFODATA     : 32;
        } FIFO_B;
    } ;

}SDIO_T;

/** 
  * @brief Digital to  Analog Converter
  */
typedef struct
{

    /** Control register */
    union 
    {
        __IOM uint32_t CTRL;                         

        struct 
        {
            __IOM  uint32_t CH1EN           : 1;
            __IOM  uint32_t CH1BOFF         : 1;
            __IOM  uint32_t CH1TEN          : 1;
            __IOM  uint32_t TSEL1           : 3;
            __IOM  uint32_t CH1WEN          : 2;
            __IOM  uint32_t CH1MAMP         : 4;
            __IOM  uint32_t CH1DMAEN        : 1;
            __IM  uint32_t RESERVED1        : 3;
            __IOM  uint32_t CH2EN           : 1;
            __IOM  uint32_t CH2BOFF         : 1;
            __IOM  uint32_t CH2TEN          : 1;
            __IOM  uint32_t TSEL2           : 3;
            __IOM  uint32_t CH2WEN          : 2;
            __IOM  uint32_t CH2MAMP         : 4;
            __IOM  uint32_t CH2DMAEN        : 1;
            __IM  uint32_t RESERVED2        : 3;
        } CTRL_B;
    } ;

    /** Software trigger register */
    union 
    {
        __IOM uint32_t SWTRG;                         

        struct 
        {
            __IOM  uint32_t CH1SWTRG        : 1;
            __IOM  uint32_t CH2SWTRG        : 1;
            __IM  uint32_t RESERVED         : 30;

        } SWTRG_B;
    } ;

    /** Channel1 12-bit right-aligned register */
    union 
    {
        __IOM uint32_t CH1DH12R;                         

        struct 
        {
            __IOM  uint32_t CH1DH       : 12;
            __IM  uint32_t RESERVED     : 20;
        } CH1DH12R_B;
    } ;

    /** Channel1 12-bit left-aligned register */
    union 
    {
        __IOM uint32_t CH1DH12L;                         

        struct 
        {
            __IM  uint32_t RESERVED1    : 4;
            __IOM  uint32_t CH1DH       : 12;
            __IM  uint32_t RESERVED2    : 16;

        } CH1DH12L_B;
    } ;

    /** Channel1 8-bit right-aligned register */
    union 
    {
        __IOM uint32_t CH1DH8R;                         

        struct 
        {
            __IOM  uint32_t CH1DH       : 8;
            __IM  uint32_t RESERVED     : 24;

        } CH1DH8R_B;
        } ;

        /** Channel2 12-bit right-aligned register */
        union 
        {
            __IOM uint32_t CH2DH12R;                         

            struct 
            {
            __IOM  uint32_t CH2DH       : 12;
            __IM  uint32_t RESERVED     : 20;
        } CH2DH12R_B;
    } ;

    /** Channel2 12-bit left-aligned register */
    union 
    {
        __IOM uint32_t CH2DH12L;                         

        struct 
        {
            __IM  uint32_t RESERVED1    : 4;
            __IOM  uint32_t CH2DH       : 12;
            __IM  uint32_t RESERVED2    : 16;

        } CH2DH12L_B;
    } ;

    /** Channel2 8-bit right-aligned register */
    union 
    {
        __IOM uint32_t CH2DH8R;                         

        struct 
        {
            __IOM  uint32_t CH2DH       : 8;
            __IM  uint32_t RESERVED     : 24;

        } CH2DH8R_B;
    } ;

    /** Channel1,Channel2 12-bit right-aligned register */
    union 
    {
        __IOM uint32_t DH12RD;                         

        struct 
        {
            __IOM  uint32_t CH1DH       : 12;
            __IM  uint32_t RESERVED1    : 4;
            __IOM  uint32_t CH2DH       : 12;
            __IM  uint32_t RESERVED2    : 4;

        } DH12RD_B;
    } ;

    /** Channel1,Channel2 12-bit left-aligned register */
    union 
    {
        __IOM uint32_t DH12LD;                         

        struct 
        {
            __IM  uint32_t RESERVED1    : 4;
            __IOM  uint32_t CH1DH       : 12;
            __IM  uint32_t RESERVED2    : 4;
            __IOM  uint32_t CH2DH       : 12;
        } DH12LD_B;
    } ;

    /** Channel1,Channel2 8-bit right-aligned register */
    union 
    {
        __IOM uint32_t DH8RD;                         

        struct 
        {
            __IOM  uint32_t CH1DH       : 8;
            __IOM  uint32_t CH2DH       : 8;
            __IM  uint32_t RESERVED1    : 16;

        } DH8RD_B;
    } ;

    /** Channel1 data output register */
    union 
    {
        __IOM uint32_t CH1DO;                         

        struct 
        {
            __IOM  uint32_t CH1DO       : 12;
            __IM  uint32_t RESERVED1    : 20;

        } CH1DO_B;
    } ;

    /** Channel2 data output register */
    union 
    {
        __IOM uint32_t CH2DO;                         

        struct 
        {
            __IOM  uint32_t CH1DO       : 12;
            __IM  uint32_t RESERVED1    : 20;

        } CH2DO_B;
    } ;

}DAC_T;

/** 
  * @brief EMMC Register
  */

typedef struct 
{
 union
 {
  __IOM uint32_t SNCTRL;
  
    struct 
    {
      __IOM uint32_t MBKEN       : 1;
      __IOM uint32_t ADMUXEN     : 1;
      __IOM uint32_t MEMTP       : 2;
      __IOM uint32_t MEMDW       : 2;
      __IOM uint32_t FACCEN      : 1;
      __IOM uint32_t RESERVED1   : 1;
      __IOM uint32_t BURSTEN     : 1;
      __IOM uint32_t WTSP        : 1;
      __IOM uint32_t WRAPEN      : 1;
      __IOM uint32_t WTCFG       : 1;
      __IOM uint32_t WREN        : 1;
      __IOM uint32_t WTEN        : 1;
      __IOM uint32_t EXTMOD      : 1;
      __IOM uint32_t RESERVED2   : 4;
      __IOM uint32_t CBURSTRW    : 1;
      __IOM uint32_t RESERVED3   : 12;
      
    }SNCTRL_B;
  };
}SNCTRL_T;

typedef struct 
{
  union 
  {
    __IOM uint32_t SNCLK;
    
    struct 
    {
      __IOM uint32_t ADDSET      : 4;
      __IOM uint32_t ADDHLD      : 4;
      __IOM uint32_t DATAHLD     : 8;
      __IOM uint32_t BUSTPD      : 4;
      __IOM uint32_t CLKDIV      : 4;
      __IOM uint32_t DATALAT     : 4;
      __IOM uint32_t ACCMOD      : 2;
      __IM uint32_t RESERVED     : 2;
    }SNCLK_T;
  };
}SNCLK_T;

typedef struct 
{
  union 
  {
    __IOM uint32_t SNWCLK;
    
    struct 
    {
      __IOM uint32_t WADDSET        : 4;
      __IOM uint32_t WADDHLD        : 4;
      __IOM uint32_t WDATAHLD       : 8;
      __IOM uint32_t RESERVED1      : 4;
      __IOM uint32_t CLKDIV         : 4;
      __IOM uint32_t DATALAT        : 4;
      __IOM uint32_t ACCMOD         : 2;
      __IM uint32_t RESERVED2       : 2;
    }SNWCLK_T;
  };
  
}SNWCLK_T;

/** 
  * @brief Flexible Static Memory Controller
  */

typedef struct
{
  __IO uint32_t SNCTRL_T[8];   
} EMMC_Bank1_T; 

/** 
  * @brief Flexible Static Memory Controller Bank1E
  */
  
typedef struct
{
  __IO uint32_t SNWCLK[7];
} EMMC_Bank1E_T;

/** 
  * @brief Flexible Static Memory Controller Bank2
  */

typedef struct 
{
  union 
  {
    __IOM uint32_t PNCTRL2;
    
    struct 
    {
      __IM uint32_t RESERVED1     : 1;
      __IOM uint32_t PWAITEN      : 1;
      __IOM uint32_t PNBKEN       : 1;
      __IOM uint32_t PMEMTP       : 1;
      __IOM uint32_t PDATAW       : 2;
      __IOM uint32_t ECCEN        : 1;
      __IM uint32_t RESERVED2     : 2;
      __IOM uint32_t CTRD         : 4;
      __IOM uint32_t ATRD         : 4;
      __IOM uint32_t ECCPS        : 3;
      __IM uint32_t RESERVED3     : 12;
    }PNCTRL2_B;
  };
  
  union 
  {
    __IOM uint32_t IFSTS2;
    
    struct 
    {
      __IOM uint32_t INTRS      : 1;
      __IOM uint32_t INTHS      : 1;
      __IOM uint32_t INTFS      : 1;
      __IOM uint32_t INTREN     : 1;
      __IOM uint32_t INTLEN     : 1;
      __IOM uint32_t INTFEN     : 1;
      __IOM uint32_t FFEMP      : 1;
      __IM uint32_t RESERVED    :16;
    }IFSTS2_B;
    
    union 
    {
      __IOM uint32_t NPCT2;
      
      struct 
      {
        __IOM uint32_t CMST     : 8;
        __IOM uint32_t CMWT     : 8;
        __IOM uint32_t CMHT    : 8;
        __IOM uint32_t CMDHT   : 8;
      }NPCT2_B;
    };
    
    union 
    {
      __IOM uint32_t NPAT2;
      
      struct 
      {
        __IOM uint32_t AMST    : 8;
        __IOM uint32_t AMWT    : 8;
        __IOM uint32_t AMHT    : 8;
        __IOM uint32_t AMDHT   : 8;
      }NPAT2_B;
    };
    
    __IOM uint32_t RESERVED;
    
    __IOM uint32_t ECC2;
  };
}EMMC_Bank2_T;

/** 
  * @brief Flexible Static Memory Controller Bank3
  */

typedef struct 
{
  union 
  {
    __IOM uint32_t PNCTRL3;
    
    struct 
    {
      __IM uint32_t RESERVED1     : 1;
      __IOM uint32_t PWAITEN      : 1;
      __IOM uint32_t PNBKEN       : 1;
      __IOM uint32_t PMEMTP       : 1;
      __IOM uint32_t PDATAW       : 2;
      __IOM uint32_t ECCEN        : 1;
      __IM uint32_t RESERVED2     : 2;
      __IOM uint32_t CTRD         : 4;
      __IOM uint32_t ATRD         : 4;
      __IOM uint32_t ECCPS        : 3;
      __IM uint32_t RESERVED3     : 12;
    }PNCTRL3_B;
  };
  
  union 
  {
    __IOM uint32_t IFSTS3;
    
    struct 
    {
      __IOM uint32_t INTRS      : 1;
      __IOM uint32_t INTHS      : 1;
      __IOM uint32_t INTFS      : 1;
      __IOM uint32_t INTREN     : 1;
      __IOM uint32_t INTLEN     : 1;
      __IOM uint32_t INTFEN     : 1;
      __IOM uint32_t FFEMP      : 1;
      __IM uint32_t RESERVED    :16;
    }IFSTS3_B;
    
    union 
    {
      __IOM uint32_t NPCT3;
      
      struct 
      {
        __IOM uint32_t CMST     : 8;
        __IOM uint32_t CMWT     : 8;
        __IOM uint32_t CMHT     : 8;
        __IOM uint32_t CMDHT    : 8;
      }NPCT3_B;
    };
    
    union 
    {
      __IOM uint32_t NPAT3;
      
      struct 
      {
        __IOM uint32_t AMST    : 8;
        __IOM uint32_t AMWT    : 8;
        __IOM uint32_t AMHT    : 8;
        __IOM uint32_t AMDHT   : 8;
      }NPAT3_B;
    };
    
    __IOM uint32_t RESERVED;
    
    __IOM uint32_t ECC3;
  };
}EMMC_Bank3_T;

/** 
  * @brief Flexible Static Memory Controller Bank4
  */
typedef struct 
{
  union 
  {
    __IOM uint32_t PNCTRL4;
    
    struct 
    {
      __IM uint32_t RESERVED1     : 1;
      __IOM uint32_t PWAITEN      : 1;
      __IOM uint32_t PNBKEN       : 1;
      __IOM uint32_t PMEMTP       : 1;
      __IOM uint32_t PDATAW       : 2;
      __IOM uint32_t ECCEN        : 1;
      __IM uint32_t RESERVED2     : 2;
      __IOM uint32_t CTRD         : 4;
      __IOM uint32_t ATRD         : 4;
      __IOM uint32_t ECCPS        : 3;
      __IM uint32_t RESERVED3     : 12;
    }PNCTRL4_B;
  };
  
  union 
  {
    __IOM uint32_t IFSTS4;
    
    struct 
    {
      __IOM uint32_t INTRS      : 1;
      __IOM uint32_t INTHS      : 1;
      __IOM uint32_t INTFS      : 1;
      __IOM uint32_t INTREN     : 1;
      __IOM uint32_t INTLEN     : 1;
      __IOM uint32_t INTFEN     : 1;
      __IOM uint32_t FFEMP      : 1;
      __IM uint32_t RESERVED    :16;
    }IFSTS4_B;
    
    union 
    {
      __IOM uint32_t NPCT4;
      
      struct 
      {
        __IOM uint32_t CMST     : 8;
        __IOM uint32_t CMWT     : 8;
        __IOM uint32_t CMHT     : 8;
        __IOM uint32_t CMDHT    : 8;
      }NPCT4_B;
    };
    
    union 
    {
      __IOM uint32_t NPAT4;
      
      struct 
      {
        __IOM uint32_t AMST    : 8;
        __IOM uint32_t AMWT    : 8;
        __IOM uint32_t AMHT    : 8;
        __IOM uint32_t AMDHT   : 8;
      }NPAT4_B;
    };
    
    union 
    {
      __IOM uint32_t PIOT4;
      
      struct 
      {
        __IOM uint32_t IOST    : 8;
        __IOM uint32_t IOWT    : 8;
        __IOM uint32_t IOHD    : 8;
        __IOM uint32_t IODHT   : 8;
      }PIOT4_B;
    };

  };
}EMMC_Bank4_T;

/**
 * @brief   QSPI 
 */
typedef struct
{
    /**
     * @brief   Control register 0  
     */
    union
    {
        __IOM uint32_t CTRL0;
        struct
        {
            __IOM uint32_t DFS          : 5;
            __IM uint32_t RESERVED1     : 3;
            __IOM uint32_t CLKPHA       : 1;
            __IOM uint32_t CLKPOL       : 1;
            __IOM uint32_t TXMODE       : 2;
            __IM uint32_t RESERVED2     : 2;
            __IOM uint32_t SSTEN        : 1;
            __IM uint32_t RESERVED3     : 7;
            __IOM uint32_t FRF          : 2;
            __IM uint32_t RESERVED4     : 8;
        }CTRL0_B;
    };

    /**
     * @brief   Control register 1  
     */
    union
    {
        __IOM uint32_t CTRL1;
        struct
        {
            __IOM uint32_t NDF          : 16;
            __IM uint32_t RESERVED      : 16;
        }CTRL1_B;
    };  

    /**
     * @brief   QSPI Enable register  
     */
    union
    {
        __IOM uint32_t SSIEN;
        struct
        {
            __IOM  uint32_t EN          : 1;
            __IM uint32_t RESERVED      : 31;
        }SSIEN_B;
    };

    uint32_t RESERVED0;
    
    /**
     * @brief   QSPI Slave enable register  
     */
    union
    {
        __IOM uint32_t SLAEN;
        struct
        {
            __IOM  uint32_t SLAEN       : 1;
            __IM uint32_t RESERVED      : 31;
        }SLAEN_B;                       
    };  

    /**
     * @brief   Baudrate register  
     */
    union
    {
        __IOM uint32_t BR;
        struct
        {
            __IOM  uint32_t CLKDIV      : 16;
            __IM uint32_t RESERVED      : 16;
        }BR_B;
    };

    /**
     * @brief   Transmission FIFO threshhold level register 
     */
    union
    {
        __IOM uint32_t TFTL;
        struct
        {
            __IOM  uint32_t TFT         : 3;
            __IM uint32_t RESERVED1     : 13;
            __IOM  uint32_t TFTH        : 3;
            __IM uint32_t RESERVED2     : 13;
        }TFTL_B;
    };    

    /**
     * @brief   Reception FIFO threshhold level register
     */
    union
    {
        __IOM uint32_t RFTL;
        struct
        {
            __IOM  uint32_t RFT         : 3;
            __IM uint32_t RESERVED      : 29;
        }RFTL_B;
    };   

    /**
     * @brief   Transmission FIFO level register
     */
    union
    {
        __IOM uint32_t TFL;
        struct
        {
            __IOM  uint32_t TFL         : 3;
            __IM uint32_t RESERVED      : 29;
        }TFL_B;
    }; 

    /**
     * @brief   Reception FIFO level register
     */
    union
    {
        __IOM uint32_t RFL;
        struct
        {
            __IOM  uint32_t RFL         : 3;
            __IM uint32_t RESERVED      : 29;
        }RFL_B;
    };   

    /**
     * @brief   Status register
     */
    union
    {
        __IOM uint32_t STS;
        struct
        {
            __IOM  uint32_t BUSYF           : 1;
            __IOM  uint32_t TFNF            : 1;
            __IOM  uint32_t TFEF            : 1;
            __IOM  uint32_t RFNEF           : 1;
            __IOM  uint32_t RFFF            : 1;
            __IM uint32_t RESERVED1         : 1;
            __IOM  uint32_t DCEF            : 1;
            __IM uint32_t RESERVED2         : 25;
        }STS_B;
    };    

    /**
     * @brief   Interrupt enable register
     */
    union
    {
        __IOM uint32_t INTEN;
        struct
        {
            __IOM  uint32_t TFEIE           : 1;
            __IOM  uint32_t TFOIE           : 1;
            __IOM  uint32_t RFUIE           : 1;
            __IOM  uint32_t RFOIE           : 1;
            __IOM  uint32_t RFFIE           : 1;
            __IOM  uint32_t MSTIE           : 1;
            __IM uint32_t RESERVED          : 26;
        }INTEN_B;
    }; 

    /**
     * @brief   Interrupt status register
     */
    union
    {
        __IOM uint32_t INTSTS;
        struct
        {
            __IOM  uint32_t TFEIF           : 1;
            __IOM  uint32_t TFOIF           : 1;
            __IOM  uint32_t RFUIF           : 1;
            __IOM  uint32_t RFOIF           : 1;
            __IOM  uint32_t RFFIF           : 1;
            __IOM  uint32_t MSTIF           : 1;
            __IM uint32_t RESERVED          : 26;
        }INTSTS_B;
    };   

    /**
     * @brief   Raw interrupt register
     */
    union
    {
        __IOM uint32_t RIS;
        struct
        {
            __IOM  uint32_t TFEIF           : 1;
            __IOM  uint32_t TFOIF           : 1;
            __IOM  uint32_t RFUIF           : 1;
            __IOM  uint32_t RFOIF           : 1;
            __IOM  uint32_t RFFIF           : 1;
            __IOM  uint32_t MSTIF           : 1;
            __IM uint32_t RESERVED          : 26;
        }RIS_B;
    };  

    /**
     * @brief   Transmission FIFO overflow interrupt clear register
     */
    union
    {
        __IOM uint32_t TFOIC;
        struct
        {
            __IOM  uint32_t TFOIC           : 1;
            __IM uint32_t RESERVED          : 31;
        }TFOIC_B;
    }; 

    /**
     * @brief   Reception FIFO overflow interrupt clear register
     */
    union
    {
        __IOM uint32_t RFOIC;
        struct
        {
            __IOM  uint32_t TFOIC           : 1;
            __IM uint32_t RESERVED          : 31;
        }RFOIC_B;
    };

    /**
     * @brief   Reception FIFO underflow interrupt clear register
     */
    union
    {
        __IOM uint32_t RFUIC;
        struct
        {
            __IOM  uint32_t RFUIC           : 1;
            __IM uint32_t RESERVED          : 31;
        }RFUIC_B;
    }; 

    /**
     * @brief   Master interrupt clear register
     */
    union
    {
        __IOM uint32_t MIC;
        struct
        {
            __IOM  uint32_t MIC             : 1;
            __IM uint32_t RESERVED          : 31;
        }MIC_B;
    };    

    /**
     * @brief   Interrupt clear register
     */
    union
    {
        __IOM uint32_t INTCLR;
        struct
        {
            __IOM  uint32_t INTCLR          : 1;
            __IM uint32_t RESERVED          : 31;
        }INTCLR_B;
    };

    uint32_t RESERVED1[5];
    
    /**
     * @brief   Data register
     */
    union
    {
        __IOM uint32_t DATA;
        struct
        {
            __IOM  uint32_t DATA            : 32;
        }DATA_B;
    };     

    uint32_t RESERVED2[35];
    
    /**
     * @brief   Reception sample register
     */
    union
    {
        __IOM uint32_t RSD;
        struct
        {
            __IM  uint32_t RESERVED1        : 7;
            __IOM uint32_t RSD              : 1;
            __IM  uint32_t RESERVED2        : 8;
            __IOM uint32_t RSE              : 1;
            __IM  uint32_t RESERVED3        : 15;
        }RSD_B;
    };  

    /**
     * @brief   Reception sample register
     */
    union
    {
        __IOM uint32_t CTRL2;
        struct
        {
            __IOM uint32_t IAT              : 2;
            __IOM uint32_t ADDRLEN          : 4;
            __IM  uint32_t RESERVED1        : 2;
            __IOM uint32_t INSLEN           : 2;
            __IM  uint32_t RESERVED2        : 1;
            __IOM uint32_t WAITCYC          : 5;
            __IM  uint32_t RESERVED3        : 14;
            __IOM uint32_t CSEN             : 1;
            __IM  uint32_t RESERVED4        : 1;
        }CTRL2_B;
    }; 

    uint32_t RESERVED3[66];
    
    /**
     * @brief   IO switch register
     */
    union
    {
        __IOM uint32_t IOSW;
        struct
        {
            __IOM uint32_t IOSW             : 1;
            __IM  uint32_t RESERVED         : 31;
        }IOSW_B;
    };     
}QSPI_T;

/**
 * @brief   SCI2C 
 */
typedef struct
{
    /**
     * @brief   Control register 0  
     */
    union
    {
        __IOM uint32_t CTRL0;
        struct
        {
            __IOM uint32_t MST              : 1;
            __IOM uint32_t SPD              : 2;
            __IOM uint32_t SAM              : 1;
            __IM uint32_t RESERVED1         : 1;
            __IOM uint32_t RSTAEN           : 1;
            __IOM uint32_t SLADIS           : 1;
            __IOM uint32_t DSA              : 1;
            __IOM uint32_t TFEIC            : 1;
            __IOM uint32_t RFFIE            : 1;
            __IOM uint32_t DSMA             : 1;
            __IM uint32_t RESERVED2         : 21;
        }CTRL0_B;
    };

    /**
     * @brief   Target address register  
     */
    union
    {
        __IOM uint32_t TARADDR;
        struct
        {
            __IOM uint32_t ADDR             : 10;
            __IOM uint32_t STA              : 1;
            __IOM uint32_t GCEN             : 1;
            __IOM uint32_t MAM              : 1;
            __IM uint32_t RESERVED          : 19;
        }TARADDR_B;
    };  

    /**
     * @brief   Slave address register  
     */
    union
    {
        __IOM uint32_t SLAADDR;
        struct
        {
            __IOM uint32_t ADDR             : 10;
            __IM uint32_t RESERVED          : 22;
        }SLAADDR_B;
    }; 

    /**
     * @brief   High speed master code register  
     */
    union
    {
        __IOM uint32_t HSMC;
        struct
        {
            __IOM uint32_t HSMC             : 4;
            __IM uint32_t RESERVED          : 28;
        }HSMC_B;
    };   

    /**
     * @brief   Data register  
     */
    union
    {
        __IOM uint32_t DATA;
        struct
        {
            __IOM uint32_t DATA             : 8;
            __IOM uint32_t CMD              : 1;
            __IOM uint32_t STOP             : 1;
            __IM uint32_t RESERVED          : 22;
        }DATA_B;
    };

    /**
     * @brief   Standard speed clock high counter register  
     */
    union
    {
        __IOM uint32_t SSCHC;
        struct
        {
            __IOM uint32_t CNT              : 16;
            __IM uint32_t RESERVED          : 16;
        }SSCHC_B;
    }; 

    /**
     * @brief   Standard speed clock low counter register
     */
    union
    {
        __IOM uint32_t SSCLC;
        struct
        {
            __IOM uint32_t CNT              : 16;
            __IM uint32_t RESERVED          : 16;
        }SSCLC_B;
    };

    /**
     * @brief   Fast speed clock high counter register 
     */
    union
    {
        __IOM uint32_t FSCHC;
        struct
        {
            __IOM uint32_t CNT              : 16;
            __IM uint32_t RESERVED          : 16;
        }FSCHC_B;
    };    

    /**
     * @brief   Fast speed clock low counter register  
     */
    union
    {
        __IOM uint32_t FSCLC;
        struct
        {
            __IOM uint32_t CNT              : 16;
            __IM uint32_t RESERVED          : 16;
        }FSCLC_B;
    };  

    /**
     * @brief   High speed clock high counter  
     */
    union
    {
        __IOM uint32_t HSCHC;
        struct
        {
            __IOM uint32_t CNT              : 16;
            __IM uint32_t RESERVED          : 16;
        }HSCHC_B;
    };  

    /**
     * @brief   High speed clock low counter register   
     */
    union
    {
        __IOM uint32_t HSCLC;
        struct
        {
            __IOM uint32_t CNT              : 16;
            __IM uint32_t RESERVED          : 16;
        }HSCLC_B;
    };  

    /**
     * @brief   Interrupt status register  
     */
    union
    {
        __IM uint32_t INTSTS;
        struct
        {
            __IM uint32_t RFUIF             : 1;
            __IM uint32_t RFOIF             : 1;
            __IM uint32_t RFFIF             : 1;
            __IM uint32_t TFOIF             : 1;
            __IM uint32_t TFEIF             : 1;
            __IM uint32_t RRIF              : 1;
            __IM uint32_t TAIF              : 1;
            __IM uint32_t RDIF              : 1;
            __IM uint32_t ACTIF             : 1;
            __IM uint32_t STPDIF            : 1;
            __IM uint32_t STADIF            : 1;
            __IM uint32_t GCIF              : 1;
            __IM uint32_t RSTADIF           : 1;
            __IM uint32_t MOHIF             : 1;
            __IM uint32_t RESERVED          : 18;
        }INTSTS_B;
    };
    /**
     * @brief   Interrupt enable register  
     */
    union
    {
        __IOM uint32_t INTEN;
        struct
        {
            __IOM uint32_t RFUIE            : 1;
            __IOM uint32_t RFOIE            : 1;
            __IOM uint32_t RFFIE            : 1;
            __IOM uint32_t TFOIE            : 1;
            __IOM uint32_t TFEIE            : 1;
            __IOM uint32_t RRIE             : 1;
            __IOM uint32_t TAIE             : 1;
            __IOM uint32_t RDIE             : 1;
            __IOM uint32_t ACTIE            : 1;
            __IOM uint32_t STPDIE           : 1;
            __IOM uint32_t STADIE           : 1;
            __IOM uint32_t GCIE             : 1;
            __IOM uint32_t RSTADIE          : 1;
            __IOM uint32_t MOHIE            : 1;
            __IM uint32_t RESERVED          : 18;
        }INTEN_B;        
    };     

    /**
     * @brief   Raw interrupt status register  
     */
    union
    {
        __IM uint32_t RIS;
        struct
        {
            __IM uint32_t RFUIF             : 1;
            __IM uint32_t RFOIF             : 1;
            __IM uint32_t RFFIF             : 1;
            __IM uint32_t TFOIF             : 1;
            __IM uint32_t TFEIF             : 1;
            __IM uint32_t RRIF              : 1;
            __IM uint32_t TAIF              : 1;
            __IM uint32_t RDIF              : 1;
            __IM uint32_t ACTIF             : 1;
            __IM uint32_t STPDIF            : 1;
            __IM uint32_t STADIF            : 1;
            __IM uint32_t GCIF              : 1;
            __IM uint32_t RSTADIF           : 1;
            __IM uint32_t RESERVED          : 18;
        }RIS_B;        
    };         

    /**
     * @brief   Reception FIFO threshold register  
     */
    union
    {
        __IOM uint32_t RFT;
        struct
        {
            __IOM uint32_t RFT              : 8;
            __IM uint32_t RESERVED          : 24;
        }RFT_B;        
    };   

    /**
     * @brief   Transmission FIFO threshold register  
     */
    union
    {
        __IOM uint32_t TFT;
        struct
        {
            __IOM uint32_t TFT              : 8;
            __IM uint32_t RESERVED          : 24;
        }TFT_B;        
    };  

    /**
     * @brief   Interruption clear register  
     */
    union
    {
        __IM uint32_t INTCLR;
        struct
        {
            __IM uint32_t INTCLR            : 1;
            __IM uint32_t RESERVED          : 31;
        }INTCLR_B;        
    };    

    /**
     * @brief   Reception FIFO underflow interruption clear register  
     */
    union
    {
        __IM uint32_t RFUIC;
        struct
        {
            __IM uint32_t RFUIC             : 1;
            __IM uint32_t RESERVED          : 31;
        }RFUIC_B;        
    };   

    /**
     * @brief   Reception FIFO overflow interruption clear register  
     */
    union
    {
        __IM uint32_t RFOIC;
        struct
        {
            __IM uint32_t RFOIC             : 1;
            __IM uint32_t RESERVED          : 31;
        }RFOIC_B;        
    };  

    /**
     * @brief   Transmission FIFO overflow interruption clear register  
     */
    union
    {
        __IM uint32_t TFOIC;
        struct
        {
            __IM uint32_t TFOIC             : 1;
            __IM uint32_t RESERVED          : 31;
        }TFOIC_B;        
    };     

    /**
     * @brief   Reception request interruption clear register  
     */
    union
    {
        __IM uint32_t RRIC;
        struct
        {
            __IM uint32_t RRIC              : 1;
            __IM uint32_t RESERVED          : 31;
        }RRIC_B;        
    }; 

    /**
     * @brief   Transmission abort interruption clear register  
     */
    union
    {
        __IM uint32_t TAIC;
        struct
        {
            __IM uint32_t TAIC              : 1;
            __IM uint32_t RESERVED          : 31;
        }TAIC_B;        
    };  

    /**
     * @brief   Receive done interruption clear register  
     */
    union
    {
        __IM uint32_t RDIC;
        struct
        {
            __IM uint32_t RDIC              : 1;
            __IM uint32_t RESERVED          : 31;
        }RDIC_B;        
    };  

    /**
     * @brief   Activity interruption clear register  
     */
    union
    {
        __IM uint32_t AIC;
        struct
        {
            __IM uint32_t AIC               : 1;
            __IM uint32_t RESERVED          : 31;
        }AIC_B;        
    };  

    /**
     * @brief   Stop detection interruption clear register  
     */
    union
    {
        __IM uint32_t STPDIC;
        struct
        {
            __IM uint32_t STPDIC            : 1;
            __IM uint32_t RESERVED          : 31;
        }STPDIC_B;        
    };  

    /**
     * @brief   Start detection interruption clear register  
     */
    union
    {
        __IM uint32_t STADIC;
        struct
        {
            __IM uint32_t STADIC            : 1;
            __IM uint32_t RESERVED          : 31;
        }STADIC_B;        
    };

    /**
     * @brief   General call interruption clear register  
     */
    union
    {
        __IM uint32_t GCIC;
        struct
        {
            __IM uint32_t GCIC              : 1;
            __IM uint32_t RESERVED          : 31;
        }GCIC_B;        
    }; 

    /**
     * @brief   Control register  1
     */
    union
    {
        __IOM uint32_t CTRL1;
        struct
        {
            __IOM uint32_t I2CEN            : 1;
            __IOM uint32_t ABR              : 1;
            __IOM uint32_t TCB              : 1;
            __IM uint32_t RESERVED          : 29;
        }CTRL1_B;        
    }; 

    /**
     * @brief   Status register  1
     */
    union
    {
        __IM uint32_t STS1;
        struct
        {
            __IM uint32_t ACTF              : 1;
            __IM uint32_t TFNFF             : 1;
            __IM uint32_t TFEF              : 1;
            __IM uint32_t RFNEF             : 1;
            __IM uint32_t RFFF              : 1;
            __IM uint32_t MAF               : 1;
            __IM uint32_t SAF               : 1;
            __IM uint32_t RESERVED          : 24;
        }STS1_B;        
    };    

    /**
     * @brief   Transmission FIFO level
     */
    union
    {
        __IOM uint32_t TFL;
        struct
        {
            __IOM uint32_t TFL              : 4;
            __IM uint32_t RESERVED          : 28;
        }TFL_B;        
    }; 

    /**
     * @brief   Reception FIFO level
     */
    union
    {
        __IOM uint32_t RFL;
        struct
        {
            __IOM uint32_t RFL              : 4;
            __IM uint32_t RESERVED          : 28;
        }RFL_B;        
    }; 

    /**
     * @brief   SDA hold time length register
     */
    union
    {
        __IOM uint32_t SDAHOLD;
        struct
        {
            __IOM uint32_t TXHOLD           : 16;
            __IOM uint32_t RXHOLD           : 8;
            __IM uint32_t RESERVED          : 8;
        }SDAHOLD_B;        
    };    

    /**
     * @brief   Transmission abort source register
     */
    union
    {
        __IM uint32_t TAS;
        struct
        {
            __IM uint32_t AD7NA             : 1;
            __IM uint32_t AD10NA1           : 1;
            __IM uint32_t AD10NA2           : 1;
            __IM uint32_t TDNA              : 1;
            __IM uint32_t GCNA              : 1;
            __IM uint32_t GCR               : 1;
            __IM uint32_t HSAD              : 1;
            __IM uint32_t SNR               : 1;
            __IM uint32_t RNR10B            : 1;
            __IM uint32_t MSTDIS            : 1;
            __IM uint32_t ARBLOST           : 1;
            __IM uint32_t LFTF              : 1;
            __IM uint32_t SAL               : 1;
            __IM uint32_t SRI               : 1;
            __IM uint32_t USRARB            : 1;
            __IM uint32_t FLUCNT            : 1;
            __IM uint32_t RESERVED          : 16;
        }TAS_B;    
    };
    
    /**
     * @brief   Slave data NACK only register
     */
    union
    {
        __IOM uint32_t SDNO;
        struct
        {
            __IOM uint32_t NACK             : 1;
            __IM uint32_t RESERVED          : 31;
        }SDNO_B;         
    };  

    /**
     * @brief   DMA control register
     */
    union
    {
        __IOM uint32_t DMACTRL;
        struct
        {
            __IOM uint32_t RXEN             : 1;
            __IOM uint32_t TXEN             : 1;
            __IM uint32_t RESERVED          : 30;
        }DMACTRL_B;         
    };    

    /**
     * @brief   DMA transmission data level register
     */
    union
    {
        __IOM uint32_t DTDL;
        struct
        {
            __IOM uint32_t DTDL             : 4;
            __IM uint32_t RESERVED          : 28;
        }DTDL_B;         
    }; 

    /**
     * @brief   DMA teception data level register
     */
    union
    {
        __IOM uint32_t DRDL;
        struct
        {
            __IOM uint32_t DRDL             : 4;
            __IM uint32_t RESERVED          : 28;
        }DRDL_B;         
    };  

    /**
     * @brief   SDA delay register
     */
    union
    {
        __IOM uint32_t SDADLY;
        struct
        {
            __IOM uint32_t SDADLY           : 8;
            __IM uint32_t RESERVED          : 24;
        }SDADLY_B;         
    };  

    /**
     * @brief   Genernal call ACK register
     */
    union
    {
        __IOM uint32_t GCA;
        struct
        {
            __IOM uint32_t GCA              : 1;
            __IM uint32_t RESERVED          : 31;
        }GCA_B;         
    };    

    /**
     * @brief   Status register2
     */
    union
    {
        __IM uint32_t STS2;
        struct
        {
            __IM uint32_t I2CEN             : 1;
            __IM uint32_t SDWB              : 1;
            __IM uint32_t SRDL              : 1;
            __IM uint32_t RESERVED          : 29;
        }STS2_B;         
    }; 

    /**
     * @brief   Low speed spike suppression limit
     */
    union
    {
        __IOM uint32_t LSSSL;
        struct
        {
            __IOM uint32_t LSSSL            : 8;
            __IM uint32_t RESERVED          : 24;
        }LSSSL_B;         
    }; 

    /**
     * @brief   High speed spike suppression limit
     */
    union
    {
        __IOM uint32_t HSSSL;
        struct
        {
            __IOM uint32_t HSSSL            : 8;
            __IM uint32_t RESERVED          : 24;
        }HSSSL_B;         
    };     

    uint32_t RESERVED[22];
    /**
     * @brief   Switch register
     */
    union
    {
        __IOM uint32_t SW;
        struct
        {
            __IOM uint32_t SW               : 1;
            __IM uint32_t RESERVED          : 31;
        }SW_B;         
    };     
}SCI2C_T;

/** @addtogroup SDRAM controler
  @{
*/
typedef struct
{
    /**
     * @brief   Configuraion register   
     */
    union
    {
        __IOM uint32_t CFG;
        struct
        {
            __IM  uint32_t RESERVED1        : 3;
            __IOM uint32_t BAW              : 2;
            __IOM uint32_t RAW              : 4;
            __IOM uint32_t CAW              : 4;
            __IOM uint32_t DATAWD           : 2;
            __IM uint32_t RESERVED2         : 1;
            __IM uint32_t RESERVED3         : 16;
        }CFG_B;
    };

    /**
     * @brief   Timing register 0
     */
    union
    {
        __IOM uint32_t TIMI0;
        struct
        {
            __IOM uint32_t CASLAT0          : 2;
            __IOM uint32_t RAS              : 4;
            __IOM uint32_t RCD              : 3;
            __IOM uint32_t RP               : 3;
            __IOM uint32_t WR               : 2;
            __IOM uint32_t ARP              : 4;
            __IOM uint32_t XSR0             : 4;
            __IOM uint32_t RC               : 4;
            __IOM uint32_t CASLAT1          : 1;
            __IOM uint32_t XSR1             : 5;
        }TIMI0_B;
    };

    /**
     * @brief   Timing register 1
     */
    union
    {
        __IOM uint32_t TIMI1;
        struct
        {
            __IOM uint32_t STBTIME          : 16;
            __IOM uint32_t ARNUM            : 4;
            __IM  uint32_t RESERVED         : 12;
        }TIMI1_B;
    };   

    /**
     * @brief   Control register 0
     */
    union
    {
        __IOM uint32_t CTRL0;
        struct
        {
            __IOM uint32_t INIT             : 1;
            __IOM uint32_t SELREF           : 1;
            __IOM uint32_t PDM              : 1;
            __IOM uint32_t PCTYPE           : 1;
            __IOM uint32_t FRBSR            : 1;
            __IOM uint32_t FRASR            : 1;
            __IOM uint32_t READPIPE         : 3;
            __IOM uint32_t MODESET          : 1;
            __IM  uint32_t RESERVED1        : 1;
            __IOM uint32_t SRMF             : 1;
            __IOM uint32_t BANKOPEN         : 5;
            __IM  uint32_t RESERVED2        : 15;
        }CTRL0_B;
    };  

    /**
     * @brief   refresh register
     */
    union
    {
        __IOM uint32_t REF;
        struct
        {
            __IOM uint32_t REFPR            : 16;
            __IM  uint32_t RESERVED         : 16;
        }REF_B;
    }; 

    /**
     * @brief   Chip select register
     */
    union
    {
        __IOM uint32_t CHIPSEL;
        struct
        {
            __IM  uint32_t CHIPSEL          : 16;
            __IM  uint32_t RESERVED         : 16;
            
        }CHIPSEL_B;
    }; 

    uint32_t RESERVED1[15];
    
    /**
     * @brief   Mask register
     */
    union
    {
        __IOM uint32_t MASK;
        struct
        {
            __IOM uint32_t MEMSIZE          : 5;
            __IOM uint32_t MEMTYPE          : 3;
            __IM  uint32_t RESERVED         : 24;
        }MASK_B;
    };  

    uint32_t RESERVED2[234];
    
    /**
     * @brief   switch register
     */
    union
    {
        __IOM uint32_t SW;
        struct
        {
            __IOM uint32_t EN               : 1;
            __IM  uint32_t RESERVED         : 31;
        }SW_B;
    };  

    /**
     * @brief   Control register 1
     */
    union
    {
        __IOM uint32_t CTRL1;
        struct
        {
            __IOM uint32_t CLKPHA           : 1;
            __IOM uint32_t RDEN             : 1;
            __IOM uint32_t RDDLAY           : 3;
            __IOM uint32_t WPEN             : 1;
            __IM  uint32_t RESERVED         : 26;
        }CTRL1_B;
    };    
}SDRC_T;

/** 
  * @brief Debug MCU
  */

typedef struct
{
    /**
     * @brief   ID register
     */
    union
    {
        __IOM uint32_t ID;
        struct
        {
            __IOM  uint32_t DEVID           : 12;
            __IM  uint32_t RESERVED         : 4;
            __IOM  uint32_t REVID           : 16;
            
        }ID_B;
    }; 

    /**
     * @brief   Control register
     */
    union
    {
        __IOM uint32_t CTRL;
        struct
        {
            __IOM  uint32_t SLP             : 1;
            __IOM  uint32_t STOP            : 1;
            __IOM  uint32_t SBY             : 1;
            __IM  uint32_t RESERVED1        : 2;
            __IOM  uint32_t TIEN            : 1;
            __IOM  uint32_t TRSMODE         : 2;
            __IOM  uint32_t IWDTSTOP        : 1;
            __IOM  uint32_t WWDTSTOP        : 1;
            __IOM  uint32_t TMR1STOP        : 1;
            __IOM  uint32_t TMR2STOP        : 1;
            __IOM  uint32_t TMR3STOP        : 1;
            __IOM  uint32_t TMR4STOP        : 1;
            __IOM  uint32_t CAN1STOP        : 1;
            __IOM  uint32_t I2C1TO          : 1;
            __IOM  uint32_t I2C2TO          : 1;
            __IOM  uint32_t TMR8STOP        : 1;
            __IOM  uint32_t TMR5STOP        : 1;
            __IOM  uint32_t TMR6STOP        : 1;
            __IOM  uint32_t TMR7STOP        : 1;
            __IOM  uint32_t CAN2STOP        : 1;
            __IM   uint32_t RESERVED2       : 3;
            __IOM  uint32_t TMR12STOP       : 1;
            __IOM  uint32_t TMR13STOP       : 1;
            __IOM  uint32_t TMR14STOP       : 1;
            __IOM  uint32_t TMR9STOP        : 1;
            __IOM  uint32_t TMR10STOP       : 1;
            __IOM  uint32_t TMR11STOP       : 1;
            __IM   uint32_t RESERVED3       : 1;
        }CTRL_B;
    };
}DBGMCU_T;

/**
 * @brief   USB Device controler
 */

typedef union
{
    __IOM uint32_t EP;

    struct
    {
        __IOM uint32_t ADDR         : 4;
        __IOM uint32_t TXSTS        : 2;
        __IOM uint32_t TXDTOG       : 1;
        __IOM uint32_t CTFT         : 1;
        __IOM uint32_t KIND         : 1;
        __IOM uint32_t TYPE         : 2;
        __IOM uint32_t SETUP        : 1;
        __IOM uint32_t RXSTS        : 2;
        __IOM uint32_t RXDTOG       : 1;
        __IOM uint32_t CTFR         : 1;
        __IM  uint32_t RESERVED     : 16;
    }EP_B;
}USB_EP_REG_T;

typedef struct
{
    /** Endpoint */
    USB_EP_REG_T EP[8];

    __IOM uint32_t RESERVED1[8];

    /**
     * @brief   Control register
     */
    union
    {
        __IOM uint32_t CTRL;
        
        struct
        {
            __IOM uint32_t FORRST       : 1;
            __IOM uint32_t PWRDOWN      : 1;
            __IOM uint32_t LPM          : 1;
            __IOM uint32_t FORSUS       : 1;
            __IOM uint32_t RESREQ       : 1;
            __IM  uint32_t RESERVED1    : 3;
            __IOM uint32_t ESOFIE       : 1;
            __IOM uint32_t SOFIE        : 1;
            __IOM uint32_t RSTIE        : 1;
            __IOM uint32_t SUSIE        : 1;
            __IOM uint32_t WUPIE        : 1;
            __IOM uint32_t ERRIE        : 1;
            __IOM uint32_t PMAOUIE      : 1;
            __IOM uint32_t CTIE         : 1;
            __IM  uint32_t RESERVED2    : 16;
        }CTRL_B;
    };

    /**
     * @brief   Interrupt status register
     */
    union
    {
        __IOM uint32_t INTSTS;

        struct
        {
            __IOM uint32_t EPID         : 4;
            __IOM uint32_t DIR          : 1;
            __IM  uint32_t RESERVED1    : 3;
            __IOM uint32_t ESOFIF       : 1;
            __IOM uint32_t SOFIF        : 1;
            __IOM uint32_t RSTIF        : 1;
            __IOM uint32_t SUSIF        : 1;
            __IOM uint32_t WUPIF        : 1;
            __IOM uint32_t ERRIF        : 1;
            __IOM uint32_t PMAOUIF      : 1;
            __IOM uint32_t CTIF         : 1;
            __IM  uint32_t RESERVED2    : 16;
        }INTSTS_B;
    };

    /**
     * @brief   Frame number register
     */
    union
    {
        __IOM uint32_t FRANUM;

        struct
        {
            __IOM uint32_t FRANUM       : 11;
            __IOM uint32_t LSOF         : 2;
            __IOM uint32_t LOCK         : 1;
            __IOM uint32_t RDMS         : 1;
            __IOM uint32_t RDPS         : 1;
            __IM  uint32_t RESERVED     : 16;
        }FRANUM_B; 
    };    

    /**
     * @brief   Device address register
     */
    union
    {
        __IOM uint32_t ADDR;

        struct
        {
            __IOM uint32_t ADDR         : 7;
            __IOM uint32_t EN           : 1;
            __IM  uint32_t RESERVED     : 24;
        }ADDR_B; 
    };

    /**
     * @brief   Buffer table address register
     */
    union
    {
        __IOM uint32_t BTA;

        struct
        {
            __IM  uint32_t RESERVED1    : 3;
            __IOM uint32_t TAB          : 13;
            __IM  uint32_t RESERVED2    : 16;
        }BTA_B; 
    };

    __IOM uint32_t RESERVED2[43];
    
    /**
     * @brief   Buffer table address register
     */
    union
    {
        __IOM uint32_t SW;

        struct
        {
            __IOM uint32_t SW           : 1;
            __IM  uint32_t RESERVED     : 31;
        }SW_B; 
    };    
}USB_T;

/** FMC base address in the alias region */
#define FMC_BASE                ((uint32_t)0x08000000)
/** SRAM base address in the alias region */
#define SRAM_BASE               ((uint32_t)0x20000000)
/** Peripheral base address in the alias region */
#define PERIPH_BASE             ((uint32_t)0x40000000)

/** SRAM base address in the bit-band region */
#define SRAM_BB_BASE            ((uint32_t)0x22000000)
/** Peripheral base address in the bit-band region */
#define PERIPH_BB_BASE          ((uint32_t)0x42000000)

/** EMMC registers base address */
#define EMMC_R_BASE             ((uint32_t)0xA0000000)

/** Peripheral memory map */
#define APB1PERIPH_BASE         PERIPH_BASE
#define APB2PERIPH_BASE         (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE          (PERIPH_BASE + 0x20000)

#define TMR2_BASE               (APB1PERIPH_BASE + 0x0000)
#define TMR3_BASE               (APB1PERIPH_BASE + 0x0400)
#define TMR4_BASE               (APB1PERIPH_BASE + 0x0800)
#define TMR5_BASE               (APB1PERIPH_BASE + 0x0C00)
#define TMR6_BASE               (APB1PERIPH_BASE + 0x1000)
#define TMR7_BASE               (APB1PERIPH_BASE + 0x1400)
#define TMR12_BASE              (APB1PERIPH_BASE + 0x1800)
#define TMR13_BASE              (APB1PERIPH_BASE + 0x1C00)
#define TMR14_BASE              (APB1PERIPH_BASE + 0x2000)
#define RTC_BASE                (APB1PERIPH_BASE + 0x2800)
#define WWDT_BASE               (APB1PERIPH_BASE + 0x2C00)
#define IWDT_BASE               (APB1PERIPH_BASE + 0x3000)
#define SPI2_BASE               (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASE               (APB1PERIPH_BASE + 0x3C00)
#define USART2_BASE             (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE             (APB1PERIPH_BASE + 0x4800)
#define UART4_BASE              (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE              (APB1PERIPH_BASE + 0x5000)
#define I2C1_BASE               (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE               (APB1PERIPH_BASE + 0x5800)
#define USB_BASE                (APB1PERIPH_BASE + 0X5C00)
#define CAN1_BASE               (APB1PERIPH_BASE + 0x6400)
#define CAN2_BASE               (APB1PERIPH_BASE + 0x6800)
#define BAKR_BASE               (APB1PERIPH_BASE + 0x6C00)
#define PMU_BASE                (APB1PERIPH_BASE + 0x7000)
#define DAC_BASE                (APB1PERIPH_BASE + 0x7400)
#define CEC_BASE                (APB1PERIPH_BASE + 0x7800)

#define AFIO_BASE               (APB2PERIPH_BASE + 0x0000)
#define EINT_BASE               (APB2PERIPH_BASE + 0x0400)
#define GPIOA_BASE              (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE              (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE              (APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASE              (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE              (APB2PERIPH_BASE + 0x1800)
#define GPIOF_BASE              (APB2PERIPH_BASE + 0x1C00)
#define GPIOG_BASE              (APB2PERIPH_BASE + 0x2000)
#define ADC1_BASE               (APB2PERIPH_BASE + 0x2400)
#define ADC2_BASE               (APB2PERIPH_BASE + 0x2800)
#define TMR1_BASE               (APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASE               (APB2PERIPH_BASE + 0x3000)
#define TMR8_BASE               (APB2PERIPH_BASE + 0x3400)
#define USART1_BASE             (APB2PERIPH_BASE + 0x3800)
#define ADC3_BASE               (APB2PERIPH_BASE + 0x3C00)
#define TMR15_BASE              (APB2PERIPH_BASE + 0x4000)
#define TMR16_BASE              (APB2PERIPH_BASE + 0x4400)
#define TMR17_BASE              (APB2PERIPH_BASE + 0x4800)
#define TMR9_BASE               (APB2PERIPH_BASE + 0x4C00)
#define TMR10_BASE              (APB2PERIPH_BASE + 0x5000)
#define TMR11_BASE              (APB2PERIPH_BASE + 0x5400)


#define SDIO_BASE               (PERIPH_BASE + 0x18000)


#define DMA1_BASE               (AHBPERIPH_BASE + 0x0000)
#define DMA1_Channel1_BASE      (AHBPERIPH_BASE + 0x0008)
#define DMA1_Channel2_BASE      (AHBPERIPH_BASE + 0x001C)
#define DMA1_Channel3_BASE      (AHBPERIPH_BASE + 0x0030)
#define DMA1_Channel4_BASE      (AHBPERIPH_BASE + 0x0044)
#define DMA1_Channel5_BASE      (AHBPERIPH_BASE + 0x0058)
#define DMA1_Channel6_BASE      (AHBPERIPH_BASE + 0x006C)
#define DMA1_Channel7_BASE      (AHBPERIPH_BASE + 0x0080)
#define DMA2_BASE               (AHBPERIPH_BASE + 0x0400)
#define DMA2_Channel1_BASE      (AHBPERIPH_BASE + 0x0408)
#define DMA2_Channel2_BASE      (AHBPERIPH_BASE + 0x041C)
#define DMA2_Channel3_BASE      (AHBPERIPH_BASE + 0x0430)
#define DMA2_Channel4_BASE      (AHBPERIPH_BASE + 0x0444)
#define DMA2_Channel5_BASE      (AHBPERIPH_BASE + 0x0458)
#define RCM_BASE                (AHBPERIPH_BASE + 0x1000)
#define CRC_BASE                (AHBPERIPH_BASE + 0x3000)

/** FMC registers base address */
#define FMC_R_BASE              (AHBPERIPH_BASE + 0x2000)
/** FMC Option Bytes base address */
#define OB_BASE                 ((uint32_t)0x1FFFF800)

/** EMMC Bank1 registers base address */
#define EMMC_Bank1_R_BASE       (EMMC_R_BASE + 0x0000)
/** EMMC Bank1E registers base address */
#define EMMC_Bank1E_R_BASE      (EMMC_R_BASE + 0x0104)
/** EMMC Bank2 registers base address */
#define EMMC_Bank2_R_BASE       (EMMC_R_BASE + 0x0060)
/** EMMC Bank3 registers base address */
#define EMMC_Bank3_R_BASE       (EMMC_R_BASE + 0x0080)
/**EMMC Bank4 registers base address  */
#define EMMC_Bank4_R_BASE       (EMMC_R_BASE + 0x00A0)

/** Debug MCU registers base address */
#define DBGMCU_BASE             ((uint32_t)0xE0042000)

#define CRC                     ((CRC_T *) CRC_BASE)
#define RTC                     ((RTC_T *) RTC_BASE)
#define PMU                     ((PMU_T *) PMU_BASE)
#define BAKR                    ((BAKR_T *) BAKR_BASE)
#define TMR1                    ((TMR_T *) TMR1_BASE)
#define TMR2                    ((TMR_T *) TMR2_BASE)
#define TMR3                    ((TMR_T *) TMR3_BASE)
#define TMR4                    ((TMR_T *) TMR4_BASE)
#define TMR5                    ((TMR_T *) TMR5_BASE)
#define TMR6                    ((TMR_T *) TMR6_BASE)
#define TMR7                    ((TMR_T *) TMR7_BASE)
#define TMR8                    ((TMR_T *) TMR8_BASE)
#define TMR9                    ((TMR_T *) TMR9_BASE)
#define TMR10                   ((TMR_T *) TMR10_BASE)
#define TMR11                   ((TMR_T *) TMR11_BASE)
#define TMR12                   ((TMR_T *) TMR12_BASE)
#define TMR13                   ((TMR_T *) TMR13_BASE)
#define TMR14                   ((TMR_T *) TMR14_BASE)
#define TMR15                   ((TMR_T *) TMR15_BASE)
#define TMR16                   ((TMR_T *) TMR16_BASE)
#define TMR17                   ((TMR_T *) TMR17_BASE)

#define DMA1                    ((DMA_T *) DMA1_BASE)
#define DMA2                    ((DMA_T *) DMA2_BASE)

#define DMA1_Channel1           ((DMA_Channel_T *) DMA1_Channel1_BASE)
#define DMA1_Channel2           ((DMA_Channel_T *) DMA1_Channel2_BASE)
#define DMA1_Channel3           ((DMA_Channel_T *) DMA1_Channel3_BASE)
#define DMA1_Channel4           ((DMA_Channel_T *) DMA1_Channel4_BASE)
#define DMA1_Channel5           ((DMA_Channel_T *) DMA1_Channel5_BASE)
#define DMA1_Channel6           ((DMA_Channel_T *) DMA1_Channel6_BASE)
#define DMA1_Channel7           ((DMA_Channel_T *) DMA1_Channel7_BASE)

#define DMA2_Channel1           ((DMA_Channel_T *) DMA2_Channel1_BASE)
#define DMA2_Channel2           ((DMA_Channel_T *) DMA2_Channel2_BASE)
#define DMA2_Channel3           ((DMA_Channel_T *) DMA2_Channel3_BASE)
#define DMA2_Channel4           ((DMA_Channel_T *) DMA2_Channel4_BASE)
#define DMA2_Channel5           ((DMA_Channel_T *) DMA2_Channel5_BASE)

#define CAN1                    ((CAN_T *) CAN1_BASE)
#define CAN2                    ((CAN_T *) CAN2_BASE)

#define I2C1                    ((I2C_T *) I2C1_BASE)
#define I2C2                    ((I2C_T *) I2C2_BASE)

#define OB                      ((OB_T *) OB_BASE)

#define ADC1                    ((ADC_T *) ADC1_BASE)
#define ADC2                    ((ADC_T *) ADC2_BASE)
#define ADC3                    ((ADC_T *) ADC3_BASE)

#define EINT                    ((EINT_T *) EINT_BASE)

#define IWDT                    ((IWDT_T *) IWDT_BASE)
#define SDIO                    ((SDIO_T *) SDIO_BASE)
#define DAC                     ((DAC_T *) DAC_BASE)

#define SPI1                    ((SPI_T *) SPI1_BASE)
#define SPI2                    ((SPI_T *) SPI2_BASE)
#define SPI3                    ((SPI_T *) SPI3_BASE)

#define WWDT                    ((WWDT_T *) WWDT_BASE)
#define USART2                  ((USART_T *) USART2_BASE)
#define USART3                  ((USART_T *) USART3_BASE)
#define UART4                   ((USART_T *) UART4_BASE)
#define UART5                   ((USART_T *) UART5_BASE)
#define AFIO                    ((AFIO_T *) AFIO_BASE)
#define GPIOA                   ((GPIO_T *) GPIOA_BASE)
#define GPIOB                   ((GPIO_T *) GPIOB_BASE)
#define GPIOC                   ((GPIO_T *) GPIOC_BASE)
#define GPIOD                   ((GPIO_T *) GPIOD_BASE)
#define GPIOE                   ((GPIO_T *) GPIOE_BASE)
#define GPIOF                   ((GPIO_T *) GPIOF_BASE)
#define GPIOG                   ((GPIO_T *) GPIOG_BASE)
#define USART1                  ((USART_T *) USART1_BASE)
#define RCM                     ((RCM_T *) RCM_BASE)
#define FMC                     ((FMC_T *)FMC_R_BASE)
#define USB                     ((USB_T *)(USB_BASE))

#define EMMC_Bank1              ((EMMC_Bank1_T *) EMMC_Bank1_R_BASE)
#define EMMC_Bank1E             ((EMMC_Bank1E_T *)EMMC_Bank1E_R_BASE)
#define EMMC_Bank2              ((EMMC_Bank2_T *) EMMC_Bank2_R_BASE)
#define EMMC_Bank3              ((EMMC_Bank3_T *) EMMC_Bank3_R_BASE)
#define EMMC_Bank4              ((EMMC_Bank4_T *) EMMC_Bank4_R_BASE)

#define DBGMCU                  ((DBGMCU_T *) DBGMCU_BASE)

#define I2C3                    ((SCI2C_T *)(I2C1_BASE))
#define I2C4                    ((SCI2C_T *)(I2C2_BASE))

#if defined (APM32F10X_MD) || defined (APM32F10X_LD)
#define QSPI                    ((QSPI_T *)EMMC_R_BASE)
#elif defined (APM32F10X_HD)
#define SDRC                    ((SDRC_T *)EMMC_R_BASE)
#endif

/* Define one bit mask */
#define BIT0                    ((uint32_t)0x00000001)
#define BIT1                    ((uint32_t)0x00000002)
#define BIT2                    ((uint32_t)0x00000004)
#define BIT3                    ((uint32_t)0x00000008)
#define BIT4                    ((uint32_t)0x00000010)
#define BIT5                    ((uint32_t)0x00000020)
#define BIT6                    ((uint32_t)0x00000040)
#define BIT7                    ((uint32_t)0x00000080)
#define BIT8                    ((uint32_t)0x00000100)
#define BIT9                    ((uint32_t)0x00000200)
#define BIT10                   ((uint32_t)0x00000400)
#define BIT11                   ((uint32_t)0x00000800)
#define BIT12                   ((uint32_t)0x00001000)
#define BIT13                   ((uint32_t)0x00002000)
#define BIT14                   ((uint32_t)0x00004000)
#define BIT15                   ((uint32_t)0x00008000)
#define BIT16                   ((uint32_t)0x00010000)
#define BIT17                   ((uint32_t)0x00020000)
#define BIT18                   ((uint32_t)0x00040000)
#define BIT19                   ((uint32_t)0x00080000)
#define BIT20                   ((uint32_t)0x00100000)
#define BIT21                   ((uint32_t)0x00200000)
#define BIT22                   ((uint32_t)0x00400000)
#define BIT23                   ((uint32_t)0x00800000)
#define BIT24                   ((uint32_t)0x01000000)
#define BIT25                   ((uint32_t)0x02000000)
#define BIT26                   ((uint32_t)0x04000000)
#define BIT27                   ((uint32_t)0x08000000)
#define BIT28                   ((uint32_t)0x10000000)
#define BIT29                   ((uint32_t)0x20000000)
#define BIT30                   ((uint32_t)0x40000000)
#define BIT31                   ((uint32_t)0x80000000)

#define SET_BIT(REG, BIT)       ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)     ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)      ((REG) & (BIT))

#define CLEAR_REG(REG)          ((REG) = (0x0))

#define WRITE_REG(REG, VAL)     ((REG) = (VAL))

#define READ_REG(REG)           ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))


#ifdef __cplusplus
}
#endif

#endif /* __APM32F10x_H */


