/*******************************************************************
*   File name:    STM_USART.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs definitions needed to deal with USART peripheral
*                 features in STM32F401
*   Version: v1.0
*******************************************************************/

#include "STM_USART.h"
#include <math.h>

#define USART_TIME_OUT          600000UL

#define CAST_USART_REG(id)              ((volatile USARTRegs_t* const) id)
#define CALC_USARTDIV(fck, baud, over8) ((fck * 1.0) / (baud * (8 * (2 - over8))))

/* For USART_SR */
#define MSK_CTS                 0x00000200
#define MSK_LBD                 0x00000100
#define MSK_TXE                 0x00000080
#define MSK_TC                  0x00000040
#define MSK_RXNE                0x00000020
#define MSK_IDLE                0x00000010
#define MSK_ORE                 0x00000008
#define MSK_NF                  0x00000004
#define MSK_FE                  0x00000002
#define MSK_PE                  0x00000001

#define MSK_UE                  0x00002000
#define USART_ENABLED           0x00002000
#define USART_DISABLED          0

#define SR_TC                   6

#define CR1_OVER8               15          /* 0: oversampling by 16, 1: oversampling by 8 */
#define CR1_UE                  13
#define CR1_M                   12          /* 0: 8 bits data, 1: 9 bits data */
#define CR1_WAKE                11          /* 0: IDLE line, 1: address mark */
#define CR1_PCE                 10
#define CR1_PS                  9           /* 0: even parity, 1: odd parity*/
#define CR1_PEIE                8
#define CR1_TXIE                7
#define CR1_TCIE                6
#define CR1_RXNEIE              5
#define CR1_IDLEIE              4
#define CR1_TE                  3
#define CR1_RE                  2
#define CR1_RWU                 1           /* 0: reciever in active mode, 1: reciever in mute mode */
#define CR1_SBK                 0           /* 1: send break character */

#define MSK_CLR_STB                 0xFFFFCFFF
#define MSK_CHECK_VALID_STB         0xF0000000
#define MSK_VALID_STB               0xB0000000
#define MSK_CLR_CHECK_VALID_STB     0x0FFFFFFF

#define MSK_CHECK_VALID_DIR         0xF0
#define MSK_VALID_DIR               0xA0

#define MSK_CHECK_VALID_MODE        0x0F
#define MSK_VALID_MODE              0x0A

#define MSK_CHECK_VALID_OVERS       0xF0
#define MSK_VALID_OVERS             0xC0

#define MSK_CHECK_VALID_DATA_SZ     0xF0
#define MSK_VALID_DATA_SZ           0xD0

#define MSK_CHECK_VALID_POL         0x0F
#define MSK_VALID_POL               0x0C

#define MSK_CHECK_VALID_PHA         0x0F
#define MSK_VALID_PHA               0x0D

#define MSK_CHECK_VALID_PARITY      0x0F
#define MSK_VALID_PARITY            0x0E

#define MSK_CHECK_VALID_ID          0x0000000F
#define MSK_VALID_ID                0x0000000E
#define MSK_CLR_CHECK_ID            0xFFFFFFF0

#define MSK_CHECK_VALID_WAKEUP      0xF0
#define MSK_VALID_WAKEUP            0xE0

/* For USART_CR2 */
#define CR2_LIN                     14
#define CR2_CLKEN                   11
#define MSK_CPOL_STEADY_LOW         0xFFFFFBFF
#define MSK_CPOL_STEADY_HIGH        0x00000400
#define MSK_CPHA_FIRST_TRANS        0xFFFFFCFF
#define MSK_CPHA_SECOND_TRANS       0x00000200
#define MSK_LBCL_NOT_OUT            0xFFFFF7FF
#define MSK_LCBL_OUT                0x00000100
#define CR2_LBDIE                   6
#define MSK_LBDL_10                 0xFFFFFFCF
#define MSK_LBDL_11                 0x00000020
#define MSK_CLR_NODE_ADD            0xFFFFFFF0      /* used in case of microprocessor communication */
#define MSK_NODE_ADD                0x0000000F

/* For USART_CR3 */
#define MSK_THREE_BIT_SAMPLE        0xFFFFF7FF
#define MSK_ONE_BIT_SAMPLE          0x00000800
#define CR3_CTSIE                   10
#define CR3_CTSE                    9
#define CR3_RTSE                    8
#define CR3_DMAT                    7
#define CR3_DMAR                    6
#define CR3_SCEN                    5
#define CR3_NACK                    4
#define CR3_HDSEL                   3
#define CR3_IRLP                    2
#define CR3_IREN                    1
#define CR3_EIE                     0

#define MSK_KEEP_RES_BRR            0xFFFF0000
#define MSK_KEEP_RES_DR             0xFFFFFF00

#define MSK_GET_OVER8               0x00008000
#define MSK_OVER8_SHIFT_VAL         15

#define LARGE_DIVFRACTION_VAL       15
#define MANTISSA_SHIFT_VAL          4

