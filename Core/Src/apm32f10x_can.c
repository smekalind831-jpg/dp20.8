/*!
 * @file       apm32f10x_can.c
 *
 * @brief      This file provides all the CAN firmware functions
 *
 * @version    V1.0.0
 *
 * @date       2019-8-6
 *
 */

#include "CAN/apm32f10x_can.h"
#include "CAN/apm32f10x_RCM.h"

/*!
 * @brief     Reset CAN registers
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @retval    None
 *
 * @note
 */
void CAN_Reset(CAN_T* CANx)
{
    if(CANx == CAN1)
    {
        RCM_EnableAPB1PeriphReset(RCM_APB1_PERIPH_CAN1);
        RCM_DisableAPB1PeriphReset(RCM_APB1_PERIPH_CAN1);
    } else
    {
        RCM_EnableAPB1PeriphReset(RCM_APB1_PERIPH_CAN2);
        RCM_DisableAPB1PeriphReset(RCM_APB1_PERIPH_CAN2);
    }
}

/*!
 * @brief     Initialization parameter configuration
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     CAN_Config:Configure parameter structures
 *
 * @retval    ERROR or SUCCEESS
 *
 * @note
 */
uint8_t CAN_Config(CAN_T* CANx, CAN_Config_T* CAN_Config)
{
    uint32_t wait_ack = 0x00000000;

    /** Exit from sleep mode */
    CANx->MCTRL_B.SLEEP = BIT_RESET;
    /** Request initialisation */
    CANx->MCTRL_B.INIT = BIT_SET;

    /** Wait the acknowledge */
    while(((CANx->MSTS_B.INITF) != BIT_SET) && (wait_ack != 0x0000FFFF))
    {
        wait_ack++;
    }
    /** Check acknowledge */
    if(((CANx->MSTS_B.INITF) != BIT_SET))
    {
        return ERROR;
    }
    else
    {
        if(CAN_Config->CAN_TTCM == ENABLE)
        {
            CANx->MCTRL_B.TTCM = BIT_SET;
        }
        else
        {
            CANx->MCTRL_B.TTCM = BIT_RESET;
        }

        if(CAN_Config->CAN_ABOM == ENABLE)
        {
            CANx->MCTRL_B.ABOM = BIT_SET;
        }
        else
        {
            CANx->MCTRL_B.ABOM = BIT_RESET;
        }

        if(CAN_Config->CAN_AWUPM == ENABLE)
        {
            CANx->MCTRL_B.AWUPM = BIT_SET;
        }
        else
        {
            CANx->MCTRL_B.AWUPM = BIT_RESET;
        }

        if(CAN_Config->CAN_ARTDIS == ENABLE)
        {
            CANx->MCTRL_B.ARTDIS = BIT_SET;
        }
        else
        {
            CANx->MCTRL_B.ARTDIS = BIT_RESET;
        }

        if(CAN_Config->CAN_RFLOCK == ENABLE)
        {
            CANx->MCTRL_B.RFLOCK = BIT_SET;
        }
        else
        {
            CANx->MCTRL_B.RFLOCK = BIT_RESET;
        }

        if(CAN_Config->CAN_TX_PS == ENABLE)
        {
            CANx->MCTRL_B.TX_PS = BIT_SET;
        }
        else
        {
            CANx->MCTRL_B.TX_PS = BIT_RESET;
        }

        /** Set the bit timing register */
        CANx->BT &= (uint32_t)0x3fffffff;
        CANx->BT |= (uint32_t)CAN_Config->CAN_MODE << 30;
        CANx->BT_B.RSJW = CAN_Config->CAN_RSJW;
        CANx->BT_B.TS1 = CAN_Config->CAN_BS1;
        CANx->BT_B.TS2 = CAN_Config->CAN_BS2;
        CANx->BT_B.BRD = CAN_Config->CAN_PRESCALER - 1;

        /** Request leave initialisation */
        CANx->MCTRL_B.INIT = BIT_RESET;

        wait_ack = 0;
        /** Wait the acknowledge */
        while(((CANx->MSTS_B.INITF) != BIT_SET) && (wait_ack != 0x0000FFFF))
        {
            wait_ack++;
        }
        /** Check acknowledge */
        if(((CANx->MSTS_B.INITF) != BIT_SET))
        {
            return ERROR;
        }
    }
    return SUCCESS;
}

