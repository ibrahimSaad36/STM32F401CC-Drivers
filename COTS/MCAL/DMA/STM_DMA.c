/*******************************************************************************************
*   File name:    STM_DMA.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs definitions to deal with DMA peripheral
*                 features in STM32F401
*   Version: v1.0
********************************************************************************************/

#include "STM_DMA.h"

#define CAST_DMA_REGS(dmaId)            ((volatile DMARegs_t* const)    dmaId)
#define CAST_STREAM_REGS(streamId)      ((volatile StreamRegs_t* const) streamId)

#define DMA_COUNTS                      2
#define TOT_STREAM_COUNTS               8
#define DMA1_IDX                        0
#define DMA2_IDX                        1
#define DMA1_BASE_ADDRESS               0x40026000
#define DMA2_BASE_ADDRESS               0x40026400

#define STREAM0_BASE_ADD                0x40026010
#define STREAM1_BASE_ADD                0x40026028
#define STREAM2_BASE_ADD                0x40026040
#define STREAM3_BASE_ADD                0x40026058
#define STREAM4_BASE_ADD                0x40026070
#define STREAM5_BASE_ADD                0x40026088
#define STREAM6_BASE_ADD                0x400260A0
#define STREAM7_BASE_ADD                0x400260B8

#define MIN_VALID_DATA_ITEMS            1
#define MAX_VALID_DATA_ITEMS            0xFFFF

#define MSK_CHECK_VALID_DMA_ID          0x000000FF
#define MSK_VALID_DMA_ID                0x000000EC
#define MSK_CLR_CHECK_VALID_DMA_ID      0xFFFFFF00

#define MSK_CHECK_VALID_STREAM          0xFF00
#define MSK_VALID_STREAM                0xEF00
#define MSK_CLR_CHECK_VALID_STREAM      0x00FF

#define MSK_CHECK_VALID_CHANNEL         0xF0
#define MSK_VALID_CHANNEL               0xD0
#define MSK_CLR_CHECK_VALID_CHANNEL     0x0F

#define MSK_CHECK_VALID_MEM_BURST       0xF0
#define MSK_VALID_MEM_BURST             0xA0
#define MSK_CLR_CHECK_VALID_MEM_BURST   0x0F

#define MSK_CHECK_VALID_PRI_BURST       0xF0
#define MSK_VALID_PRI_BURST             0xB0
#define MSK_CLR_CHECK_VALID_PRI_BURST   0x0F

#define MSK_CHECK_VALID_PRIO            0xF0
#define MSK_VALID_PRIO                  0xC0
#define MSK_CLR_CHECK_VALID_PRIO        0x0F

#define MSK_CHECK_VALID_BUFF_MODE       0x0F
#define MSK_VALID_BUFF_MODE             0x0A

#define MSK_CHECK_VALID_INC_MODE        0x0F
#define MSK_VALID_INC_MODE              0x0D

#define MSK_CHECK_VALID_DSIZE           0xF0
#define MSK_VALID_DSIZE                 0xE0
#define MSK_CLR_CHECK_VALID_DSIZE       0x0F

#define MSK_CHECK_VALID_STREAM_DIR      0xF0
#define MSK_VALID_STREAM_DIR            0xF0
#define MSK_CLR_CHECK_VALID_DIR         0x0F

#define MSK_CHECK_VALID_FLOW_CTRL       0x0F
#define MSK_VALID_FLOW_CTRL             0x0C

#define MSK_CHECK_VALID_FIFO_LVL        0xFF00
#define MSK_VALID_FIFO_LVL              0xCC00
#define MSK_CLR_CHECK_VALID_FIFO_LVL    0x00FF

#define SxCR_CHSEL_SHIFT                25
#define SxCR_MBURST_SHIFT               23
#define SxCR_PBURST_SHIFT               21
#define SxCR_CT                         19  /* current target (0: mem0, 1: mem1) */
#define SxCR_DBM                        18
#define SxCR_PRIO_SHIFT                 16
#define SxCR_PINCOS                     15
#define SxCR_MSIZE_SHIFT                13
#define SxCR_PSIZE_SHIFT                11
#define SxCR_MINC                       10
#define SxCR_PINC                       9
#define SxCR_CIRC                       8
#define SxCR_DIR_SHIFT                  6     
#define SxCR_PFCTRL                     5   /* 0 DMA is the flow controller, 1: the peripheral */
#define SxCR_TCIE                       4
#define SxCR_HTIE                       3
#define SxCR_TEIE                       2
#define SxCR_DMEIE                      1            
#define SxCR_EN                         0

#define SxFCR_FEIE                      7
#define SxFCR_DMDIS                     2
/* will be used to read flags, clear them also (cleared by writing one) */
/* use lower register for (0, 1, 2, 3) and higher register for (7, 6, 5, 4) */
#define MSK_TCIF37                      0x08000000
#define MSK_TCIF26                      0x00200000
#define MSK_TCIF15                      0x00000800
#define MSK_TCIF04                      0x00000020