#define USART_HANDLERS              3
#define USART1_HANDLER              0
#define USART2_HANDLER              1
#define USART6_HANDLER              2

#define USART1_BASE_ADD             0x40011000
#define USART2_BASE_ADD             0x40004400
#define USART6_BASE_ADD             0x40011400

struct
{
    pu8 buffer;
    u16 size;
    u16 index;
    u8 flag;
}sendBufferReq[USART_HANDLERS], recieveBufferReq[USART_HANDLERS];

typedef struct
{
    u32 USART_SR;
    u32 USART_DR;
    u32 USART_BRR;
    u32 USART_CR1;
    u32 USART_CR2;
    u32 USART_CR3;
    u32 USART_GTPR;
}USARTRegs_t;

extern void USART1_IRQHandler(void);
extern void USART2_IRQHandler(void);
extern void USART6_IRQHandler(void);

static u8 asyncTxFlag[USART_HANDLERS], asyncRxFlag[USART_HANDLERS], sendDmaFlag[USART_HANDLERS], recieveDmaFlag[USART_HANDLERS];
static usartSendCallBack_t asyncTxCharCallback[USART_HANDLERS], sendBufferCallbacks[USART_HANDLERS], sendDmaCallbacks[USART_HANDLERS];
static usartRecieveCallBack_t asyncRxCharCallback[USART_HANDLERS];
static usartRecieveBufferCallBack_t recieveBufferCallbacks[USART_HANDLERS];
static usartRecieveDmaCllBack_t recieveDmaCallbacks[USART_HANDLERS];
static u32 sysClock;

static void getSysClock(u32 clock);