/*!
 * @brief     Congig the CAN peripheral according to the specified parameters in the CAN_FilterStruct.
 *
 * @param     CAN_FilterStruct :config the filterstruct
 *
 * @retval    None
 *
 * @note
 */
void CAN_FilterConfig(CAN_FILTER_CONFIG_T* CAN_FilterStruct)
{
    CAN1->FMCTRL_B.FINIT = BIT_SET;

    CAN1->FEN &= ~(1 << CAN_FilterStruct->CAN_FILTER_NUMBER);

    /** Filter Scale */
    if(CAN_FilterStruct->CAN_FILTER_SCALE == CAN_FILTER_SCALE_16BIT)
    {
        /** 16-bit scale for the filter */
        CAN1->FBW &= ~(1 << CAN_FilterStruct->CAN_FILTER_NUMBER);

        CAN1->sFilterRegister[CAN_FilterStruct->CAN_FILTER_NUMBER].F1 =
            ((0x0000FFFF & CAN_FilterStruct->CAN_FILTER_MASK_IDLOW) << 16) |
            (0x0000FFFF & CAN_FilterStruct->CAN_FILTER_IDLOW);

        CAN1->sFilterRegister[CAN_FilterStruct->CAN_FILTER_NUMBER].F2 =
            ((0x0000FFFF & CAN_FilterStruct->CAN_FILTER_MASK_IDHIGH) << 16) |
            (0x0000FFFF & CAN_FilterStruct->CAN_FILTER_IDHIGH);
    }

    if(CAN_FilterStruct->CAN_FILTER_SCALE == CAN_FILTER_SCALE_32BIT)
    {
        CAN1->FBW |= (1 << CAN_FilterStruct->CAN_FILTER_NUMBER);

        CAN1->sFilterRegister[CAN_FilterStruct->CAN_FILTER_NUMBER].F1 =
            ((0x0000FFFF & CAN_FilterStruct->CAN_FILTER_IDHIGH) << 16) |
            (0x0000FFFF & CAN_FilterStruct->CAN_FILTER_IDLOW);

        CAN1->sFilterRegister[CAN_FilterStruct->CAN_FILTER_NUMBER].F2 =
            ((0x0000FFFF & CAN_FilterStruct->CAN_FILTER_MASK_IDHIGH) << 16) |
            (0x0000FFFF & CAN_FilterStruct->CAN_FILTER_MASK_IDLOW);
    }

    /** Filter Mode */
    if(CAN_FilterStruct->CAN_FILTER_MODE == CAN_FILTER_MODE_IDMASK)
    {
        CAN1->FM &= ~(1 << CAN_FilterStruct->CAN_FILTER_NUMBER);
    }
    else
    {
        CAN1->FM |= (1 << CAN_FilterStruct->CAN_FILTER_NUMBER);
    }

    /** Filter FIFO assignment */
    if(CAN_FilterStruct->CAN_FILTER_FIFO_ASSIGNMENT == CAN_FILTER_FIFO0)
    {
        CAN1->FFA &= ~(1 << CAN_FilterStruct->CAN_FILTER_NUMBER);
    }
    if(CAN_FilterStruct->CAN_FILTER_FIFO_ASSIGNMENT == CAN_FILTER_FIFO1)
    {
        CAN1->FFA |= (1 << CAN_FilterStruct->CAN_FILTER_NUMBER);
    }

    /** Filter activation */
    if(CAN_FilterStruct->CAN_FILTER_ACTIVATION == ENABLE)
    {
        CAN1->FEN |= (1 << CAN_FilterStruct->CAN_FILTER_NUMBER);
    }
    CAN1->FMCTRL_B.FINIT = BIT_RESET;
}