#define MSK_HTIF37                      0x04000000
#define MSK_HTIF26                      0x00100000
#define MSK_HTIF15                      0x00000400
#define MSK_HTIF04                      0x00000010

#define MSK_TEIF37                      0x02000000
#define MSK_TEIF26                      0x00080000
#define MSK_TEIF15                      0x00000200
#define MSK_TEIF04                      0x00000008

#define MSK_DMEIF37                     0x01000000
#define MSK_DMEIF26                     0x00040000
#define MSK_DMEIF15                     0x00000100
#define MSK_DMEIF04                     0x00000004

#define MSK_FEIF37                      0x00400000
#define MSK_FEIF26                      0x00010000
#define MSK_FEIF15                      0x00000040
#define MSK_FEIF04                      0x00000001

#define SxFCR_FEIE                      7
#define SxFCR_DMDIS                     2
#define MSK_SxFCR_FS                    0x00000038

/* For SxCR */
#define MSK_CLR_CHSEL                   0xF1FFFFFF
#define MSK_CLR_MBURST                  0xFE7FFFFF
#define MSK_CLR_PBURST                  0xFF9FFFFF
#define MSK_CLR_PL                      0xFFFCFFFF
#define MSK_CLR_MSIZE                   0xFFFF9FFF
#define MSK_CLR_PSIZE                   0xFFFFE7FF
#define MSK_CLR_DIR                     0xFFFFFF3F

/* For SxFCR */
#define MSK_CLR_FTH                     0xFFFFFFFC

#define MSK_CHECK_STRAM_STATE           0x00000001
#define MSK_STRAM_ENABLED               1
#define MSK_STREAM_DISABLED             0

typedef struct
{
    u32 DMA_SxCR;
    u32 DMA_SxNDTR;
    u32 DMA_SxPAR;
    u32 DMA_SxM0AR;
    u32 DMA_SxM1AR;
    u32 DMA_SxFCR;
}StreamRegs_t;

typedef struct
{
    u32 DMA_LISR;
    u32 DMA_HISR;
    u32 DMA_LIFCR;
    u32 DMA_HIFCR;
}DMARegs_t;

extern void DMA1_Stream0_IRQHandler(void);
extern void DMA1_Stream1_IRQHandler(void);
extern void DMA1_Stream2_IRQHandler(void);
extern void DMA1_Stream3_IRQHandler(void);
extern void DMA1_Stream4_IRQHandler(void);
extern void DMA1_Stream5_IRQHandler(void);
extern void DMA1_Stream6_IRQHandler(void);
extern void DMA1_Stream7_IRQHandler(void);

extern void DMA2_Stream0_IRQHandler(void);
extern void DMA2_Stream1_IRQHandler(void);
extern void DMA2_Stream2_IRQHandler(void);
extern void DMA2_Stream3_IRQHandler(void);
extern void DMA2_Stream4_IRQHandler(void);
extern void DMA2_Stream5_IRQHandler(void);
extern void DMA2_Stream6_IRQHandler(void);
extern void DMA2_Stream7_IRQHandler(void);

static dmaCallBack_t dma1HCCallBacks [TOT_STREAM_COUNTS] = {NULL};
static dmaCallBack_t dma2HCCallBacks [TOT_STREAM_COUNTS] = {NULL};
static dmaCallBack_t dma1TCCallBacks [TOT_STREAM_COUNTS] = {NULL};
static dmaCallBack_t dma2TCCallBacks [TOT_STREAM_COUNTS] = {NULL};
static dmaErrorCallBack_t dma1ErrorCallBacks [TOT_STREAM_COUNTS] = {NULL};
static dmaErrorCallBack_t dma2ErrorCallBacks [TOT_STREAM_COUNTS] = {NULL};

static void dmaHandler(u32 dmaBaseAdd, u8 streamIndex);
static DMA_ErrorStatus_t checkValidData(u32 dmaId, u16 streamId);
static DMA_ErrorStatus_t checkValidDataAndCanConfig(u32 dmaId, u16 streamId);

