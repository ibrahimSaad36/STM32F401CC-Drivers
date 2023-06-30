/**
 * @file main.c
 * @author Ibrahim Saad
 * @brief 
 * @version 0.1
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../../COTS/MCAL/RCC/STM_RCC.h"
#include "../../COTS/MCAL/GPIO/STM_GPIO.h"
#include "../../COTS/HAL/HC-05/HC_05.h"
#include "../../COTS/HAL/L298N/L298N.h"
#include "L298N_Runnable.h"
#include "Sched.h"

int main(int argc, char* argv[])
{
    /* system init */
    rcc_selectSystemClock(systemClock_HSE);
    rcc_enablePeripheral(peripheralBus_AHB1, peripheralGPIOA);
    rcc_enablePeripheral(peripheralBus_AHB1, peripheralGPIOB);
    rcc_enablePeripheral(peripheralBus_AHB1, peripheralGPIOC);
    /* drivers init */
    hc05_init();
    l298n_init();
    /* sched takes the control */
    sched_init();
    sched_start();
    return 0;
}
