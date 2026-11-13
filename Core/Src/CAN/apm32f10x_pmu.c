/*!
 * @file       apm32f10x_pmu.c
 *
 * @brief      This file provides all the PMU firmware functions.
 *
 * @version    V1.0.0
 *
 * @date       2019-8-6
 *
 */

#include "CAN/apm32f10x_pmu.h"
#include "CAN/apm32f10x_rcm.h"

/*!
 * @brief     Reset the PMU peripheral register.
 *
 * @param     None
 *
 * @retval    None
 *
 * @note
 */
void PMU_Reset(void )
{
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_PMU);
    RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_PMU);
}

/*!
 * @brief     Enables access to the RTC and backup registers.
 *
 * @param     None
 *
 * @retval    None
 *
 * @note
 */
void PMU_EnableBackupAccess(void )
{
    PMU->CTRL_B.BAKRWEN = ENABLE ;
}

/*!
 * @brief     Disables access to the RTC and backup registers.
 *
 * @param     None
 *
 * @retval    None
 *
 * @note
 */
void PMU_DisableBackupAccess(void )
{
    PMU->CTRL_B.BAKRWEN = DISABLE;
}

/*!
 * @brief     Enables the Power Voltage Detector(PVM).
 *
 * @param     None
 *
 * @retval    None
 *
 * @note
 */
void PMU_EnablePVM(void)
{
    PMU->CTRL_B.PVMEN = ENABLE;
}

/*!
 * @brief     Disables the Power Voltage Detector(PVM).
 *
 * @param     None
 *
 * @retval    None
 *
 * @note
 */
void PMU_DisablePVM(void)
{
    PMU->CTRL_B.PVMEN = DISABLE;
}

/*!
 * @brief     Configure a voltage threshold detected by a power supply voltage detector (PVD).
 *
 * @param     Val£ºspecifies the PVM detection level
 *
 * @retval    None
 *
 * @note
 */
void PMU_SetPVMLeve(PMU_PVMLevel Val)
{

    /* Clear PLS[7:5] bits */
    PMU->CTRL_B.PLMS = 0x0000;
    /* Store the new value */
    PMU->CTRL_B.PLMS = Val;
}

/*!
 * @brief     Enables the WakeUp Pin functionality.
 *
 * @param     None
 *
 * @retval    None
 *
 * @note
 */
void PMU_EnableWakeUpPin(void )
{
    PMU->CTRLF_B.WUPEN = ENABLE ;
}

/*!
 * @brief     Diaables the WakeUp Pin functionality.
 *
 * @param     None
 *
 * @retval    None
 *
 * @note
 */
void PMU_DisableWakeUpPin(void )
{
    PMU->CTRLF_B.WUPEN = DISABLE ;
}

/*!
 * @brief     Enters STOP mode.
 *
 * @param     LPSM_val: specifies the regulator state in STOP mode.
 *
 * @param     STOP_Val: specifies if STOP mode in entered with WFI or WFE instruction.
 *
 * @retval    None
 *
 * @note
 */
void PMU_EnterSTOPMode(PMU_LPSM LPSM_val, PMU_STOPEntry STOP_Val)
{
    /** Clear PLS and LPSM bits */
    PMU->CTRL_B.SBMDS = 0x00;
    PMU->CTRL_B.LPSM = 0x00;
    /** Set LPSM bit according to PWR_Regulator value */
    PMU->CTRL_B.SBMDS = LPSM_val;
    /** Select STOP mode entry*/
    if(STOP_Val == PMU_STOPEntry_WFI)
    {
        /** Request Wait For Interrupt */
        __WFI();
    } else
    {
        /** Request Wait For Event */
        __WFE();
    }

    /** Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)0x04);

}

/*!
 * @brief     Enters STANDBY mode.
 *
 * @param     None
 *
 * @retval    None
 *
 * @note
 */
void PMU_EnterStandbyMode(void )
{
    /* Clear Wake-up flag */
    PMU->CTRL_B.RWUPF = BIT_SET;
    /* Select STANDBY mode */
    PMU->CTRL_B.SBMDS = BIT_SET;
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= (uint8_t )0x04;
#if defined ( __CC_ARM   )
    __force_stores();
#endif
    /* Request Wait For Interrupt */
    __WFI();

}

/*!
 * @brief     Read the specified PWR flag is set or not.
 *
 * @param     Reg£ºspecifies the flag to check
 *
 * @retval    The new state of PMU_FLAG (SET or RESET).
 *
 * @note
 */
uint8_t PMU_ReadFlag(PMU_Falg Reg)
{
    uint8_t BitStatus = BIT_RESET;
	
    if(Reg == PMU_FLAG_WUPF)
    {
        BitStatus = PMU->CTRLF_B.WUPF;
    } else if(Reg == PMU_FLAG_SBMF)
    {
        BitStatus = PMU->CTRLF_B.SBMF;
    } else if(Reg == PMU_FLAG_PVMF)
    {
        BitStatus = PMU->CTRLF_B.PVMF;
    }
    return BitStatus;
}

/*!
 * @brief     Clears the PWR's pending flags.
 *
 * @param     Reg£ºspecifies the flag to clear.
 *
 * @retval    None
 *
 * @note
 */
void PMU_ClerFlag(PMU_Falg Reg)
{
    if(Reg == PMU_FLAG_WUPF)
    {
        PMU->CTRL_B.RWUPF = BIT_SET;
    } else if(Reg == PMU_FLAG_SBMF)
    {
        PMU->CTRL_B.RSBF = BIT_SET;
    }
}
