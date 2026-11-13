/*!
 * @file       apm32f10x_gpio.h
 *
 * @brief      This file contains all the functions prototypes for the GPIO firmware library
 *
 * @details   
 *
 * @version    V1.0.0
 * 
 * @date       2019-8-6
 *
 */

#ifndef __APM32F10x_GPIO_H
#define __APM32F10x_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "apm32f10x.h"
                                    
/**
 * @brief   GPIO Output Maximum frequency selection
 */
typedef enum
{ 
   GPIO_SPEED_10MHz = 1,
   GPIO_SPEED_2MHz, 
   GPIO_SPEED_50MHz
}GPIO_SPEED_T;

/**
 * @brief   Configuration Mode enumeration
 */
typedef enum
{
    GPIO_MODE_ANALOG       = 0x0,             ///!<   Analog mode
    GPIO_MODE_IN_FLOATING  = 0x04,            ///!<   Floating input
    GPIO_MODE_IN_PD        = 0x28,            ///!<   Input with pull-down
    GPIO_MODE_IN_PU        = 0x48,            ///!<   Input with pull-up
    GPIO_MODE_OUT_PP       = 0x80,            ///!<   General purpose output push-pull
    GPIO_MODE_OUT_OD       = 0x84,            ///!<   General purpose output Open-drain
    GPIO_MODE_AF_PP        = 0x88,            ///!<   Alternate function output Push-pull
    GPIO_MODE_AF_OD        = 0x8C,            ///!<   Alternate function output Open-drain
}GPIO_MODE_T;

/**
 * @brief   GPIO Config structure definition
 */
typedef struct
{
   /** pins to be configured */
   uint16_t pin;
   /** speed for the selected pins */
   GPIO_SPEED_T speed;
   /** operating mode for the selected pins */
   GPIO_MODE_T mode;
}GPIO_ConfigStruct_T;

/**
 * @brief   GPIO pin 
 */
enum
{
   GPIO_PIN_0       = BIT0,
   GPIO_PIN_1       = BIT1,
   GPIO_PIN_2       = BIT2,
   GPIO_PIN_3       = BIT3,
   GPIO_PIN_4       = BIT4,
   GPIO_PIN_5       = BIT5,
   GPIO_PIN_6       = BIT6,
   GPIO_PIN_7       = BIT7,
   GPIO_PIN_8       = BIT8,
   GPIO_PIN_9       = BIT9,
   GPIO_PIN_10      = BIT10,
   GPIO_PIN_11      = BIT11,
   GPIO_PIN_12      = BIT12,
   GPIO_PIN_13      = BIT13,
   GPIO_PIN_14      = BIT14,
   GPIO_PIN_15      = BIT15,
   GPIO_PIN_All     = 0XFFFF
};

/**
 * @brief   GPIO remap type define
 */
typedef enum
{
   GPIO_NO_REMAP_SPI1          = 0x00000010,
   GPIO_REMAP_SPI1             = 0x00000011,
   
   GPIO_NO_REMAP_I2C1          = 0x00000110,
   GPIO_REMAP_I2C1             = 0x00000111,

   GPIO_NO_REMAP_USART1       = 0x00000210,
   GPIO_REMAP_USART1          = 0x00000211,

   GPIO_NO_REMAP_USART2       = 0x00000311,
   GPIO_REMAP_USART2          = 0x00000311,

   GPIO_NO_REMAP_USART3       = 0x00000430,
   GPIO_PARTIAL_REMAP_USART3    = 0x00000431,
   GPIO_FULL_REMAP_USART3       = 0x00000433,

   GPIO_NO_REMAP_TIM1          = 0x00000630,
   GPIO_PARTIAL_REMAP_TIM1    = 0x00000631,
   GPIO_FULL_REMAP_TIM1       = 0x00000633,

   GPIO_NO_REMAP1_TIM2       = 0x00000830,
   GPIO_PARTIAL_REMAP1_TIM2   = 0x00000831,
   GPIO_PARTIAL_REMAP2_TIM2    = 0x00000832,
   GPIO_FULL_REMAP_TIM2       = 0x00000831,

   GPIO_NO_REMAP_TIM3          = 0x00000A30,
   GPIO_PARTIAL_REMAP_TIM3    = 0x00000A32,
   GPIO_FULL_REMAP_TIM3       = 0x00000A33,

   GPIO_NO_REMAP_TIM4          = 0x00000B10,
   GPIO_REMAP_TIM4          = 0x00000B11,

   GPIO_NO_REMAP1_CAN1       = 0x00000B30,
   GPIO_REMAP1_CAN1          = 0x00000B32,
   GPIO_REMAP2_CAN1          = 0x00000C33,

   GPIO_NO_REMAP_PD01          = 0x00000F10,
   GPIO_REMAP_PD01          = 0x00000F11,

   GPIO_NO_REMAP_TIM5CH4_LSI    = 0x00001010,
   GPIO_REMAP_TIM5CH4_LSI       = 0x00001011,

   GPIO_NO_REMAP_ADC1_ETRGINJ    = 0x00001110,
   GPIO_REMAP_ADC1_ETRGINJ    = 0x00001111,

   GPIO_NOREMAP_ADC1_ETRGREG    = 0x00001210,
   GPIO_REMAP_ADC1_ETRGREG    = 0x00001211,

   GPIO_NO_REMAP_ADC2_ETRGINJ    = 0x00001310,
   GPIO_REMAP_ADC2_ETRGINJ    = 0x00001311,

   GPIO_NO_REMAP_ADC2_ETRGREG    = 0x00001410,
   GPIO_REMAP_ADC2_ETRGREG    = 0x00001411,

   GPIO_NO_REMAP_SWJ_NoJTRST    = 0x00001870,
   GPIO_REMAP_SWJ_NoJTRST       = 0x00001871,
   GPIO_REMAP_SWJ_JTAGDisable    = 0x00001872,
   GPIO_REMAP_SWJ_Disable       = 0x00001873,

   GPIO_NO_REMAP_TIM9          = 0x00010510,
   GPIO_REMAP_TIM9          = 0x00010511,

   GPIO_NO_REMAP_TIM10       = 0x00010610,
   GPIO_REMAP_TIM10          = 0x00010611,

   GPIO_NO_REMAP_TIM11       = 0x00010710,
   GPIO_REMAP_TIM11          = 0x00010711,

   GPIO_NO_REMAP_TIM13       = 0x00010810,
   GPIO_REMAP_TIM13          = 0x00010811,

   GPIO_NO_REMAP_TIM14       = 0x00010910,
   GPIO_REMAP_TIM14          = 0x00010911,

   GPIO_NO_REMAP_EMMC_NADV    = 0x00010A10,
   GPIO_REMAP_EMMC_NADV       = 0x00010A11,
}GPIO_REMAP_T;

