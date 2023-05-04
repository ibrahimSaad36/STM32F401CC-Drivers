/*******************************************************************************************
*   File name:    STM_DMA.h
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs needed to deal with DMA peripheral
*                 features in STM32F401
*   Version: v1.0
********************************************************************************************/


#ifndef STM_DMA_H
#define STM_DMA_H

#include "../../LIB/Std_types.h"

/*
    To Consider:
        For DMA1:
            For stream0:                            For stream1:
                Channel0 >> SPI3_RX                     Channel0 >> Not Connected
                Channel1 >> I2C1_RX                     Channel1 >> I2C3_RX
                Channel2 >> TIM4_CH1                    Channel2 >> Not Connected
                Channel3 >> I2S3_EXT_RX                 Channel3 >> TIM2_UP, TIM2_CH3
                Channel4, Channel5 >> Not Connected     Channel4, Channel5 >> Not Connected
                Channel6 >> TIM5_CH3, TIM5_UP           Channel6 >> TIM5_CH4, TIM5_TRIG
                Channel7 >> Not Connected               Channel7 >> Not Connected

            For stream2:                            For stream3:
                Channel0 >> SPI3_RX                     Channel0 >> SPI2_RX
                Channel1 >> Not Connected               Channel1 >> Not Connected
                Channel2 >> I2S3_EXT_RX                 Channel2 >> TIM4_CH2
                Channel3 >> I2C3_RX                     Channel3 >> I2S2_EXT_RX
                Channel4 >> Not Connected               Channel4, Channel5 >> Not Connected
                Channel5 >> TIM3_CH4, TIM3_UP     
                Channel6 >> TIM5_CH1                    Channel6 >> TIM5_CH4, TIM5_TRIG
                Channel7 >> I2C2_RX                     Channel7 >> I2C2_RX

            For stream4:                            For stream5:
                Channel0 >> SPI2_TX                     Channel0 >> SPI3_TX
                Channel1 >> Not Connected               Channel1 >> I2C1_RX
                Channel2 >> I2S2_EXT_TX                 Channel2 >> I2S3_EXT_TX
                Channel3 >> I2C3_TX                     Channel3 >> TIM2_CH1
                Channel4 >> Not Connected               Channel4 >> USART2_RX
                                                        Channel5 >> TIM3_CH2
                Channel5 >> TIM3_CH1, TIM3_TRIG     
                Channel6 >> TIM5_CH2                    Channel6 >> I2C3_TX
                Channel7 >> Not Connected               Channel7 >> Not Connected

            For stream6:                            For stream7:
                Channel0 >> Not Connected               Channel0 >> SPI3_TX
                Channel1 >> I2C1_TX                     Channel1 >> I2C1_TX
                Channel2 >> TIM4_UP                     Channel2 >> TIM4_CH3
                Channel3 >> TIM2_CH2, TIM2_CH4          Channel3 >> TIM2_UP, TIM2_CH4
                Channel4 >> USART2_TX                   Channel4 >> Not Connected
                                                        Channel5 >> TIM3_CH3
                Channel5 >> Not Connected     
                Channel6 >> TIM5_UP                     Channel6 >> Not Connected
                Channel7 >> Not Connected               Channel7 >> I2C2_TX

        For DMA2:
            For stream0:                            For stream1:
                Channel0 >> ADC1                        Channel0 >> Not Connected
                Channel1 >> Not Connected               Channel1 >> Not Connected
                Channel2 >> Not Connected               Channel2 >> Not Connected
                Channel3 >> SPI1_RX                     Channel3 >> Not Connected
                Channel4 >> SPI4_RX                     Channel4 >> SPI4_TX
                Channel5 >> Not Connected               Channel5 >> USART6_RX
                Channel6 >> TIM1_TRIG                   Channel6 >> TIM1_CH1
                Channel7 >> Not Connected               Channel7 >> Not Connected

            For stream2:                            For stream3:
                Channel0 >> Not Connected               Channel0 >> Not Connected
                Channel1 >> Not Connected               Channel1 >> Not Connected
                Channel2 >> Not Connected               Channel2 >> Not Connected
                Channel3 >> SPI1_RX                     Channel3 >> SPI1_TX
                Channel4 >> USART1_RX                   Channel4 >> SDIO
                Channel5 >> USART6_RX                   Channel5 >> SPI4_RX
                Channel6 >> TIM1_CH2                    Channel6 >> TIM1_CH1
                Channel7 >> Not Connected               Channel7 >> Not Connected

            For stream4:                            For stream5:
                Channel0 >> ADC1                            Channel0 >> Not Connected
                Channel1 >> Not Connected                   Channel1 >> Not Connected
                Channel2 >> Not Connected                   Channel2 >> Not Connected
                Channel3 >> Not Connected                   Channel3 >> SPI1_TX
                Channel4 >> Not Connected                   Channel4 >> USART1_RX
                                                            Channel5 >> Not Connected
                Channel5 >> SPI4_TX     
                Channel6 >> TIM1_CH4, TIM1_TRIG, TIM1_COM   Channel6 >> TIM1_UP
                Channel7 >> Not Connected                   Channel7 >> Not Connected

            For stream6:                                    For stream7:
                Channel0 >> TIM1_CH1, TIM1_CH2, TIM1_CH3        Channel0 >> Not Connected
                Channel1 >> Not Connected                       Channel1 >> Not Connected
                Channel2 >> Not Connected                       Channel2 >> Not Connected
                Channel3 >> Not Connected                       Channel3 >> Not Connected
                Channel4 >>  SDIO                               Channel4 >> USART1_TX
                                                                Channel5 >> USART6_TX
                Channel5 >> USART6_TX     
                Channel6 >> TIM1_CH3                            Channel6 >> Not Connected
                Channel7 >> Not Connected                       Channel7 >> Not Connected
*/

