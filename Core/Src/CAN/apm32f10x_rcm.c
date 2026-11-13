/*!
 * @file       apm32f10x_RCM.c
 *
 * @brief      This file provides all the RCM firmware functions
 *
 * @details      
 *
 * @version    V1.0.0
 *  
 * @date       2019-8-6
 *
 */
#include "apm32f10x_RCM.h"


/*!
 * @brief     Resets the clock configuration      
 *
 * @param     None
 *
 * @retval    None   
 *
 * @note 
 */
void RCM_Reset(void)
{
   /** Set HIRCEN bit */
   RCM->CTRL_B.HIRCEN = BIT_SET;
   /** Reset SCSEL, AHBDIV, APB1DIV, APB2DIV, ADCDIV and COC bits */
   RCM->CFG &= (uint32_t)0xF8FF0000;
   /** Reset HXTEN, CSSEN and PLLEN bits */
   RCM->CTRL &= (uint32_t)0xFEF6FFFF;
   /** Reset HXTBYP bit */
   RCM->CTRL_B.HXTBYP = BIT_RESET;
   /** Reset PLLSEL, PLLXTDIV, PLLMF and USBDIV bits */
   RCM->CFG &= (uint32_t)0xFF80FFFF;
   /** Disable all interrupts and clear pending bits */
   RCM->INT = 0x009F0000;
}

/*!
 * @brief     Configures HXT
 *
 * @param     state   state of the HXT
 *
 * @retval    None
 *
 * @note      HXT can not be stopped if it is used directly or through the PLL as system clock
 */
void RCM_ConfigHXT(RCM_HXT_STATE_T state)
{
   /** Reset HXTEN bit */
   RCM->CTRL_B.HXTEN = BIT_RESET;

   /** Reset HXTBYP bit */
   RCM->CTRL_B.HXTBYP = BIT_RESET;

   if(state == RCM_HXT_OPEN)
   {
      RCM->CTRL_B.HXTEN = BIT_SET;
   }
   else if(state == RCM_HXT_BYPASS)
   {
      RCM->CTRL_B.HXTBYP = BIT_RESET;
      RCM->CTRL_B.HXTEN = BIT_SET;
   }
}

/*!
 * @brief     Waits for HXT be ready
 *
 * @param     None
 *
 * @retval    SUCCESS      HXT oscillator is stable and ready to use
 *            ERROR      HXT oscillator not yet ready
 * @note 
 */
uint8_t RCM_WaitHXTReady(void)
{
   __IO uint32_t cnt;

   for(cnt = 0; cnt < HXT_STARTUP_TIMEOUT; cnt++)
   {
      if(RCM->CTRL_B.HXTRDYF == BIT_SET)
      {
         return SUCCESS;
      }
   }

   return ERROR;
}

 /*!
 * @brief     Set HIRC trimming value
 *
 * @param     HIRCTrim   HIRC trimming value 
 *
 * @retval    None
 *
 * @note 
 */
void RCM_SetHIRCTrim(uint8_t HIRCTrim)
{
   RCM->CTRL_B.HIRCTRIM = HIRCTrim;
}

 /*!
 * @brief     Enable HIRC
 *
 * @param     None
 *
 * @retval    None
 *
 * @note      HIRC can not be stopped if it is used directly or through the PLL as system clock
 */
void RCM_EnableHIRC(void)
{
   RCM->CTRL_B.HIRCEN = BIT_SET;
}

/*!
* @brief      Disable HIRC
*
* @param      None
*
* @retval     None
*
* @note       HIRC can not be stopped if it is used directly or through the PLL as system clock
*/

void RCM_DisableHIRC(void)
{
   RCM->CTRL_B.HIRCEN = BIT_RESET;
}

/*!
 * @brief     Configures the PLL clock source and multiplication factor
 *
 * @param     pllSelect   PLL entry clock source select
 *
 * @param     pllMf      PLL multiplication factor
 *
 * @retval    None
 *
 * @note      This function must be used only when the PLL is disabled
 */
void RCM_ConfigPLL(RCM_PLLSEL_T pllSelect, RCM_PLLMF_T pllMf)
{
   RCM->CFG_B.PLLMF = pllMf;
   RCM->CFG_B.PLLSEL = pllSelect & 0x01;
   RCM->CFG_B.PLLXTDIV = (pllSelect >> 1) & 0x01;
}

