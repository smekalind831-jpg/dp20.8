/*!
 * @file       apm32f10x_RCM.h
 *
 * @brief      This file contains all the functions prototypes for the RCM firmware library
 *
 * @details   
 *
 * @version    V1.0.0
 *     
 * @date       2019-8-6
 *
 */

#ifndef __APM32F10x_RCM_H
#define __APM32F10x_RCM_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "apm32f10x.h"// "stm32f103xb.h"

/**
 * @brief   HXT state
 */
typedef enum
{
   RCM_HXT_CLOSE,            ///!< CLOSE HXT
   RCM_HXT_OPEN,             ///!< OPEN HXT
   RCM_HXT_BYPASS,           ///!< HXT BYPASS
}RCM_HXT_STATE_T;

/**
 * @brief   PLL multiplication factor
 */
typedef enum
{
   RCM_PLLMF_2,
   RCM_PLLMF_3,
   RCM_PLLMF_4,
   RCM_PLLMF_5,
   RCM_PLLMF_6,
   RCM_PLLMF_7,
   RCM_PLLMF_8,
   RCM_PLLMF_9,
   RCM_PLLMF_10,
   RCM_PLLMF_11,
   RCM_PLLMF_12,
   RCM_PLLMF_13,
   RCM_PLLMF_14,
   RCM_PLLMF_15,
   RCM_PLLMF_16,   
}RCM_PLLMF_T;

/**
 * @brief   System clock select
 */
typedef enum
{
   RCM_SYSCLK_SEL_HIRC,
   RCM_SYSCLK_SEL_HXT,
   RCM_SYSCLK_SEL_PLL
}RCM_SYSCLK_SEL_T;

/**
 * @brief   AHB divider Number
 */
typedef enum
{
   RCM_SYSCLK_DIV_1 = 7,
   RCM_SYSCLK_DIV_2,
   RCM_SYSCLK_DIV_4,
   RCM_SYSCLK_DIV_8,
   RCM_SYSCLK_DIV_16,
   RCM_SYSCLK_DIV_64,
   RCM_SYSCLK_DIV_128,
   RCM_SYSCLK_DIV_256,
   RCM_SYSCLK_DIV_512,
}RCM_AHBDIV_T;

/**
 * @brief   APB divider Number
 */
typedef enum
{
   RCM_HCLK_DIV_1 = 3,
   RCM_HCLK_DIV_2,
   RCM_HCLK_DIV_4,
   RCM_HCLK_DIV_8,
   RCM_HCLK_DIV_16
}RCM_APBDIV_T;

/**
 * @brief   USB divider Number
 */
typedef enum
{
   RCM_PLLCLK_DIV_1_5,
   RCM_PLLCLK_DIV_1
}RCM_USBDIV_T;

/**
 * @brief   ADC divider Number
 */
typedef enum
{
   RCM_PCLK2_DIV_2,
   RCM_PCLK2_DIV_4,
   RCM_PCLK2_DIV_6,
   RCM_PCLK2_DIV_8,
}RCM_ADCDIV_T;

/**
 * @brief   LXT State
 */
typedef enum
{
   RCM_LXT_CLOSE,
   RCM_LXT_OPEN,
   RCM_LXT_BYPASS
}RCM_LXT_STATE_T;

/**
 * @brief   RTC clock select
 */
typedef enum
{
   RCM_RTCCLK_SEL_LXT = 1,
   RCM_RTCCLK_SEL_LIRC,
   RCM_RTCCLK_SEL_HXT_DIV_8
}RCM_RTCCLK_SEL_T;

/**
 * @brief   Clock output control
 */
typedef enum
{
   RCM_COC_NO_CLOCK = 3,
   RCM_COC_SYSCLK,
   RCM_COC_HIRC,
   RCM_COC_HXT,
   RCM_COC_PLLCLK_DIV_2,
}RCM_COCCLK_T;

/**
 * @brief   PLL entry clock select
 */
typedef enum
{
   RCM_PLLSEL_HIRC_DIV_2 = 0,
   RCM_PLLSEL_HXT = 1,
   RCM_PLLSEL_HXT_DIV2 = 3,
}RCM_PLLSEL_T;

/**
 * @brief   RCM Interrupt Source
 */
