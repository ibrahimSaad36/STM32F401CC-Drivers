/**
 * @file HC_05.c
 * @author Ibrahim Saad
 * @brief This file contains all the APIs definitions needed to deal with hc05 bluetooth 
          module
 * @version 0.1
 * @date 2023-06-16
 * @copyright Copyright (c) 2023
 */

#include "HC_05.h"

extern const HC05Cfg_t hc05ConfigArr [hc05Counts];

HC05_ErrorStatus_t hc05_init(void)
{
    HC05_ErrorStatus_t hc05ErrorStatus = hc05_retNotOk;
    USART_ErrorStatus_t usartErrorStatus = usart_retOk;
    GPIO_ErrorStatus_t gpioErrorStatus = gpio_retOk;
    u8 hc05Cnts = sizeof(hc05ConfigArr) / sizeof(HC05Cfg_t), i;
    for(i = 0; (i < hc05Cnts) && (usartErrorStatus == usart_retOk) && (gpioErrorStatus == gpio_retOk); i++)
    {
        if(hc05ConfigArr[i].usartId == usartId_1 || hc05ConfigArr[i].usartId == usartId_2)
        {
            if(hc05ConfigArr[i].usartCfg.usartDirection == usartDirection_RX)
            {
                gpioErrorStatus = gpio_initPin(&hc05ConfigArr[i].RX);
                gpioErrorStatus = gpio_selectAlternativeFunction(hc05ConfigArr[i].RX.port, hc05ConfigArr[i].RX.pin, aFunction7);
            }
            else if(hc05ConfigArr[i].usartCfg.usartDirection == usartDirection_TX)
            {
                gpioErrorStatus = gpio_initPin(&hc05ConfigArr[i].TX);
                gpioErrorStatus = gpio_selectAlternativeFunction(hc05ConfigArr[i].TX.port, hc05ConfigArr[i].TX.pin, aFunction7);
            }
            else if(hc05ConfigArr[i].usartCfg.usartDirection == usartDirection_TXRX)
            {
                gpioErrorStatus = gpio_initPin(&hc05ConfigArr[i].RX);
                gpioErrorStatus = gpio_initPin(&hc05ConfigArr[i].TX);
                gpioErrorStatus = gpio_selectAlternativeFunction(hc05ConfigArr[i].RX.port, hc05ConfigArr[i].RX.pin, aFunction7);
                gpioErrorStatus = gpio_selectAlternativeFunction(hc05ConfigArr[i].TX.port, hc05ConfigArr[i].TX.pin, aFunction7);
            }
            else
            {
                hc05ErrorStatus = hc05_retInvalidUsartDirection;
                break;
            }
            
        }
        else if(hc05ConfigArr[i].usartId == usartId_6)
        {
            if(hc05ConfigArr[i].usartCfg.usartDirection == usartDirection_RX)
            {
                gpioErrorStatus = gpio_initPin(&hc05ConfigArr[i].RX);
                gpioErrorStatus = gpio_selectAlternativeFunction(hc05ConfigArr[i].RX.port, hc05ConfigArr[i].RX.pin, aFunction8);
            }
            else if(hc05ConfigArr[i].usartCfg.usartDirection == usartDirection_TX)
            {
                gpioErrorStatus = gpio_initPin(&hc05ConfigArr[i].TX);
                gpioErrorStatus = gpio_selectAlternativeFunction(hc05ConfigArr[i].TX.port, hc05ConfigArr[i].TX.pin, aFunction8);
            }
            else if(hc05ConfigArr[i].usartCfg.usartDirection == usartDirection_TXRX)
            {
                gpioErrorStatus = gpio_initPin(&hc05ConfigArr[i].RX);
                gpioErrorStatus = gpio_initPin(&hc05ConfigArr[i].TX);
                gpioErrorStatus = gpio_selectAlternativeFunction(hc05ConfigArr[i].RX.port, hc05ConfigArr[i].RX.pin, aFunction8);
                gpioErrorStatus = gpio_selectAlternativeFunction(hc05ConfigArr[i].TX.port, hc05ConfigArr[i].TX.pin, aFunction8);
            }
            else
            {
                hc05ErrorStatus = hc05_retInvalidUsartDirection;
                break;
            }
        }
        else
        {
            hc05ErrorStatus = hc05_retInvalidUsartId;
            break;
        }
        if(hc05ConfigArr[i].considerStatePin)
        {
            gpioErrorStatus = gpio_initPin(&hc05ConfigArr[i].statePin);
        }
        usartErrorStatus = usart_init(hc05ConfigArr[i].usartId, &hc05ConfigArr[i].usartCfg);
        usartErrorStatus = usart_enable(hc05ConfigArr[i].usartId);
    }
    if(gpioErrorStatus == gpio_retOk && usartErrorStatus == usart_retOk)
    {
        hc05ErrorStatus = hc05_retOk;
    }
    return hc05ErrorStatus;
}

