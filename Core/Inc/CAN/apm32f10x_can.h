/*!
 * @file       apm32f10x_can.h
 *
 * @brief      This file contains all the functions prototypes for the CAN firmware library
 *
 * @version    V1.0.0
 *
 * @date       2019-8-6
 *
 */

#ifndef __APM32F10x_CAN_H
#define __APM32F10x_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "apm32f10x.h"

/**
 * @brief CAN mode
 */
typedef enum
{
  CAN_MODE_NORMAL,
  CAN_MODE_LOOPBACK,
  CAN_MODE_SILENT,
  CAN_MODE_SILENT_LOOPBACK
} CAN_MODE_T;

/**
 * @brief CAN synchronisation jump width
 */
typedef enum
{
  CAN_RSJW_1tq,
  CAN_RSJW_2tq,
  CAN_RSJW_3tq,
  CAN_RSJW_4tq
} CAN_RSJW_T;

/**
 * @brief CAN time quantum in bit segment 1
 */
typedef enum
{
  CAN_BS1_1tq,
  CAN_BS1_2tq,
  CAN_BS1_3tq,
  CAN_BS1_4tq,
  CAN_BS1_5tq,
  CAN_BS1_6tq,
  CAN_BS1_7tq,
  CAN_BS1_8tq,
  CAN_BS1_9tq,
  CAN_BS1_10tq,
  CAN_BS1_11tq,
  CAN_BS1_12tq,
  CAN_BS1_13tq,
  CAN_BS1_14tq,
  CAN_BS1_15tq,
  CAN_BS1_16tq
} CAN_BS1_T;

/**
 * @brief CAN time quantum in bit segment 2
 */
typedef enum
{
  CAN_BS2_1tq,
  CAN_BS2_2tq,
  CAN_BS2_3tq,
  CAN_BS2_4tq,
  CAN_BS2_5tq,
  CAN_BS2_6tq,
  CAN_BS2_7tq,
  CAN_BS2_8tq,
} CAN_BS2_T;

/**
 * @brief CAN config structure definition
 */
typedef struct
{
  uint16_t CAN_PRESCALER;
  CAN_MODE_T CAN_MODE;
  CAN_RSJW_T CAN_RSJW;
  CAN_BS1_T CAN_BS1;
  CAN_BS2_T CAN_BS2;
  uint16_t CAN_TTCM;
  uint16_t CAN_ABOM;
  uint16_t CAN_AWUPM;
  uint16_t CAN_ARTDIS;
  uint16_t CAN_RFLOCK;
  uint16_t CAN_TX_PS;
} CAN_Config_T;

/**
 * @brief CAN filter FIFO
 */
typedef enum
{
  CAN_FILTER_FIFO0,
  CAN_FILTER_FIFO1
} CAN_FILTER_FIFO_T;

/**
 * @brief CAN filter mode
 */
typedef enum
{
  CAN_FILTER_MODE_IDMASK,
  CAN_FILTER_MODE_IDLIST
} CAN_FILTER_MODE_T;

/**
 * @brief CAN filter scale
 */
typedef enum
{
  CAN_FILTER_SCALE_16BIT,
  CAN_FILTER_SCALE_32BIT
} CAN_FILTER_SCALE_T;

/**
 * @brief     CAN filter config structure definition
 */
typedef struct
{
  uint16_t CAN_FILTER_IDHIGH;
  uint16_t CAN_FILTER_IDLOW;
  uint16_t CAN_FILTER_MASK_IDHIGH;
  uint16_t CAN_FILTER_MASK_IDLOW;
  CAN_FILTER_FIFO_T CAN_FILTER_FIFO_ASSIGNMENT;
  uint8_t CAN_FILTER_NUMBER;
  CAN_FILTER_MODE_T CAN_FILTER_MODE;
  CAN_FILTER_SCALE_T CAN_FILTER_SCALE;
  uint16_t CAN_FILTER_ACTIVATION;
} CAN_FILTER_CONFIG_T;

