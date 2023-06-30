/**
 * @file L298N_Runnable.c
 * @author Ibrahim Saad
 * @brief 
 * @version 0.1
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "L298N_Runnable.h"

typedef L298N_ErrorStatus_t (*motorPreviousStateCbf) (L298NInfo_t);

enum
{
    motorInitialState,
    motorStarted,
}motorState = motorInitialState;

static motorPreviousStateCbf previousStateCbf = NULL;

void l298n_runnable(void)
{
    if(motorState == motorInitialState)
    {
        l298n_stop(l298nRcCar);
        previousStateCbf = l298n_stop;
        motorState = motorStarted;
    }
    else
    {
        u8 bluetoothCmd = hc05_getCommand();
        switch(bluetoothCmd)
        {
            case 'F':
                previousStateCbf = l298n_forward;
                l298n_forward(l298nRcCar);
                break;
            case 'B':
                previousStateCbf = l298n_backward;
                l298n_backward(l298nRcCar);
                break;
            case 'R':
                previousStateCbf = l298n_turnRight;
                l298n_turnRight(l298nRcCar);
                break;
            case 'L':
                previousStateCbf = l298n_turnLeft;
                l298n_turnLeft(l298nRcCar);
                break;
            default:
                previousStateCbf(l298nRcCar);
        }
    }
}