#define dmaId_1                             0x400260EC
#define dmaId_2                             0x400264EC

#define streamId_0                          0xEF10
#define streamId_1                          0xEF28
#define streamId_2                          0xEF40
#define streamId_3                          0xEF58
#define streamId_4                          0xEF70
#define streamId_5                          0xEF88
#define streamId_6                          0xEFA0
#define streamId_7                          0xEFB8

#define channelId_0                         0xD0
#define channelId_1                         0xD1
#define channelId_2                         0xD2
#define channelId_3                         0xD3
#define channelId_4                         0xD4
#define channelId_5                         0xD5
#define channelId_6                         0xD6
#define channelId_7                         0xD7

#define streamPriority_Low                  0xC0
#define streamPriority_Medium               0xC1
#define streamPriority_High                 0xC2
#define streamPriority_VeryHigh             0xC3

#define fifoLevel_Half                      0xCC01
#define fifoLevel_OneQuarter                0xCC00
#define fifoLevel_ThirdQuarter              0xCC02
#define fifoLevel_Full                      0xCC03

#define peripheralIncMode_Fixed             0x1D
#define peripheralIncMode_IncBySize         0x2D
#define peripheralIncMode_Aligned           0x3D

#define memoryIncMode_Fixed                 0x4D
#define memoryIncMode_IncBySize             0x5D

#define bufferMode_Regular                  0x0A
#define bufferMode_Double                   0x1A
#define bufferMode_Circular                 0x2A

#define flowControl_DMA                     0x4C
#define flowControl_Peripheral              0x5C

#define memoryBurstMode_Single              0xA0
#define memoryBurstMode_Inc4                0xA1
#define memoryBurstMode_Inc8                0xA2
#define memoryBurstMode_Inc16               0xA3

#define peripheralBurstMode_Single          0xB0
#define peripheralBurstMode_Inc4            0xB1
#define peripheralBurstMode_Inc8            0xB2
#define peripheralBurstMode_Inc16           0xB3

#define memorySize_Byte                     0xE0
#define memorySize_HalfWord                 0xE1
#define memorySize_Word                     0xE2

#define peripheralSize_Byte                 0xE0
#define peripheralSize_HalfWord             0xE1
#define peripheralSize_Word                 0xE2

#define streamDirection_PriToMem            0xF0
#define streamDirection_MemToPri            0xF1
#define streamDirection_MemToMem            0xF2

typedef void (*dmaCallBack_t)(void);
typedef void (*dmaErrorCallBack_t)(u8 errorStatus);