USART_ErrorStatus_t usart_init(u32 usartId, usartConfig_t* usartConfig)
{
    USART_ErrorStatus_t errorStatus = usart_retNotOk;
    if((usartId & MSK_CHECK_VALID_ID) == MSK_VALID_ID)
    {
        if(usartConfig == NULL)
        {
            errorStatus = usart_retNullPointer;
        }
        else
        {
            u32 id = usartId & MSK_CLR_CHECK_ID;
            if((usartConfig->usartMode & MSK_CHECK_VALID_MODE) == MSK_VALID_MODE)
            {
                if(usartConfig->usartMode == usartMode_Sync)
                {
                    CAST_USART_REG(id)->USART_CR2 |= (1 << CR2_CLKEN);
                }
                else
                {
                    CAST_USART_REG(id)->USART_CR2 &= ~(1 << CR2_CLKEN);
                }
                if((usartConfig->usartDirection & MSK_CHECK_VALID_DIR) == MSK_VALID_DIR)
                {
                    if(usartConfig->usartDirection == usartDirection_RX)
                    {
                        CAST_USART_REG(id)->USART_CR1 |= (1 << CR1_RE);
                        CAST_USART_REG(id)->USART_CR1 |= (1 << CR1_RXNEIE);
                    }
                    else if(usartConfig->usartDirection == usartDirection_TX)
                    {
                        CAST_USART_REG(id)->USART_CR1 |= (1 << CR1_TE);
                        CAST_USART_REG(id)->USART_CR1 |= (1 << CR1_TCIE);
                    }
                    else
                    {
                        CAST_USART_REG(id)->USART_CR1 |= (1 << CR1_RE);
                        CAST_USART_REG(id)->USART_CR1 |= (1 << CR1_TE);
                        CAST_USART_REG(id)->USART_CR1 |= (1 << CR1_RXNEIE);
                        CAST_USART_REG(id)->USART_CR1 |= (1 << CR1_TCIE);
                    }
                    if((usartConfig->overSamplingMode & MSK_CHECK_VALID_OVERS) == MSK_VALID_OVERS)
                    {
                        if(usartConfig->overSamplingMode == oversampling_8)
                        {
                            CAST_USART_REG(id)->USART_CR1 |= (1 << CR1_OVER8);
                        }
                        else
                        {
                            CAST_USART_REG(id)->USART_CR1 &= ~(1 << CR1_OVER8);
                        }
                        if((usartConfig->parity & MSK_CHECK_VALID_PARITY) == MSK_VALID_PARITY)
                        {
                            if(usartConfig->parity == parity_NONE)
                            {
                                CAST_USART_REG(id)->USART_CR1 &= ~(1 << CR1_PCE);
                            }
                            else
                            {
                                CAST_USART_REG(id)->USART_CR1 |= (1 << CR1_PCE);
                                if(usartConfig->parity == parity_EVEN)
                                {
                                    CAST_USART_REG(id)->USART_CR1 &= ~(1 << CR1_PS);
                                }
                                else
                                {
                                    CAST_USART_REG(id)->USART_CR1 |= (1 << CR1_PS);
                                }
                            }
                            if((usartConfig->stopBit & MSK_CHECK_VALID_STB) == MSK_VALID_STB)
                            {
                                u32 temp = CAST_USART_REG(id)->USART_CR2;
                                temp &= MSK_CLR_STB & MSK_CLR_CHECK_VALID_STB;
                                temp |= usartConfig->stopBit & MSK_CLR_CHECK_VALID_STB;
                                CAST_USART_REG(id)->USART_CR2 = temp;
                                if((usartConfig->dataSize & MSK_CHECK_VALID_DATA_SZ) == MSK_VALID_DATA_SZ)
                                {
                                    if(usartConfig->dataSize == dataSize_8)
                                    {
                                        CAST_USART_REG(id)->USART_CR1 &= ~(1 << CR1_M);
                                    }
                                    else
                                    {
                                        CAST_USART_REG(id)->USART_CR1 |= (1 << CR1_M);
                                    }
                                    if((usartConfig->wakeUpMode & MSK_CHECK_VALID_WAKEUP) == MSK_VALID_WAKEUP)
                                    {
                                        if(usartConfig->wakeUpMode == wakeUp_IDLE)
                                        {
                                            CAST_USART_REG(id)->USART_CR1 &= ~(1 << CR1_WAKE);
                                        }
                                        else
                                        {
                                            CAST_USART_REG(id)->USART_CR1 |= (1 << CR1_WAKE);
                                        }
                                        if((usartConfig->polarity & MSK_CHECK_VALID_POL) == MSK_VALID_POL)
                                        {
                                            if(usartConfig->polarity == polarity_SteadyLow)
                                            {
                                                CAST_USART_REG(id)->USART_CR2 &= MSK_CPOL_STEADY_LOW;
                                            }
                                            else
                                            {
                                                CAST_USART_REG(id)->USART_CR2 |= MSK_CPOL_STEADY_HIGH;
                                            }
                                            if((usartConfig->pahse & MSK_CHECK_VALID_PHA) == MSK_VALID_PHA)
                                            {
                                                f32 usartDiv, divFraction;
                                                u8 over8;
                                                u16 mantissa;
                                                if(usartConfig->pahse == phase_FirstTrans)
                                                {
                                                    CAST_USART_REG(id)->USART_CR2 &= MSK_CPHA_FIRST_TRANS;
                                                }
                                                else
                                                {
                                                    CAST_USART_REG(id)->USART_CR2 |= MSK_CPHA_SECOND_TRANS;
                                                }
                                                ClockHandler_getClockCallback(getSysClock);
                                                over8 = (CAST_USART_REG(id)->USART_CR1 & MSK_GET_OVER8) >> MSK_OVER8_SHIFT_VAL;
                                                usartDiv = CALC_USARTDIV(sysClock, usartConfig->baud, over8);
                                                mantissa = usartDiv;
                                                divFraction = (f32) ceil(16.0 * (usartDiv - ((u16) usartDiv)));
                                                if(divFraction > LARGE_DIVFRACTION_VAL)
                                                {
                                                    mantissa++;
                                                    divFraction = 0;
                                                }
                                                temp = CAST_USART_REG(id)->USART_BRR;
                                                temp &= MSK_KEEP_RES_BRR;
                                                temp |= (u8) divFraction;
                                                temp |= mantissa << MANTISSA_SHIFT_VAL;
                                                CAST_USART_REG(id)->USART_BRR = temp;
                                                errorStatus = usart_retOk;
                                            }
                                            else
                                            {
                                                errorStatus = usart_retInvalidPhase;
                                            }
                                        }
                                        else
                                        {
                                            errorStatus = usart_retInvalidPolarity;
                                        }
                                    }
                                    else
                                    {
                                        errorStatus = usart_retInvalidWakeUpMode;
                                    }
                                }
                                else
                                {
                                    errorStatus = usart_retInvalidDataSize;
                                }
                            }
                            else
                            {
                                errorStatus = usart_retInvalidStopBits;
                            }
                        }
                        else
                        {
                            errorStatus = usart_retInvalidParity;
                        }
                    }
                    else
                    {
                        errorStatus = usart_retInvalidOversampling;
                    }
                }
                else
                {
                    errorStatus = usart_retInvalidDirection;
                }
            }
            else
            {
                errorStatus = usart_retInvalidMode;
            }
        }
    }
    else
    {
        errorStatus = usart_retInvalidId;
    }
    return errorStatus;
}

USART_ErrorStatus_t usart_sendCharSync(u32 usartId, u8 usartChar)
{
    USART_ErrorStatus_t errorStatus = usart_retNotOk;
    if((usartId & MSK_CHECK_VALID_ID) == MSK_VALID_ID)
    {
        usartId &= MSK_CLR_CHECK_ID;
        if((CAST_USART_REG(usartId)->USART_CR1 & MSK_UE) == USART_ENABLED)
        {
            u32 timeout = USART_TIME_OUT;
            while (!(CAST_USART_REG(usartId)->USART_SR & MSK_TXE) && timeout > 0)
            {
                timeout--;
            }
            if(CAST_USART_REG(usartId)->USART_SR & MSK_TXE)
            {
                CAST_USART_REG(usartId)->USART_DR = usartChar;
                timeout = USART_TIME_OUT;
                while (!(CAST_USART_REG(usartId)->USART_SR & MSK_TC) && timeout > 0)
                {
                    timeout--;
                }
                if(CAST_USART_REG(usartId)->USART_SR & MSK_TC)
                {
                    errorStatus = usart_retOk;
                }
                else
                {
                    errorStatus = usart_retTimeOut;
                }
            }
            else
            {
                errorStatus = usart_retTimeOut;
            }
        }
        else
        {
            errorStatus = usart_retUsartDisabled;
        }
    }
    else
    {
        errorStatus = usart_retInvalidId;
    }
    return errorStatus;
}

