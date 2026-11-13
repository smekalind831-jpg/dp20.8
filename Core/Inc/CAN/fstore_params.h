/*
 *******************************************************************************
 * file    bi_config.h
 * author  Rezonans
 * brief   Настройки блока индикации для хранения в fram
 *******************************************************************************
 */
#ifndef __FSTORE_PARAMS_H__
#define __FSTORE_PARAMS_H__

#include "apm32f10x.h"
#include "global_macro.h"
#include "prg_struct.h"
#include "GlobalVar.h"
#include "main.h"

typedef struct
{
	unsigned long SPN;
	void *pvalue;
	unsigned char size;
}parameter_struct;

extern const parameter_struct Nastr_params[20];

extern void default_params(void);

#endif  //__FSTORE_PARAMS_H__