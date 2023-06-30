/**
 * @file FLASH.h
 * @author Ibrahim Saad
 * @brief This is an interface for the user to use the flash driver features for
          STM32F401CC
 * @version 0.1
 * @date 2023-06-13
 * @copyright Copyright (c) 2023
 */

#ifndef FLASH_H
#define FLASH_H

#include "../../LIB/Std_types.h"

#define psize_x8            0x000000EE
#define pszie_x16           0x000001EE
#define pszie_x32           0x000002EE
#define psize_x64           0x000003EE

#define sectorNo_0          0xB0        /* Sector0 is 16KB */
#define sectorNo_1          0xB1        /* Sector1 is 16KB */
#define sectorNo_2          0xB2        /* Sector2 is 16KB */
#define sectorNo_3          0xB3        /* Sector3 is 16KB */
#define sectorNo_4          0xB4        /* Sector4 is 64KB */
#define sectorNo_5          0xB5        /* Sector5 is 128KB */

#define latency_0WS         0xD0
#define latency_1WS         0xD1
#define latency_2WS         0xD2
#define latency_3WS         0xD3
#define latency_4WS         0xD4
#define latency_5WS         0xD5

typedef enum
{
    flash_retNotOk,
    flash_retOk,
    flash_retNullPointer,
    flash_retFlashLocked,
    flash_retBusy,
    flash_retNotBusy,
    flash_retInvalidPsize,
    flash_retInvalidSectorNumber,
    flash_retInvalidLatency,
    flash_retReadProtectionError,
    flash_retWriteProtectionError,
    flash_retProgrammingSequenceError,
    flash_retProgrammingParallelismError,
    flash_retProgrammingAlignmentError,
    flash_retOperationError,
    flash_retTimeout,
    flash_retPGNotSet,
}FLASH_ErrorStatus_t;

typedef void (*operationErrorCbf_t) (FLASH_ErrorStatus_t);

FLASH_ErrorStatus_t flash_lock();
FLASH_ErrorStatus_t flash_unlock();
FLASH_ErrorStatus_t flash_getStatus();
FLASH_ErrorStatus_t flash_setPsize(u32 psize);
FLASH_ErrorStatus_t flash_setLatency(u8 latency);
FLASH_ErrorStatus_t flash_enableOperationErrorInterrupt(operationErrorCbf_t cbf);
FLASH_ErrorStatus_t flash_eraseSector(u8 sectorNo);
FLASH_ErrorStatus_t flash_massErase();
FLASH_ErrorStatus_t flash_calculateSectorStartAddress(u8 sectorNo, pu32 result);
FLASH_ErrorStatus_t flash_startProgramming();
FLASH_ErrorStatus_t flash_stopProgramming();
FLASH_ErrorStatus_t flash_writeData(u32 data, pu32 address);
FLASH_ErrorStatus_t flash_readData(pu32 address, pu32 data);

#endif  /* FLASH_H */