/**
 * @brief CAN identifier type
 */
typedef enum
{
  CAN_ID_STANDARD = 0x00000000,
  CAN_ID_EXTENDED = 0x00000004
} CAN_IDE_T;

/**
 * @brief CAN_remote_transmission_request
 */
typedef enum
{
  CAN_RTR_DATA = 0x00000000,
  CAN_RTR_REMOTE = 0x00000002
} CAN_RTR_T;
/**
 * @brief  CAN Tx message structure definition
 */
typedef struct
{
  uint32_t STDID; //Specifies the standard identifier.This parameter can be a value between 0 to 0x7FF.
  uint32_t EXTID;
  CAN_IDE_T IDE;
  CAN_RTR_T RTR;
  uint8_t DLC;
  uint8_t DATA[8];
} CAN_TX_MESSAGE;

/**
 * @brief Mailboxes definition
 */
typedef enum
{
  CAN_TX_MAILBIX_1,
  CAN_TX_MAILBIX_2,
  CAN_TX_MAILBIX_3
} CAN_TX_MAILBIX_T;

/**
 * @brief  CAN Rx message structure definition
 */
//- структура сообщения, принятая из аппаратного буфера FIFO и размещаемая в памяти
typedef struct
{
  uint32_t STDID; //Specifies the standard identifier.This parameter can be a value between 0 to 0x7FF.
  uint32_t EXTID;
  uint32_t IDE;  ///!< CAN_IDE_T
  uint32_t RTR;  ///!< CAN_RTR_T
  uint8_t DLC;
  uint8_t DATA[8];
  uint8_t FMI;
} CAN_RX_MESSAGE_T;

/**
 * @brief CAN receive FIFO number constants
 */
typedef enum
{
  CAN_RX_FIFO1,
  CAN_RX_FIFO2
} CAN_RX_FIFO_T;

/**
 * @brief CAN Operating Mode
 */
typedef enum
{
  CAN_OPERATINGMODE_INITIALIZATION,
  CAN_OPERATINGMODE_NORMAL,
  CAN_OPERATINGMODE_SLEEP
} CAN_OPERATING_MODE_T;

/**
 * @brief CAN Interrupts
 */
typedef enum
{
  CAN_INT_EORM  = 0x00000001,
  CAN_INT_FMNP1 = 0x00000002,
  CAN_INT_FFI1  = 0x00000004,
  CAN_INT_FOFI1 = 0x00000008,
  CAN_INT_FMNP2 = 0x00000010,
  CAN_INT_FFI2  = 0x00000020,
  CAN_INT_FOFI2 = 0x00000040,
  CAN_INT_EWGI  = 0x00000100,
  CAN_INT_EPVI  = 0x00000200,
  CAN_INT_BOFI  = 0x00000400,
  CAN_INT_LECI  = 0x00000800,
  CAN_INT_ERRI  = 0x00008000,
  CAN_INT_WUPI  = 0x00010000,
  CAN_INT_SLPI  = 0x00020000
} CAN_INT_T;

/**
 * @brief CAN Interrupts Flage
 */