/*!
 * @brief     Initialize CAN with an initial value
 *
 * @param     CAN_Config :Configure parameter structures
 *
 * @retval    None
 *
 * @note
 */
void CAN_StructInit(CAN_Config_T* CAN_Config)
{
    CAN_Config->CAN_TTCM = DISABLE;
    CAN_Config->CAN_ABOM = DISABLE;
    CAN_Config->CAN_AWUPM = DISABLE;
    CAN_Config->CAN_ARTDIS = DISABLE;
    CAN_Config->CAN_RFLOCK = DISABLE;
    CAN_Config->CAN_TX_PS = DISABLE;
    CAN_Config->CAN_MODE = CAN_MODE_NORMAL;
    CAN_Config->CAN_RSJW = CAN_RSJW_1tq;
    CAN_Config->CAN_BS1 = CAN_BS1_4tq;
    CAN_Config->CAN_BS2 = CAN_BS2_3tq;
    CAN_Config->CAN_PRESCALER = 1;
}

/*!
 * @brief     Select the start bank filter for slave CAN.
 *
 * @param     CAN_BankNumber:Select the start slave bank filter from 1..27.
 *
 * @retval    None
 *
 * @note      
 */
void CAN_SlaveStartBank(uint8_t CAN_BankNumber)
{
    CAN1->FMCTRL_B.FINIT = BIT_SET;

    CAN1->FMCTRL &= 0xFFFFC0F1;
    CAN1->FMCTRL |= (uint32_t)(CAN_BankNumber) << 8;

    CAN1->FMCTRL_B.FINIT = BIT_RESET;
}

/*!
 * @brief     Enables the DBG Freeze for CAN.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @retval    None
 *
 * @note
 */
void CAN_EnableDBGFreeze(CAN_T* CANx)
{
    CANx->MCTRL_B.DBF = ENABLE;
}

/*!
 * @brief     Disable the DBG Freeze for CAN.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @retval    None
 *
 * @note
 */
void CAN_DisableDBGFreeze(CAN_T* CANx)
{
    CANx->MCTRL_B.DBF = DISABLE;
}

/*!
 * @brief     Enables the CAN Time TriggerOperation communication mode.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @retval    None
 *
 * @note
 */
void CAN_EnableTTCComMode(CAN_T* CANx)
{
    CANx->MCTRL_B.TTCM = ENABLE;

    CANx->sTxMailBox[0].TXDLT_B.TXTS = BIT_SET;
    CANx->sTxMailBox[1].TXDLT_B.TXTS = BIT_SET;
    CANx->sTxMailBox[2].TXDLT_B.TXTS = BIT_SET;
}

/*!
 * @brief     Disable the CAN Time TriggerOperation communication mode.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @retval    None
 *
 * @note
 */
void CAN_DisableTTCComMode(CAN_T* CANx)
{
    CANx->MCTRL_B.TTCM = DISABLE;

    CANx->sTxMailBox[0].TXDLT_B.TXTS = BIT_RESET;
    CANx->sTxMailBox[1].TXDLT_B.TXTS = BIT_RESET;
    CANx->sTxMailBox[2].TXDLT_B.TXTS = BIT_RESET;
}

/*!
 * @brief     Initiates the transmission of a message.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     TxMessage:pointer to a structure which contains CAN Id, CAN  DLC and CAN data.
 *
 * @retval    The number of the mailbox that is used for transmission or 0 is No mailbox is empty
 *
 * @note
 */
