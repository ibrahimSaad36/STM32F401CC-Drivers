/**
 * @file L298N_Cfg.c
 * @author Ibrahim Saad
 * @brief This file contains all definitions for the configurations needed to deal with L298N DC motor driver
 * @version 0.1
 * @copyright Copyright (c) 2023
 * 
 */

#include "L298N_Cfg.h"

const L298N_Cfg l298nCfgArr [l298nCount] =
            {
                    [l298nRcCar]    = 
                                        {
                                            .MotorA = {
                                                            .out1 = {
                                                                        .port = gpioPortB,
                                                                        .pin = PIN2,
                                                                        .speed = speedHigh,
                                                                        .mode = modeOutPP,
                                                                    },
                                                            .out2 = {
                                                                        .port = gpioPortB,
                                                                        .pin = PIN3,
                                                                        .speed = speedHigh,
                                                                        .mode = modeOutPP,
                                                                    },
                                                      },
                                            .MotorB = {
                                                            .out1 = {
                                                                        .port = gpioPortB,
                                                                        .pin = PIN4,
                                                                        .speed = speedHigh,
                                                                        .mode = modeOutPP,
                                                                    },
                                                            .out2 = {
                                                                        .port = gpioPortB,
                                                                        .pin = PIN5,
                                                                        .speed = speedHigh,
                                                                        .mode = modeOutPP,
                                                                    },
                                                      },
                                        }
            };