typedef enum
{
    dma_retNotOk = 0,
    dma_retOk,
    dma_retInvalidDMAId,
    dma_retInvalidStreamId,
    dma_retInvalidChannelId,
    dma_retStreamIsDisabled,
    dma_retNullPointer,
    dma_retTransferError,
    dma_retFIFOError,
    dma_retDirectModeError,
    dma_retInvalidConfig,
    dma_retConfigWhileEnabledStream,
    dma_retInvalidBufferMode,
    dma_retInvalidDataItems,
    dma_retInvalidFlowControlOption,
    dma_retInvalidStreamPriority,
    dma_retInvalidFifoLevel,
    dma_retInvalidStreamDirection,
    dma_retInvalidIncrementMode,
    dma_retInvalidDataSize,
    dma_retInvalidPeripheralBurstMode,
    dma_retInvalidMemoryBurstMode,
}DMA_ErrorStatus_t;

typedef struct 
{
    u32* pripheralAddress;
    u32* memory0Address;
    u32* memory1Address;
    u16 streamId;
    u16 fifoLevel;
    u16 dataItems;
    u8 channelId;
    u8 streamPriority;
    u8 streamDirection;
    u8 flowControl;
    u8 memorySize;
    u8 peripheralSize;
    u8 peripheralIncMode;
    u8 memoryIncMode;
    u8 memoryBurstMode;
    u8 peripheralBurstMode;
    u8 bufferMode;
}streamCfg_t;

DMA_ErrorStatus_t dma_streamInit(u32 dmaId, const streamCfg_t* streamCfg);
DMA_ErrorStatus_t dma_enableStream(u32 dmaId, u16 streamId);
DMA_ErrorStatus_t dma_disableStream(u32 dmaId, u16 streamId);
DMA_ErrorStatus_t dma_setBufferMode(u32 dmaId, u16 streamId, u8 bufferMode);
DMA_ErrorStatus_t dma_setDataItems(u32 dmaId, u16 streamId, u16 dataItems);
DMA_ErrorStatus_t dma_setStreamPriority(u32 dmaId, u16 streamId, u8 streamPriority);
DMA_ErrorStatus_t dma_setStreamDirection(u32 dmaId, u16 streamId, u8 streamDirection);
DMA_ErrorStatus_t dma_setFlowControl(u32 dmaId, u16 streamId, u8 flowControl);
DMA_ErrorStatus_t dma_setChannelId(u32 dmaId, u16 streamId, u8 channelId);
DMA_ErrorStatus_t dma_setPeripheralSize(u32 dmaId, u16 streamId, u8 peripheralSize);
DMA_ErrorStatus_t dma_setMemorySize(u32 dmaId, u16 streamId, u8 memorySize);
DMA_ErrorStatus_t dma_setPeripheralAddress(u32 dmaId, u16 streamId, u32* peripheralAddress);
DMA_ErrorStatus_t dma_setMemory0Address(u32 dmaId, u16 streamId, u32* memory0Address);
DMA_ErrorStatus_t dma_setMemory1Address(u32 dmaId, u16 streamId, u32* memory1Address);
DMA_ErrorStatus_t dma_getRemainingDataItems(u32 dmaId, u16 streamId, pu16 dataItems);
DMA_ErrorStatus_t dma_enableHalfTransferInterrupt(u32 dmaId, u16 streamId);
DMA_ErrorStatus_t dma_disableHalfTransferInterrupt(u32 dmaId, u16 streamId);
DMA_ErrorStatus_t dma_enableTransferInterrupt(u32 dmaId, u16 streamId);
DMA_ErrorStatus_t dma_disableTransferInterrupt(u32 dmaId, u16 streamId);
DMA_ErrorStatus_t dma_enableErrorsInterrupt(u32 dmaId, u16 streamId);
DMA_ErrorStatus_t dma_disableErrorsInterrupt(u32 dmaId, u16 streamId);
DMA_ErrorStatus_t dma_registerHalfCompleteCallback(u32 dmaId, u16 streamId, dmaCallBack_t cbf);
DMA_ErrorStatus_t dma_registerTransferCompleteCallback(u32 dmaId, u16 streamId, dmaCallBack_t cbf);
DMA_ErrorStatus_t dma_registerErrorsCallback(u32 dmaId, u16 streamId, dmaErrorCallBack_t cbf);

#endif