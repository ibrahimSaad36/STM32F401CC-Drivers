/**
 * @file CRC.c
 * @author Ibrahim Saad
 * @brief This is the source file which contains the implementation of all APIs
          needed to use CRC calculation unit for STM32F401CC
 * @version 0.1
 * @date 2023-06-13
 * @copyright Copyright (c) 2023
 */

#include "CRC.h"

#define CRC_CR_RESET    0

struct CRCRegs_t
{
    u32 CRC_DR;
    u32 CRC_IDR;        /* Only 8 bits used from 0:7 */
    u32 CRC_CR;
};

static volatile struct CRCRegs_t* const crcRegs = (volatile struct CRCRegs_t* const) 0x40023000;

static void approximateWaiting_4ClockCycles();

void crc_setDataRegister(u32 data)
{
    crcRegs->CRC_DR = data;
    approximateWaiting_6ClockCycles();
}

CRC_ErrorStatus_t crc_getCRC(pu32 crcValue)
{
    CRC_ErrorStatus_t errorStatus = crc_retNotOk;
    if(crcValue)
    {
        *crcValue = crcRegs->CRC_DR;
        errorStatus = crc_retOk;
    }
    else
    {
        errorStatus = crc_retNullPointer;
    }
    return errorStatus;
}

void crc_resetCRCValue()
{
    crcRegs->CRC_CR |= (1 << CRC_CR_RESET);
}

CRC_ErrorStatus_t crc_getDataBlockCRC(pu32 arr, u16 length, pu32 crcValue)
{
    CRC_ErrorStatus_t errorStatus = crc_retNotOk;
    if(arr && crcValue)
    {
        u16 i;
        for(i = 0; i < length; i++)
        {
            crcRegs->CRC_DR = arr[i];
            approximateWaiting_4ClockCycles();
        }
        *crcValue = crcRegs->CRC_DR;
        errorStatus = crc_retOk;
    }
    else
    {
        errorStatus = crc_retNullPointer;
    }
    return errorStatus;
}

static void approximateWaiting_4ClockCycles()
{
    u8 i;
    for(i = 0; i < 4; i++)
    {
        __asm("nop");
    }
}
