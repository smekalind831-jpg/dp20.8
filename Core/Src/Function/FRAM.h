
#include "main.h"
#include "Defs1.h"

//\ ======== Функции ==============
extern void FRAM_WRITE ( unsigned int ADR, unsigned char* arr, unsigned char Size );
extern void FRAM_READ  ( unsigned int ADR, unsigned char Size );
extern void W_Protection(unsigned char en);
extern unsigned char R_Status(void);
extern void W_Status (uint8_t Status);
extern unsigned char R_Bute (uint16_t ADR);
extern void W_Bute (uint16_t ADR, unsigned char Bute);
//\ ========== CMD ================
#define WREN		0x06  //Set Write Enable Latch
#define WRDI		0x04  //Write Disable 
#define RDSR		0x05  //Read Status Register 
#define WRSR		0x01  //Write Status Register 
#define READ		0x03  //Read Memory Data
#define WRITE	        0x02  //Write Memory Data 

 //\ =========== флаги ==============
#define WEL         0x02 // запись пока не возможна (статус тоже)
//\ =========== маски ==============
//\ =========== глобальные =========
//extern   Con SPI1_CUR; //\ задачи SPI
             