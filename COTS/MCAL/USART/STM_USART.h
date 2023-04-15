/*******************************************************************
*   File name:    STM_USART.h
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs needed to deal with USART peripheral
*                 features in STM32F401
*   Version: v1.0
*******************************************************************/

#ifndef STM_USART_H
#define STM_USART_H

#include "../../LIB/Std_types.h"
#include "../../HAL/ClockHandler/ClockHandler.h"

/* Note:
        * For USART2: CTS on (PA0, PD3), RTS on (PA1, PD4), TX on (PA2, PD5), RX on (PA3, PD6), CK on (PA4, PD7)
        * For USART6: TX on (PC6, PA11), RX on (PC7, PA12), CK on PC8
        * FOR USART1: RX on (PB7, PA10), TX on (PB6, PA9), CTS on (PA11,), RTS on (PA12, ) , CK on (PA8)
*/

#define usartId_1           0x4001100E
#define usartId_2           0x4000440E
#define usartId_6           0x4001140E

#define usartDirection_TX   0xA2
#define usartDirection_RX   0xA3
#define usartDirection_TXRX 0xA4

#define usartMode_Sync      0x2A
#define usartMode_Async     0x3A

#define oversampling_8      0xC2
#define oversampling_16     0xc3

#define dataSize_8          0xD5
#define dataSize_9          0xD6

#define wakeUp_IDLE         0xE1
#define wakeUp_ADD          0xE3

#define stopBit_HALF        0xB0001000
#define stopBit_ONE         0xB0000000
#define stopBit_ONEHALF     0xB0003000
#define stopBit_TWO         0xB0002000

#define polarity_SteadyLow  0x3C
#define polarity_SteadyHigh 0x5C

#define phase_FirstTrans    0x3D
#define phase_secondTrans   0x4D

#define parity_ODD          0x2E
#define parity_EVEN         0x3E
#define parity_NONE         0x4E

typedef void (*usartSendCallBack_t)(void);
typedef void (*usartRecieveCallBack_t) (u8 rxData, u8 errorStatus);
typedef void (*usartRecieveBufferCallBack_t)(void); 

typedef enum
{
    usart_retNotOk = 0,
    usart_retOk,
    usart_retNullPointer,
    usart_retTimeOut,
    usart_retUsartDisabled,
    usart_retTxBusy,
    usart_retRxBusy,
    usart_retInvalidId,
    usart_retInvalidMode,
    usart_retInvalidDirection,
    usart_retInvalidParity,
    usart_retInvalidPolarity,
    usart_retInvalidPhase,
    usart_retInvalidOversampling,
    usart_retInvalidDataSize,
    usart_retInvalidWakeUpMode,
    usart_retInvalidStopBits,
    usart_retParityError,
    usart_retFrameError,
    usart_retDataOverRun,
}USART_ErrorStatus_t;

typedef struct
{
    u8 usartMode;
    u8 usartDirection;
    u8 overSamplingMode;
    u8 parity;
    u32 stopBit;
    u8 dataSize;
    u8 wakeUpMode;
    u8 polarity;
    u8 pahse;
    u16 baud;
}usartConfig_t;

USART_ErrorStatus_t usart_init(u32 usartId, usartConfig_t* usartConfig);
USART_ErrorStatus_t usart_enable(u32 usartId);
USART_ErrorStatus_t usart_disable(u32 usartId);
USART_ErrorStatus_t usart_sendCharSync(u32 usartId, u8 usartChar);
USART_ErrorStatus_t usart_recieveCharSync(u32 usartId, pu8 usartChar);
USART_ErrorStatus_t usart_sendCharAsync(u32 usart, u8 usartChar, usartSendCallBack_t sendCbf);
USART_ErrorStatus_t usart_recieveCharAsync(u32 usartId, usartRecieveCallBack_t recieveCbf);
USART_ErrorStatus_t usart_sendBufferAsyncZeroCopy(u32 usartId, const pu8 buffer, u16 bufferSize, usartSendCallBack_t sendCbf);
USART_ErrorStatus_t usart_recieveBufferAsyncZeroCopy(u32 usartId, pu8 buffer, u16 bufferSize, usartRecieveBufferCallBack_t recieveCbf);

#endif
