/*******************************************************************
*   File name:    Switch.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs definitions to deal with Switch HAL
*                 driver with STM32F401
*   Version: v1.0
*******************************************************************/

#include "Switch.h"

#define MSK_CHECK_VALID_MODE        0xF0
#define MSK_VALID_MODE              0xD0

#define PULL_UP                     1
#define PULL_DOWN                   0
#define SWITCH_STATE_CHANGE_TIMES   5

extern const switchConfig_t switchConfigArr [switchesCount];
static u8 switchesPrevStates [switchesCount];
static u8 switchesActualStates [switchesCount];
static u8 counter [switchesCount];

switch_ErrorStatus_t sw_init(void)
{
    GPIO_ErrorStatus_t  gpioErrorStatus = gpio_retOk;
    switch_ErrorStatus_t swErrorStatus = sw_retNotOk;
    u8 arrSize = sizeof(switchConfigArr) / sizeof(switchConfig_t);
    u8 iterator;
    gpioCfg_t gpioCfg;
    for (iterator = 0; (iterator < arrSize) && (gpioErrorStatus == gpio_retOk); iterator++)
    {
        if ((switchConfigArr[iterator].mode & MSK_CHECK_VALID_MODE) == MSK_VALID_MODE)
        {
            switch (switchConfigArr[iterator].mode)
            {
                case switchModePullUp:
                    gpioCfg.mode = modeInpPullUp;
                    break;
                case switchModePullDown:
                    gpioCfg.mode = modeInpPullDown;
                    break;
                case switchModeExtPullUp:
                case switchModeExtPullDown:
                    gpioCfg.mode = modeInpFloat;
                    break;
            }
            gpioCfg.pin = switchConfigArr[iterator].pin;
            gpioCfg.port = switchConfigArr[iterator].gpioPort;
            gpioCfg.speed = speedHigh;
            gpioErrorStatus = gpio_initPin(&gpioCfg);
        }
        else
        {
            swErrorStatus = sw_retInvalidMode;
            break;
        }
    }
    switch (gpioErrorStatus)
    {
        case gpio_retInvalidPin:
            swErrorStatus = sw_retInvalidPin;
            break;
        case gpio_retInvalidPort:
            swErrorStatus = sw_retInvalidPort;
            break;
        case gpio_retOk:
            swErrorStatus = sw_retOk;
            break;
        default:
            swErrorStatus = sw_retNotOk;
    }
    return swErrorStatus;
}

/*switch_ErrorStatus_t sw_getSwitchState(u8 switchName, pu8 switchState)
{
    GPIO_ErrorStatus_t  gpioErrorStatus = gpio_retOk;
    switch_ErrorStatus_t swErrorStatus = sw_retNotOk;
    if (switchName >= 0 && switchName < (sizeof(switchConfigArr) / sizeof(switchConfig_t)))
    {
        if ((switchConfigArr[switchName].mode & MSK_CHECK_VALID_MODE) == MSK_VALID_MODE)
        {
            u8 state;
            gpioErrorStatus = gpio_getPinValue(switchConfigArr[switchName].gpioPort, switchConfigArr[switchName].pin, &state);
            switch (gpioErrorStatus)
            {
                case gpio_retInvalidPin:
                    swErrorStatus = sw_retInvalidPin;
                    break;
                case gpio_retInvalidPort:
                    swErrorStatus = sw_retInvalidPort;
                    break;
                case gpio_retOk:
                    if (switchConfigArr[switchName].mode == switchModePullUp || switchConfigArr[switchName].mode == switchModeExtPullUp)
                    {
                        *switchState = state ^ PULL_UP;
                    }
                    else
                    {
                        *switchState = state ^ PULL_DOWN;
                    }
                    swErrorStatus = sw_retOk;
                    break;
                default:
                    swErrorStatus = sw_retNotOk;
            }
        }
        else
        {
            swErrorStatus = sw_retInvalidMode;
        }
    }
    else
    {
        swErrorStatus = sw_retInvalidSwitchName;
    }
    return swErrorStatus;
}*/

void switch_refreshReadingsTask(void)
{
    u8 iterator, currentState;
    GPIO_ErrorStatus_t gpioErrorStatus;
    for(iterator = 0; iterator < (sizeof(switchConfigArr) / sizeof(switchConfig_t)); iterator++)
    {
        gpioErrorStatus = gpio_getPinValue(switchConfigArr[iterator].gpioPort, switchConfigArr[iterator].pin, &currentState);
        if(gpioErrorStatus == gpio_retOk)
        {
            if(currentState == switchesPrevStates[iterator])
            {
                counter[iterator]++;
                if(counter[iterator] == SWITCH_STATE_CHANGE_TIMES)
                {
                    switchesActualStates[iterator] = currentState;
                }
            }
            else
            {
                counter[iterator] = 0;
            }
            switchesPrevStates[iterator] = currentState;
        }
    }
}

switch_ErrorStatus_t sw_getSwitchState(u8 switchName, pu8 switchState)
{
    switch_ErrorStatus_t errorStatus = sw_retNotOk;
    if (switchName >= 0 && switchName < (sizeof(switchConfigArr) / sizeof(switchConfig_t)))
    {
        if(switchState == NULL)
        {
            errorStatus = sw_retNullPointer;
        }
        else
        {
            
            if (switchConfigArr[switchName].mode == switchModePullUp || switchConfigArr[switchName].mode == switchModeExtPullUp)
            {
                *switchState = switchesActualStates[switchName] ^ PULL_UP;
            }
            else
            {
                *switchState = switchesActualStates[switchName] ^ PULL_DOWN;
            }
            errorStatus = sw_retOk;
        }
    }
    return errorStatus;
}
