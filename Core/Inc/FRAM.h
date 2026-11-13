
#include "main.h"

//\ ======== Функции ==============
extern void FRAM_WRITE ( unsigned int ADR, unsigned char* arr, unsigned char Size );
extern void WriteProtection(unsigned char en);
unsigned char ReadStatusRegister(void);
void W_Status (uint8_t Status);
//\ ========== CMD ================
#define WREN		0x06  //Set Write Enable Latch
#define WRDI		0x04  //Write Disable 
#define RDSR		0x05  //Read Status Register 
#define WRSR		0x01  //Write Status Register 
#define READ		0x03  //Read Memory Data
#define WRITE	    0x02  //Write Memory Data 

//\ =========== флаги ==============
#define WEL   0x02 // запись пока не возможно (статус тоже)