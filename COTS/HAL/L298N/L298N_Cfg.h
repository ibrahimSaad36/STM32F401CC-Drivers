/**
 * @file L298N_Cfg.h
 * @author Ibrahim Saad
 * @brief This file contains all declaration for the configurations needed to deal with L298N DC motor driver
 * @version 0.1
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef L298N_CFG_H
#define L298N_CFG_H

#include "../../LIB/Std_types.h"
#include "../../MCAL/GPIO/STM_GPIO.h"


typedef struct
{
    struct
    {
        gpioCfg_t out1, out2;
    }MotorA;
    struct
    {
        gpioCfg_t out1, out2;
    }MotorB;
}L298N_Cfg;

typedef enum
{
    l298nRcCar,
    l298nCount,         /* DON'T modify the value of this enumerator or remove it */
}L298NInfo_t;

#endif      /* L298N_CFG_H */