/**
 * @file HexParser.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef HEX_PARSER_H
#define HEX_PARSER_H

#include "../../COTS/LIB/STD_TYPES.h"

#define MAX_DATA_SIZE                       16
#define MAX_BUFFER_SIZE                     45
#define DATA_RECORD                         0x00
#define END_OF_FILE_RECORD                  0x01
#define EXTENDED_SEGMENT_ADDRESS_RECORD     0x02
#define START_SEGMENT_ADDRESS_RECORD        0x03
#define EXTENDED_LINEAR_ADDRESS_RECORD      0x04
#define START_LINEAR_ADDRESS_RECORD         0x05

typedef struct 
{
    u8 length;
    u16 address;
    u8 type;
    u8 data[MAX_DATA_SIZE];
    u8 checksum;
} HexRecord_t;

u8 hexParser_asciiToDigit(u8 ascii);
u8 hexParser_hexByteToDecimal(const pu8 byte);
u8 hexParser_calculateChecksum(const HexRecord_t* record);
u8 hexParser_parseHexRecord(u8 *buffer, HexRecord_t *record);
u16 hexParser_parse16UpperBitsAddRecord(const pu8 record);
u32 hexParser_getStartExecutionAdrress(const pu8 record);
void hexParser_bytesTo32Bits(const pu8 bytes, pu8 words, u8 size, pu8 sizeWords);

#endif      /* HEX_PARSER_H */