uint8_t CAN_Transmit(CAN_T* CANx, CAN_TX_MESSAGE* TxMessage)
{
    uint8_t transmit_milbox = 0;
	
    /** Select one empty transmit mailbox */
    if((CANx->TXSTS & 0x04000000) == 0x04000000)
    {
        transmit_milbox = 0;
    }
    else if((CANx->TXSTS & 0x08000000) == 0x08000000)
    {
        transmit_milbox = 1;
    }
    else if((CANx->TXSTS & 0x10000000) == 0x10000000)
    {
        transmit_milbox = 2;
    } else
    {
        return CAN_TxStatus_NoMailBox;  ///!< No mailbox is empty
    }

    /** Set up the Id */
    CANx->sTxMailBox[transmit_milbox].TXI &= 0x00000001;
    if(TxMessage->IDE == CAN_ID_STANDARD)
    {
        CANx->sTxMailBox[transmit_milbox].TXI |= (TxMessage->STDID << 21) | (TxMessage->RTR);
    } else
    {
        CANx->sTxMailBox[transmit_milbox].TXI |= (TxMessage->EXTID << 3) | (TxMessage->IDE) | (TxMessage->RTR);
    }

    /** Set up the DLC */
    TxMessage->DLC &= 0x0F;
    CANx->sTxMailBox[transmit_milbox].TXDLT &= (uint32_t)0xFFFFFFF0;
    CANx->sTxMailBox[transmit_milbox].TXDLT |= TxMessage->DLC;

    /** Set up the data field */
    CANx->sTxMailBox[transmit_milbox].TXDL = ((uint32_t)TxMessage->DATA[3] << 24) | ((uint32_t)TxMessage->DATA[2] << 16)
            | ((uint32_t)TxMessage->DATA[1] << 8) | ((uint32_t)TxMessage->DATA[0]);
    CANx->sTxMailBox[transmit_milbox].TXDH = ((uint32_t)TxMessage->DATA[7] << 24) | ((uint32_t)TxMessage->DATA[6] << 16)
            | ((uint32_t)TxMessage->DATA[5] << 8) | ((uint32_t)TxMessage->DATA[4]);
    /** Request transmission */
    CANx->sTxMailBox[transmit_milbox].TXI |= 0x00000001;
    
    return transmit_milbox;
}

/*!
 * @brief     Checks the transmission of a message.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     transmitMailbox: the number of the mailbox
 *
 * @retval    state: 0:CAN_TxStatus_Failed ;1: CAN_TxStatus_Ok
 *
 * @note
 */
uint8_t CAN_TransmitStatus(CAN_T* CANx, CAN_TX_MAILBIX_T transmitMailbox)
{
    uint32_t state = 0;

    switch (transmitMailbox)
    {
        case (CAN_TX_MAILBIX_1): 
            state =   CANx->TXSTS &  (0x00000001 | 0x00000002 | 0x04000000);
        break;
        case (CAN_TX_MAILBIX_2): 
            state =   CANx->TXSTS &  (0x00000100 | 0x00000200 | 0x08000000);
        break;
        case (CAN_TX_MAILBIX_3): 
            state =   CANx->TXSTS &  (0x00010000 | 0x00020000 | 0x10000000);
        break;
        default:
            state = 0;
        break;
    }
    switch (state)
    {
        /* transmit pending  */
        case (0x0): state = 2;
        break;
        /* transmit failed  */
        case (0x00000001 | 0x04000000): state = 0;
        break;
        case (0x00000100 | 0x08000000): state = 0;
        break;
        case (0x00010000 | 0x10000000): state = 0;
        break;
        /* transmit succeeded  */
        case (0x00000001 | 0x00000002 | 0x04000000):state = 1;
        break;
        case (0x00000100 | 0x00000200 | 0x08000000):state = 1;
        break;
        case (0x00010000 | 0x00020000 | 0x10000000):state = 1;
        break;
        default: state = 0;
        break;
    }
    return (uint8_t) state;    
}

/*!
 * @brief     Cancels a transmit request.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     mailBox: the number of the mailbox
 *
 * @retval    None
 *
 * @note
 */
