/**
 * @file L298N.c
 * @author Ibrahim Saad
 * @brief This file contains all definitions needed to deal with L298N DC motor driver
 * @version 0.1
 * @copyright Copyright (c) 2023
 * 
 */

#include "L298N.h"

const L298N_Cfg l298nCfgArr [l298nCount];

L298N_ErrorStatus_t l298n_init(void)
{
    L298N_ErrorStatus_t errorStatus = l298n_retNotOk;
    GPIO_ErrorStatus_t gpioErrorStatus = gpio_retNotOk;
    u8 i, size = sizeof(l298nCfgArr) / sizeof(l298nCfgArr[0]);
    for(i = 0; (i < size) && (gpioErrorStatus == gpio_retOk); i++)
    {
        gpioErrorStatus = gpio_initPin(&l298nCfgArr[i].MotorA.out1);
        gpioErrorStatus = gpio_initPin(&l298nCfgArr[i].MotorA.out2);
        gpioErrorStatus = gpio_initPin(&l298nCfgArr[i].MotorB.out1);
        gpioErrorStatus = gpio_initPin(&l298nCfgArr[i].MotorB.out2);
    }
    if(gpioErrorStatus == gpio_retOk)
    {
        errorStatus = l298n_retOk;
    }
    return errorStatus;
}

L298N_ErrorStatus_t l298n_forward(L298NInfo_t module)
{
    L298N_ErrorStatus_t errorStatus = l298n_retNotOk;
    GPIO_ErrorStatus_t gpioErrorStatus = gpio_retNotOk;
    if(module >= 0 && module <= (sizeof(l298nCfgArr) / sizeof(l298nCfgArr[0])))
    {
        gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorA.out1.port, l298nCfgArr[module].MotorA.out1.pin, HIGH);
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorA.out2.port, l298nCfgArr[module].MotorA.out2.pin, LOW );
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorB.out1.port, l298nCfgArr[module].MotorB.out1.pin, HIGH);
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorB.out2.port, l298nCfgArr[module].MotorB.out2.pin, LOW);
        if(gpioErrorStatus == gpio_retOk)
        {
            errorStatus = l298n_retNotOk;
        }
    }
    else
    {
        errorStatus = l298n_retInvalidModuleName;
    }
    return errorStatus;
}

L298N_ErrorStatus_t l298n_backward(L298NInfo_t module)
{
    L298N_ErrorStatus_t errorStatus = l298n_retNotOk;
    GPIO_ErrorStatus_t gpioErrorStatus = gpio_retOk;
    if(module >= 0 && module <= (sizeof(l298nCfgArr) / sizeof(l298nCfgArr[0])))
    {
        gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorA.out1.port, l298nCfgArr[module].MotorA.out1.pin, LOW);
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorA.out2.port, l298nCfgArr[module].MotorA.out2.pin, HIGH);
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorB.out1.port, l298nCfgArr[module].MotorB.out1.pin, LOW);
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorB.out2.port, l298nCfgArr[module].MotorB.out2.pin, HIGH);
        if(gpioErrorStatus == gpio_retOk)
        {
            errorStatus = l298n_retNotOk;
        }
    }
    else
    {
        errorStatus = l298n_retInvalidModuleName;
    }
    return errorStatus;
}

L298N_ErrorStatus_t l298n_turnRight(L298NInfo_t module)
{
    L298N_ErrorStatus_t errorStatus = l298n_retNotOk;
    GPIO_ErrorStatus_t gpioErrorStatus = gpio_retNotOk;
    if(module >= 0 && module <= (sizeof(l298nCfgArr) / sizeof(l298nCfgArr[0])))
    {
        gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorA.out1.port, l298nCfgArr[module].MotorA.out1.pin, HIGH);
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorA.out2.port, l298nCfgArr[module].MotorA.out2.pin, LOW );
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorB.out1.port, l298nCfgArr[module].MotorB.out1.pin, LOW);
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorB.out2.port, l298nCfgArr[module].MotorB.out2.pin, HIGH);
        if(gpioErrorStatus == gpio_retOk)
        {
            errorStatus = l298n_retNotOk;
        }
    }
    else
    {
        errorStatus = l298n_retInvalidModuleName;
    }
    return errorStatus;
}

L298N_ErrorStatus_t l298n_turnLeft(L298NInfo_t module)
{
    L298N_ErrorStatus_t errorStatus = l298n_retNotOk;
    GPIO_ErrorStatus_t gpioErrorStatus = gpio_retNotOk;
    if(module >= 0 && module <= (sizeof(l298nCfgArr) / sizeof(l298nCfgArr[0])))
    {
        gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorA.out1.port, l298nCfgArr[module].MotorA.out1.pin, LOW);
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorA.out2.port, l298nCfgArr[module].MotorA.out2.pin, HIGH);
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorB.out1.port, l298nCfgArr[module].MotorB.out1.pin, HIGH);
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorB.out2.port, l298nCfgArr[module].MotorB.out2.pin, LOW);
        if(gpioErrorStatus == gpio_retOk)
        {
            errorStatus = l298n_retNotOk;
        }
    }
    else
    {
        errorStatus = l298n_retInvalidModuleName;
    }
    return errorStatus;
}

L298N_ErrorStatus_t l298n_stop(L298NInfo_t module)
{
    L298N_ErrorStatus_t errorStatus = l298n_retNotOk;
    GPIO_ErrorStatus_t gpioErrorStatus = gpio_retNotOk;
    if(module >= 0 && module <= (sizeof(l298nCfgArr) / sizeof(l298nCfgArr[0])))
    {
        gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorA.out1.port, l298nCfgArr[module].MotorA.out1.pin, LOW);
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorA.out2.port, l298nCfgArr[module].MotorA.out2.pin, LOW);
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorB.out1.port, l298nCfgArr[module].MotorB.out1.pin, LOW);
	    gpioErrorStatus = gpio_writePinValue(l298nCfgArr[module].MotorB.out2.port, l298nCfgArr[module].MotorB.out2.pin, LOW);
        if(gpioErrorStatus == gpio_retOk)
        {
            errorStatus = l298n_retNotOk;
        }
    }
    else
    {
        errorStatus = l298n_retInvalidModuleName;
    }
    return errorStatus;
}