/*!
* @brief      Enables PLL
*
* @param      None
*
* @retval     None
*
* @note       The PLL can not be disabled if it is used as system clock
*/ 
void RCM_EnablePLL(void)
{
   RCM->CTRL_B.PLLEN = BIT_SET;
}

/*!
* @brief      Disable PLL
*
* @param      None
*
* @retval     None
*
* @note       The PLL can not be disabled if it is used as system clock
*/  
void RCM_DisablePLL(void)
{
   RCM->CTRL_B.PLLEN = BIT_RESET;
}

/*!
 * @brief     Configures the system clock (SYSCLK)
 *
 * @param     sysClkSelect   specifies the clock source used as system clock
 *
 * @retval    None 
 *
 * @note 
 */
void RCM_ConfigSYSCLK(RCM_SYSCLK_SEL_T sysClkSelect)
{
   RCM->CFG_B.SCSEL = sysClkSelect;
}

/*!
 * @brief     returns the clock source used as system clock
 *
 * @param     None
 *
 * @retval    The clock source used as system clock 
 *
 * @note 
 */ 
RCM_SYSCLK_SEL_T RCM_GetSYSCLKSource(void)
{
   RCM_SYSCLK_SEL_T sysClock;

   sysClock = (RCM_SYSCLK_SEL_T)RCM->CFG_B.SCS;

   return sysClock;
}

/*!
 * @brief     Configures the AHB clock (HCLK)
 *
 * @param     AHBDiv   AHB divider number
 *
 * @retval    None 
 *
 * @note 
 */ 
void RCM_ConfigAHB(RCM_AHBDIV_T AHBDiv)
{
   RCM->CFG_B.AHBDIV = AHBDiv;
}

/*!
 * @brief     Configures the APB1 clock (PCLK1)
 *
 * @param     APB1Div      defines the APB1 clock divider. This clock is derived from the AHB clock (HCLK)
 *
 * @retval    None 
 *
 * @note 
 */ 
void RCM_ConfigAPB1(RCM_APBDIV_T APB1Div)
{
   RCM->CFG_B.APB1DIV = APB1Div;
}

/*!
 * @brief     Configures the APB2 clock (PCLK2))
 *
 * @param     APB2Div    defines the APB2 clock divider. This clock is derived from the AHB clock (HCLK)
 *
 * @retval    None 
 *
 * @note 
 */ 
void RCM_ConfigAPB2(RCM_APBDIV_T APB2Div)
{
   RCM->CFG_B.APB2DIV = APB2Div;
}

/*!
 * @brief     Enable RCM interrupts
 *
 * @param     interrupt   specifies the RCM interrupt sources to be enabled or disabled
 *
 * @retval    None 
 *
 * @note 
 */ 
void RCM_EnableInterrupt(RCM_INT_T interrupt)
{
   uint32_t temp;

   temp = interrupt << 8;

   RCM->INT |= temp;
}

/*!
 * @brief     Disable RCM interrupts
 *
 * @param     interrupt   specifies the RCM interrupt sources to be enabled or disabled
 *
 * @retval    None 
 *
 * @note 
 */ 
void RCM_DisableInterrupt(RCM_INT_T interrupt)
{
   uint32_t temp;

   temp = interrupt << 8;

   RCM->INT &= (uint32_t)~temp;
}
/*!
 * @brief     Configures the USB clock (USBCLK)
 *
 * @param     USBDiv   specifies the USB clock divider number.USB clock is derived from the PLL output
 *
 * @retval    None 
 *
 * @note 
 */ 
void RCM_ConfigUSBCLK(RCM_USBDIV_T USBDiv)
{
   RCM->CFG_B.USBDIV = USBDiv;
}

/*!
 * @brief     Configures the ADC clock (ADCCLK)
 *
 * @param     ADCDiv    defines the ADC clock divider. This clock is derived from the APB2 clock (PCLK2)
 *
 * @retval    None 
 *
 * @note 
 */ 
void RCM_ConfigADCCLK(RCM_ADCDIV_T ADCDiv)
{
   RCM->CFG_B.ADCDIV = ADCDiv;
}

