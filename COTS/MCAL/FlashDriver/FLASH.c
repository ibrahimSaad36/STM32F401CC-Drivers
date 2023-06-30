/**
 * @file FLASH.c
 * @author Ibrahim Saad
 * @brief This is the source file which contains all the source code to deal with
 *        the flash memory in STM32F401CC
 * @version 0.1
 * @date 2023-06-13
 * @copyright Copyright (c) 2023
 */

#include "FLASH.h"

#define FLASH_BASE_ADD                  0x08000000

#define FLASH_WAIT_TIMEOUT              0xFFFF

#define KEYR_KEY1                       0x45670123
#define KEYR_KEY2                       0xCDEF89AB
#define OTPKEYR_KEY1                    0x08192A3B
#define OTPKEYR_KEY2                    0x4C5D6E7F

#define MSK_CHECK_VALID_LATENCY         0xF0
#define MSK_VALID_LATENCY               0xD0
#define MSK_CLR_CHECK_VALID_LATENCY     0x0F

#define MSK_CHECK_VALID_PSIZE           0x000000FF
#define MSK_VALID_PSIZE                 0x000000EE
#define MSK_CLR_CHECK_VALID_PSIZE       0xFFFFFF00

#define MSK_CHECK_VALID_SECTOR_NO       0xF0
#define MSK_VALID_SECTOR_NO             0xB0
#define MSK_CLR_CHECK_VALID_SECTOR_NO   0x0F   

/* For FLASH_ACR bits [31:13, 7:4 reserved], LATENCY from 3:0 */
#define ACR_DCRST           12
#define ACR_ICRST           11
#define ACR_DCEN            10
#define ACR_ICEN            9
#define ACR_PRFTEN          8
#define MSK_CLR_LATENCY     0xFFFFFFF0
#define MSK_READ_LATENCY    0x000000FF

/* For FLASH_SR bits [31:17, 15:9, 3:2 reserved] */
#define MSK_SR_BSY          0x00010000
#define MSK_SR_RDERR        0x00000100
#define MSK_SR_PGSERR       0x00000080
#define MSK_SR_PGPERR       0x00000040
#define MSK_SR_PGAERR       0x00000020
#define MSK_SR_WRPERR       0x00000010
#define MSK_SR_OPERR        0x00000002
#define MSK_SR_EOP          0x00000001

/* For FLASH_CR bits [30:26, 23:17, 15:10 reserved] */
#define CR_LOCK             31
#define CR_ERRIE            25
#define CR_EOPIE            24
#define CR_STRT             16
#define MSK_CLR_PSIZE       0xFFFFFCFF
#define MSK_PSIZE_x8        MSK_CLR_PSIZE
#define MSK_PSIZE_x16       0x00000100
#define MSK_PSIZE_x32       0x00000200
#define MSK_PSIZE_x64       0x00000300
#define MSK_CLR_SNB         0xFFFFFF88
#define CR_SNB_SHIFT        3
#define CR_MER              2
#define CR_SER              1
#define CR_PG               0

struct FlashRegs_t
{
    u32 FLASH_ACR;
    u32 FLASH_KEYR;
    u32 FLASH_OPTKEYR;
    u32 FLASH_SR;
    u32 FLASH_CR;
    u32 FLASH_OPTCR;
};

static volatile struct FlashRegs_t* const flashRegs = (volatile struct FlashRegs_t* const) 0x40023C00;  
static operationErrorCbf_t operationErrorCallBack = NULL;

extern void FLASH_IRQHandler(void);
static FLASH_ErrorStatus_t checkBusyFlag();

FLASH_ErrorStatus_t flash_lock()
{
    FLASH_ErrorStatus_t errorStatus = checkBusyFlag();
    if(errorStatus == flash_retNotBusy)
    {
        flashRegs->FLASH_CR |= (1 << CR_LOCK);
        errorStatus = flash_retOk;
    }
    return errorStatus;
}

