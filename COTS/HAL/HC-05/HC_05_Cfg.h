/**
 * @file HC_05_Cfg.h
 * @author Ibrahim Saad
 * @brief This file contains all configurations needed to deal with hc05 bluetooth 
          module
 * @version 0.1
 * @date 2023-06-16
 * @copyright Copyright (c) 2023
 */

#ifndef HC_05_CFG_H
#define HC_05_CFG_H

#include "../../LIB/Std_types.h"
#include "../../MCAL/USART/STM_USART.h"
#include "../../MCAL/GPIO/STM_GPIO.h"

typedef enum
{
    FALSE = 0,
    TRUE = 1,
}Boolean_t;

typedef enum
{
    rcCarHc05,
    hc05Counts,
}Hc05Info_t;

typedef struct
{
    usartConfig_t usartCfg;
    gpioCfg_t TX, RX, statePin;
    u32 usartId;
    Boolean_t considerStatePin;
}HC05Cfg_t;

#endif      /* HC_05_CFG_H */