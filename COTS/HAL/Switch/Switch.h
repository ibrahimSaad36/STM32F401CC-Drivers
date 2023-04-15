/*******************************************************************
*   File name:    Switch.h
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs declaration to deal with Switch HAL
*                 driver with STM32F401
*   Version: v1.0
*******************************************************************/

#ifndef SWIITCH_H
#define SWIITCH_H

#include "Switch_Cfg.h"

#define switchStatePressed        1
#define switchStateReleased       0

typedef enum
{
    sw_retNotOk = 0,
    sw_retOk,
    sw_retInvalidMode,
    sw_retInvalidPin,
    sw_retInvalidSwitchName,
    sw_retInvalidPort,
    sw_retNullPointer,
}switch_ErrorStatus_t;

switch_ErrorStatus_t sw_init(void);

void switch_refreshReadingsTask(void);

switch_ErrorStatus_t sw_getSwitchState(u8 switchName, pu8 switchState);

#endif
