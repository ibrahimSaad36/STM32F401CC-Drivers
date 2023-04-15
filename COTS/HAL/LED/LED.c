/*******************************************************************
*   File name:    LED.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs definitions to deal with LED HAL
*                 driver with STM32F401
*   Version: v1.0
*******************************************************************/

#include "LED.h"

#define MSK_CHECK_VALID_MODE        0xF0
#define MSK_VALID_MODE              0xC0
#define MSK_CLR_CHECK_MODE          0x0F

#define MSK_CHECK_VALID_STATE       0xF0
#define MSK_VALID_STATE             0xB0
#define MSK_CLR_CHECK_STATE         0x0F

extern const ledConfig_t ledConfigArr [ledCount];


led_ErrorStatus_t led_init(void)
{
    GPIO_ErrorStatus_t gpioErrorStatus = gpio_retOk;
    led_ErrorStatus_t ledErrorStatus = led_retNotOk;
    u8 arrSize = sizeof(ledConfigArr) / sizeof(ledConfig_t);
    gpioCfg_t gpioCfg;
    u8 iterator;
    for (iterator = 0; (iterator < arrSize) && (gpioErrorStatus == gpio_retOk); iterator++)
    {
        gpioCfg.mode = modeOutPP;
        gpioCfg.pin = ledConfigArr[iterator].pin;
        gpioCfg.port = ledConfigArr[iterator].gpioPort;
        gpioCfg.speed = ledConfigArr[iterator].speed;
        gpioErrorStatus = gpio_initPin(&gpioCfg);
    }
    switch (gpioErrorStatus)
    {
        case gpio_retInvalidSpeed:
            ledErrorStatus = led_retInvalidSpeed;
            break;
        case gpio_retInvalidPin:
            ledErrorStatus = led_retInvalidPin;
            break;
        case gpio_retInvalidPort:
            ledErrorStatus = led_retInvalidPort;
            break;
        case gpio_retOk:
            ledErrorStatus = led_retOk;
            break;
        default:
            ledErrorStatus = led_retNotOk;
    }
    return ledErrorStatus;
}

led_ErrorStatus_t led_setLedState(ledsInfo_t ledName, u8 ledState)
{
    GPIO_ErrorStatus_t gpioErrorStatus = gpio_retNotOk;
    led_ErrorStatus_t ledErrorStatus = led_retNotOk;
    if(ledName >= 0 && ledName < (sizeof(ledConfigArr) / sizeof(ledConfig_t)))
    {
        if ((ledState & MSK_CHECK_VALID_STATE) == MSK_VALID_STATE)
        {
            if (ledConfigArr[ledName].mode == ledModeActiveHigh || ledConfigArr[ledName].mode == ledModeActiveLow)
            {
                u8 state = (ledConfigArr[ledName].mode & MSK_CLR_CHECK_MODE) ^ (ledState & MSK_CLR_CHECK_STATE);
                gpioErrorStatus = gpio_writePinValue(ledConfigArr[ledName].gpioPort, ledConfigArr[ledName].pin, state);
                switch (gpioErrorStatus)
                {
                    case gpio_retInvalidPin:
                        ledErrorStatus = led_retInvalidPin;
                        break;
                    case gpio_retInvalidPort:
                        ledErrorStatus = led_retInvalidPort;
                        break;
                    case gpio_retOk:
                        ledErrorStatus = led_retOk;
                        break;
                    default:
                        ledErrorStatus = led_retNotOk;
                }
            }
            else
            {
                ledErrorStatus = led_retInvalidMode;
            }
        }
        else
        {
            ledErrorStatus = led_retInvalidState;
        }
    }
    else
    {
        ledErrorStatus = led_retInvalidLedName;
    }
    return ledErrorStatus;
}

led_ErrorStatus_t led_toggle(ledsInfo_t ledName)
{
    GPIO_ErrorStatus_t gpioErrorStatus = gpio_retNotOk;
    led_ErrorStatus_t ledErrorStatus = led_retNotOk;
    if(ledName >= 0 && ledName < (sizeof(ledConfigArr) / sizeof(ledConfig_t)))
    {
        gpioErrorStatus = gpio_togglePin(ledConfigArr[ledName].gpioPort, ledConfigArr[ledName].pin);
        switch (gpioErrorStatus)
        {
            case gpio_retInvalidPin:
                ledErrorStatus = led_retInvalidPin;
                break;
            case gpio_retInvalidPort:
                ledErrorStatus = led_retInvalidPort;
                break;
            case gpio_retOk:
                ledErrorStatus = led_retOk;
                break;
            default:
                ledErrorStatus = led_retNotOk;
        }
    }
    else
    {
        ledErrorStatus = led_retInvalidLedName;
    }
    return ledErrorStatus;
}
