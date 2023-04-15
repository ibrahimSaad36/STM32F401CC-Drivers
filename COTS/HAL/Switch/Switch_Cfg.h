/*******************************************************************
*   File name:    Switch_Cfg.h
*   Author:       Ibrahim Saad
*   Description:  This file contains all configurations data types to deal with Switch HAL
*                 driver with STM32F401
*   Version: v1.0
*******************************************************************/

#ifndef SWIITCH_CFG_H
#define SWIITCH_CFG_H

#include "../../LIB/Std_types.h"
#include "../../MCAL/GPIO/STM_GPIO.h"

#define switchModePullUp          0xD0
#define switchModePullDown        0xD1
#define switchModeExtPullUp       0xD2
#define switchModeExtPullDown     0xD3

typedef struct
{
    u8 gpioPort;
    u8 pin;
    u8 mode;
    pu8 name;
}switchConfig_t;

typedef enum
{
    swAlarm = 0,
    swStart,
    swStop,
    switchesCount,  /* don't change value of this enumerator or remove it*/
}switchesInfo_t;

#endif