HC05_ErrorStatus_t hc05_recieveByteSync(Hc05Info_t module, pu8 ch)
{
    HC05_ErrorStatus_t errorStatus = hc05_retNotOk;
    if(module >= 0 && (module <= (sizeof(hc05ConfigArr) / sizeof(HC05Cfg_t))))
    {
        if(ch)
        {
            if(hc05ConfigArr[module].considerStatePin)
            {
                u8 state;
                if(gpio_getPinValue(hc05ConfigArr[module].statePin.port
                    , hc05ConfigArr[module].statePin.pin, &state) == gpio_retOk)
                {
                    if(state == gpioVal_SET)
                    {
                        if(usart_recieveCharSync(hc05ConfigArr[module].usartId, ch) == usart_retOk)
                        {
                            errorStatus = hc05_retOk;
                        }
                    }
                    else
                    {
                        errorStatus = hc05_retModuleNotPaired;
                    }
                }
            }
            else
            {
                if(usart_recieveCharSync(hc05ConfigArr[module].usartId, ch) == usart_retOk)
                {
                    errorStatus = hc05_retOk;
                }
            }
        }
        else
        {
            errorStatus = hc05_retNullPointer;
        }
    }
    else
    {
        errorStatus = hc05_retInvalidModuleName;
    }
    return errorStatus;
}

HC05_ErrorStatus_t hc05_sendByteSync(Hc05Info_t module, u8 ch)
{
    HC05_ErrorStatus_t errorStatus = hc05_retNotOk;
    if(module >= 0 && (module <= (sizeof(hc05ConfigArr) / sizeof(HC05Cfg_t))))
    {
        if(hc05ConfigArr[module].considerStatePin)
        {
            u8 state;
            if(gpio_getPinValue(hc05ConfigArr[module].statePin.port
                , hc05ConfigArr[module].statePin.pin, &state) == gpio_retOk)
            {
                if(state == gpioVal_SET)
                {
                    if(usart_sendCharSync(hc05ConfigArr[module].usartId, ch) == usart_retOk)
                    {
                        errorStatus = hc05_retOk;
                    }
                }
                else
                {
                    errorStatus = hc05_retModuleNotPaired;
                }
            }
        }
        else
        {
            if(usart_sendCharSync(hc05ConfigArr[module].usartId, ch) == usart_retOk)
            {
                errorStatus = hc05_retOk;
            }
        }
    }
    else
    {
        errorStatus = hc05_retInvalidModuleName;
    }
    return errorStatus;
}

HC05_ErrorStatus_t hc05_recieveBufferSyncZeroCopy(Hc05Info_t module, pu8 buffer, u16 bufferSize)
{
    HC05_ErrorStatus_t errorStatus = hc05_retNotOk;
    if(module >= 0 && (module <= (sizeof(hc05ConfigArr) / sizeof(HC05Cfg_t))))
    {
        if(buffer)
        {
            if(hc05ConfigArr[module].considerStatePin)
            {
                u8 state;
                if(gpio_getPinValue(hc05ConfigArr[module].statePin.port
                    , hc05ConfigArr[module].statePin.pin, &state) == gpio_retOk)
                {
                    if(state == gpioVal_SET)
                    {
                        if(usart_recieveBufferSyncZeroCopy(hc05ConfigArr[module].usartId
                            , buffer, bufferSize) == usart_retOk)
                        {
                            errorStatus = hc05_retOk;
                        }
                    }
                    else
                    {
                        errorStatus = hc05_retModuleNotPaired;
                    }
                }
            }
            else
            {
                if(usart_recieveBufferSyncZeroCopy(hc05ConfigArr[module].usartId
                    , buffer, bufferSize) == usart_retOk)
                {
                    errorStatus = hc05_retOk;
                }
            }
        }
        else
        {
            errorStatus = hc05_retNullPointer;
        }
    }
    else
    {
        errorStatus = hc05_retInvalidModuleName;
    }
    return errorStatus;
}

HC05_ErrorStatus_t hc05_sendBufferSyncZeroCopy(Hc05Info_t module, pu8 buffer, u16 bufferSize)
{
    HC05_ErrorStatus_t errorStatus = hc05_retNotOk;
    if(module >= 0 && (module <= (sizeof(hc05ConfigArr) / sizeof(HC05Cfg_t))))
    {
        if(buffer)
        {
            if(hc05ConfigArr[module].considerStatePin)
            {
                u8 state;
                if(gpio_getPinValue(hc05ConfigArr[module].statePin.port
                    , hc05ConfigArr[module].statePin.pin, &state) == gpio_retOk)
                {
                    if(state == gpioVal_SET)
                    {
                        if(usart_sendBufferSyncZeroCopy(hc05ConfigArr[module].usartId
                            , buffer, bufferSize) == usart_retOk)
                        {
                            errorStatus = hc05_retOk;
                        }
                    }
                    else
                    {
                        errorStatus = hc05_retModuleNotPaired;
                    }
                }
            }
            else
            {
                if(usart_sendBufferSyncZeroCopy(hc05ConfigArr[module].usartId
                    , buffer, bufferSize) == usart_retOk)
                {
                    errorStatus = hc05_retOk;
                }
            }
        }
        else
        {
            errorStatus = hc05_retNullPointer;
        }
    }
    else
    {
        errorStatus = hc05_retInvalidModuleName;
    }
    return errorStatus;
}
