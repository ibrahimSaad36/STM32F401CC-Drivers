/**
 * @file L298N.h
 * @author Ibrahim Saad
 * @brief This file contains all declaration needed to deal with L298N DC motor driver
 * @version 0.1
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef L298N_H
#define L298N_h

#include "L298N_Cfg.h"


typedef enum
{
    l298n_retNotOk = 0,
    l298n_retOk,
    l298n_retInvalidModuleName,
}L298N_ErrorStatus_t;

L298N_ErrorStatus_t l298n_init(void);
L298N_ErrorStatus_t l298n_forward(L298NInfo_t module);
L298N_ErrorStatus_t l298n_backward(L298NInfo_t module);
L298N_ErrorStatus_t l298n_turnRight(L298NInfo_t module);
L298N_ErrorStatus_t l298n_turnLeft(L298NInfo_t module);
L298N_ErrorStatus_t l298n_stop(L298NInfo_t module);

#endif      /* L298N_H */