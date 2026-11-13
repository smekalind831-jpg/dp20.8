/*!
 * @file       apm32f10x_pmu.h
 *
 * @brief      This file contains all the functions prototypes for the PMU firmware library.
 *
 * @version    V1.0.0
 *
 * @date       2019-8-6
 *
 */

#ifndef __APM32F10x_PMU_H
#define __APM32F10x_PMU_H

#ifdef __cplusplus
extern "C" {
#endif

#include "apm32f10x.h"

typedef enum
{
  PMU_PVMLevel_2V2    = 0x0000,    ///!<  PVM detection level set to 2.2V
  PMU_PVMLevel_2V3    = 0x0002,    ///!<  PVM detection level set to 2.3V
  PMU_PVMLevel_2V4    = 0x0004,    ///!<  PVM detection level set to 2.4V
  PMU_PVMLevel_2V5    = 0x0006,    ///!<  PVM detection level set to 2.5V
  PMU_PVMLevel_2V6    = 0x0008,    ///!<  PVM detection level set to 2.6V
  PMU_PVMLevel_2V7    = 0x000A,    ///!<  PVM detection level set to 2.7V
  PMU_PVMLevel_2V8    = 0x000C,    ///!<  PVM detection level set to 2.8V
  PMU_PVMLevel_2V9    = 0x000E,    ///!<  PVM detection level set to 2.9V

} PMU_PVMLevel;

typedef enum
{
  PMU_Regulator_ON       = 0x00,
  PMU_Regulator_LowPower = 0x01

} PMU_LPSM;

typedef enum
{
  PMU_STOPEntry_WFI     = 0x01,
  PMU_STOPEntry_WFE     = 0x02

} PMU_STOPEntry;

typedef enum
{
  PMU_FLAG_WUPF,
  PMU_FLAG_SBMF,
  PMU_FLAG_PVMF

} PMU_Falg;

/*  Function used to set the PMU configuration to the default reset state *****/
void PMU_Reset(void );

/* Configuration and Operation modes functions **************************************************/
void PMU_EnableBackupAccess(void );
void PMU_DisableBackupAccess(void );
void PMU_EnablePVM(void);
void PMU_DisablePVM(void);
void PMU_SetPVMLeve(PMU_PVMLevel Val);
void PMU_EnableWakeUpPin(void );
void PMU_DisableWakeUpPin(void );
void PMU_EnterSTOPMode(PMU_LPSM LPSM_val, PMU_STOPEntry STOP_Val);
void PMU_EnterStandbyMode(void );

/* flags management functions **********************************/
uint8_t PMU_ReadFlag(PMU_Falg Reg);
void PMU_ClerFlag(PMU_Falg Reg);

#ifdef __cplusplus
}
#endif

#endif /* __APM32F10x_PMU_H */