DMA_ErrorStatus_t dma_streamInit(u32 dmaId, const streamCfg_t* streamCfg)
{
    DMA_ErrorStatus_t errorStatus = checkValidDataAndCanConfig(dmaId, streamCfg->streamId);
    if(errorStatus == dma_retOk)
    {
        u16 stream = streamCfg->streamId;
        dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
        stream &= MSK_CLR_CHECK_VALID_STREAM;
        if(streamCfg->pripheralAddress)
        {
            CAST_STREAM_REGS(dmaId + stream)->DMA_SxPAR = streamCfg->pripheralAddress;
            if(streamCfg->memory0Address)
            {
                CAST_STREAM_REGS(dmaId + stream)->DMA_SxM0AR = streamCfg->memory0Address;
                if((streamCfg->bufferMode & MSK_CHECK_VALID_BUFF_MODE) == MSK_VALID_BUFF_MODE)
                {
                    switch(streamCfg->bufferMode)
                    {
                        case bufferMode_Regular:
                            CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR &= ~(1 << SxCR_DBM);
                            CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR &= ~(1 << SxCR_CIRC);
                            CAST_STREAM_REGS(dmaId + stream)->DMA_SxFCR |= (1 << SxFCR_DMDIS);
                            break;
                        case bufferMode_Double:
                            if(streamCfg->memory1Address)
                            {
                                CAST_STREAM_REGS(dmaId + stream)->DMA_SxM1AR = streamCfg->memory1Address;
                                CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (1 << SxCR_DBM);
                                CAST_STREAM_REGS(dmaId + stream)->DMA_SxFCR &= ~(1 << SxFCR_DMDIS);
                            }
                            else
                            {
                                errorStatus = dma_retNullPointer;
                            }
                            break;
                        case bufferMode_Circular:
                            CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (1 << SxCR_CIRC);
                            CAST_STREAM_REGS(dmaId + stream)->DMA_SxFCR &= ~(1 << SxFCR_DMDIS);
                            break;
                    }
                    if(errorStatus != dma_retNullPointer)
                    {
                        if(streamCfg->dataItems >= MIN_VALID_DATA_ITEMS && streamCfg->dataItems <= MAX_VALID_DATA_ITEMS)
                        {
                            CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR = streamCfg->dataItems;
                            if((streamCfg->channelId & MSK_CHECK_VALID_CHANNEL) == MSK_VALID_CHANNEL)
                            {
                                CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR &= MSK_CLR_CHSEL;
                                CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (streamCfg->channelId & MSK_CLR_CHECK_VALID_CHANNEL) << SxCR_CHSEL_SHIFT;
                                if((streamCfg->flowControl & MSK_CHECK_VALID_FLOW_CTRL) == MSK_VALID_FLOW_CTRL)
                                {
                                    if(streamCfg->flowControl == flowControl_DMA)
                                    {
                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR &= ~(1 << SxCR_PFCTRL);
                                    }
                                    else
                                    {
                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (1 << SxCR_PFCTRL);
                                    }
                                    if((streamCfg->streamPriority & MSK_CHECK_VALID_PRIO) == MSK_VALID_PRIO)
                                    {
                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR &= MSK_CLR_PL;
                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (streamCfg->streamPriority & MSK_CLR_CHECK_VALID_PRIO) << SxCR_PRIO_SHIFT;
                                        if((streamCfg->fifoLevel & MSK_CHECK_VALID_FIFO_LVL) == MSK_VALID_FIFO_LVL)
                                        {
                                            CAST_STREAM_REGS(dmaId + stream)->DMA_SxFCR &= MSK_CLR_FTH;
                                            CAST_STREAM_REGS(dmaId + stream)->DMA_SxFCR |= (streamCfg->fifoLevel & MSK_CLR_CHECK_VALID_FIFO_LVL);
                                            if((streamCfg->streamDirection & MSK_CHECK_VALID_STREAM_DIR) == MSK_VALID_STREAM_DIR)
                                            {
                                                CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR &= MSK_CLR_DIR;
                                                CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (streamCfg->streamDirection & MSK_CLR_CHECK_VALID_DIR) << SxCR_DIR_SHIFT;
                                                if((streamCfg->peripheralIncMode & MSK_CHECK_VALID_INC_MODE) == MSK_VALID_INC_MODE
                                                    && (streamCfg->memoryIncMode & MSK_CHECK_VALID_INC_MODE) == MSK_VALID_INC_MODE)
                                                {
                                                    if(streamCfg->peripheralIncMode == peripheralIncMode_Fixed)
                                                    {
                                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR &= ~(1 << SxCR_PINC);
                                                    }
                                                    else if(streamCfg->peripheralIncMode == peripheralIncMode_IncBySize)
                                                    {
                                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (1 << SxCR_PINC);
                                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR &= ~(1 << SxCR_PINCOS);
                                                    }
                                                    else
                                                    {
                                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (1 << SxCR_PINC);
                                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (1 << SxCR_PINCOS);
                                                    }

                                                    if(streamCfg->memoryIncMode == memoryIncMode_Fixed)
                                                    {
                                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR &= ~(1 << SxCR_MINC);
                                                    }
                                                    else
                                                    {
                                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (1 << SxCR_MINC);
                                                    }
                                                    if((streamCfg->peripheralSize & MSK_CHECK_VALID_DSIZE) == MSK_VALID_DSIZE
                                                        && (streamCfg->memorySize & MSK_CHECK_VALID_DSIZE) == MSK_VALID_DSIZE)
                                                    {
                                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR &= MSK_CLR_MSIZE;
                                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (streamCfg->memorySize & MSK_CLR_CHECK_VALID_DSIZE) << SxCR_MSIZE_SHIFT;
                                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR &= MSK_CLR_PSIZE;
                                                        CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (streamCfg->peripheralSize & MSK_CLR_CHECK_VALID_DSIZE) << SxCR_PSIZE_SHIFT;
                                                        if((streamCfg->peripheralBurstMode & MSK_CHECK_VALID_PRI_BURST) == MSK_VALID_PRI_BURST)
                                                        {
                                                            CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR &= MSK_CLR_PBURST;
                                                            CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (streamCfg->peripheralBurstMode & MSK_CLR_CHECK_VALID_PRI_BURST) << SxCR_PBURST_SHIFT;
                                                            if((streamCfg->memoryBurstMode & MSK_CHECK_VALID_MEM_BURST) == MSK_VALID_MEM_BURST)
                                                            {
                                                                CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR &= MSK_CLR_MBURST;
                                                                CAST_STREAM_REGS(dmaId + stream)->DMA_SxCR |= (streamCfg->memoryBurstMode & MSK_CLR_CHECK_VALID_MEM_BURST) << SxCR_MBURST_SHIFT;
                                                                errorStatus = dma_retOk;
                                                            }
                                                            else
                                                            {
                                                                errorStatus = dma_retInvalidMemoryBurstMode;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            errorStatus = dma_retInvalidPeripheralBurstMode;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        errorStatus = dma_retInvalidDataSize;
                                                    }
                                                }
                                                else
                                                {
                                                    errorStatus = dma_retInvalidIncrementMode;
                                                }
                                            }
                                            else
                                            {
                                                errorStatus = dma_retInvalidStreamDirection;
                                            }
                                        }
                                        else
                                        {
                                            errorStatus = dma_retInvalidFifoLevel;
                                        }
                                    }
                                    else
                                    {
                                        errorStatus = dma_retInvalidStreamPriority;
                                    }
                                }
                                else
                                {
                                    errorStatus = dma_retInvalidFlowControlOption;
                                }
                            }
                            else
                            {
                                errorStatus = dma_retInvalidChannelId;
                            }
                        }
                        else
                        {
                            errorStatus = dma_retInvalidDataItems;
                        }
                    }
                }
                else
                {
                    errorStatus = dma_retInvalidBufferMode;
                }
            }
            else
            {
                errorStatus = dma_retNullPointer;
            }
        }
        else
        {
            errorStatus = dma_retNullPointer;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_enableStream(u32 dmaId, u16 streamId)
{
    DMA_ErrorStatus_t errorStatus = checkValidDataAndCanConfig(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR |= (1 << SxCR_EN);
    }
    else
    {
        if(errorStatus == dma_retConfigWhileEnabledStream)
        {
            errorStatus = dma_retOk;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_disableStream(u32 dmaId, u16 streamId)
{
    DMA_ErrorStatus_t errorStatus = dma_retNotOk;
    if((dmaId & MSK_CHECK_VALID_DMA_ID) == MSK_VALID_DMA_ID)
    {
        if((streamId & MSK_CHECK_VALID_STREAM) == MSK_VALID_STREAM)
        {
            dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
            streamId &= MSK_CLR_CHECK_VALID_STREAM;
            if((CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR & MSK_CHECK_STRAM_STATE) == MSK_STRAM_ENABLED)
            {
                CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR &= ~(1 << SxCR_EN);
                errorStatus = dma_retOk;
            }
            else
            {
                errorStatus = dma_retStreamIsDisabled;
            }
        }
        else
        {
            errorStatus = dma_retInvalidStreamId;
        }
    }
    else
    {
        errorStatus = dma_retInvalidDMAId;
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_setStreamPriority(u32 dmaId, u16 streamId, u8 streamPriority)
{
    DMA_ErrorStatus_t errorStatus = checkValidDataAndCanConfig(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if((streamPriority & MSK_CHECK_VALID_PRIO) == MSK_VALID_PRIO)
        {
            dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
            streamId &= MSK_CHECK_VALID_STREAM;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR &= MSK_CLR_PL;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR |= (streamPriority & MSK_CLR_CHECK_VALID_PRIO) << SxCR_PRIO_SHIFT;
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retInvalidStreamPriority;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_setStreamDirection(u32 dmaId, u16 streamId, u8 streamDirection)
{
    DMA_ErrorStatus_t errorStatus = checkValidDataAndCanConfig(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if((streamDirection & MSK_CHECK_VALID_STREAM_DIR) == MSK_VALID_STREAM_DIR)
        {
            dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
            streamId &= MSK_CLR_CHECK_VALID_STREAM;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR &= MSK_CLR_DIR;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR |= (streamDirection & MSK_CLR_CHECK_VALID_DIR) << SxCR_DIR_SHIFT;
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retInvalidStreamDirection;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_setDataItems(u32 dmaId, u16 streamId, u16 dataItems)
{
    DMA_ErrorStatus_t errorStatus = checkValidDataAndCanConfig(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if(dataItems >= MIN_VALID_DATA_ITEMS && dataItems <= MAX_VALID_DATA_ITEMS)
        {
            dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
            streamId &= MSK_CLR_CHECK_VALID_STREAM;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR = dataItems;
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retInvalidDataItems;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_setFlowControl(u32 dmaId, u16 streamId, u8 flowControl)
{
    DMA_ErrorStatus_t errorStatus = checkValidDataAndCanConfig(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if((flowControl & MSK_CHECK_VALID_FLOW_CTRL) == MSK_VALID_FLOW_CTRL)
        {
            dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
            streamId &= MSK_CHECK_VALID_STREAM;
            if(flowControl == flowControl_DMA)
            {
                CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR &= ~(1 << SxCR_PFCTRL);
            }
            else
            {
                CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR |= (1 << SxCR_PFCTRL);
            }
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retInvalidFlowControlOption;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_setChannelId(u32 dmaId, u16 streamId, u8 channelId)
{
    DMA_ErrorStatus_t errorStatus = checkValidDataAndCanConfig(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if((channelId & MSK_CHECK_VALID_CHANNEL) == MSK_VALID_CHANNEL)
        {
            dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
            streamId &= MSK_CHECK_VALID_STREAM;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR &= MSK_CLR_CHSEL;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR |= (channelId & MSK_CLR_CHECK_VALID_CHANNEL) << SxCR_CHSEL_SHIFT;
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retInvalidChannelId;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_setPeripheralSize(u32 dmaId, u16 streamId, u8 peripheralSize)
{
    DMA_ErrorStatus_t errorStatus = checkValidDataAndCanConfig(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if((peripheralSize & MSK_CHECK_VALID_DSIZE) == MSK_VALID_DSIZE)
        {
            dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
            streamId &= MSK_CHECK_VALID_STREAM;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR &= MSK_CLR_PSIZE;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR |= (peripheralSize & MSK_CLR_CHECK_VALID_DSIZE) << SxCR_PSIZE_SHIFT;
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retInvalidDataSize;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_setMemorySize(u32 dmaId, u16 streamId, u8 memorySize)
{
    DMA_ErrorStatus_t errorStatus = checkValidDataAndCanConfig(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if((memorySize & MSK_CHECK_VALID_DSIZE) == MSK_VALID_DSIZE)
        {
            dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
            streamId &= MSK_CHECK_VALID_STREAM;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR &= MSK_CLR_MSIZE;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR |= (memorySize & MSK_CLR_CHECK_VALID_DSIZE) << SxCR_MSIZE_SHIFT;
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retInvalidDataSize;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_setPeripheralAddress(u32 dmaId, u16 streamId, u32* peripheralAddress)
{
    DMA_ErrorStatus_t errorStatus = checkValidDataAndCanConfig(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if(peripheralAddress)
        {
            dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
            streamId &= MSK_CHECK_VALID_STREAM;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxPAR = peripheralAddress;
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retNullPointer;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_setMemory0Address(u32 dmaId, u16 streamId, u32* memory0Address)
{
    DMA_ErrorStatus_t errorStatus = checkValidDataAndCanConfig(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if(memory0Address)
        {
            dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
            streamId &= MSK_CHECK_VALID_STREAM;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxM0AR = memory0Address;
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retNullPointer;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_setMemory1Address(u32 dmaId, u16 streamId, u32* memory1Address)
{
    DMA_ErrorStatus_t errorStatus = checkValidDataAndCanConfig(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if(memory1Address)
        {
            dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
            streamId &= MSK_CHECK_VALID_STREAM;
            CAST_STREAM_REGS(dmaId + streamId)->DMA_SxM1AR = memory1Address;
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retNullPointer;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_getRemainingDataItems(u32 dmaId, u16 streamId, pu16 dataItems)
{
    DMA_ErrorStatus_t errorStatus = checkValidData(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if(dataItems)
        {
            dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
            streamId &= MSK_CLR_CHECK_VALID_STREAM;
            *dataItems = CAST_STREAM_REGS(dmaId + streamId)->DMA_SxNDTR;
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retNullPointer;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_enableHalfTransferInterrupt(u32 dmaId, u16 streamId)
{
    DMA_ErrorStatus_t errorStatus = checkValidData(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
        streamId &= MSK_CLR_CHECK_VALID_STREAM;
        CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR |= (1 << SxCR_HTIE);
        errorStatus = dma_retOk;
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_setBufferMode(u32 dmaId, u16 streamId, u8 bufferMode)
{
    DMA_ErrorStatus_t errorStatus = checkValidDataAndCanConfig(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if((bufferMode & MSK_CHECK_VALID_BUFF_MODE) == MSK_VALID_BUFF_MODE)
        {
            switch(bufferMode)
            {
                case bufferMode_Regular:
                    CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR &= ~(1 << SxCR_DBM);
                    CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR &= ~(1 << SxCR_CIRC);
                    CAST_STREAM_REGS(dmaId + streamId)->DMA_SxFCR |= (1 << SxFCR_DMDIS);
                    break;
                case bufferMode_Double:
                    CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR |= (1 << SxCR_DBM);
                    CAST_STREAM_REGS(dmaId + streamId)->DMA_SxFCR &= ~(1 << SxFCR_DMDIS);
                    break;
                case bufferMode_Circular:
                    CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR |= (1 << SxCR_CIRC);
                    CAST_STREAM_REGS(dmaId + streamId)->DMA_SxFCR &= ~(1 << SxFCR_DMDIS);
                    break;
            }
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retInvalidBufferMode;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_disableHalfTransferInterrupt(u32 dmaId, u16 streamId)
{
    DMA_ErrorStatus_t errorStatus = checkValidData(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
        streamId &= MSK_CLR_CHECK_VALID_STREAM;
        CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR &= ~(1 << SxCR_HTIE);
        errorStatus = dma_retOk;
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_enableTransferInterrupt(u32 dmaId, u16 streamId)
{
    DMA_ErrorStatus_t errorStatus = checkValidData(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
        streamId &= MSK_CLR_CHECK_VALID_STREAM;
        CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR |= (1 << SxCR_TCIE);
        errorStatus = dma_retOk;
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_disableTransferInterrupt(u32 dmaId, u16 streamId)
{
    DMA_ErrorStatus_t errorStatus = checkValidData(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
        streamId &= MSK_CLR_CHECK_VALID_STREAM;
        CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR &= ~(1 << SxCR_TCIE);
        errorStatus = dma_retOk;
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_enableErrorsInterrupt(u32 dmaId, u16 streamId)
{
    DMA_ErrorStatus_t errorStatus = checkValidData(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
        streamId &= MSK_CLR_CHECK_VALID_STREAM;
        CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR |= (1 << SxCR_TEIE);
        CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR |= (1 << SxCR_DMEIE);
        CAST_STREAM_REGS(dmaId + streamId)->DMA_SxFCR |= (1 << SxFCR_FEIE);
        errorStatus = dma_retOk;
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_disableErrorsInterrupt(u32 dmaId, u16 streamId)
{
    DMA_ErrorStatus_t errorStatus = checkValidData(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
        streamId &= MSK_CLR_CHECK_VALID_STREAM;
        CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR &= ~(1 << SxCR_TEIE);
        CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR &= ~(1 << SxCR_DMEIE);
        CAST_STREAM_REGS(dmaId + streamId)->DMA_SxFCR &= ~(1 << SxFCR_FEIE);
        errorStatus = dma_retOk;
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_registerHalfCompleteCallback(u32 dmaId, u16 streamId, dmaCallBack_t cbf)
{
    DMA_ErrorStatus_t errorStatus = checkValidData(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if(cbf)
        {
            u8 indexStream = 0;
            switch(streamId)
            {
                case streamId_0:
                    indexStream = 0;
                    break;
                case streamId_1:
                    indexStream = 1;
                    break;
                case streamId_2:
                    indexStream = 2;
                    break;
                case streamId_3:
                    indexStream = 3;
                    break;
                case streamId_4:
                    indexStream = 4;
                    break;
                case streamId_5:
                    indexStream = 5;
                    break;
                case streamId_6:
                    indexStream = 6;
                    break;
                case streamId_7:
                    indexStream = 7;
                    break;
            }
            if(dmaId == dmaId_2)
            {
                dma2HCCallBacks[indexStream] = cbf;
            }
            else
            {
                dma1HCCallBacks[indexStream] = cbf;
            }
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retNullPointer;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_registerTransferCompleteCallback(u32 dmaId, u16 streamId, dmaCallBack_t cbf)
{
    DMA_ErrorStatus_t errorStatus = checkValidData(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if(cbf)
        {
            u8 indexStream = 0;
            switch(streamId)
            {
                case streamId_0:
                    indexStream = 0;
                    break;
                case streamId_1:
                    indexStream = 1;
                    break;
                case streamId_2:
                    indexStream = 2;
                    break;
                case streamId_3:
                    indexStream = 3;
                    break;
                case streamId_4:
                    indexStream = 4;
                    break;
                case streamId_5:
                    indexStream = 5;
                    break;
                case streamId_6:
                    indexStream = 6;
                    break;
                case streamId_7:
                    indexStream = 7;
                    break;
            }
            if(dmaId == dmaId_2)
            {
                dma2TCCallBacks[indexStream] = cbf;
            }
            else
            {
                dma1TCCallBacks[indexStream] = cbf;
            }
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retNullPointer;
        }
    }
    return errorStatus;
}

DMA_ErrorStatus_t dma_registerErrorsCallback(u32 dmaId, u16 streamId, dmaErrorCallBack_t cbf)
{
    DMA_ErrorStatus_t errorStatus = checkValidData(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        if(cbf)
        {
            u8 indexStream = 0;
            switch(streamId)
            {
                case streamId_0:
                    indexStream = 0;
                    break;
                case streamId_1:
                    indexStream = 1;
                    break;
                case streamId_2:
                    indexStream = 2;
                    break;
                case streamId_3:
                    indexStream = 3;
                    break;
                case streamId_4:
                    indexStream = 4;
                    break;
                case streamId_5:
                    indexStream = 5;
                    break;
                case streamId_6:
                    indexStream = 6;
                    break;
                case streamId_7:
                    indexStream = 7;
                    break;
            }
            if(dmaId == dmaId_2)
            {
                dma2ErrorCallBacks[indexStream] = cbf;
            }
            else
            {
                dma1ErrorCallBacks[indexStream] = cbf;
            }
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retNullPointer;
        }
    }
    return errorStatus;
}

static DMA_ErrorStatus_t checkValidData(u32 dmaId, u16 streamId)
{
    DMA_ErrorStatus_t errorStatus = dma_retNotOk;
    if((dmaId & MSK_CHECK_VALID_DMA_ID) == MSK_VALID_DMA_ID)
    {
        if((streamId & MSK_CHECK_VALID_STREAM) == MSK_VALID_STREAM)
        {
            errorStatus = dma_retOk;
        }
        else
        {
            errorStatus = dma_retInvalidStreamId;
        }
    }
    else
    {
        errorStatus = dma_retInvalidDMAId;
    }
    return errorStatus;
}

static DMA_ErrorStatus_t checkValidDataAndCanConfig(u32 dmaId, u16 streamId)
{
    DMA_ErrorStatus_t errorStatus = checkValidData(dmaId, streamId);
    if(errorStatus == dma_retOk)
    {
        dmaId &= MSK_CLR_CHECK_VALID_DMA_ID;
        streamId &= MSK_CLR_CHECK_VALID_STREAM;
        if((CAST_STREAM_REGS(dmaId + streamId)->DMA_SxCR & MSK_CHECK_STRAM_STATE) == MSK_STREAM_DISABLED)
        {
            errorStatus = dma_retConfigWhileEnabledStream;
        }
        else
        {
            errorStatus = dma_retOk;
        }
    }
    return errorStatus;
}

void dmaHandler(u32 dmaBaseAdd, u8 streamIndex)
{
    switch(streamIndex)
    {
        case 0:
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_HTIF04)
            {
                if(dma1HCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= ~MSK_HTIF04;
                    dma1HCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_TCIF04)
            {
                if(dma1TCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_TCIF04;
                    dma1TCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_FEIF04)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_FEIF04;
                    dma1ErrorCallBacks[streamIndex] (dma_retFIFOError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_TEIF04)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_TEIF04;
                    dma1ErrorCallBacks[streamIndex] (dma_retTransferError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_DMEIF04)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_DMEIF04;
                    dma1ErrorCallBacks[streamIndex] (dma_retDirectModeError);
                }
            }
            break;
        case 1:
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_HTIF15)
            {
                if(dma1HCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= ~MSK_HTIF15;
                    dma1HCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_TCIF15)
            {
                if(dma1TCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_TCIF15;
                    dma1TCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_FEIF15)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_FEIF15;
                    dma1ErrorCallBacks[streamIndex] (dma_retFIFOError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_TEIF15)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_TEIF15;
                    dma1ErrorCallBacks[streamIndex] (dma_retTransferError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_DMEIF15)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_DMEIF15;
                    dma1ErrorCallBacks[streamIndex] (dma_retDirectModeError);
                }
            }
            break;
        case 2:
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_HTIF26)
            {
                if(dma1HCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= ~MSK_HTIF26;
                    dma1HCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_TCIF26)
            {
                if(dma1TCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_TCIF26;
                    dma1TCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_FEIF26)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_FEIF26;
                    dma1ErrorCallBacks[streamIndex] (dma_retFIFOError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_TEIF26)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_TEIF26;
                    dma1ErrorCallBacks[streamIndex] (dma_retTransferError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_DMEIF26)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_DMEIF26;
                    dma1ErrorCallBacks[streamIndex] (dma_retDirectModeError);
                }
            }
            break;
        case 3:
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_HTIF37)
            {
                if(dma1HCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= ~MSK_HTIF37;
                    dma1HCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_TCIF37)
            {
                if(dma1TCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_TCIF37;
                    dma1TCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_FEIF37)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_FEIF37;
                    dma1ErrorCallBacks[streamIndex] (dma_retFIFOError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_TEIF37)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_TEIF37;
                    dma1ErrorCallBacks[streamIndex] (dma_retTransferError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_LISR & MSK_DMEIF37)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_LIFCR &= MSK_DMEIF37;
                    dma1ErrorCallBacks[streamIndex] (dma_retDirectModeError);
                }
            }
            break;
        case 4:
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_HTIF04)
            {
                if(dma1HCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= ~MSK_HTIF04;
                    dma1HCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_TCIF04)
            {
                if(dma1TCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_TCIF04;
                    dma1TCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_FEIF04)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_FEIF04;
                    dma1ErrorCallBacks[streamIndex] (dma_retFIFOError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_TEIF04)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_TEIF04;
                    dma1ErrorCallBacks[streamIndex] (dma_retTransferError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_DMEIF04)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_DMEIF04;
                    dma1ErrorCallBacks[streamIndex] (dma_retDirectModeError);
                }
            }
            break;
        case 5:
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_HTIF15)
            {
                if(dma1HCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= ~MSK_HTIF15;
                    dma1HCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_TCIF15)
            {
                if(dma1TCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_TCIF15;
                    dma1TCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_FEIF15)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_FEIF15;
                    dma1ErrorCallBacks[streamIndex] (dma_retFIFOError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_TEIF15)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_TEIF15;
                    dma1ErrorCallBacks[streamIndex] (dma_retTransferError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_DMEIF15)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_DMEIF15;
                    dma1ErrorCallBacks[streamIndex] (dma_retDirectModeError);
                }
            }
            break;
        case 6:
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_HTIF26)
            {
                if(dma1HCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= ~MSK_HTIF26;
                    dma1HCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_TCIF26)
            {
                if(dma1TCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_TCIF26;
                    dma1TCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_FEIF26)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_FEIF26;
                    dma1ErrorCallBacks[streamIndex] (dma_retFIFOError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_TEIF26)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_TEIF26;
                    dma1ErrorCallBacks[streamIndex] (dma_retTransferError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_DMEIF26)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_DMEIF26;
                    dma1ErrorCallBacks[streamIndex] (dma_retDirectModeError);
                }
            }
            break;
        case 7:
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_HTIF37)
            {
                if(dma1HCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= ~MSK_HTIF37;
                    dma1HCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_TCIF37)
            {
                if(dma1TCCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_TCIF37;
                    dma1TCCallBacks[streamIndex]();
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_FEIF37)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_FEIF37;
                    dma1ErrorCallBacks[streamIndex] (dma_retFIFOError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_TEIF37)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_TEIF37;
                    dma1ErrorCallBacks[streamIndex] (dma_retTransferError);
                }
            }
            if(CAST_DMA_REGS(dmaBaseAdd)->DMA_HISR & MSK_DMEIF37)
            {
                if(dma1ErrorCallBacks[streamIndex])
                {
                    CAST_DMA_REGS(dmaBaseAdd)->DMA_HIFCR &= MSK_DMEIF37;
                    dma1ErrorCallBacks[streamIndex] (dma_retDirectModeError);
                }
            }
            break;
    }
}

void DMA1_Stream0_IRQHandler(void)
{
    dmaHandler(DMA1_BASE_ADDRESS, 0);
}

void DMA1_Stream1_IRQHandler(void)
{
    dmaHandler(DMA1_BASE_ADDRESS, 1);
}

void DMA1_Stream2_IRQHandler(void)
{
    dmaHandler(DMA1_BASE_ADDRESS, 2);
}

void DMA1_Stream3_IRQHandler(void)
{
    dmaHandler(DMA1_BASE_ADDRESS, 3);
}

void DMA1_Stream4_IRQHandler(void)
{
    dmaHandler(DMA1_BASE_ADDRESS, 4);
}

void DMA1_Stream5_IRQHandler(void)
{
    dmaHandler(DMA1_BASE_ADDRESS, 5);
}

void DMA1_Stream6_IRQHandler(void)
{
    dmaHandler(DMA1_BASE_ADDRESS, 6);
}

void DMA1_Stream7_IRQHandler(void)
{
    dmaHandler(DMA1_BASE_ADDRESS, 7);
}

void DMA2_Stream0_IRQHandler(void)
{
    dmaHandler(DMA2_BASE_ADDRESS, 0);
}

void DMA2_Stream1_IRQHandler(void)
{
    dmaHandler(DMA2_BASE_ADDRESS, 1);
}

void DMA2_Stream2_IRQHandler(void)
{
    dmaHandler(DMA2_BASE_ADDRESS, 2);
}

void DMA2_Stream3_IRQHandler(void)
{
    dmaHandler(DMA2_BASE_ADDRESS, 3);
}

void DMA2_Stream4_IRQHandler(void)
{
    dmaHandler(DMA2_BASE_ADDRESS, 4);
}

void DMA2_Stream5_IRQHandler(void)
{
    dmaHandler(DMA2_BASE_ADDRESS, 5);
}

void DMA2_Stream6_IRQHandler(void)
{
    dmaHandler(DMA2_BASE_ADDRESS, 6);
}

void DMA2_Stream7_IRQHandler(void)
{
    dmaHandler(DMA2_BASE_ADDRESS, 7);
}
