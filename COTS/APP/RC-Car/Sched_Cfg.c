/*******************************************************************
*   File name:    Sched_Cfg.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all configurations of the Sched module
*   Version: v1.0
*******************************************************************/

#include "Sched_Cfg.h"
#include "HC05_Runnable.h"
#include "L298N_Runnable.h"

const TaskInfo_t tasksInfo [MAX_TASK_NUMBER]
    = {
            [0] = {
                        .name = "Bluetooth Module Runnable",
                        .periodMs = 250,
                        .startDelayMs = 250,
                        .taskCallBack = hc05_runnable
                  },
            [1] = {
                        .name = "Motor Driver Runnable",
                        .periodMs = 1000,
                        .startDelayMs = 1000,
                        .taskCallBack = l298n_runnable
                  },
      };