void CAN_CancelTransmit(CAN_T* CANx, CAN_TX_MAILBIX_T mailBox)
{
    switch (mailBox)
    {
    case CAN_TX_MAILBIX_1:
        CANx->TXSTS_B.ATRM1 = BIT_SET;
        break;
    case CAN_TX_MAILBIX_2:
        CANx->TXSTS_B.ATRM2 = BIT_SET;
        break;
    case CAN_TX_MAILBIX_3:
        CANx->TXSTS_B.ATRM3 = BIT_SET;
        break;
    default:
        break;
    }
}

/*!
 * @brief     Receives a message
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     FIFONumber:    Receive FIFO number
 *
 * @param     RxMessage:    pointer to a structure receive message
 *
 * @retval    None
 *
 * @note
 */
void CAN_Receive(CAN_T* CANx, CAN_RX_FIFO_T FIFONumber, CAN_RX_MESSAGE_T* RxMessage)
{
    /* Get the Id */
    RxMessage->IDE = ((uint8_t)0x04 & (CANx->sRxMailBox[FIFONumber].RXI));
    if(RxMessage->IDE == CAN_ID_STANDARD)
    {
        RxMessage->STDID = (CANx->sRxMailBox[FIFONumber].RXI >> 21) & 0x000007FF;
    }
    else
    {
        RxMessage->EXTID = (CANx->sRxMailBox[FIFONumber].RXI >> 3) & 0x1FFFFFFF;
    }

    RxMessage->RTR = CANx->sRxMailBox[FIFONumber].RXI_B.RTR;
    RxMessage->DLC = CANx->sRxMailBox[FIFONumber].RXDLT_B.RXDL;
    RxMessage->FMI = CANx->sRxMailBox[FIFONumber].RXDLT_B.FMI;
    /** Get the data field */
    RxMessage->DATA[0] = CANx->sRxMailBox[FIFONumber].RXDL_B.DATA1;
    RxMessage->DATA[1] = CANx->sRxMailBox[FIFONumber].RXDL_B.DATA2;
    RxMessage->DATA[2] = CANx->sRxMailBox[FIFONumber].RXDL_B.DATA3;
    RxMessage->DATA[3] = CANx->sRxMailBox[FIFONumber].RXDL_B.DATA4;
    RxMessage->DATA[4] = CANx->sRxMailBox[FIFONumber].RXDH_B.DATA5;
    RxMessage->DATA[5] = CANx->sRxMailBox[FIFONumber].RXDH_B.DATA6;
    RxMessage->DATA[6] = CANx->sRxMailBox[FIFONumber].RXDH_B.DATA7;
    RxMessage->DATA[7] = CANx->sRxMailBox[FIFONumber].RXDH_B.DATA8;

    if(FIFONumber == CAN_RX_FIFO1)
    {
        CANx->RF1_B.RFOM1 = BIT_SET;
    }
    else
    {
        CANx->RF2_B.RFOM2 = BIT_SET;
    }
}

/*!
 * @brief     Releases the specified FIFO.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     FIFONumber:    Receive FIFO number
 *
 * @retval    None
 *
 * @note
 */
void CAN_FIFORelease(CAN_T* CANx, CAN_RX_FIFO_T FIFONumber)
{
    if(FIFONumber == CAN_RX_FIFO1)
    {
        CANx->RF1_B.RFOM1 = BIT_SET;
    }
    else
    {
        CANx->RF2_B.RFOM2 = BIT_SET;
    }
}

/*!
 * @brief     Returns the number of pending messages.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     FIFONumber:    Receive FIFO number
 *
 * @retval    which is the number of pending message.
 *
 * @note
 */
uint8_t CAN_MessagePending(CAN_T* CANx, CAN_RX_FIFO_T FIFONumber)
{
    if(FIFONumber == CAN_RX_FIFO1)
    {
        return  CANx->RF1 & 0x03;
    }
    else
    {
        return  CANx->RF2 & 0x03;
    }
}

/*!
 * @brief     Select the CAN Operation mode
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     CAN_OperatingMode: CAN Operating Mode
 *
 * @retval    modeState:status of the requested mode
 *            0:CAN failed entering the specific mode
 *            1:CAN Succeed entering the specific mode
 * @note
 */
