/*******************************************************************
*   File name:    Sched.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIS definitions of the Sched module
*   Version: v1.0
*******************************************************************/

#include "Sched.h"

extern const TaskInfo_t tasksInfo [MAX_TASK_NUMBER];
Runnable_Handler_t tasksHandlers [MAX_TASK_NUMBER];

static volatile u8 osFlag;

static void sched_callback(void);
static void sched_run(void);

Sched_ErrorStatus_t sched_init(void)
{
    Sched_ErrorStatus_t errorStatus = sched_retNotOk;
    SYSTICK_ErrorStatus_t systickErrorStatus;
    u8 iterator;
    for(iterator = 0; iterator < MAX_TASK_NUMBER; iterator++)
    {
        tasksHandlers[iterator].taskInfo = &tasksInfo[iterator];
        tasksHandlers[iterator].remainingTimeMs = tasksInfo[iterator].startDelayMs;
    }
    systickErrorStatus = systick_setCallBack(sched_callback);
    systickErrorStatus = systick_setReloadMS(SCHED_TICK_MS);
    switch (systickErrorStatus)
    {
        case systick_retOk:
            errorStatus = sched_retOk;
            break;
        case systic_retInvalidReloadValue:
            errorStatus = sched_retInvalidTick;
			break;
        default:
            errorStatus = sched_retNotOk;
    }
    return errorStatus;
}

void sched_start(void)
{
    systick_start();
    while (1)
    {
        if(osFlag)
        {
            sched_run();
            osFlag = 0;
        }
    }
}

Sched_ErrorStatus_t sched_pauseRunnable(Runnable_Handler_t runnable, u16 pauseTimeMs)
{}

static void sched_callback(void)
{
    if(osFlag == 0)
    {
        osFlag = 1;
    }
    else
    {
        /* CPU load over 100% */
    }
}

static void sched_run(void)
{
    u8 iterator;
    for(iterator = 0; iterator < MAX_TASK_NUMBER; iterator++)
    {
        if(tasksHandlers[iterator].remainingTimeMs == 0)
        {
            if(tasksHandlers[iterator].taskInfo->taskCallBack)
            {
                tasksHandlers[iterator].taskInfo->taskCallBack();
                tasksHandlers[iterator].remainingTimeMs = tasksHandlers[iterator].taskInfo->periodMs;
            }
        }
        tasksHandlers[iterator].remainingTimeMs -= SCHED_TICK_MS;
    }
}