FLASH_ErrorStatus_t flash_unlock()
{
    FLASH_ErrorStatus_t errorStatus = checkBusyFlag();
    if(errorStatus == flash_retNotBusy)
    {
        flashRegs->FLASH_KEYR = KEYR_KEY1;
        flashRegs->FLASH_KEYR = KEYR_KEY2;
        errorStatus = flash_retOk;
    }
    return errorStatus;
}

FLASH_ErrorStatus_t flash_getStatus()
{
    return checkBusyFlag();
}

FLASH_ErrorStatus_t flash_setPsize(u32 psize)
{
    FLASH_ErrorStatus_t errorStatus = checkBusyFlag();
    if(errorStatus == flash_retNotBusy)
    {
        if((psize & MSK_CHECK_VALID_PSIZE) == MSK_VALID_PSIZE)
        {
            u32 temp = flashRegs->FLASH_CR;
            temp &= MSK_CLR_PSIZE;
            psize &= MSK_CLR_CHECK_VALID_PSIZE;
            temp |= psize;
            flashRegs->FLASH_CR = temp;
            errorStatus = flash_retOk;
        }
        else
        {
            errorStatus = flash_retInvalidPsize;
        }
    }
    return errorStatus;
}

FLASH_ErrorStatus_t flash_setLatency(u8 latency)
{
    FLASH_ErrorStatus_t errorStatus = checkBusyFlag();
    if(errorStatus == flash_retNotBusy)
    {
        if((latency & MSK_CHECK_VALID_LATENCY) == MSK_VALID_LATENCY)
        {
            u32 temp = flashRegs->FLASH_ACR;
            temp &= MSK_CLR_LATENCY;
            latency &= MSK_CLR_CHECK_VALID_LATENCY;
            temp |= latency;
            flashRegs->FLASH_ACR = temp;
            if((flashRegs->FLASH_ACR & MSK_READ_LATENCY) == latency)
            {
                errorStatus = flash_retOk;
            }
            else
            {
                errorStatus = flash_retNotOk;
            }
        }
        else
        {
            errorStatus = flash_retInvalidLatency;
        }
    }
    return errorStatus;
}

FLASH_ErrorStatus_t flash_enableOperationErrorInterrupt(operationErrorCbf_t cbf)
{
    FLASH_ErrorStatus_t errorStatus = checkBusyFlag();
    if(errorStatus == flash_retNotBusy)
    {
        if(cbf)
        {
            flashRegs->FLASH_CR |= (1 << CR_ERRIE);
            operationErrorCallBack = cbf;
            errorStatus = flash_retOk;
        }
        else
        {
            errorStatus = flash_retNullPointer;
        }
    }
    return errorStatus;
}

FLASH_ErrorStatus_t flash_eraseSector(u8 sectorNo)
{
    FLASH_ErrorStatus_t errorStatus = checkBusyFlag();
    if((sectorNo & MSK_CHECK_VALID_SECTOR_NO) == MSK_VALID_SECTOR_NO)
    {
        if(errorStatus == flash_retBusy)
        {
            u16 timeout = FLASH_WAIT_TIMEOUT;
            while((flashRegs->FLASH_SR & MSK_SR_BSY) && timeout > 0)
            {
                timeout--;
            }
        }
        if(errorStatus == flash_retNotBusy)
        {
            u32 temp = flashRegs->FLASH_CR;
            u16 timeout = FLASH_WAIT_TIMEOUT;
            temp &= MSK_CLR_SNB;
            sectorNo &= MSK_CLR_CHECK_VALID_SECTOR_NO;
            temp |= sectorNo << CR_SNB_SHIFT;
            temp |= (1 << CR_SER);
            flashRegs->FLASH_CR = temp;
            flashRegs->FLASH_CR |= (1 << CR_STRT);
            while((flashRegs->FLASH_SR & MSK_SR_BSY) && timeout > 0)
            {
                timeout--;
            }
            if(!(flashRegs->FLASH_SR & MSK_SR_BSY))
            {
                errorStatus = flash_retOk;
            }
            else
            {
                errorStatus = flash_retTimeout;
            }
        }
        else
        {
            errorStatus = flash_retTimeout;
        }
    }
    else
    {
        errorStatus = flash_retInvalidSectorNumber;
    }
    return errorStatus;
}