typedef enum
{
  /** Error flag*/
  CAN_FLAG_EWF = 0x10F00001, ///!<  Error Warning Flag
  CAN_FLAG_EPF = 0x10F00002, ///!<  Error Passive Flag
  CAN_FLAG_BOF = 0x10F00004, ///!<  Bus-Off Flag
  CAN_FLAG_LEC = 0x30F00070, ///!<  Last error code Flag
  /** Operating Mode Flags */
  CAN_FLAG_WUPIF = 0x31000008,///!<Wake up Flag
  CAN_FLAG_SLAK = 0x31000012,///!<Sleep acknowledge Flag
  /** Receive Flags */
  CAN_FLAG_FMNPF1 = 0x12000003, ///!<  FIFO 1 Message Pending Flag
  CAN_FLAG_FFIF1  = 0x32000008, ///!<  FIFO 1 Full Flag
  CAN_FLAG_FOFF1 = 0x32000010, ///!<  FIFO 1 Overrun Flag
  CAN_FLAG_FMNPF2 = 0x14000003, ///!<  FIFO 2 Message Pending Flag
  CAN_FLAG_FFIF2  = 0x34000008, ///!<  FIFO 3 Full Flag
  CAN_FLAG_FOFF2 = 0x34000010, ///!<  FIFO 2 Overrun Flag
  /** Transmit Flags */
  CAN_FLAG_EORM1   = 0x38000001, ///!<  Request MailBox1 Flag
  CAN_FLAG_EORM2   = 0x38000100, ///!<  Request MailBox2 Flag
  CAN_FLAG_EORM3   = 0x38010000, ///!<  Request MailBox3 Flag

} CAN_FLAG_T;

#define CAN_TxStatus_NoMailBox      ((uint8_t)0x04) /*!< CAN cell did not provide an empty mailbox */

/*  Function used to set the CAN configuration to the default reset state *****/
void CAN_Reset(CAN_T* CANx);

/* Initialization and Configuration functions *********************************/
uint8_t CAN_Config(CAN_T* CANx, CAN_Config_T* CAN_Config);
void CAN_FilterConfig(CAN_FILTER_CONFIG_T* CAN_FilterStruct);
void CAN_StructInit(CAN_Config_T* CAN_Config);
void CAN_SlaveStartBank(uint8_t CAN_BankNumber);
void CAN_EnableDBGFreeze(CAN_T* CANx);
void CAN_DisableDBGFreeze(CAN_T* CANx);
void CAN_EnableTTCComMode(CAN_T* CANx);
void CAN_DisableTTCComMode(CAN_T* CANx);

/* Transmit functions *********************************************************/
uint8_t CAN_Transmit(CAN_T* CANx, CAN_TX_MESSAGE* TxMessage);
uint8_t CAN_TransmitStatus(CAN_T* CANx, CAN_TX_MAILBIX_T transmitMailbox);
void CAN_CancelTransmit(CAN_T* CANx, CAN_TX_MAILBIX_T mailBox);

/* Receive functions **********************************************************/
void CAN_Receive(CAN_T* CANx, CAN_RX_FIFO_T FIFONumber, CAN_RX_MESSAGE_T* RxMessage);
void CAN_FIFORelease(CAN_T* CANx, CAN_RX_FIFO_T FIFONumber);
uint8_t CAN_MessagePending(CAN_T* CANx, CAN_RX_FIFO_T FIFONumber);

/* Operation modes functions **************************************************/
uint8_t CAN_OperatingModeRequest(CAN_T* CANx, CAN_OPERATING_MODE_T CAN_OperatingMode);
uint8_t CAN_Sleep(CAN_T* CANx);
uint8_t CAN_WakeUp(CAN_T* CANx);

/* Error management functions *************************************************/
uint8_t CAN_ReadLastErrorCode(CAN_T* CANx);
uint8_t ReadReceiveErrorCounter(CAN_T* CANx);
uint8_t CAN_ReadLSBTransmitErrorCounter(CAN_T* CANx);

/* Interrupts and flags management functions **********************************/
void CAN_EnableInterrupt(CAN_T* CANx, CAN_INT_T interrupts);
void CAN_DisableInterrupt(CAN_T* CANx, CAN_INT_T interrupts);
extern uint8_t CAN_ReadFlagStatus(CAN_T* CANx, CAN_FLAG_T flag);
void CAN_ClearFlag(CAN_T* CANx, CAN_FLAG_T flag);
uint8_t CAN_ReadINTStatus(CAN_T* CANx, CAN_INT_T interrupts);
void CAN_ClearINTFlag(CAN_T* CANx, CAN_INT_T flag);



#ifdef __cplusplus
}
#endif

#endif /* __APM32F10x_CAN_H */