USART_ErrorStatus_t usart_recieveCharSync(u32 usartId, pu8 usartChar)
{
    USART_ErrorStatus_t errorStatus = usart_retNotOk;
    if((usartId & MSK_CHECK_VALID_ID) == MSK_VALID_ID)
    {
        usartId &= MSK_CLR_CHECK_ID;
        if((CAST_USART_REG(usartId)->USART_CR1 & MSK_UE) == USART_ENABLED)
        {
            u32 timeout = USART_TIME_OUT;
            while (!(CAST_USART_REG(usartId)->USART_SR & MSK_RXNE) && timeout > 0)
            {
                timeout--;
            }
            if(CAST_USART_REG(usartId)->USART_SR & MSK_RXNE)
            {
                if(CAST_USART_REG(usartId)->USART_SR & MSK_ORE)
                {
                    errorStatus = usart_retDataOverRun;
                }
                else if(CAST_USART_REG(usartId)->USART_SR & MSK_FE)
                {
                    errorStatus = usart_retFrameError;
                }
                else if(CAST_USART_REG(usartId)->USART_SR & MSK_PE)
                {
                    errorStatus = usart_retParityError;
                }
                else
                {
                    *usartChar = (u8) CAST_USART_REG(usartId)->USART_DR;
                    errorStatus = usart_retOk;
                }
            }
            else
            {
                errorStatus = usart_retTimeOut;
            }
        }
        else
        {
            errorStatus = usart_retUsartDisabled;
        }
    }
    else
    {
        errorStatus = usart_retInvalidId;
    }
    return errorStatus;
}

USART_ErrorStatus_t usart_sendCharAsync(u32 usartId, u8 usartChar, usartSendCallBack_t sendCbf)
{
    USART_ErrorStatus_t errorStatus = usart_retNotOk;
    u8 index = 0;
    if((usartId & MSK_CHECK_VALID_ID) == MSK_VALID_ID)
    {
        if(sendCbf == NULL)
        {
            errorStatus = usart_retNullPointer;
        }
        else
        {
            switch(usartId)
            {
                case usartId_1:
                    index = USART1_HANDLER;
                    break;
                case usartId_2:
                    index = USART2_HANDLER;
                    break;
                case usartId_6:
                    index = USART6_HANDLER;
                    break;
            }
            usartId &= MSK_CLR_CHECK_ID;
            if((CAST_USART_REG(usartId)->USART_CR1 & MSK_UE) == USART_ENABLED)
            {
                if(asyncTxFlag[index])
                {
                    errorStatus = usart_retTxBusy;
                }
                else
                {
                    
                    asyncTxFlag[index] = 1;
                    asyncTxCharCallback[index] = sendCbf;
                    CAST_USART_REG(usartId)->USART_DR = usartChar;
                }
            }
            else
            {
                errorStatus = usart_retUsartDisabled;
            }
        }
    }
    else
    {
        errorStatus = usart_retInvalidId;
    }
    return errorStatus;
}

USART_ErrorStatus_t usart_recieveCharAsync(u32 usartId, usartRecieveCallBack_t recieveCbf)
{
    USART_ErrorStatus_t errorStatus = usart_retNotOk;
    u8 index = 0;
    if((usartId & MSK_CHECK_VALID_ID) == MSK_VALID_ID)
    {
        if(recieveCbf == NULL)
        {
            errorStatus = usart_retNullPointer;
        }
        else
        {
            switch(usartId)
            {
                case usartId_1:
                    index = USART1_HANDLER;
                    break;
                case usartId_2:
                    index = USART2_HANDLER;
                    break;
                case usartId_6:
                    index = USART6_HANDLER;
                    break;
            }
            usartId &= MSK_CLR_CHECK_ID;
            if((CAST_USART_REG(usartId)->USART_CR1 & MSK_UE) == USART_ENABLED)
            {
                if(asyncRxFlag[index])
                {
                    errorStatus = usart_retRxBusy;
                }
                else
                {
                    asyncRxFlag[index] = 1;
                    asyncRxCharCallback[index] = recieveCbf;
                }
            }
            else
            {
                errorStatus = usart_retUsartDisabled;
            }
        }
    }
    else
    {
        errorStatus = usart_retInvalidId;
    }
    return errorStatus;
}

