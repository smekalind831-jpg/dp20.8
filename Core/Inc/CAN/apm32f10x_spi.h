/*!
 * @file        apm32f10x_spi.h
 *
 * @brief         This file contains all the functions prototypes for the SPI firmware library
 *
 * @details    
 *
 * @version        V1.0.0
 *
 * @date        2019-8-6
 *
 */

#ifndef __APM32F10x_SPI_H
#define __APM32F10x_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif
     
#include "apm32f10x.h"

typedef enum
{
    SPI_DIRECTION_2LINES_FULLDUPLEX = 0x0000,
    SPI_DIRECTION_2LINES_RXONLY = 0x0400,
    SPI_DIRECTION_1LINE_RX = 0x8000,
    SPI_DIRECTION_1LINE_TX = 0xC000
}SPI_DIRECTION_T;

/**
 * @brief    Configuration Mode enumeration
 */
typedef enum
{
    SPI_MODE_MASTER = 0x0104,
    SPI_MODE_SLAVE = 0x0000
}SPI_MODE_T;

/**
 * @brief    SPI Data size
 */
typedef enum
{
    SPI_DATASIZE_16B = 0x0800,
    SPI_DATASIZE_8B = 0x0000
}SPI_DATASIZE_T;

/**
 * @brief    SPI Clock Polarity
 */
typedef enum
{
    SPI_CPOL_LOW = 0x0000,
    SPI_CPOL_HIGH = 0x0002
}SPI_CPOL_T;

/**
 * @brief    SPI Clock Phase
 */
typedef enum
{
    SPI_CPHA_1EDGE = 0x0000,
    SPI_CPHA_2EDGE = 0x0001
}SPI_CPHA_T;

/**
 * @brief    SPI Slave Select management
 */
typedef enum
{
    SPI_NSS_SOFT = 0x0200,
    SPI_NSS_HARD = 0x0000
}SPI_NSS_T;

/**
 * @brief    SPI BaudRate Prescaler
 */
typedef enum
{
    SPI_BAUDRATE_PRESCALER_2 = 0x0000,
    SPI_BAUDRATE_PRESCALER_4 = 0x0008,
    SPI_BAUDRATE_PRESCALER_8 = 0x0010,
    SPI_BAUDRATE_PRESCALER_16 = 0x0018,
    SPI_BAUDRATE_PRESCALER_32 = 0x0020,
    SPI_BAUDRATE_PRESCALER_64 = 0x0028,
    SPI_BAUDRATE_PRESCALER_128 = 0x0039,
    SPI_BAUDRATE_PRESCALER_256 = 0x0038,
}SPI_BAUDRATE_PRESCALER_T;

/**
 * @brief    SPI MSB LSB transmission
 */
typedef enum
{
    SPI_FIRSTBIT_MSB = 0x0000,
    SPI_FIRSTBIT_LSB = 0x0080
}SPI_FIRSTBIT_T;

/**
 * @brief    SPI Config structure definition
 */
typedef struct
{
    uint16_t direction;
    uint16_t mode;
    uint16_t dataSize;
    uint16_t cpol;
    uint16_t cpha;
    uint16_t nss;
    uint16_t baudRatePrescaler;
    uint16_t firstBit;
    uint16_t crcPolynomial;
}SPI_ConfigStruct_T;


/**
 * @brief    I2S_Mode
 */
typedef enum 
{
    I2S_MODE_SLAVE_TX = 0x0000,
    I2S_MODE_SLAVE_RX = 0x0100,
    I2S_MODE_MASTER_TX = 0x0200,
    I2S_MODE_MASTER_RX = 0x0300
}I2S_MODE;

/**
 * @brief    I2S_Standard
 */
typedef enum 
{
    I2S_STANDARD_PHILLIPS = 0x0000,
    I2S_STANDARD_MSB      = 0x0010,
    I2S_STANDARD_LSB      = 0x0020,
    I2S_STANDARD_PCMSHORT = 0x0030,
    I2S_STANDARD_PCMLONG  = 0x00B0
}I2S_STANDARD;

/**
 * @brief    I2S_Data_Format
 */
typedef enum 
{
    I2S_DATAFORMAT_16b = 0x0000,
    I2S_DATAFORMAT_16bbextended = 0x0001,
    I2S_DATAFORMAT_24b = 0x0003,
    I2S_DATAFORMAT_32b = 0x0005
}I2S_DATA_FORMAT;

/**
 * @brief    I2S_MCLK_Output
 */
typedef enum 
{
    I2S_MCLK_OUTPUT_DISABLE = 0x0000,
    I2S_MCLK_OUTPUT_ENABLE = 0x0200,
}I2S_MCLK_OUTPUT;

/**
 * @brief    I2S_Audio_Frequency
 */
typedef enum 
{
    I2S_AUDIO_FREQUENCY_192k = 192000,
    I2S_AUDIO_FREQUENCY_96k  = 96000,
    I2S_AUDIO_FREQUENCY_48k  = 48000,
    I2S_AUDIO_FREQUENCY_44k  = 44100,
    I2S_AUDIO_FREQUENCY_32k  = 32000,
    I2S_AUDIO_FREQUENCY_22k  = 22050,
    I2S_AUDIO_FREQUENCY_16k  = 16000,
    I2S_AUDIO_FREQUENCY_11k  = 11025,
    I2S_AUDIO_FREQUENCY_8k   = 8000,
    I2S_AUDIO_FREQUENCY_Default = 2
}I2S_AUDIO_FREQUENCY;

