/*******************************************************************
*   File name:    Sched.h
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIS of the Sched module
*   Version: v1.0
*******************************************************************/

#ifndef SCHED_H
#define SCHED_H

#include "Sched_Cfg.h"
#include "SysTick.h"

typedef struct 
{
    const TaskInfo_t* taskInfo;
    u16 remainingTimeMs;
}Runnable_Handler_t;


typedef enum {
    sched_retNotOk = 0,
    sched_retOk,
    sched_retInvalidTick,
}Sched_ErrorStatus_t;

Sched_ErrorStatus_t sched_init(void);
void sched_start(void);
Sched_ErrorStatus_t sched_pauseRunnable(Runnable_Handler_t runnable, u16 pauseTimeMs);

#endif
