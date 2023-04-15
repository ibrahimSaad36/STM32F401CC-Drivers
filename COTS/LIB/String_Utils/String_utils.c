#include "String_utils.h"

void tostring(u8 str[], u32 num)
{
    u32 i, rem, len = 0, n;
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

u8 strLength(u8 str[])
{
    u8 len = 0, i = 0;
    while(str[i] != '\0')
    {
        len++;
        i++;
    }
    return len;
}

void strConcat(u8* str1, u8* str2)
{
    u8 str1Len = strLength(str1);
    u8 i = 0;
    while(str2[i] != '\0')
    {
        str1[str1Len + i] = str2[i];
        i++;
    }
    str1[str1Len + i] = '\0';
}