USART_ErrorStatus_t usart_sendBufferAsyncZeroCopy(u32 usartId, const pu8 buffer, u16 bufferSize, usartSendCallBack_t sendCbf)
{
    USART_ErrorStatus_t errorStatus = usart_retNotOk;
    u8 index = 0;
    if((usartId & MSK_CHECK_VALID_ID) == MSK_VALID_ID)
    {
        switch(usartId)
        {
            case usartId_1:
                index = USART1_HANDLER;
                break;
            case usartId_2:
                index = USART2_HANDLER;
                break;
            case usartId_6:
                index = USART6_HANDLER;
                break;
        }
        usartId &= MSK_CLR_CHECK_ID;
        if((CAST_USART_REG(usartId)->USART_CR1 & MSK_UE) == USART_ENABLED)
        {
            if(sendBufferReq[index].flag == 0)
            {
                if(sendCbf)
                {
                    sendBufferReq[index].buffer = buffer;
                    sendBufferReq[index].size = bufferSize;
                    sendBufferReq[index].index = 0;
                    CAST_USART_REG(usartId)->USART_DR = sendBufferReq[index].buffer[sendBufferReq[index].index];
                    sendBufferReq[index].index++;
                    sendBufferReq[index].flag = 1;
                    sendBufferCallbacks[index] = sendCbf;
                }
                else
                {
                    errorStatus = usart_retNullPointer;
                }
            }
            else
            {
                errorStatus = usart_retTxBusy;
            }
        }
        else
        {
            errorStatus = usart_retUsartDisabled;
        }
    }
    else
    {
        errorStatus = usart_retInvalidId;
    }
    return errorStatus;
}

USART_ErrorStatus_t usart_recieveBufferAsyncZeroCopy(u32 usartId, pu8 buffer, u16 bufferSize, usartRecieveBufferCallBack_t recieveCbf)
{
    USART_ErrorStatus_t errorStatus = usart_retNotOk;
    u8 index = 0;
    if((usartId & MSK_CHECK_VALID_ID) == MSK_VALID_ID)
    {
        switch(usartId)
        {
            case usartId_1:
                index = USART1_HANDLER;
                break;
            case usartId_2:
                index = USART2_HANDLER;
                break;
            case usartId_6:
                index = USART6_HANDLER;
                break;
        }
        usartId &= MSK_CLR_CHECK_ID;
        if((CAST_USART_REG(usartId)->USART_CR1 & MSK_UE) == USART_ENABLED)
        {
            if(recieveCbf)
            {
                if(recieveBufferReq[index].flag == 0)
                {
                    recieveBufferReq[index].buffer = buffer;
                    recieveBufferReq[index].size = bufferSize;
                    recieveBufferReq[index].index = 0;
                    recieveBufferReq[index].flag = 1;
                    recieveBufferCallbacks[index] = recieveCbf;
                }
                else
                {
                    errorStatus = usart_retRxBusy;
                }
            }
            else
            {
                errorStatus = usart_retNullPointer;
            }
        }
        else
        {
            errorStatus = usart_retUsartDisabled;
        }
    }
    else
    {
        errorStatus = usart_retInvalidId;
    }
    return errorStatus;
}

USART_ErrorStatus_t usart_enable(u32 usartId)
{
    USART_ErrorStatus_t errorStatus = usart_retNotOk;
    if((usartId & MSK_CHECK_VALID_ID) == MSK_VALID_ID)
    {
        usartId &= MSK_CLR_CHECK_ID;
        CAST_USART_REG(usartId)->USART_CR1 |= (1 << CR1_UE);
        CAST_USART_REG(usartId)->USART_SR &= ~(1 << SR_TC);
        errorStatus = usart_retOk;
    }
    else
    {
        errorStatus = usart_retInvalidId;
    }
    return errorStatus;
}

USART_ErrorStatus_t usart_disable(u32 usartId)
{
    USART_ErrorStatus_t errorStatus = usart_retNotOk;
    if((usartId & MSK_CHECK_VALID_ID) == MSK_VALID_ID)
    {
        usartId &= MSK_CLR_CHECK_ID;
        CAST_USART_REG(usartId)->USART_CR1 &= ~(1 << CR1_UE);
        errorStatus = usart_retOk;
    }
    else
    {
        errorStatus = usart_retInvalidId;
    }
    return errorStatus;
}