/*!
 * @brief     Configures the External Low Speed oscillator (LXT)
 *
 * @param     state    specifies the new state of the LXT
 *
 * @retval    None 
 *
 * @note 
 */ 
void RCM_ConfigLXT(RCM_LXT_STATE_T state)
{
   RCM->BD_B.LXTEN = BIT_RESET;
   RCM->BD_B.LXTBYP = BIT_RESET;

   if(state == RCM_LXT_OPEN)
   {
      RCM->BD_B.LXTEN = BIT_SET;
   }
   else if(state == RCM_LXT_BYPASS)
   {
      RCM->BD_B.LXTBYP = BIT_SET;
      RCM->BD_B.LXTEN = BIT_SET;
   }
}

/*!
 * @brief     Enables the Internal Low Speed oscillator (LIRC)
 *
 * @param     None
 *
 * @retval    None 
 *
 * @note 
 */ 
void RCM_EnableLIRC(void)
{
   RCM->CSTS_B.LIRCEN = BIT_SET;
}

/*!
 * @brief     Disables the Internal Low Speed oscillator (LIRC)
 *
 * @param     None
 *
 * @retval    None 
 *
 * @note 
 */  
void RCM_DisableLIRC(void)
{
   RCM->CSTS_B.LIRCEN = BIT_RESET;
}

/*!
 * @brief     Configures the RTC clock (RTCCLK)
 *
 * @param     rtcClkSelect   specifies the RTC clock source
 *
 * @retval    None
 *
 * @note       Once the RTC clock is selected it can't be changed unless the Backup domain is reset
 */
void RCM_ConfigRTCCLK(RCM_RTCCLK_SEL_T rtcClkSelect)
{
   RCM->BD_B.RTCSEL = rtcClkSelect;
}

/*!
 * @brief     Enables the RTC clock
 *
 * @param     None
 *
 * @retval    None
 *
 * @note       
 */ 
void RCM_EnableRTCCLK(void)
{
   RCM->BD_B.RTCEN = BIT_SET;
}

/*!
 * @brief     Disables the RTC clock
 *
 * @param     None
 *
 * @retval    None
 *
 * @note       
 */ 
void RCM_DisableRTCCLK(void)
{
   RCM->BD_B.RTCEN = BIT_RESET;
}

/*!
 * @brief     Get frequency of SYSCLK
 *
 * @param     None
 *
 * @retval    Return frequency of SYSCLK
 *
 * @note       
 */ 
uint32_t RCM_GetSYSCLKFreq(void)
{
   uint32_t sysClock, pllMull, pllSource;

   /** get sys clock */
   sysClock = RCM->CFG_B.SCSEL;
   switch(sysClock)
   {
      /** sys clock is HIRC */
      case RCM_SYSCLK_SEL_HIRC:
         sysClock = HIRC_VALUE;
         break;
         
      /** sys clock is HXT */   
      case RCM_SYSCLK_SEL_HXT:
         sysClock = HXT_VALUE;
         break;

      /** sys clock is PLL */   
      case RCM_SYSCLK_SEL_PLL:
         pllMull = RCM->CFG_B.PLLMF + 2;
         pllSource = RCM->CFG_B.PLLSEL;

         /** PLL entry clock source is HXT */
         if(pllSource == BIT_SET)
         {
            sysClock = HXT_VALUE * pllMull;
            
            /** HXT clock divided by 2 */
            if(pllSource == RCM->CFG_B.PLLXTDIV)
            {
               sysClock >>= 1;
            }
         }
         /** PLL entry clock source is HIRC/2 */
         else
         {
            sysClock = (HIRC_VALUE >> 1) * pllMull;
         }
         break;      

      default:
         sysClock  = HIRC_VALUE;
         break;
   }

   return sysClock;
}

/*!
 * @brief     Get frequency of HCLK(AHB)
 *
 * @param     None
 *
 * @retval    Return frequency of HCLK
 *
 * @note       
 */ 
uint32_t RCM_GetHCLKFreq(void)
{
   uint32_t divider;
   uint32_t sysClk, hclk;
   uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

   sysClk = RCM_GetSYSCLKFreq();
   divider = AHBPrescTable[RCM->CFG_B.AHBDIV];
   hclk = sysClk >> divider;

   return hclk;
}