uint8_t CAN_OperatingModeRequest(CAN_T* CANx, CAN_OPERATING_MODE_T CAN_OperatingMode)
{
    uint8_t states = 0;
    uint32_t time_out = 0x0000FFFF;
	
    if(CAN_OperatingMode == CAN_OPERATINGMODE_INITIALIZATION)
    {
        /** Request initialisation */
        CANx->MCTRL_B.SLEEP = BIT_RESET;
        CANx->MCTRL_B.INIT = BIT_SET;

        /* Wait the acknowledge */
        while((CANx->MSTS_B.INITF != BIT_SET && CANx->MSTS_B.SLAK != BIT_RESET) && (time_out != 0))
        {
            time_out --;
        }
        if((CANx->MSTS_B.INITF == BIT_SET && CANx->MSTS_B.SLAK == BIT_RESET))
        {
            states = 1;
        }
    }
    else if(CAN_OperatingMode == CAN_OPERATINGMODE_NORMAL)
    {
        /** Request leave initialisation and sleep mode  and enter Normal mode */
        CANx->MCTRL_B.SLEEP = BIT_RESET;
        CANx->MCTRL_B.INIT = BIT_RESET;

        time_out = 0x0000FFFF;

        while((CANx->MSTS_B.INITF != BIT_RESET || CANx->MSTS_B.SLAK != BIT_RESET) && (time_out != 0))
        {
            time_out --;
        }
        if((CANx->MSTS_B.INITF != BIT_RESET || CANx->MSTS_B.SLAK != BIT_RESET))
        {
            states = 1;
        }
    }
    else if(CAN_OperatingMode == CAN_OPERATINGMODE_SLEEP)
    {
        /** Request Sleep mode */
        CANx->MCTRL_B.SLEEP = BIT_SET;
        CANx->MCTRL_B.INIT = BIT_RESET;

        time_out = 0x0000FFFF;

        while((CANx->MSTS_B.INITF != BIT_RESET && CANx->MSTS_B.SLAK != BIT_SET) && (time_out != 0))
        {
            time_out --;
        }
        if((CANx->MSTS_B.INITF != BIT_RESET && CANx->MSTS_B.SLAK != BIT_SET))
        {
            states = 1;
        }
    }
    return states ;
}

/*!
 * @brief     Into the low power mode.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @retval    status: 0: Into sleep fail, 1: Into sleep success
 *
 * @note
 */
uint8_t CAN_Sleep(CAN_T* CANx)
{
    /** Request Sleep mode */
    CANx->MCTRL_B.SLEEP = BIT_SET;
    CANx->MCTRL_B.INIT = BIT_RESET;

    if((CANx->MSTS_B.INITF != BIT_RESET && CANx->MSTS_B.SLAK != BIT_SET))
    {
        return 1;
    }
    return 0;
}

/*!
 * @brief     Wakes the CAN up.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @retval    status: 0: WakeUp CAN fail, 1: WakeUp CAN success
 *
 * @note
 */
uint8_t CAN_WakeUp(CAN_T* CANx)
{
    uint32_t time_out = 0x0000FFFF;
	
    /** Wake up request */
    CANx->MCTRL_B.SLEEP = BIT_RESET;
    while((CANx->MSTS_B.SLAK != BIT_SET) && (time_out != 0))
    {
        time_out --;
    }
    if(CANx->MSTS_B.SLAK == BIT_SET)
    {
        return 1;
    }
    return 0;
}

/*!
 * @brief     Read the CANx's last error code (LEC)
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @retval    CAN_ErrorCode
 *
 * @note
 */
uint8_t CAN_ReadLastErrorCode(CAN_T* CANx)
{
    return CANx->ESTS_B.LEC;
}