USART_ErrorStatus_t usart_sendDma(u32 usartId, u8 state)
{
    USART_ErrorStatus_t errorStatus = usart_retNotOk;
    if((usartId & MSK_CHECK_VALID_ID) == MSK_VALID_ID)
    {
        if(state)
        {
            switch(usartId)
            {
                case usartId_1:
                    sendDmaFlag[0] = 1;
                    break;
                case usartId_2:
                    sendDmaFlag[1] = 1;
                    break;
                case usartId_6:
                    sendDmaFlag[2] = 1;
                    break;
            }
            usartId &= MSK_CLR_CHECK_ID;
            CAST_USART_REG(usartId)->USART_CR3 |= (1 << CR3_DMAT);
        }
        else
        {
            switch(usartId)
            {
                case usartId_1:
                    sendDmaFlag[0] = 0;
                    break;
                case usartId_2:
                    sendDmaFlag[1] = 0;
                    break;
                case usartId_6:
                    sendDmaFlag[2] = 0;
                    break;
            }
            usartId &= MSK_CLR_CHECK_ID;
            CAST_USART_REG(usartId)->USART_CR3 &= ~(1 << CR3_DMAT);
        }
        errorStatus = usart_retOk;
    }
    else
    {
        errorStatus = usart_retInvalidId;
    }
    return errorStatus;
}

USART_ErrorStatus_t usart_setSendDmaCallback(u32 usartId, usartSendCallBack_t cbf)
{
    USART_ErrorStatus_t errorStatus = usart_retNotOk;
    if((usartId & MSK_CHECK_VALID_ID) == MSK_VALID_ID)
    {
        if(cbf)
        {
            switch(usartId)
            {
                case usartId_1:
                    sendDmaCallbacks[0] = cbf;
                    break;
                case usartId_2:
                    sendDmaCallbacks[1] = cbf;
                    break;
                case usartId_6:
                    sendDmaCallbacks[2] = cbf;
                    break;
            }
            errorStatus = usart_retOk;
        }
        else
        {
            errorStatus = usart_retNullPointer;
        }
    }
    else
    {
        errorStatus = usart_retInvalidId;
    }
    return errorStatus;
}

USART_ErrorStatus_t usart_recieveDma(u32 usartId, u8 state)
{
    USART_ErrorStatus_t errorStatus = usart_retNotOk;
    if((usartId & MSK_CHECK_VALID_ID) == MSK_VALID_ID)
    {
        if(state)
        {
            switch(usartId)
            {
                case usartId_1:
                    recieveDmaFlag[0] = 1;
                    break;
                case usartId_2:
                    recieveDmaFlag[1] = 1;
                    break;
                case usartId_6:
                    recieveDmaFlag[2] = 1;
                    break;
            }
            usartId &= MSK_CLR_CHECK_ID;
            CAST_USART_REG(usartId)->USART_CR3 |= (1 << CR3_DMAR);
        }
        else
        {
            switch(usartId)
            {
                case usartId_1:
                    recieveDmaFlag[0] = 0;
                    break;
                case usartId_2:
                    recieveDmaFlag[1] = 0;
                    break;
                case usartId_6:
                    recieveDmaFlag[2] = 0;
                    break;
            }
            usartId &= MSK_CLR_CHECK_ID;
            CAST_USART_REG(usartId)->USART_CR3 &= ~(1 << CR3_DMAR);
        }
        errorStatus = usart_retOk;
    }
    else
    {
        errorStatus = usart_retInvalidId;
    }
    return errorStatus;
}

USART_ErrorStatus_t usart_setRecieveDmaCallback(u32 usartId, usartRecieveDmaCllBack_t cbf)
{
    USART_ErrorStatus_t errorStatus = usart_retNotOk;
    if((usartId & MSK_CHECK_VALID_ID) == MSK_VALID_ID)
    {
        if(cbf)
        {
            switch(usartId)
            {
                case usartId_1:
                    recieveDmaCallbacks[0] = cbf;
                    break;
                case usartId_2:
                    recieveDmaCallbacks[1] = cbf;
                    break;
                case usartId_6:
                    recieveDmaCallbacks[2] = cbf;
                    break;
            }
            errorStatus = usart_retOk;
        }
        else
        {
            errorStatus = usart_retNullPointer;
        }
    }
    else
    {
        errorStatus = usart_retInvalidId;
    }
    return errorStatus;
}

static void getSysClock(u32 clock)
{
    sysClock = clock;
}