/*!
 * @brief     Get frequency of PCLK1 And PCLK2
 *
 * @param     PCLK1   return frequency of PCLK1
 *
 * @param     PCLK1   return frequency of PCLK2 
 *
 * @retval    None
 *
 * @note       
 */ 
void RCM_GetPCLKFreq(uint32_t *PCLK1, uint32_t *PCLK2)
{
   uint32_t hclk, divider;
   uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};

   hclk = RCM_GetHCLKFreq();

   if(PCLK1)
   {
      divider = APBPrescTable[RCM->CFG_B.APB1DIV];
      *PCLK1 = hclk >> divider;
   }

   if(PCLK2)
   {
      divider = APBPrescTable[RCM->CFG_B.APB2DIV];
      *PCLK2 = hclk >> divider;
   }
}

/*!
 * @brief     Get frequency of ADC CLK
 *
 * @param     None
 *
 * @retval    Return frequency of ADC CLK
 *
 * @note       
 */ 
/*
uint32_t RCM_GetADCCLKFreq(void)
{
   uint32_t adcClk, pclk2, divider;
   uint8_t ADCPrescTable[4] = {2, 4, 6, 8};

   RCM_GetPCLKFreq(0, &pclk2);
   
   // Get ADC CLK 
   divider = ADCPrescTable[RCM->CFG_B.ADCDIV];
     adcClk = pclk2 / divider;   

     return adcClk;
} */

/*!
 * @brief     Enables AHB peripheral clock
 *
 * @param     AHBPeriph   specifies the AHB peripheral to gates its clock
 *
 * @retval    None
 *
 * @note       
 */ 
void RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_T AHBPeriph)
{
   RCM->AHBCLKEN |= AHBPeriph;
}

/*!
 * @brief     Disable AHB peripheral clock
 *
 * @param     AHBPeriph   specifies the AHB peripheral to gates its clock
 *
 * @retval    None
 *
 * @note       
 */ 
void RCM_DisableAHBPeriphClock(RCM_AHB_PERIPH_T AHBPeriph)
{
   RCM->AHBCLKEN &= (uint32_t)~AHBPeriph;
}

/*!
 * @brief     Enable the High Speed APB (APB2) peripheral clock
 *
 * @param     APB2Periph   specifies the APB2 peripheral to gates its clock
 *
 * @retval    None
 *
 * @note       
 */  
void RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_T APB2Periph)
{
   RCM->APB2CLKEN |= APB2Periph;
}

/*!
 * @brief     Disable the High Speed APB (APB2) peripheral clock
 *
 * @param     APB2Periph   specifies the APB2 peripheral to gates its clock
 *
 * @retval    None
 *
 * @note       
 */ 
void RCM_DisableAPB2PeriphClock(RCM_APB2_PERIPH_T APB2Periph)
{
   RCM->APB2CLKEN &= (uint32_t)~APB2Periph;
}

/*!
 * @brief     Enable the Low Speed APB (APB1) peripheral clock
 *
 * @param     RCM_APB1Periph   specifies the APB1 peripheral to gates its clock
 *
 * @retval    None
 *
 * @note       
 */ 
void RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_T APB1Periph)
{
   RCM->APB1CLKEN |= APB1Periph;
}

/*!
 * @brief     Disable the Low Speed APB (APB1) peripheral clock
 *
 * @param     RCM_APB1Periph   specifies the APB1 peripheral to gates its clock
 *
 * @retval    None
 *
 * @note       
 */ 
void RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_T APB1Periph)
{
   RCM->APB1CLKEN &= (uint32_t)~APB1Periph;
}

/*!
 * @brief     Enable High Speed APB (APB2) peripheral reset
 *
 * @param     APB2Periph   specifies the APB2 peripheral to reset
 *
 * @retval     None
 *
 * @note       
 */ 
void RCM_EnableAPB2PeriphReset(RCM_APB2_PERIPH_T APB2Periph)
{
   RCM->APB2RST |= APB2Periph;
}

/*!
 * @brief     Disable High Speed APB (APB2) peripheral reset
 *
 * @param     APB2Periph   specifies the APB2 peripheral to reset
 *
 * @retval    None
 *
 * @note       
 */  
