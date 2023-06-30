/**
 * @file CRC.c
 * @author Ibrahim Saad
 * @brief This is the header file which contains the declaration of all APIs
          needed to use CRC calculation unit for STM32F401CC
 * @version 0.1
 * @date 2023-06-13
 * @copyright Copyright (c) 2023
 */

#ifndef CRC_H
#define CRC_H

#include "../../LIB/Std_types.h"

typedef enum
{
    crc_retNotOk,
    crc_retOk,
    crc_retNullPointer,
}CRC_ErrorStatus_t;

void crc_setDataRegister(u32 data);
CRC_ErrorStatus_t crc_getCRC(pu32 crcValue);
void crc_resetCRCValue();
CRC_ErrorStatus_t crc_getDataBlockCRC(pu32 arr, u16 length, pu32 crcValue);

#endif /* CRC_H */