/**
 * @brief    I2S_Clock_Polarity
 */
typedef enum 
{
    I2S_CPOL_LOW = 0x0000,
    I2S_CPOL_HIGH = 0x0008
}I2S_CLOCK_POLARITY;

/**
 * @brief    I2S Config structure definition
 */
typedef struct 
{
    I2S_MODE mode;

    I2S_STANDARD standard;

    I2S_DATA_FORMAT dataFormat;

    I2S_MCLK_OUTPUT MCLKOutput;

    I2S_AUDIO_FREQUENCY audioFreq;

    I2S_CLOCK_POLARITY CPOL;

}I2S_ConfigStruct_T;



/**
 * @brief    SPI Direction select
 */
typedef enum
{
    SPI_DIRECTION_RX = 0xBFFF,
    SPI_DIRECTION_TX = 0x4000
}SPI_DIRECTION_SELECT_T;

/**
 * @brief    SPI interrupts definition
 */
typedef enum
{
    SPI_I2S_INT_TXBEIE  = 0x71,
    SPI_I2S_INT_RXBNEIE = 0x60,
    SPI_I2S_INT_ERR  = 0x50,
    SPI_I2S_INT_RXOF  = 0x56,
    SPI_INT_MEF     = 0x55,
    SPI_INT_CRCEF   = 0x54,
    I2S_INT_UDF      = 0x53
}SPI_I2S_INT_T;

/**
 * @brief    SPI flags definition
 */
typedef enum
{
    SPI_FLAG_RXNE    = 0x0001,
    SPI_FLAG_TXE     = 0x0002,
    I2S_FLAG_CHSIDE  = 0x0004,
    I2S_FLAG_UDR     = 0x0008,
    SPI_FLAG_CRCERR  = 0x0010,
    SPI_FLAG_MODF    = 0x0020,
    SPI_FLAG_OVR     = 0x0040,
    SPI_FLAG_BSY     = 0x0080
}SPI_FLAG_T;


/**
 * @brief    SPI_I2S_DMA_transfer_requests
 */
typedef enum
{
    SPI_I2S_DMAReq_TX = 0x0002,
    SPI_I2S_DMAReq_RX = 0x0001
}SPI_I2S_DMAReq;



/*  Function used to set the SPI configuration to the default reset state *****/
void SPI_I2S_Reset(SPI_T* spi);
/* Configuration functions **********************************************************/
void SPI_Config(SPI_T* spi, SPI_ConfigStruct_T* configStruct);
void I2S_Config(SPI_T* spi, I2S_ConfigStruct_T* configStruct);
void SPI_StructInit(SPI_ConfigStruct_T* configStruct);
void I2S_StructInit(I2S_ConfigStruct_T* configStruct);
void SPI_Enable(SPI_T* spi);
void SPI_Disable(SPI_T* spi);
void I2S_Enable(SPI_T* spi);
void I2S_Disable(SPI_T* spi);
void SPI_I2S_Int_Enable(SPI_T* spi, SPI_I2S_INT_T SPI_I2S_Int);
void SPI_I2S_Int_Disable(SPI_T* spi, SPI_I2S_INT_T SPI_I2S_Int);
void SPI_I2S_DMAEnable(SPI_T* spi, SPI_I2S_DMAReq spi_i2s_DMAReq);
void SPI_I2S_DMADisable(SPI_T* spi, SPI_I2S_DMAReq spi_i2s_DMAReq);
void SPI_I2S_SendData(SPI_T* spi, uint16_t data);
uint16_t SPI_I2S_ReceiveData(SPI_T* spi);
void SPI_NSSInternalSoftwareSet(SPI_T* spi);
void SPI_NSSInternalSoftwareReset(SPI_T* spi);
void SPI_SSOutputEnable(SPI_T* spi);
void SPI_SSOutputDisable(SPI_T* spi);
void SPI_DataSizeConfig(SPI_T* spi, uint16_t dataSize);
void SPI_TransmitCRC_Enable(SPI_T* spi);
void SPI_CalculateCRC_Enable(SPI_T* spi);
void SPI_CalculateCRC_Disable(SPI_T* spi);
uint16_t SPI_ReadCRC_TX(SPI_T* spi);
uint16_t SPI_ReadCRC_RX(SPI_T* spi);
uint16_t SPI_ReadCRCPolynomial(SPI_T* spi);
void SPI_BiDirectionalLineConfig(SPI_T* spi, uint16_t direction);
uint16_t SPI_I2S_ReadFlag(SPI_T* spi, SPI_FLAG_T flag);
void SPI_I2S_ClearFlag(SPI_T* spi, SPI_FLAG_T flag);
uint16_t SPI_I2S_ReadIntFlag(SPI_T* spi, SPI_I2S_INT_T interrupt);
void SPI_I2S_ClearIntFlag(SPI_T* spi, SPI_I2S_INT_T interrupt);
#ifdef __cplusplus
}
#endif

#endif /* __APM32F10x_SPI_H */
