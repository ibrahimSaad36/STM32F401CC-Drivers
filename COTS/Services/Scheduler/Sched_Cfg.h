/*******************************************************************
*   File name:    Sched_Cfg.h
*   Author:       Ibrahim Saad
*   Description:  This file contains all declarations for the configuration of the Sched module
*   Version: v1.0
*******************************************************************/

#ifndef SCHED_CFG_H
#define SCHED_CFG_H

#include "../../LIB/Std_types.h"

#define MAX_TASK_NUMBER     3
#define SCHED_TICK_MS       1   /* tick time of sched in milli seconds*/

typedef void (*TaskCallBack_t)(void);

typedef struct
{
    const pu8 name;
    u16 periodMs;
    TaskCallBack_t taskCallBack;
    u16 startDelayMs;
}TaskInfo_t;


#endif
