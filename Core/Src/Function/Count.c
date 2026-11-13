
#include "Count.h"

// =============== –ассчЄт угла индивидуально без оборотов =====================
unsigned int CountCS ( unsigned int CS, 
                       unsigned int Offset, 
                       char RT)  //\ сырой код с CS, смещение
{
  unsigned int Deg;
 
   Deg = ( CS >= Offset )?( CS - Offset ):( 0x1000 + CS - Offset ); 

   //- угол с учетом направлени€ вращени€ 
  if (Deg)
     if ( RT == CLOCKWISE )
      Deg = 0x1000 - Deg;   //- 0x1000 = 4096 = 360 град

return Deg;
}// CountCS

// ==================== ѕодсчЄт оборотлов индивидуально ========================
                           //\ угол без смещен€, старый угол,
 unsigned int CountTURN ( unsigned int no_off, unsigned int Old,
                          unsigned int tur, unsigned int max) //\ текущее оборотов, максимум оборотов,
 {  
   unsigned char R_Off, R_Old; //\ положение при вкл пит
   
   //\ доп условие от ложного срабатывани€
   
   R_Old = ( Old > ( 0x800 ) ) ? 1 : 0; //\ обесточили с б'ќльшей стороны 
   R_Off = ( no_off > ( 0x800 ) ) ? 1 : 0; //\ включили с б'ќльшей стороны 
    
  if( ( R_Old && !R_Off ) /*&& ( Old > no_off )*/ && ( ( Old - no_off ) > ( 0x1FF ) ) ) //\не просто увеличение угла 90 град
  {
    tur = (tur + 1 ) % ( max ); //\ макс число оборотов дл€ SC1, дл€ SC2 обнул€ть после 0х8000 отсчЄтов  
  }
  else if( ( !R_Old && R_Off ) /*&& ( Old < no_off)*/ && ( ( no_off - Old ) > ( 0x1FF ) ) ) //\ смена значени€ с переходом через 0
  {
    tur = ( tur == 0 ) ? ( max - 1 ) : ( ( tur - 1 ) % max ); //\ с меньшего на большее 
  }
 
  //\ angle.old_angle = angle.angle_no_offset; //\закоментировал 14.04.26 это есть до рассчЄта no_offsets
     
 // tur = tur % max; 
  return tur;
} // CountTURN 

                    //\ ¬ерси€ решени€ 2
/* –ассчитываем два значени€ no_off и два числа оборотов  ј “ќ потомсоедин€ем ?
будет примен€тьс€ дл€ 512 - оборотных. (группы 02, 03)*/
 //====================== ѕресчЄт полного угла индивидуально ====================
                  //\ максимально дискрет, максимально оборотов
unsigned int CountDEG ( unsigned int numDisc, unsigned int max,
                      //\ число оборотов, угол без смещени€ 
                       unsigned int tur, unsigned int no_off, unsigned int* wave )
{
  unsigned int Deg;
  unsigned int discret;
  
  discret = max ? (uint16_t) ( numDisc/max ) : 4096; //\ дискрет/об (если оборотов меньше 32, считаем 1 дискрета на оборот
     
   Deg = no_off / ( 4096/discret ) + ( tur * discret ); //+ 1; //\ исключить 0 ??

   *wave = no_off; // непрерывный  угол в приделах всех оборотов
 

  return Deg; 
}

