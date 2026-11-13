/*!
 * @file       misc.c
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

#include "CAN/misc.h"

#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)


/*!
 * @brief     Configures the priority grouping: pre-emption priority and subpriority.
 *
 * @param     priorityGroup   specifies the priority grouping bits length.
 *
 * @note 
 */
void NVIC_ConfigPriorityGroup(NVIC_PRIORITY_GROUP_T priorityGroup)
{
   SCB->AIRCR = AIRCR_VECTKEY_MASK | priorityGroup;
}


/*!
 * @brief     Enable NVIC request
 *
 * @param     irq: the NVIC interrupt request, detailed in IRQn_Type
 *            For the complete APM32 Devices IRQ Channels list,please refer to apm32f10x.h file
 *
 * @param     preemptionPriority: the pre-emption priority needed to set   
 *            
 * @param     subPriority: the subpriority needed to set   
 *
 * @note 
 */
void NVIC_EnableIRQRequest(IRQn_Type irq, uint8_t preemptionPriority, uint8_t subPriority)
{
   uint32_t tempPriority, tempPrePri, tempSubPri;
   uint32_t priorityGrp;
   
   /** Get priority group */
   priorityGrp = (SCB->AIRCR) & (uint32_t)0x700U;

   /** get pre-emption priority and subpriority */
   switch(priorityGrp)
   {
      case NVIC_PRIORITY_GROUP_0:
         tempPrePri = 0;
         tempSubPri = 4;
         break;

      case NVIC_PRIORITY_GROUP_1:
         tempPrePri = 1;
         tempSubPri = 3;
         break;

      case NVIC_PRIORITY_GROUP_2:
         tempPrePri = 2;
         tempSubPri = 2;
         break;

      case NVIC_PRIORITY_GROUP_3:
         tempPrePri = 3;
         tempSubPri = 1;
         break;   

      case NVIC_PRIORITY_GROUP_4:
         tempPrePri = 4;
         tempSubPri = 0;
         break;   

      default:
         NVIC_ConfigPriorityGroup(NVIC_PRIORITY_GROUP_0);
         tempPrePri = 0;
         tempSubPri = 4;
         break;
   }

   tempPrePri = 4 - tempPrePri;
   tempSubPri = 4 - tempSubPri;
   tempPriority = preemptionPriority << tempPrePri;
   tempPriority |= subPriority & (0x0f >> tempSubPri);
   tempPriority <<= 4;
   NVIC->IP[irq] = (uint8_t)tempPriority;
   
    /* enable the selected IRQ */
    NVIC->ISER[irq >> 0x05U] = (uint32_t)0x01U << (irq & (uint8_t)0x1FU);   
}

/*!
 * @brief     Disable NVIC request 
 *
 * @param     irq: the NVIC interrupt request, detailed in IRQn_Type
 *
 * @note 
 */
void NVIC_DisableIRQRequest(IRQn_Type irq)
{
    /* disable the selected IRQ.*/
    NVIC->ICER[irq >> 0x05U] = (uint32_t)0x01U << (irq & (uint8_t)0x1FU);
}

/*!
 * @brief     Sets the vector table location and Offset.
 *
 * @param     vectTab   specifies if the vector table is in RAM or FLASH memory
 *
 * @param     Offset   Vector Table base offset field. This value must be a multiple of 0x200
 *
 * @note 
 */
void NVIC_SetVectorTable(NVIC_VECT_T vectTab, uint32_t offset)
{ 
    SCB->VTOR = vectTab | (offset & (uint32_t)0x1FFFFF80);
}

/*!
 * @brief     set the state of the low power mode
 *
 * @param     lowPowerMode: the low power mode state
 *
 * @note 
 */
void NVIC_SetSystemLowPower(NVIC_LP_T lowPowerMode)
{
   SCB->SCR |= lowPowerMode;
}


/*!
 * @brief     reset the state of the low power mode
 *
 * @param     lowPowerMode: the low power mode state
 *
 * @note 
 */
void NVIC_ResetystemLowPower(NVIC_LP_T lowPowerMode)
{
   SCB->SCR &= (uint32_t)(~(uint32_t)lowPowerMode);
}

/*!
 * @brief     Configures the SysTick clock source
 *
 * @param     clkSource: specifies the SysTick clock source
 *
 * @note 
 */
void SysTick_ConfigCLKSource(SYSTICK_CLK_SOURCE_T clkSource)
{
   if (clkSource == SYSTICK_CLK_SOURCE_HCLK)
   {
      SysTick->CTRL |= SYSTICK_CLK_SOURCE_HCLK;
   }
   else
   {
      SysTick->CTRL &= SYSTICK_CLK_SOURCE_HCLK_DIV8;
   }
}