/*!
 * @brief     Read the CANx Receive Error Counter(RXEC)
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @retval    CAN Receive Error Counter.
 *
 * @note      In case of an error during reception, this counter is incremented
 *            by 1 or by 8 depending on the error condition as defined by the CAN
 *            standard. After every successful reception, the counter is
 *            decremented by 1 or reset to 120 if its value was higher than 128.
 *            When the counter value exceeds 127, the CAN controller enters the
 *            error passive state.
 */
uint8_t ReadReceiveErrorCounter(CAN_T* CANx)
{
    return CANx->ESTS_B.RXEC;
}

/*!
 * @brief     Read the LSB of the 9-bit CANx Transmit Error Counter(TXEC).
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @retval    LSB of the 9-bit CAN Transmit Error Counter.
 *
 * @note
 */
uint8_t CAN_ReadLSBTransmitErrorCounter(CAN_T* CANx)
{
    return CANx->ESTS_B.TXEC;
}

/*!
 * @brief     Enables the specified CANx interrupts.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     interrupts: specifies the CAN interrupt sources
 *
 * @retval    None
 *
 * @note
 */
void CAN_EnableInterrupt(CAN_T* CANx, CAN_INT_T interrupts)
{
    CANx->IEN |= interrupts;
}

/*!
 * @brief     Disable the specified CANx interrupts.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     interrupts: specifies the CAN interrupt sources
 *
 * @retval    None
 *
 * @note
 */
void CAN_DisableInterrupt(CAN_T* CANx, CAN_INT_T interrupts)
{
    CANx->IEN &= ~interrupts;
}

/*!
 * @brief     Read whether the specified CAN flag is set or not.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     flag: CAN_FLAG_T
 *
 * @retval    flag staus:  RESET or SET
 *
 * @note
 */
uint8_t CAN_ReadFlagStatus(CAN_T* CANx, CAN_FLAG_T flag)
{
    uint8_t status = 0;
	
    if((flag & 0x00F00000) != RESET )
    {
        if((CANx->ESTS & (flag & 0x000FFFFF)) != RESET)
        {
            status = SET;
        }
        else
        {
            status = RESET;
        }
    }
    else if((flag & 0x01000000) != RESET )
    {
        if((CANx->MSTS & (flag & 0x000FFFFF)) != RESET )
        {
            status = SET;
        }
        else
        {
            status = RESET ;
        }
    }
    else if((flag & 0x08000000) != RESET )
    {
        if((CANx->TXSTS & (flag & 0x000FFFFF)) != RESET )
        {
            status = SET;
        }
        else
        {
            status = RESET;
        }
    }
    else if((flag & 0x02000000) != RESET )
    {
        if((CANx->RF1 & (flag & 0x000FFFFF)) != RESET )
        {
            status = SET;
        }
        else
        {
            status = RESET;
        }
    }
    else
    {
        if((CANx->RF2 & (flag & 0x000FFFFF)) != RESET)
        {
            status = SET;
        }
        else
        {
            status = RESET;
        }
    }
    return status;
}

/*!
 * @brief     Clears the CAN's pending flags.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     flag: CAN_FLAG_T
 *
 * @retval    None
 *
 * @note
 */
void CAN_ClearFlag(CAN_T* CANx, CAN_FLAG_T flag)
{
    uint32_t flagtmp = 0;
	
    /** ESTS register */
    if(flag == 0x30F00070)
    {
        CANx->ESTS = RESET;
    }
    else
    {
        flagtmp = flag & 0x000FFFFF;
        if((flag & 0x02000000) != RESET)
        {
            CANx->RF1 = flagtmp;
        }
        else if((flag & 0x04000000) != RESET)
        {
            CANx->RF2 = flagtmp;
        }
        else if((flag & 0x08000000) != RESET)
        {
            CANx->TXSTS = flagtmp;
        }
        else
        {
            CANx->MSTS = flagtmp;
        }
    }
}

/*!
 * @brief     Read whether the specified CANx interrupt has occurred or not.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     interrupts: specifies the CAN interrupt sources
 *
 * @retval    status : SET or RESET
 *
 * @note
 */
