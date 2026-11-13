/*!
 * @file        apm32f10x_bakr.h
 *
 * @brief           This file contains all the functions prototypes for the BAKR firmware
 *            library.
 *
 * @details
 *
 * @version        V1.0.0
 *
 * @date        2019-8-6
 *
 */

#ifndef __APM32F10x_BAKR_H
#define __APM32F10x_BAKR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "apm32f10x.h"

typedef enum
{
  BKP_TamperPinLevel_High,
  BKP_TamperPinLevel_Low
} BAKR_TamperPinLevel;

typedef enum
{
  BKP_RTCOutputSource_None,
  BKP_RTCOutputSource_CalibClock,
  BKP_RTCOutputSource_Alarm,
  BKP_RTCOutputSource_Second

} BAKR_RTCOutputSource;

/**
 * @brief BAKR DATA register Addr
 */
typedef enum
{
  BAKR_DATA1 =  ((uint16_t)0x0004),
  BAKR_DATA2 =  ((uint16_t)0x0008),
  BAKR_DATA3 =  ((uint16_t)0x000C),
  BAKR_DATA4 =  ((uint16_t)0x0010),
  BAKR_DATA5 =  ((uint16_t)0x0014),
  BAKR_DATA6 =  ((uint16_t)0x0018),
  BAKR_DATA7 =  ((uint16_t)0x001C),
  BAKR_DATA8 =  ((uint16_t)0x0020),
  BAKR_DATA9 =  ((uint16_t)0x0024),
  BAKR_DATA10 = ((uint16_t)0x0028),
  BAKR_DATA11 = ((uint16_t)0x0040),
  BAKR_DATA12 = ((uint16_t)0x0044),
  BAKR_DATA13 = ((uint16_t)0x0048),
  BAKR_DATA14 = ((uint16_t)0x004C),
  BAKR_DATA15 = ((uint16_t)0x0050),
  BAKR_DATA16 = ((uint16_t)0x0054),
  BAKR_DATA17 = ((uint16_t)0x0058),
  BAKR_DATA18 = ((uint16_t)0x005C),
  BAKR_DATA19 = ((uint16_t)0x0060),
  BAKR_DATA20 = ((uint16_t)0x0064),
  BAKR_DATA21 = ((uint16_t)0x0068),
  BAKR_DATA22 = ((uint16_t)0x006C),
  BAKR_DATA23 = ((uint16_t)0x0070),
  BAKR_DATA24 = ((uint16_t)0x0074),
  BAKR_DATA25 = ((uint16_t)0x0078),
  BAKR_DATA26 = ((uint16_t)0x007C),
  BAKR_DATA27 = ((uint16_t)0x0080),
  BAKR_DATA28 = ((uint16_t)0x0084),
  BAKR_DATA29 = ((uint16_t)0x0088),
  BAKR_DATA30 = ((uint16_t)0x008C),
  BAKR_DATA31 = ((uint16_t)0x0090),
  BAKR_DATA32 = ((uint16_t)0x0094),
  BAKR_DATA33 = ((uint16_t)0x0098),
  BAKR_DATA34 = ((uint16_t)0x009C),
  BAKR_DATA35 = ((uint16_t)0x00A0),
  BAKR_DATA36 = ((uint16_t)0x00A4),
  BAKR_DATA37 = ((uint16_t)0x00A8),
  BAKR_DATA38 = ((uint16_t)0x00AC),
  BAKR_DATA39 = ((uint16_t)0x00B0),
  BAKR_DATA40 = ((uint16_t)0x00B4),
  BAKR_DATA41 = ((uint16_t)0x00B8),
  BAKR_DATA42 = ((uint16_t)0x00BC)
} BAKR_DATA;

/*  Function used to set the BAKR configuration to the default reset state *****/
void BAKR_Reset(void );

/* Initialization and Configuration functions *********************************/
void BAKR_ConfigTamperPinLevel(BAKR_TamperPinLevel Val);
void BAKR_EnableTamperPin(void );
void BAKR_DisableTamperPin(void );
void BAKR_SelectRTCOutput(BAKR_RTCOutputSource Soure);
void BAKR_SetRTCCalibrationValue(uint8_t CalibrationValue);
void BAKE_SetBackupRegister(BAKR_DATA DATA, uint16_t Data );
uint16_t BAKR_GetBackupRegister(BAKR_DATA DATA);

/* Interrupts and flags management functions **********************************/
void BAKR_EnableInterrupt(void );
void BAKR_DisableInterrupt(void );
uint8_t BAKR_Readflag(void );
void BAKR_ClearFlag(void );
uint8_t BAKR_ReadIntFlag(void );
void BAKR_ClearIntFlag(void );

#ifdef __cplusplus
}
#endif

#endif /* __APM32F10x_BAKR_H */

