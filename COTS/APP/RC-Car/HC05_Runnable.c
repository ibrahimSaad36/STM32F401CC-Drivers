/**
 * @file HC05_Runnable.c
 * @author Ibrahim Saad
 * @brief 
 * @version 0.1
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "HC05_Runnable.h"

#define PERIOD_TO_PRINT_MODES       60      /* it means 60 * 1000ms >> 60 seconds */

enum
{
    hc05InitialState,
    hc05Ok,
}hc05State = hc05InitialState;

static u8 hc05Cmd;
static u8 sleepCounter;

void hc05_runnable(void)
{
    if(hc05State == hc05InitialState)
    {
        hc05_sendBufferSyncZeroCopy(rcCarHc05, "\n\rSupported Commands Are: \n\r", 28);
        hc05_sendBufferSyncZeroCopy(rcCarHc05, "F: To move car Forward\n\r", 24);
        hc05_sendBufferSyncZeroCopy(rcCarHc05, "B: To move car Backward\n\r", 24);
        hc05_sendBufferSyncZeroCopy(rcCarHc05, "R: To move turn the car Right\n\r", 31);
        hc05_sendBufferSyncZeroCopy(rcCarHc05, "L: To move turn the car Left\n\r", 30);
        hc05State = hc05Ok;
    }
    else
    {
        u8 cmd = 0;
        if(hc05_recieveByteSync(rcCarHc05, &cmd) == hc05_retOk)
        {
            sleepCounter = 0;
            hc05Cmd = cmd;
        }
        else
        {
            sleepCounter++;
            if(sleepCounter == PERIOD_TO_PRINT_MODES)
            {
                hc05_sendBufferSyncZeroCopy(rcCarHc05, "\n\rSupported Commands Are: \n\r", 28);
                hc05_sendBufferSyncZeroCopy(rcCarHc05, "F: To move car Forward\n\r", 24);
                hc05_sendBufferSyncZeroCopy(rcCarHc05, "B: To move car Backward\n\r", 24);
                hc05_sendBufferSyncZeroCopy(rcCarHc05, "R: To move turn the car Right\n\r", 31);
                hc05_sendBufferSyncZeroCopy(rcCarHc05, "L: To move turn the car Left\n\r", 30);
                sleepCounter = 0;
            }
        }
    }
}

u8 hc05_getCommand()
{
    return hc05Cmd;
}
