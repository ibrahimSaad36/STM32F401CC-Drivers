/**
 * @file HC05_Runnable.h
 * @author Ibrahim Saad
 * @brief 
 * @version 0.1
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef HC05_RUNNABLE_H
#define HC05_RUNNABLE_H

#include "../../COTS/LIB/Std_types.h"
#include "../../COTS/HAL/HC-05/HC_05.h"

void hc05_runnable(void);
u8 hc05_getCommand();

#endif      /* HC05_RUNNABLE_H */