uint8_t CAN_ReadINTStatus(CAN_T* CANx, CAN_INT_T interrupts)
{
    uint8_t status = 0;
	
    if((CANx->IEN & interrupts) != RESET)
    {
        switch (interrupts)
        {
        case CAN_INT_EORM:
            status = CANx->TXSTS_B.EORM1 | CANx->TXSTS_B.EORM2 | CANx->TXSTS_B.EORM3;
            break;
        case CAN_INT_FMNP1:
            status = CANx->RF1_B.FMNP1;
            break;
        case CAN_INT_FFI1:
            status = CANx->RF1_B.FULL1;
            break;
        case CAN_INT_FOFI1:
            status = CANx->RF1_B.FOF1;
            break;
        case CAN_INT_FMNP2:
            status = CANx->RF2_B.FMNP2;
            break;
        case CAN_INT_FFI2:
            status = CANx->RF2_B.FULL2;
            break;
        case CAN_INT_FOFI2:
            status = CANx->RF2_B.FOF2;
            break;
        case CAN_INT_WUPI:
            status = CANx->MSTS_B.WUPIF;
            break;
        case CAN_INT_SLPI:
            status = CANx->MSTS_B.SLPIF;
            break;
        case CAN_INT_EWGI:
            status = CANx->ESTS_B.EWF;
            break;
        case CAN_INT_EPVI:
            status = CANx->ESTS_B.EPF;
            break;
        case CAN_INT_BOFI:
            status = CANx->ESTS_B.BOF;
            break;
        case CAN_INT_LECI:
            status = CANx->ESTS_B.LEC;
            break;
        case CAN_INT_ERRI:
            status = CANx->MSTS_B.ERRIF;
            break;
        default:
            status = RESET;
            break;
        }
    }
    else
    {
        status = RESET;
    }
    return status;
}

/*!
 * @brief     Clears the CANx's interrupt flag.
 *
 * @param     CANx:CAN(1~2) to select.
 *
 * @param     flag:  interrupt pending bit to clear
 *
 * @retval    None
 *
 * @note
 */
void CAN_ClearINTFlag(CAN_T* CANx, CAN_INT_T flag)
{
    switch (flag)
    {
    case CAN_INT_EORM:
        CANx->TXSTS_B.EORM1 = BIT_RESET;
        CANx->TXSTS_B.EORM2 = BIT_RESET;
        CANx->TXSTS_B.EORM3 = BIT_RESET;
        break;
    case CAN_INT_FFI1:
        CANx->RF1_B.FULL1 = BIT_RESET;
        break;
    case CAN_INT_FOFI1:
        CANx->RF1_B.FOF1 = BIT_RESET;
        break;
    case CAN_INT_FFI2:
        CANx->RF2_B.FULL2 = BIT_RESET;
        break;
    case CAN_INT_FOFI2:
        CANx->RF2_B.FOF2 = BIT_RESET;
        break;
    case CAN_INT_WUPI:
        CANx->MSTS_B.WUPIF = BIT_RESET;
        break;
    case CAN_INT_SLPI:
        CANx->MSTS_B.SLPIF = BIT_RESET;
        break;
    case CAN_INT_EWGI:
        /** Note : the corresponding Flag is cleared by hardware depending
                    of the CAN Bus status*/
        CANx->ESTS = 0x0004;
        break;
    case CAN_INT_EPVI:
        /** Note : the corresponding Flag is cleared by hardware depending
                   of the CAN Bus status*/
        CANx->ESTS = 0x0004;
        break;
    case CAN_INT_BOFI:
        /** Note : the corresponding Flag is cleared by hardware depending
                   of the CAN Bus status*/
        CANx->ESTS = 0x0004;
        break;
    case CAN_INT_LECI:
        CANx->ESTS_B.LEC = BIT_RESET;
        break;
    case CAN_INT_ERRI:
        CANx->MSTS_B.ERRIF = BIT_RESET;
        break;
    default:
        break;
    }
}