void RCM_DisableAPB2PeriphReset(RCM_APB2_PERIPH_T APB2Periph)
{
   RCM->APB2RST &= (uint32_t)~APB2Periph;
}

/*!
 * @brief     Enable Low Speed APB (APB1) peripheral reset
 *
 * @param     APB1Periph   specifies the APB1 peripheral to reset
 *
 * @retval    None
 *
 * @note 
 */
void RCM_EnableAPB1PeriphReset(RCM_APB1_PERIPH_T APB1Periph)
{
   RCM->APB1RST |= APB1Periph;
}

/*!
 * @brief     Disable Low Speed APB (APB1) peripheral reset
 *
 * @param     APB1Periph   specifies the APB1 peripheral to reset
 *
 * @retval    None
 *
 * @note 
 */
void RCM_DisableAPB1PeriphReset(RCM_APB1_PERIPH_T APB1Periph)
{
   RCM->APB1RST &= (uint32_t)~APB1Periph;
}

/*!
 * @brief     Enable the Backup domain reset
 *
 * @param     None
 *
 * @retval    None
 *
 * @note 
 */
void RCM_EnableBackupReset(void)
{
   RCM->BD_B.BDRST = BIT_SET;
}

/*!
 * @brief     Disable the Backup domain reset
 *
 * @param     None
 *
 * @retval    None
 *
 * @note 
 */
void RCM_DisableBackupReset(void)
{
   RCM->BD_B.BDRST = BIT_RESET;
}

/*!
 * @brief     Enable Clock Security System
 *
 * @param     None
 *
 * @retval    None
 *
 * @note 
 */
void RCM_EnableCCS(void)
{
   RCM->CTRL_B.CSSEN = BIT_SET;
}

/*!
 * @brief     Disable Clock Security System
 *
 * @param     None
 *
 * @retval    None
 *
 * @note 
 */
void RCM_DisableCCS(void)
{
   RCM->CTRL_B.CSSEN = BIT_RESET;
}

/*!
 * @brief     Selects clock ouput source
 *
 * @param     cocClock   specifies the clock source to output
 *
 * @retval    None
 *
 * @note 
 */
void RCM_ConfigCOC(RCM_COCCLK_T cocClock)
{
   RCM->CFG_B.COC = cocClock;
}

/*!
 * @brief     Read the specified RCM flag status
 *
 * @param     flag   specifies the flag to check
 *
 * @retval    The new state of flag (SET or RESET)
 *
 * @note 
 */
uint8_t RCM_ReadFlag(RCM_FLAG_T flag)
{
   uint32_t reg, bit;

   bit = (uint32_t)(1 << (flag & 0xff));

   reg = (flag >> 8) & 0xff;
   
   switch(reg)
   {
      case 0:
         reg = RCM->CTRL;
         break;

      case 1:
         reg = RCM->BD;
         break;

      case 2:
         reg = RCM->CSTS;
         break;

      default:
         break;
   }

   if(reg & bit)
   {
      return SET;
   }
   
   return RESET;
}

/*!
 * @brief     Clears the RCM reset flags
 *
 * @param     None
 *
 * @retval    None
 *
 * @note      The reset flags are:
 *            RCM_FLAG_PINRST, RCM_FLAG_PWRST, RCM_FLAG_SWRST
 *            RCM_FLAG_IWDTRST, RCM_FLAG_WWDTRST, RCM_FLAG_LPRRST
 */
void RCM_ClearFlag(void)
{
   RCM->CSTS_B.CLRRSTF = BIT_SET;
}

/*!
 * @brief     Read the specified RCM interrupt Flag
 *
 * @param     interrupt   specifies the RCM interrupt source to check
 *
 * @retval    The new state of intFlag (SET or RESET)
 *
 * @note 
 */
uint8_t RCM_ReadIntFlag(RCM_INT_T interrupt)
{
   uint8_t ret;

   ret = (RCM->INT & interrupt) ? SET : RESET;
   
     return  ret;
}

/*!
 * @brief     Clears the interrupt flag
 *
 * @param     interrupt   specifies the interrupt pending bit to clear
 *
 * @retval    None
 *
 * @note 
 */
void RCM_ClearIntFlag(RCM_INT_T interrupt)
{
   uint32_t temp;

   temp = interrupt << 16;
   RCM->INT |= temp;
}

