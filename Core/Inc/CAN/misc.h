/*!
 * @file       misc.h
 *
 * @brief    
 *
 * @details   
 *
 * @version    V1.0.0
 *   
 * @date       2019-8-13
 *
 */

#ifndef __MISC_H
#define __MISC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "apm32f10x.h" //"stm32f103xb.h"

/**
 * @brief   NVIC Vect table
 */
typedef enum
{
   NVIC_VECT_TAB_RAM = 0x20000000,
   NVIC_VECT_TAB_FLASH = 0x08000000,
}NVIC_VECT_T;

/**
 * @brief   system low power mode
 */
typedef enum
{
   NVIC_LP_SEVONPEND = 0x10,
   NVIC_LP_SLEEPDEEP = 0x04,
   NVIC_LP_SLEEPONEXIT = 0x02
}NVIC_LP_T;

/**
 * @brief   nvic priority group
 */
typedef enum
{
    NVIC_PRIORITY_GROUP_0 = 0x700,       ///!<   0 bits for pre-emption priority,4 bits for subpriority
    NVIC_PRIORITY_GROUP_1 = 0x600,       ///!<   1 bits for pre-emption priority,3 bits for subpriority
    NVIC_PRIORITY_GROUP_2 = 0x500,       ///!<   2 bits for pre-emption priority,2 bits for subpriority
    NVIC_PRIORITY_GROUP_3 = 0x400,       ///!<   3 bits for pre-emption priority,1 bits for subpriority
    NVIC_PRIORITY_GROUP_4 = 0x300        ///!<   4 bits for pre-emption priority,0 bits for subpriority
}NVIC_PRIORITY_GROUP_T;

/**
 * @brief   SysTick Clock source
 */
typedef enum
{
   SYSTICK_CLK_SOURCE_HCLK_DIV8 = (uint8_t)0xFFFFFFFB,
   SYSTICK_CLK_SOURCE_HCLK = 0x00000004
}SYSTICK_CLK_SOURCE_T;


void NVIC_ConfigPriorityGroup(NVIC_PRIORITY_GROUP_T priorityGroup);
void NVIC_EnableIRQRequest(IRQn_Type irq, uint8_t preemptionPriority, uint8_t subPriority);
void NVIC_DisableIRQRequest(IRQn_Type irq);
void NVIC_SetVectorTable(NVIC_VECT_T vectTab, uint32_t offset);
void NVIC_SetSystemLowPower(NVIC_LP_T lowPowerMode);
void NVIC_ResetystemLowPower(NVIC_LP_T lowPowerMode);
void SysTick_ConfigCLKSource(SYSTICK_CLK_SOURCE_T clkSource);
#ifdef __cplusplus
}
#endif

#endif /* __MISC_H */

