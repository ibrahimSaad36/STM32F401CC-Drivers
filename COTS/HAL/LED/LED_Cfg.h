/*******************************************************************
*   File name:    LED_Cfg.h
*   Author:       Ibrahim Saad
*   Description:  This file contains all configurations data types to deal with LED HAL
*                 driver with STM32F401
*   Version: v1.0
*******************************************************************/

#ifndef LED_CFG_H
#define LED_CFG_H

#include "../../LIB/Std_types.h"
#include "../../MCAL/GPIO/STM_GPIO.h"

#define ledStateOn            0xB1
#define ledStateOff           0xB0

#define ledModeActiveHigh     0xC0
#define ledModeActiveLow      0xC1

typedef struct
{
    u8 gpioPort;
    u8 pin;
    u8 mode;
    u8 speed;
    pu8 name;
    pu8 color;
}ledConfig_t;

typedef enum
{
    ledAlarm = 0,
    ledStart,
    ledStop,
    ledBuiltIn,
    ledCount,  /* don't change value of this enumerator or remove it*/
}ledsInfo_t;

#endif