typedef enum
{
    RCM_INT_LIRCRDY   = BIT0,          ///!<   LIRC ready interrupt
    RCM_INT_LXTRDY  = BIT1,          ///!<   LXT ready interrupt
    RCM_INT_HIRCRDY = BIT2,          ///!<   HIRC ready interrupt
    RCM_INT_HXTRDY  = BIT3,          ///!<   HXT ready interrupt
    RCM_INT_PLLRDY  = BIT4,          ///!<   PLL ready interrupt
    RCM_INT_CSS      = BIT7              ///!<   Clock security system interrupt
}RCM_INT_T;

/**
 * @brief   AHB peripheral 
 */
typedef enum
{
   RCM_AHB_PERIPH_DMA1      = BIT0,
   RCM_AHB_PERIPH_DMA2      = BIT1,
   RCM_AHB_PERIPH_SRAM      = BIT2,
   RCM_AHB_PERIPH_FPU       = BIT3,
   RCM_AHB_PERIPH_FMC       = BIT4,
   RCM_AHB_PERIPH_QSPI      = BIT5,
   RCM_AHB_PERIPH_CRC       = BIT6,
   RCM_AHB_PERIPH_EMMC      = BIT8,
   RCM_AHB_PERIPH_SDIO      = BIT10,
}RCM_AHB_PERIPH_T;

/**
 * @brief   AHB2 peripheral
 */
typedef enum
{
   RCM_APB2_PERIPH_AFIO    = BIT0,
   RCM_APB2_PERIPH_GPIOA   = BIT2,
   RCM_APB2_PERIPH_GPIOB    = BIT3,
   RCM_APB2_PERIPH_GPIOC    = BIT4,
   RCM_APB2_PERIPH_GPIOD    = BIT5,
   RCM_APB2_PERIPH_GPIOE    = BIT6,
   RCM_APB2_PERIPH_GPIOF   = BIT7,
   RCM_APB2_PERIPH_GPIOG    = BIT8,
   RCM_APB2_PERIPH_ADC1    = BIT9,
   RCM_APB2_PERIPH_ADC2    = BIT10,
   RCM_APB2_PERIPH_TRM1    = BIT11,
   RCM_APB2_PERIPH_SPI1    = BIT12,
   RCM_APB2_PERIPH_TRM8    = BIT13,
   RCM_APB2_PERIPH_USART1    = BIT14,
   RCM_APB2_PERIPH_ADC3    = BIT15,
   RCM_APB2_PERIPH_TRM15    = BIT16,
   RCM_APB2_PERIPH_TRM16    = BIT17,
   RCM_APB2_PERIPH_TRM17    = BIT18,
   RCM_APB2_PERIPH_TRM9    = BIT19,
   RCM_APB2_TPERIPH_TRM10    = BIT20,
   RCM_APB2_PERIPH_TRM11    = BIT21,
}RCM_APB2_PERIPH_T;

/**
 * @brief   AHB1 peripheral
 */
typedef enum
{
   RCM_APB1_PERIPH_TRM2    = BIT0,
   RCM_APB1_PERIPH_TRM3    = BIT1,
   RCM_APB1_PERIPH_TRM4    = BIT2,
   RCM_APB1_PERIPH_TRM5    = BIT3,
   RCM_APB1_PERIPH_TRM6    = BIT4,
   RCM_APB1_PERIPH_TRM7    = BIT5,
   RCM_APB1_PERIPH_TRM12    = BIT6,
   RCM_APB1_PERIPH_TRM13    = BIT7,
   RCM_APB1_PERIPH_TRM14    = BIT8,
   RCM_APB1_PERIPH_WWDT    = BIT11,
   RCM_APB1_PERIPH_SPI2    = BIT14,
   RCM_APB1_PERIPH_SPI3    = BIT15,
   RCM_APB1_PERIPH_USART2    = BIT17,
   RCM_APB1_PERIPH_USART3    = BIT18,
   RCM_APB1_PERIPH_UART4    = BIT19,
   RCM_APB1_PERIPH_UART5    = BIT20,
   RCM_APB1_PERIPH_I2C1    = BIT21,
   RCM_APB1_PERIPH_I2C2    = BIT22,
   RCM_APB1_PERIPH_USB    = BIT23,
   RCM_APB1_PERIPH_CAN1    = BIT25,
   RCM_APB1_PERIPH_CAN2    = BIT26,
   RCM_APB1_PERIPH_BAKR    = BIT27,
   RCM_APB1_PERIPH_PMU    = BIT28,
   RCM_APB1_PERIPH_DAC    = BIT29,
}RCM_APB1_PERIPH_T;