void USART1_IRQHandler(void)
{
    if(CAST_USART_REG(USART1_BASE_ADD)->USART_SR & MSK_TC)
    {
        if(sendDmaFlag[USART1_HANDLER])
        {
            if(sendDmaCallbacks[USART1_HANDLER])
            {
                sendDmaCallbacks[USART1_HANDLER]();
            }
        }
        if(asyncTxFlag[USART1_HANDLER])
        {
            if(asyncTxCharCallback[USART1_HANDLER])
            {
                asyncTxFlag[USART1_HANDLER] = 0;
                CAST_USART_REG(USART1_BASE_ADD)->USART_SR &= ~(1 << SR_TC);
                asyncTxCharCallback[USART1_HANDLER]();
            }
        }
        if(sendBufferReq[USART1_HANDLER].flag)
        {
            if(sendBufferReq[USART1_HANDLER].index == sendBufferReq[USART1_HANDLER].size)
            {
                sendBufferReq[USART1_HANDLER].flag = 0;
                sendBufferReq[USART1_HANDLER].buffer = NULL;
                sendBufferReq[USART1_HANDLER].size = 0;
                if(sendBufferCallbacks[USART1_HANDLER])
                {
                    sendBufferCallbacks[USART1_HANDLER]();
                }
            }
            else
            {
                CAST_USART_REG(USART1_BASE_ADD)->USART_DR = sendBufferReq[USART1_HANDLER].buffer[sendBufferReq[USART1_HANDLER].index];
                sendBufferReq[USART1_HANDLER].index++;
            }
        }
    }
    if(CAST_USART_REG(USART1_BASE_ADD)->USART_SR & MSK_RXNE)
    {
        if(recieveDmaFlag[USART1_HANDLER])
        {
            if(recieveDmaCallbacks[USART1_HANDLER])
            {
                recieveDmaCallbacks[USART1_HANDLER]();
            }
        }
        if(asyncRxFlag[USART1_HANDLER])
        {
            if(asyncRxCharCallback[USART1_HANDLER])
            {
                USART_ErrorStatus_t errorStatus;
                u8 rxData = 0;
                if(CAST_USART_REG(USART1_BASE_ADD)->USART_SR & MSK_ORE)
                {
                    errorStatus = usart_retDataOverRun;
                }
                else if(CAST_USART_REG(USART1_BASE_ADD)->USART_SR & MSK_FE)
                {
                    errorStatus = usart_retFrameError;
                }
                else if(CAST_USART_REG(USART1_BASE_ADD)->USART_SR & MSK_PE)
                {
                    errorStatus = usart_retParityError;
                }
                else
                {
                    rxData = (u8) CAST_USART_REG(USART1_BASE_ADD)->USART_DR;
                    errorStatus = usart_retOk;
                }
                asyncRxFlag[USART1_HANDLER] = 0;
                asyncRxCharCallback[USART1_HANDLER](rxData, errorStatus);
            }
        }
        if(recieveBufferReq[USART1_HANDLER].flag)
        {
            recieveBufferReq[USART1_HANDLER].buffer[recieveBufferReq[USART1_HANDLER].index] = CAST_USART_REG(USART1_BASE_ADD)->USART_DR;
            recieveBufferReq[USART1_HANDLER].index++;
            if(recieveBufferReq[USART1_HANDLER].index == recieveBufferReq[USART1_HANDLER].size)
            {
                recieveBufferReq[USART1_HANDLER].buffer = NULL;
                recieveBufferReq[USART1_HANDLER].size = 0;
                recieveBufferReq[USART1_HANDLER].flag = 0;
                if(recieveBufferCallbacks[USART1_HANDLER])
                {
                    recieveBufferCallbacks[USART1_HANDLER]();
                }
            }
        }
    }
}

void USART2_IRQHandler(void)
{
    if(CAST_USART_REG(USART2_BASE_ADD)->USART_SR & MSK_TC)
    {
        if(sendDmaFlag[USART2_HANDLER])
        {
            if(sendDmaCallbacks[USART2_HANDLER])
            {
                sendDmaCallbacks[USART2_HANDLER]();
            }
        }
        if(asyncTxFlag[USART2_HANDLER])
        {
            if(asyncTxCharCallback[USART2_HANDLER])
            {
                asyncTxFlag[USART2_HANDLER] = 0;
                asyncTxCharCallback[USART2_HANDLER]();
            }
        }
        if(sendBufferReq[USART2_HANDLER].flag)
        {
            if(sendBufferReq[USART2_HANDLER].index == sendBufferReq[USART2_HANDLER].size)
            {
                sendBufferReq[USART2_HANDLER].flag = 0;
                sendBufferReq[USART2_HANDLER].buffer = NULL;
                sendBufferReq[USART2_HANDLER].size = 0;
                if(sendBufferCallbacks[USART2_HANDLER])
                {
                    sendBufferCallbacks[USART2_HANDLER]();
                }
            }
            else
            {
                CAST_USART_REG(USART2_BASE_ADD)->USART_DR = sendBufferReq[USART2_HANDLER].buffer[sendBufferReq[USART2_HANDLER].index];
                sendBufferReq[USART2_HANDLER].index++;
            }
        }
    }
    if(CAST_USART_REG(USART2_BASE_ADD)->USART_SR & MSK_RXNE)
    {
        if(recieveDmaFlag[USART2_HANDLER])
        {
            if(recieveDmaCallbacks[USART2_HANDLER])
            {
                recieveDmaCallbacks[USART2_HANDLER]();
            }
        }
        if(asyncRxFlag[USART2_HANDLER])
        {
            if(asyncRxCharCallback[USART2_HANDLER])
            {
                USART_ErrorStatus_t errorStatus;
                u8 rxData = 0;
                if(CAST_USART_REG(USART2_BASE_ADD)->USART_SR & MSK_ORE)
                {
                    errorStatus = usart_retDataOverRun;
                }
                else if(CAST_USART_REG(USART2_BASE_ADD)->USART_SR & MSK_FE)
                {
                    errorStatus = usart_retFrameError;
                }
                else if(CAST_USART_REG(USART2_BASE_ADD)->USART_SR & MSK_PE)
                {
                    errorStatus = usart_retParityError;
                }
                else
                {
                    rxData = (u8) CAST_USART_REG(USART2_BASE_ADD)->USART_DR;
                    errorStatus = usart_retOk;
                }
                asyncRxFlag[USART2_HANDLER] = 0;
                asyncRxCharCallback[USART2_HANDLER](rxData, errorStatus);
            }
        }
        if(recieveBufferReq[USART2_HANDLER].flag)
        {
            recieveBufferReq[USART2_HANDLER].buffer[recieveBufferReq[USART2_HANDLER].index] = CAST_USART_REG(USART2_BASE_ADD)->USART_DR;
            recieveBufferReq[USART2_HANDLER].index++;
            if(recieveBufferReq[USART2_HANDLER].index == recieveBufferReq[USART2_HANDLER].size)
            {
                recieveBufferReq[USART2_HANDLER].buffer = NULL;
                recieveBufferReq[USART2_HANDLER].size = 0;
                recieveBufferReq[USART2_HANDLER].flag = 0;
                if(recieveBufferCallbacks[USART2_HANDLER])
                {
                    recieveBufferCallbacks[USART2_HANDLER]();
                }
            }
        }
    }
}