FLASH_ErrorStatus_t flash_massErase()
{
    FLASH_ErrorStatus_t errorStatus = checkBusyFlag();
    if(errorStatus == flash_retBusy)
    {
        u16 timeout = FLASH_WAIT_TIMEOUT;
        while((flashRegs->FLASH_SR & MSK_SR_BSY) && timeout > 0)
        {
            timeout--;
        }
    }
    if(!(flashRegs->FLASH_SR & MSK_SR_BSY))
    {
        u16 timeout = FLASH_WAIT_TIMEOUT;
        flashRegs->FLASH_CR |= (1 << CR_MER);
        flashRegs->FLASH_CR |= (1 << CR_STRT);
        while((flashRegs->FLASH_SR & MSK_SR_BSY) && timeout > 0)
        {
            timeout--;
        }
        if(!(flashRegs->FLASH_SR & MSK_SR_BSY))
        {
            errorStatus = flash_retOk;
        }
        else
        {
            errorStatus = flash_retTimeout;
        }
    }
    else
    {
        errorStatus = flash_retTimeout;
    }
    return errorStatus;
}

FLASH_ErrorStatus_t flash_calculateSectorStartAddress(u8 sectorNo, pu32 result)
{
    FLASH_ErrorStatus_t errorStatus = flash_retNotOk;
    if((sectorNo & MSK_CHECK_VALID_SECTOR_NO) == MSK_VALID_SECTOR_NO)
    {
        if(result)
        {
            sectorNo &= MSK_CLR_CHECK_VALID_SECTOR_NO;
            if(sectorNo == sectorNo_0 || sectorNo == sectorNo_1 
                || sectorNo == sectorNo_2 || sectorNo == sectorNo_3)
            {
                result = (pu32) (FLASH_BASE_ADD + (sectorNo * 16 * 1024));
            }
            else if(sectorNo == sectorNo_4)
            {
                result = (pu32) (FLASH_BASE_ADD + (sectorNo * 64 * 1024));
            }
            else
            {
                result = (pu32) (FLASH_BASE_ADD + (sectorNo * 128 * 1024));
            }
            errorStatus = flash_retOk;
        }
        else
        {
            errorStatus = flash_retNullPointer;
        }
    }
    else
    {
        errorStatus = flash_retInvalidSectorNumber;
    }
    return errorStatus;
}

FLASH_ErrorStatus_t flash_startProgramming()
{
    FLASH_ErrorStatus_t errorStatus = checkBusyFlag();
    if(errorStatus == flash_retBusy)
    {
        u16 timeout = FLASH_WAIT_TIMEOUT;
        while((flashRegs->FLASH_SR & MSK_SR_BSY) && timeout > 0)
        {
            timeout--;
        }
    }
    if(!(flashRegs->FLASH_SR & MSK_SR_BSY))
    {
        flashRegs->FLASH_CR |= (1 << CR_PG);
        errorStatus = flash_retOk;
    }
    else
    {
        errorStatus = flash_retTimeout;
    }
    return errorStatus;
}

FLASH_ErrorStatus_t flash_stopProgramming()
{
    FLASH_ErrorStatus_t errorStatus = checkBusyFlag();
    if(errorStatus == flash_retBusy)
    {
        u16 timeout = FLASH_WAIT_TIMEOUT;
        while((flashRegs->FLASH_SR & MSK_SR_BSY) && timeout > 0)
        {
            timeout--;
        }
    }
    if(!(flashRegs->FLASH_SR & MSK_SR_BSY))
    {
        flashRegs->FLASH_CR &= ~(1 << CR_PG);
        errorStatus = flash_retOk;
    }
    else
    {
        errorStatus = flash_retTimeout;
    }
    return errorStatus;
}

