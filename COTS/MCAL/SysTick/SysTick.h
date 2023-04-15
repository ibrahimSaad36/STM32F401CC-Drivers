/*******************************************************************
*   File name:    SysTick.h
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs needed to deal with Systick core peripheral
*                 features in STM32F401
*   Version: v1.0
*******************************************************************/

#ifndef SYSTICK_H
#define SYSTICK_H

#include "../../LIB/Std_types.h"
#include "../../HAL/ClockHandler/ClockHandler.h"

#define clockSource_AHBPer8         0xD0
#define clockSource_AHB             0xD1

typedef void (*Systick_cbf_t) (void);

typedef enum
{
    systick_retNotOk = 0,
    systick_retOk,
    systick_retNullPointer,
    systic_retInvalidReloadValue,
}SYSTICK_ErrorStatus_t;

SYSTICK_ErrorStatus_t systick_start(void);
SYSTICK_ErrorStatus_t systick_stop(void);
SYSTICK_ErrorStatus_t systick_getCurrentValue(pu32 currentValue);
SYSTICK_ErrorStatus_t systick_setReloadValue(u32 reloadValue);
SYSTICK_ErrorStatus_t systick_setReloadMS(u32 preloadMS);
SYSTICK_ErrorStatus_t systick_setReloadUS(u32 preloadUS);
SYSTICK_ErrorStatus_t systick_setCallBack(Systick_cbf_t cbf);

#endif