/**
 * @brief   gpio port source define
 */
typedef enum
{
   GPIO_PORT_SOURCE_A,
   GPIO_PORT_SOURCE_B,
   GPIO_PORT_SOURCE_C,
   GPIO_PORT_SOURCE_D,
   GPIO_PORT_SOURCE_E,
   GPIO_PORT_SOURCE_F,
   GPIO_PORT_SOURCE_G,
}GPIO_PORT_SOURCE_T;

/**
 * @brief   gpio pin source define
 */
typedef enum
{
   GPIO_PIN_SOURCE_0,
   GPIO_PIN_SOURCE_1,
   GPIO_PIN_SOURCE_2,
   GPIO_PIN_SOURCE_3,
   GPIO_PIN_SOURCE_4,
   GPIO_PIN_SOURCE_5,
   GPIO_PIN_SOURCE_6,
   GPIO_PIN_SOURCE_7,
   GPIO_PIN_SOURCE_8,
   GPIO_PIN_SOURCE_9,
   GPIO_PIN_SOURCE_10,
   GPIO_PIN_SOURCE_11,
   GPIO_PIN_SOURCE_12,
   GPIO_PIN_SOURCE_13,
   GPIO_PIN_SOURCE_14,
   GPIO_PIN_SOURCE_15,
}GPIO_PIN_SOURCE_T;


void GPIO_Reset(GPIO_T* port);
void AFIO_Reset(void);
void GPIO_Config(GPIO_T* port, GPIO_ConfigStruct_T* configStruct);
void GPIO_StructInit(GPIO_ConfigStruct_T* configStruct);
uint8_t GPIO_ReadInputBit(GPIO_T* port, uint16_t pin);
uint16_t GPIO_ReadInputPort(GPIO_T* port);
uint8_t GPIO_ReadOutputBit(GPIO_T* port, uint16_t pin);
uint16_t GPIO_ReadOutputPort(GPIO_T* port);
void GPIO_SetBits(GPIO_T* port, uint16_t pin);
void GPIO_ResetBits(GPIO_T* port, uint16_t GPIO_Pin);
void GPIO_WriteOutputPort(GPIO_T* port, uint16_t portValue);
void GPIO_ConfigPinLock(GPIO_T* port, uint16_t pin);
void GPIO_ConfigEventOutput(GPIO_PORT_SOURCE_T portSource, GPIO_PIN_SOURCE_T pinSource);
void GPIO_EnableEventOutput(void);
void GPIO_DisableEventOutput(void);
void GPIO_ConfigPinRemap(GPIO_REMAP_T remap);
void GPIO_ConfigEINTLine(GPIO_PORT_SOURCE_T portSource, GPIO_PIN_SOURCE_T pinSource);

#ifdef __cplusplus
}
#endif

#endif /* __APM32F10x_GPIO_H */