void USART6_IRQHandler(void)
{
    if(CAST_USART_REG(USART6_BASE_ADD)->USART_SR & MSK_TC)
    {
        if(sendDmaFlag[USART6_HANDLER])
        {
            if(sendDmaCallbacks[USART6_HANDLER])
            {
                sendDmaCallbacks[USART6_HANDLER]();
            }
        }
        if(asyncTxFlag[USART6_HANDLER])
        {
            if(asyncTxCharCallback[USART6_HANDLER])
            {
                asyncTxFlag[USART6_HANDLER] = 0;
                asyncTxCharCallback[USART6_HANDLER]();
            }
        }
        if(sendBufferReq[USART6_HANDLER].flag)
        {
            if(sendBufferReq[USART6_HANDLER].index == sendBufferReq[USART6_HANDLER].size)
            {
                sendBufferReq[USART6_HANDLER].flag = 0;
                sendBufferReq[USART6_HANDLER].buffer = NULL;
                sendBufferReq[USART6_HANDLER].size = 0;
                if(sendBufferCallbacks[USART6_HANDLER])
                {
                    sendBufferCallbacks[USART6_HANDLER]();
                }
            }
            else
            {
                CAST_USART_REG(USART6_BASE_ADD)->USART_DR = sendBufferReq[USART6_HANDLER].buffer[sendBufferReq[USART6_HANDLER].index];
                sendBufferReq[USART6_HANDLER].index++;
            }
        }
    }
    if(CAST_USART_REG(USART6_BASE_ADD)->USART_SR & MSK_RXNE)
    {
        if(recieveDmaFlag[USART6_HANDLER])
        {
            if(recieveDmaCallbacks[USART6_HANDLER])
            {
                recieveDmaCallbacks[USART6_HANDLER]();
            }
        }
        if(asyncRxFlag[USART6_HANDLER])
        {
            if(asyncRxCharCallback[USART6_HANDLER])
            {
                USART_ErrorStatus_t errorStatus;
                u8 rxData = 0;
                if(CAST_USART_REG(USART6_BASE_ADD)->USART_SR & MSK_ORE)
                {
                    errorStatus = usart_retDataOverRun;
                }
                else if(CAST_USART_REG(USART6_BASE_ADD)->USART_SR & MSK_FE)
                {
                    errorStatus = usart_retFrameError;
                }
                else if(CAST_USART_REG(USART6_BASE_ADD)->USART_SR & MSK_PE)
                {
                    errorStatus = usart_retParityError;
                }
                else
                {
                    rxData = (u8) CAST_USART_REG(USART6_BASE_ADD)->USART_DR;
                    errorStatus = usart_retOk;
                }
                asyncRxFlag[USART6_HANDLER] = 0;
                asyncRxCharCallback[USART6_HANDLER](rxData, errorStatus);
            }
        }
        if(recieveBufferReq[USART6_HANDLER].flag)
        {
            recieveBufferReq[USART6_HANDLER].buffer[recieveBufferReq[USART6_HANDLER].index] = CAST_USART_REG(USART6_BASE_ADD)->USART_DR;
            recieveBufferReq[USART6_HANDLER].index++;
            if(recieveBufferReq[USART6_HANDLER].index == recieveBufferReq[USART6_HANDLER].size)
            {
                recieveBufferReq[USART6_HANDLER].buffer = NULL;
                recieveBufferReq[USART6_HANDLER].size = 0;
                recieveBufferReq[USART6_HANDLER].flag = 0;
                if(recieveBufferCallbacks[USART6_HANDLER])
                {
                    recieveBufferCallbacks[USART6_HANDLER]();
                }
            }
        }
    }
}