FLASH_ErrorStatus_t flash_writeData(u32 data, pu32 address)
{
    FLASH_ErrorStatus_t errorStatus = checkBusyFlag();
    if(flashRegs->FLASH_CR >> CR_PG)
    {
        if(!address)
        {
            errorStatus = flash_retNullPointer;
        }
        else
        {
            if(errorStatus == flash_retBusy)
            {
                u16 timeout = FLASH_WAIT_TIMEOUT;
                while((flashRegs->FLASH_SR & MSK_SR_BSY) && timeout > 0)
                {
                    timeout--;
                }
            }
            if(!(flashRegs->FLASH_SR & MSK_SR_BSY))
            {
                u16 timeout = FLASH_WAIT_TIMEOUT;
                *address = data;
                while((flashRegs->FLASH_SR & MSK_SR_BSY) && timeout > 0)
                {
                    timeout--;
                }
                if(!(flashRegs->FLASH_SR & MSK_SR_BSY))
                {
                    if(flashRegs->FLASH_SR & MSK_SR_PGAERR)
                    {
                        flashRegs->FLASH_SR &= ~MSK_SR_PGAERR;
                        errorStatus = flash_retProgrammingAlignmentError;
                    }
                    else if(flashRegs->FLASH_SR & MSK_SR_PGSERR)
                    {
                        flashRegs->FLASH_SR &= ~MSK_SR_PGSERR;
                        errorStatus = flash_retProgrammingSequenceError;
                    }
                    else if(flashRegs->FLASH_SR & MSK_SR_PGPERR)
                    {
                        flashRegs->FLASH_SR &= ~MSK_SR_PGPERR;
                        errorStatus = flash_retProgrammingParallelismError;
                    }
                    else if(flashRegs->FLASH_SR & MSK_SR_WRPERR)
                    {
                        flashRegs->FLASH_SR &= ~MSK_SR_WRPERR;
                        errorStatus = flash_retWriteProtectionError;
                    }
                    else
                    {
                        errorStatus = flash_retOk;
                    }
                }
                else
                {
                    errorStatus = flash_retTimeout;
                }
            }
            else
            {
                errorStatus = flash_retTimeout;
            }
        }
    }
    else
    {
        errorStatus = flash_retPGNotSet;
    }
    return errorStatus;
}

FLASH_ErrorStatus_t flash_readData(pu32 address, pu32 data)
{
    FLASH_ErrorStatus_t errorStatus = checkBusyFlag();
    if(!address || !data)
    {
        errorStatus = flash_retNullPointer;
    }
    else
    {
        if(errorStatus == flash_retBusy)
        {
            u16 timeout = FLASH_WAIT_TIMEOUT;
            while((flashRegs->FLASH_SR & MSK_SR_BSY) && timeout > 0)
            {
                timeout--;
            }
        }
        if(!(flashRegs->FLASH_SR & MSK_SR_BSY))
        {
            *data = *address;
            if(flashRegs->FLASH_SR & MSK_SR_RDERR)
            {
                flashRegs->FLASH_SR &= ~MSK_SR_RDERR;
                errorStatus = flash_retReadProtectionError;
            }
            else
            {
                errorStatus = flash_retOk;
            }
        }
        else
        {
            errorStatus = flash_retTimeout;
        }
    }
    return errorStatus;
}

static FLASH_ErrorStatus_t checkBusyFlag()
{
    FLASH_ErrorStatus_t errorStatus = flash_retNotBusy;
    if(flashRegs->FLASH_SR & MSK_SR_BSY)
    {
        errorStatus = flash_retBusy;
    }
    return errorStatus;
}

/* IRQ No 4 */
void FLASH_IRQHandler(void)
{
    if(flashRegs->FLASH_SR & MSK_SR_OPERR)
    {
        if(operationErrorCallBack)
        {
            operationErrorCallBack(flash_retOperationError);
        }
    }
}