/**
 * @brief   RCM FLAG define
 */
typedef enum
{
    RCM_FLAG_HIRCRDY    = 0x001,       ///!<   HIRC Ready Flag
    RCM_FLAG_HXTRDY     = 0x011,       ///!<   HXT Ready Flag
    RCM_FLAG_PLLRDY     = 0x019,       ///!<   PLL Ready Flag
    RCM_FLAG_LXTRDY     = 0x101,       ///!<   LXT Ready Flag
    RCM_FLAG_LIRCRDY    = 0x201,       ///!<   LIRC Ready Flag
    RCM_FLAG_PINRST     = 0x21A,       ///!<   PIN reset flag
    RCM_FLAG_PWRRST     = 0x21B,       ///!<   POR/PDR reset flag
    RCM_FLAG_SWRST      = 0x21C,       ///!<   Software reset flag
    RCM_FLAG_IWDTRST    = 0x21D,       ///!<   Independent watchdog reset flag
    RCM_FLAG_WWDTRST    = 0x21E,       ///!<   Window watchdog reset flag
    RCM_FLAG_LPRRST     = 0x21F,       ///!<   Low-power reset flag
}RCM_FLAG_T;


void RCM_Reset(void);
void RCM_ConfigHXT(RCM_HXT_STATE_T state);
uint8_t RCM_WaitHXTReady(void);

void RCM_SetHIRCTrim(uint8_t HIRCTrim);
void RCM_EnableHIRC(void);
void RCM_DisableHIRC(void);

void RCM_ConfigPLL(RCM_PLLSEL_T pllSelect, RCM_PLLMF_T pllMf);
void RCM_EnablePLL(void);
void RCM_DisablePLL(void);

void RCM_ConfigSYSCLK(RCM_SYSCLK_SEL_T sysClkSelect);
RCM_SYSCLK_SEL_T RCM_GetSYSCLKSource(void);

void RCM_ConfigAHB(RCM_AHBDIV_T AHBDiv);
void RCM_ConfigAPB1(RCM_APBDIV_T APB1Div);
void RCM_ConfigAPB2(RCM_APBDIV_T APB2Div);

void RCM_EnableInterrupt(RCM_INT_T interrupt);
void RCM_DisableInterrupt(RCM_INT_T interrupt);

void RCM_ConfigUSBCLK(RCM_USBDIV_T USBDiv);

void RCM_ConfigADCCLK(RCM_ADCDIV_T ADCDiv);

void RCM_ConfigLXT(RCM_LXT_STATE_T state);
void RCM_EnableLIRC(void);
void RCM_DisableLIRC(void);

void RCM_ConfigRTCCLK(RCM_RTCCLK_SEL_T rtcClkSelect);
void RCM_EnableRTCCLK(void);
void RCM_DisableRTCCLK(void);

uint32_t RCM_GetSYSCLKFreq(void);
uint32_t RCM_GetHCLKFreq(void);
void RCM_GetPCLKFreq(uint32_t *PCLK1, uint32_t *PCLK2);
uint32_t RCM_GetADCCLKFreq(void);

void RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_T AHBPeriph);
void RCM_DisableAHBPeriphClock(RCM_AHB_PERIPH_T AHBPeriph);
void RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_T APB2Periph);
void RCM_DisableAPB2PeriphClock(RCM_APB2_PERIPH_T APB2Periph);
void RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_T APB1Periph);
void RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_T APB1Periph);

void RCM_EnableAPB2PeriphReset(RCM_APB2_PERIPH_T APB2Periph);
void RCM_DisableAPB2PeriphReset(RCM_APB2_PERIPH_T APB2Periph);
void RCM_EnableAPB1PeriphReset(RCM_APB1_PERIPH_T APB1Periph);
void RCM_DisableAPB1PeriphReset(RCM_APB1_PERIPH_T APB1Periph);

void RCM_EnableBackupReset(void);
void RCM_DisableBackupReset(void);

void RCM_EnableCCS(void);
void RCM_DisableCCS(void);

void RCM_ConfigCOC(RCM_COCCLK_T cocClock);

uint8_t RCM_ReadFlag(RCM_FLAG_T flag);
void RCM_ClearFlag(void);
uint8_t RCM_ReadIntFlag(RCM_INT_T interrupt);
void RCM_ClearIntFlag(RCM_INT_T interrupt);

#ifdef __cplusplus
}
#endif

#endif /* __APM32F10x_RCM_H */

