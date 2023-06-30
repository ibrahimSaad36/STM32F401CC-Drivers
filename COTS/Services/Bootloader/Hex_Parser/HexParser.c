/**
 * @file HexParser.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "HexParser.h"

u8 hexParser_asciiToDigit(u8 ascii)
{
    u8 digit = 0;
    if(ascii >= '0' && ascii <= '9')
    {
        digit = ascii - '0';
    }
    else if(ascii >= 'A' && ascii <= 'F')
    {
        digit = ascii - 'A' + 10;
    }
    else if(ascii >= 'a' && ascii <= 'f')
    {
        digit = ascii - 'a' + 10;
    }
    return digit;
}

u8 hexParser_hexByteToDecimal(const pu8 byte)
{
    return ((hexParser_asciiToDigit(byte[0]) << 4) | hexParser_asciiToDigit(byte[1]));
}

u8 hexParser_calculateChecksum(const HexRecord_t* record)
{
    u8 checksum = 0, i;
    checksum += record->length;
    checksum += (u8)(record->address >> 8);
    checksum += (u8)(record->address & 0xFF);
    checksum += record->type;
    for (i = 0; i < record->length; i++)
    {
        checksum += record->data[i];
    }
    checksum = ~checksum + 1;
    return checksum;
}

u8 hexParser_parseHexRecord(u8 *buffer, HexRecord_t *record)
{
    u8 retVal = 0;
    if (buffer[0] != ':') 
    {
    	retVal = 0;
    }
    else
    {
        record->length = hexParser_hexByteToDecimal(&buffer[1]);
        record->address = (hexParser_hexByteToDecimal(&buffer[3]) << 8) | hexParser_hexByteToDecimal(&buffer[5]);
        record->type = hexParser_hexByteToDecimal(&buffer[7]);
        int i;
        for (i = 0; i < record->length; i++) 
        {
            record->data[i] = hexParser_hexByteToDecimal(&buffer[9 + i * 2]);
        }
        record->checksum = hexParser_hexByteToDecimal(&buffer[9 + i * 2]);
        u8 sum = record->length + (record->address >> 8) + (record->address & 0xFF) + record->type;
        for (i = 0; i < record->length; i++) 
        {
            sum += record->data[i];
        }
        sum = ~sum + 1;
        if (sum != record->checksum) 
        {
        	retVal = 0;
        }
        else
        {
            retVal = 1;
        }
    }
    return retVal;
}

u16 hexParser_parse16UpperBitsAddRecord(const pu8 record)
{
    u16 address = 0;
    if(record[8] == '4')
    {
        /* extended linear address record */
        address = (hexParser_hexByteToDecimal(&record[9]) << 24) | (hexParser_hexByteToDecimal(&record[11]) << 16);
    }
    return address;
}

u32 hexParser_getStartExecutionAdrress(const pu8 record)
{
    u32 ret = 0xFFFFFFFF;
    if(record[8] == '5')
    {
        /* extended linear address record */
        ret = (hexParser_hexByteToDecimal(&record[9]) << 24) | (hexParser_hexByteToDecimal(&record[11]) << 16)
                | (hexParser_hexByteToDecimal(&record[13]) << 8) | hexParser_hexByteToDecimal(&record[15]);
    }
    return ret;
}

void hexParser_bytesTo32Bits(const pu8 bytes, pu8 words, u8 size, pu8 sizeWords)
{
    u8 i, j;
    for (i = 0, j = 0; i < size; i += 4, j++) {
        u32 val = 0;
        if (i < size) val |= ((u32)bytes[i]);
        if (i + 1 < size) val |= ((u32)bytes[i+1]) << 8;
        if (i + 2 < size) val |= ((u32)bytes[i+2]) << 16;
        if (i + 3 < size) val |= ((u32)bytes[i+3] << 24);
        words[j] = val;
    }
    *sizeWords = j;
}

/*
int main(void)
{
    u8 i, cs;
    u8 buffer[MAX_BUFFER_SIZE] = ":10010000214601360121470136007EFE09D2190140";
    HexRecord_t record;
    printf("Record: %s\n", buffer);
    if (parseHexRecord(buffer, &record) == 1) 
    {
        printf("Record data Length: %u\n", record.length);
        printf("Record address: %0x\n", record.address);
        printf("Record data type: %u\n", record.type);
        printf("Record data Length: \n");
        for(i = 0; i < record.length; i++)
        {
            printf("%x   ", record.data[i]);
        }
        printf("\nRecord check sum: %x\n", record.checksum);
        cs = calculateChecksum(&record);
        if(cs == record.checksum)
        {
            printf("Check sum is ok\n");
        }
        else
        {
            printf("Check sum is not ok\n");
        }
    }

    return 0;
}
*/
