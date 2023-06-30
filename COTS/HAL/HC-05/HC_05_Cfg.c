/**
 * @file HC_05_Cfg.c
 * @author Ibrahim Saad
 * @brief This file contains all configurations definitions needed to deal with hc05 bluetooth 
          module
 * @version 0.1
 * @date 2023-06-16
 * @copyright Copyright (c) 2023
 */

#include "HC_05_Cfg.h"

/* Note:
        * For USART2: CTS on (PA0, PD3), RTS on (PA1, PD4), TX on (PA2, PD5), RX on (PA3, PD6), CK on (PA4, PD7)
        * For USART6: TX on (PC6, PA11), RX on (PC7, PA12), CK on PC8
        * FOR USART1: RX on (PB7, PA10), TX on (PB6, PA9), CTS on (PA11,), RTS on (PA12, ) , CK on (PA8)
        * AF7  (USART1-2)
        * AF8  (USART6)
*/

const HC05Cfg_t hc05ConfigArr [hc05Counts]
    = {
            [rcCarHc05] = {
                                .usartCfg = {
                                                .usartMode = usartMode_Async,
                                                .baud = 9600,
                                                .dataSize = dataSize_8,
                                                .overSamplingMode = oversampling_8,
                                                .pahse = phase_FirstTrans,
                                                .parity = parity_NONE,
                                                .polarity = polarity_SteadyHigh,
                                                .stopBit = stopBit_ONE,
                                                .usartDirection = usartDirection_TXRX,
                                                .wakeUpMode = wakeUp_IDLE,
                                            },
                                
                                .RX =       {
                                                .port = gpioPortB,
                                                .pin = PIN7,
                                                .speed = speedHigh,
                                                .mode = modeAF_PP_PU,
                                            },
                                .TX =       {
                                                .port = gpioPortB,
                                                .pin = PIN6,
                                                .speed = speedHigh,
                                                .mode = modeAF_PP,
                                            },
                                .usartId = usartId_1,
                                .considerStatePin = FALSE,
                                
                                /* Only statePin is considered if considerStatePin = TRUE */
                                .statePin = {
                                                .port = gpioPortA,
                                                .pin = PIN0,
                                                .speed = speedMedium,
                                                .mode = modeInpPullDown,
                                            },
                          }
      };