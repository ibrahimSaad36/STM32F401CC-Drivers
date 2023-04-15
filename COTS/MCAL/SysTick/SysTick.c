/*******************************************************************
*   File name:    SysTick.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs definitions needed to deal with Systick core peripheral
*                 features in STM32F401
*   Version: v1.0
*******************************************************************/

#include "SysTick.h"

#define SYSTICK_RESOLUTION              16777216UL
#define CONVERT_MILLI_SEC               1000
#define CONVERT_MICRO_SEC               1000000

/* STK_CTRL bits */
#define STK_CTRL_COUNTFLAG              16
#define STK_CTRL_CLKSOURCE              2
#define STK_CTRL_TICKINT                1
#define STK_CTRL_ENABLE                 0

#define MSK_CHECK_VALID_CLK_SRC         0xF0
#define MSK_VALID_CLK_SRC               0xD0

#define MSK_EN_SYSTICKCLR               0xFFFFFFF8
#define MSK_EN_SYSTICK_AHB_PER8         0x00000003
#define MSK_EN_SYSTICK_AHB              0x00000007
#define MSK_STOP_SYSTIC                 0xFFFFFFFC
#define MSK_CHECK_SYSTICK_AHB           

#define MSK_GET_SYSTICK_VAL             0x00FFFFFF

typedef struct
{
    u32 STK_CTRL;
    u32 STK_LOAD;
    u32 STK_VAL;
    u32 STK_CALIB;
}SysTickRegs;

/* SysTick Handler */
extern void SysTick_Handler(void);

static volatile SysTickRegs* const systickRegs = (volatile SysTickRegs* const) (0xE000E010);
static u32 sysClock;
static Systick_cbf_t systickCallback = NULL;

static void getSysClock(u32 clock);

static SYSTICK_ErrorStatus_t setReloadValue(u32 reload)
{
    SYSTICK_ErrorStatus_t errorStatus = systick_retOk;
    systickRegs->STK_LOAD = reload & MSK_GET_SYSTICK_VAL;
    return errorStatus;
}

SYSTICK_ErrorStatus_t systick_start(void)
{
    SYSTICK_ErrorStatus_t errorStatus = systick_retNotOk;
    u32 temp = systickRegs->STK_CTRL;
    temp &= MSK_EN_SYSTICKCLR;
    temp |= MSK_EN_SYSTICK_AHB_PER8;
    systickRegs->STK_CTRL = temp;
    errorStatus = systick_retOk;
    return errorStatus;
}

SYSTICK_ErrorStatus_t systick_stop(void)
{
    systickRegs->STK_CTRL &= MSK_STOP_SYSTIC;
    return systick_retOk;
}

SYSTICK_ErrorStatus_t systick_getCurrentValue(pu32 currentValue)
{
    SYSTICK_ErrorStatus_t errorStatus = systick_retNotOk;
    if (currentValue == NULL)
    {
        errorStatus = systick_retNullPointer;
    }
    else
    {
        *currentValue = systickRegs->STK_VAL & MSK_GET_SYSTICK_VAL;
        errorStatus = systick_retOk;
    }
    return errorStatus;
}

SYSTICK_ErrorStatus_t systick_setReloadValue(u32 reloadValue)
{
    return setReloadValue(reloadValue);
}

SYSTICK_ErrorStatus_t systick_setReloadMS(u32 preloadMS)
{
    f32 reloadVal;
    SYSTICK_ErrorStatus_t errorStatus = systick_retNotOk;
    ClockHandler_getClockCallback(getSysClock);
    reloadVal = (sysClock / 8.0) * ((preloadMS * 1.0) / CONVERT_MILLI_SEC);
    if (reloadVal <= SYSTICK_RESOLUTION)
    {
        errorStatus = setReloadValue(reloadVal - 1);
    }
    else
    {
        errorStatus = systic_retInvalidReloadValue;
    }
    return errorStatus;
}

SYSTICK_ErrorStatus_t systick_setReloadUS(u32 preloadUS)
{
    f32 reloadVal;
    SYSTICK_ErrorStatus_t errorStatus = systick_retNotOk;
    ClockHandler_getClockCallback(getSysClock);
    reloadVal = (sysClock / 8.0) * ((preloadUS * 1.0) / CONVERT_MICRO_SEC);
    if (reloadVal <= SYSTICK_RESOLUTION)
    {
        errorStatus = setReloadValue(reloadVal - 1);
    }
    else
    {
        errorStatus = systic_retInvalidReloadValue;
    }
    return errorStatus;
}

SYSTICK_ErrorStatus_t systick_setCallBack(Systick_cbf_t cbf)
{
    SYSTICK_ErrorStatus_t errorStatus = systick_retNotOk;
    if (cbf == NULL)
    {
        errorStatus = systick_retNullPointer;
    }
    else
    {
        systickCallback = cbf;
        errorStatus = systick_retOk;
    }
    return errorStatus;
}

void SysTick_Handler(void)
{
    if(systickCallback)
    {
        systickCallback();
    }
}

static void getSysClock(u32 clock)
{
    sysClock = clock;
}
