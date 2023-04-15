/*******************************************************************
*   File name:    Sched_Cfg.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all configurations of the Sched module
*   Version: v1.0
*******************************************************************/

#include "Sched_Cfg.h"

extern void toggleLED1(void);
extern void toggleLED2(void);
extern void switchTask(void);

const TaskInfo_t tasksInfo [MAX_TASK_NUMBER]
    = {
            [0] = {
                        .name = "First LED Toggle",
                        .periodMs = 250,
                        .startDelayMs = 250,
                        .taskCallBack = toggleLED1
                  },
            [1] = {
                        .name = "Second LED Toggle",
                        .periodMs = 500,
                        .startDelayMs = 500,
                        .taskCallBack = toggleLED2
                  },
            [2] = {
                        .name = "Switch Debouncing",
                        .periodMs = 20,
                        .startDelayMs = 0,
                        .taskCallBack = switchTask
                  },
      };
