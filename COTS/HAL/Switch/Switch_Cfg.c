/*******************************************************************
*   File name:    Switch_Cfg.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all Configurations to deal with Switch HAL
*                 driver with STM32F401
*   Version: v1.0
*******************************************************************/

#include "Switch_Cfg.h"

/*************************************************************************
 * Use these values:
      - For gpioPort use:
            >> gpioPortA
            >> gpioPortB
            >> gpioPortC
            >> gpioPortD
            >> gpioPortE
            >> gpioPortH
      - For pin use:
            >> PIN0
            >> PIN1
            >> PIN2
                till PIN15
      - For mode use:
            >> switchModePullUp
            >> switchModePullDown
            >> switchModeExtPullUp
            >> switchModeExtPullDown
      - For speed use:
            >> speedLow
            >> speedHigh
            >> speedVeryHigh
      - For name:
            >> it's string contains description name of switch just for debugging info 
**************************************************************************/

const switchConfig_t switchConfigArr [switchesCount] = 
    {
        [swAlarm] = {
                        .gpioPort = gpioPortA,
                        .pin = PIN2,
                        .mode = switchModePullUp,
                        .name = "Alarm",
                     },
        [swStart] = {
                        .gpioPort = gpioPortA,
                        .pin = PIN4,
                        .mode = switchModePullDown,
                        .name = "Start",
                     },
        [swStop] = {
                        .gpioPort = gpioPortB,
                        .pin = PIN4,
                        .mode = switchModeExtPullDown,
                        .name = "Stop",
                     }
    };