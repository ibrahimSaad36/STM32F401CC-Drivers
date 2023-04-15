/*******************************************************************
*   File name:    ClockHandler.h
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs needed to deal with clock handler module
*   Version: v1.0
*******************************************************************/

#ifndef CLOCK_HANDLER_H
#define CLOCK_HANDLER_H

#include "../../LIB/Std_types.h"
#include "../../MCAL/RCC/STM_RCC.h"

#define HSI_CLOCK       ((u32) 16000000)
#define HSE_CLOCK       ((u32) 25000000)

typedef void (*clkHandlercbf_t)(u32);

void ClockHandler_getClockCallback(clkHandlercbf_t cbf);

#endif
