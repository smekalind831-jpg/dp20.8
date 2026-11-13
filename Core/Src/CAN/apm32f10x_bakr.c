/*!
 * @file       apm32f10x_bakr.c
 *
 * @brief      This file provides all the BAKR firmware functions.
 *
 * @version    V1.0.0
 *
 * @date       2019-8-6
 *
 */

#include "CAN/apm32f10x_bakr.h"
#include "CAN/apm32f10x_RCM.h"

/*!
* @brief      Reset the BAKR peripheral registers to their default reset values.
*
* @param      None
*
* @retval     None
*
* @note
*/
void BAKR_Reset(void )
{
    RCM_EnableBackupReset();
    RCM_DisableBackupReset();
}

/*!
* @brief      Deinitializes the BAKR peripheral registers to their default reset values.
*
* @param      Val£ºspecifies the RTC output source.
*
* @retval     None
*
* @note
*/
void BAKR_ConfigTamperPinLevel(BAKR_TamperPinLevel Val)
{
    BAKR->CTRL_B.TPTM = Val;
}

/*!
* @brief      Enables the Tamper Pin activation.
*
* @param      None
*
* @retval     None
*
* @note
*/
void BAKR_EnableTamperPin(void )
{
    BAKR->CTRL_B.TPEN = ENABLE ;
}

/*!
* @brief      Disables the Tamper Pin activation.
*
* @param      None
*
* @retval     None
*
* @note
*/
void BAKR_DisableTamperPin(void )
{
    BAKR->CTRL_B.TPEN = DISABLE ;
}

/*!
* @brief      Enables the Tamper Pin Interrupt.
*
* @param      None
*
* @retval     None
*
* @note
*/
void BAKR_EnableInterrupt(void )
{
    BAKR->CTRLF_B.TPIEN = ENABLE ;
}

/*!
* @brief      Disables the Tamper Pin Interrupt.
*
* @param      None
*
* @retval     None
*
* @note
*/
void BAKR_DisableInterrupt(void )
{
    BAKR->CTRLF_B.TPIEN = DISABLE ;
}

/*!
* @brief      Select the RTC output source to output on the Tamper pin.
*
* @param      BAKR_RTCOutputSource:specifies the RTC output source.
*
* @retval     None
*
* @note
*/
void BAKR_SelectRTCOutput(BAKR_RTCOutputSource Soure)
{
    if(Soure == BKP_RTCOutputSource_None)
    {
        BAKR->RTCCTRL = RESET;
    } else if(Soure == BKP_RTCOutputSource_CalibClock)
    {
        BAKR->RTCCTRL_B.RTCO = BIT_SET;
    } else if(Soure == BKP_RTCOutputSource_Alarm)
    {
        BAKR->RTCCTRL_B.ASOEN = BIT_SET;
    } else if(Soure == BKP_RTCOutputSource_Second)
    {
        BAKR->RTCCTRL_B.ASOC = BIT_SET;
    }
}

/*!
* @brief      Sets RTC Clock Calibration value.
*
* @param      CalibrationValue:Calibration value
*
* @retval     None
*
* @note
*/
void BAKR_SetRTCCalibrationValue(uint8_t CalibrationValue)
{
    BAKR->RTCCTRL_B.CALP = CalibrationValue;
}

/*!
* @brief      Set user data to the specified Data Backup Register.
*
* @param      DATA : specifies the Data Backup Register.
*
* @param      Data : data to set
*
* @retval     None
*
* @note
*/
void BAKE_SetBackupRegister(BAKR_DATA DATA, uint16_t Data )
{
    __IOM uint32_t tmp = 0;

    tmp = (uint32_t)BAKR_BASE;
    tmp += DATA;

    *(__IOM uint32_t *) tmp = Data;
}

/*!
* @brief      Get user data to the specified Data Backup Register.
*
* @param      DATA : specifies the Data Backup Register.
*
* @retval     The content of the specified Data Backup Register
*
* @note
*/
uint16_t BAKR_GetBackupRegister(BAKR_DATA DATA)
{
    __IOM uint32_t tmp = 0;

    tmp = (uint32_t)BAKR_BASE;
    tmp += DATA;

    return (*(__IOM uint32_t *) tmp);
}

/*!
* @brief      Read whether the Tamper Pin Event flag is set or not.
*
* @param      None
*
* @retval     Tamper Pin Event flag state
*
* @note
*/
uint8_t BAKR_Readflag(void )
{
    return BAKR->CTRLF_B.TPEF;
}

/*!
* @brief      Clears Tamper Pin Event pending flag.
*
* @param      None
*
* @retval     None
*
* @note
*/
void BAKR_ClearFlag(void )
{
    BAKR->CTRLF_B.RTPEF = BIT_SET;
}

/*!
* @brief      Get whether the Tamper Pin Interrupt has occurred or not.
*
* @param      None
*
* @retval     Tamper Pin Interrupt State
*
* @note
*/
uint8_t BAKR_ReadIntFlag(void )
{
    return BAKR->CTRLF_B.TPIF;
}

/*!
* @brief      Clears Tamper Pin Interrupt pending bit.
*
* @param      None
*
* @retval     None
*
* @note
*/
void BAKR_ClearIntFlag(void )
{
    BAKR->CTRLF_B.RTPIF = BIT_SET;
}


