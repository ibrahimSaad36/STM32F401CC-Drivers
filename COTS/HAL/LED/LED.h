/*******************************************************************
*   File name:    LED.h
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs declaration to deal with LED HAL
*                 driver with STM32F401
*   Version: v1.0
*******************************************************************/

#ifndef LED_H
#define LED_H

#include "LED_Cfg.h"

typedef enum
{
    led_retNotOk = 0,
    led_retOk,
    led_retInvalidMode,
    led_retInvalidSpeed,
    led_retInvalidPin,
    led_retInvalidPort,
    led_retInvalidLedName,
    led_retInvalidState,
}led_ErrorStatus_t;

led_ErrorStatus_t led_init(void);
led_ErrorStatus_t led_setLedState(ledsInfo_t ledName, u8 ledState);
led_ErrorStatus_t led_toggle(ledsInfo_t ledName);

#endif
