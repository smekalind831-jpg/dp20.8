

#include "main.h"

//\ ===================== вычисляет угол с учётом калибровки ===================
unsigned int CountCS ( unsigned int CS, 
                       unsigned int Offset, 
                       char RT );

//\====================== число полных оборотов ================================
                           //\ угол без смещеня, старый угол,  направление    
extern unsigned int CountTURN ( unsigned int no_off, unsigned int Old,
                                unsigned int tur,    unsigned int max ); //\ текущее оборотов, максимум оборотов

//\======================= вычисляет угол с учётом оборотов (индивидуально) ====
extern unsigned int CountDEG ( unsigned int numDisc, unsigned int max, //\ максимально дискрет, максимально оборотов,
                               unsigned int tur,     unsigned int no_off, unsigned int* wave);//\ число оборотов, угол без смещения

extern unsigned char Restory ( unsigned char Flag ); 