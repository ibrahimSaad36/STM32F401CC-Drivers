/*******************************************************************
*   File name:    LED_Cfg.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all Configurations to deal with LED HAL
*                 driver with STM32F401
*   Version: v1.0
*******************************************************************/

#include "LED_Cfg.h"

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
            >> ledModeActiveHigh
            >> ledModeActiveLow
      - For speed use:
            >> speedLow
            >> speedHigh
            >> speedVeryHigh
      - For name:
            >> it's string contains description name of LED just for debugging info
      - For color:
            >> it's string contains color of LED just for debugging info  
**************************************************************************/

const ledConfig_t ledConfigArr [ledCount] = 
    {
        [ledAlarm] = {
                        .gpioPort = gpioPortA,
                        .pin = PIN3,
                        .mode = ledModeActiveHigh,
                        .speed = speedHigh,
                        .name = "Alarm",
                        .color = "Green"
                     },
        [ledStart] = {
                        .gpioPort = gpioPortB,
                        .pin = PIN0,
                        .mode = ledModeActiveHigh,
                        .speed = speedHigh,
                        .name = "Alarm",
                        .color = "Green"
                     },
        [ledStop] = {
                        .gpioPort = gpioPortC,
                        .pin = PIN14,
                        .mode = ledModeActiveHigh,
                        .speed = speedHigh,
                        .name = "Alarm",
                        .color = "Green"
                     },
	  [ledBuiltIn] = {
				.gpioPort = gpioPortC,
				.pin = PIN13,
				.mode = ledModeActiveLow,
				.speed = speedHigh,
				.name = "Alarm",
				.color = "Green"
				}
    };
    
