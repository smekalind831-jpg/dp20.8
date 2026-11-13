#ifndef _RBUS_H // Блокируем повторное включение этого модуля
#define _RBUS_H
//******************************************************************************
// Секция include
//******************************************************************************
/*#include "MG32x02z.h"
#include "MG32x02z_URT.h"
#include "MG32x02z_URT_DRV.h"
#include "MG32x02z__Common_DRV.h"*/
#include <stdio.h>
//#include "GlobalVar.h"
#include <version.h>
#include <param.h>
#include "main.h"
#include "Defs1.h"
//******************************************************************************
// Секция определения констант
//******************************************************************************
#define URTX    				URT0
#define INQUIRE1                0x19
#define INQUIRE2                0x1C
#define INQUIRE3                0xDb
#define INQUIRE4                0x5A
#define NUMBER1                 0x19
#define NUMBER2                 0x1C
#define NUMBER3                 0x1B
#define NUMBER4                 0x1A


#define ADDR_SERV               0x3F
#define REQUEST_SERIAL_NUMBER   0x23
#define REQUEST_DEVICE_NAME     0x4E
#define REQUEST_PROGRAMM_VERSION 0x56
#define REQUEST_SETTING_MODE    0x53
#define REQUEST_SET_PARAMETERS  0x54

#ifndef SET
#define SET    1
#endif
#ifndef RESET
#define RESET  0
#endif
#ifndef TRUE
#define TRUE   1
#endif
#ifndef FALSE
#define FALSE  0
#endif

//\ ======================== Константы ========================================


//******************************************************************************
// Секция определения типов
//******************************************************************************

//******************************************************************************
// Секция определения глобальных переменных
//******************************************************************************
 
//******************************************************************************
// Секция прототипов глобальных функций
//******************************************************************************

//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************
extern void Work_reqest (unsigned int Ang); //\ угрол
extern void Sereal( unsigned int HIG, unsigned int LO); //\ серийный №
extern void Name (void);//char* name); //\ в ACII название
extern void Version (void); //\ версия + дата
extern void ERR_stat (void); //\ Error !!! нет описания
extern void Settings ( unsigned char Tk, unsigned char  Np, work* angl ); //\режима настройки 
extern void SettingSet( unsigned char Nt, unsigned char Vn, unsigned char  Vn1 );

//******************************************************************************
// Секция определения макросов
//******************************************************************************

#endif
//******************************************************************************
// ENF OF FILE
//******************************************************************************
