//******************************************************************************
// global_macro.h - Макроопределения
// НПП "Резонанс" 2018г.
//******************************************************************************

#ifndef __global_macro_h__
#define __global_macro_h__


#define Null 0

#define f2int(f)    (int)((f)*256+0.5)
#define f2long(f)    (long)((f)*(1<<16)+(f > 0 ? 0.5 : -0.5))
#define int2f(i)    (((float)i)/256)
#define long2f(i)    (((float)i)/(1<<16))

#define NUMBER_OF(a) ( sizeof(a) / sizeof(a[0]) )
#define ITEMS_OF(a) ( sizeof(a) / sizeof(a[0]) )
#define ARRAY_SIZE(a) ( sizeof(a) / sizeof(a[0]) )
#define code const

#define LOW(Word) ((unsigned char)(Word)&0xFF)
#define HIGH(Word) ((unsigned char)((Word)>>8)&0xFF)
#define LWORD(DWord) ((unsigned short)(DWord)&0xFFFF)
#define HWORD(DWord) ((unsigned short)((DWord)>>16)&0xFFFF)
#define LNIB(Byte) ((Byte)&0x0F)
#define HNIB(Byte) (((Byte)>>4)&0x0F)
#define MAKE_WORD(HByte, LByte) (((HByte)<<8) + (LByte))
#define MAKE_DWORD(HWord, LWord) (((HWord)<<16) + (LWord))
#define BITS2BYTE(h, g, f, e, d, c, b, a) (((h)<<7)+((g)<<6)+((f)<<5)+((e)<<4)+\
					   ((d)<<3)+((c)<<2)+((b)<<1)+((a)<<0))
#define OFFSET_OF(st, m) ((unsigned long)&((st *)0)->m)

#define OBJ_TO_BYTE_P(obj)			((uint8_t*)&(obj))
#define OBJ_P_TO_BYTE_P(obj)		((uint8_t*)(obj))
#define ARR_TO_OBJ_P(obj, arr)		((obj*)(arr))

#define ATM_TO_MPA  0.101325027f
#define MPA_TO_ATM  (1/ATM_TO_MPA)
#define KPA_TO_ATM  (9.8692/1000)
#define ATMCONST (6.25f/256)

#define BITBAND_SRAM(a,b) ((SRAM_BB_BASE + (a-SRAM_BASE)*32 + (b*4)))  // Convert SRAM address


#endif
