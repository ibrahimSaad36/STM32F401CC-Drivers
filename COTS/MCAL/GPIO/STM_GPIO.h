/*******************************************************************************************
*   File name:    STM_GPIO.h
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs needed to deal with GPIO peripherals
*                 features in STM32F401
*   Version: v1.0
********************************************************************************************/

#ifndef STM_GPIO_H
#define STM_GPIO_H

#include "../../LIB/Std_types.h"

#define gpioPortA           0x400200CC
#define gpioPortB           0x400204CC
#define gpioPortC           0x400208CC
#define gpioPortD           0x40020CCC
#define gpioPortE           0x400210CC
#define gpioPortH           0x40021CCC

#define PIN0                0xE0
#define PIN1                0xE1
#define PIN2                0xE2
#define PIN3                0xE3
#define PIN4                0xE4
#define PIN5                0xE5
#define PIN6                0xE6
#define PIN7                0xE7
#define PIN8                0xE8
#define PIN9                0xE9
#define PIN10               0xEA
#define PIN11               0xEB
#define PIN12               0xEC
#define PIN13               0xED
#define PIN14               0xEE
#define PIN15               0xEF

#define aFunction0          0xF0        /* AF0  (system) */
#define aFunction1          0xF1        /* AF1  (TIM1/TIM2) */
#define aFunction2          0xF2        /* AF2  (TIM3-5) */
#define aFunction3          0xF3        /* AF3  (TIM9-11) */
#define aFunction4          0xF4        /* AF4  (I2C1-3) */
#define aFunction5          0xF5        /* AF5  (SPI1-4) */
#define aFunction6          0xF6        /* AF6  (SPI3) */
#define aFunction7          0xF7        /* AF7  (USART1-2) */
#define aFunction8          0xF8        /* AF8  (USART6)*/
#define aFunction9          0xF9        /* AF9  (I2C2-3) */
#define aFunction10         0xFA        /* AF10 (OTG_FS) */
#define aFunction11         0xFB        /* AF11  */
#define aFunction12         0xFC        /* AF12 (SDIO) */
#define aFunction13         0xFD        /* AF13 */
#define aFunction14         0xFE        /* AF14 */
#define aFunction15         0xFF        /* AF15 (EVENTOUT) */

#define speedLow            0xA0
#define speedMedium         0xA1
#define speedHigh           0xA2
#define speedVeryHigh       0xA3

#define modeInpFloat        0b11100000
#define modeInpPullUp       0b11101000
#define modeInpPullDown     0b11110000
#define modeOutPP           0b11100001
#define modeOutPP_PU        0b11101001
#define modeOutPP_PD        0b11110001
#define modeOutOD           0b11100101
#define modeOutOD_PU        0b11101101
#define modeOutOD_PD        0b11110101
#define modeAF_PP           0b11100010
#define modeAF_PP_PU        0b11101010
#define modeAF_PP_PD        0b11110010
#define modeAF_OD           0b11100110
#define modeAF_OD_PU        0b11101110
#define modeAF_OD_PD        0b11110110
#define modeAnalogInOut     0b11100011

#define HIGH                1
#define LOW                 0

#define gpioVal_SET         1
#define gpioVal_RESET       0

typedef enum
{
    gpio_retNotOk = 0,
    gpio_retOk,
    gpio_retNullPointer,
    gpio_retInvalidPort,
    gpio_retInvalidPin,
    gpio_retInvalidSpeed,
    gpio_retInvalidMode,
    gpio_retInvalidAF,
}GPIO_ErrorStatus_t;

typedef struct
{
    u32 port;
    u32 pin;
    u32 speed;
    u32 mode;
}gpioCfg_t;

/* Default of all I/O pins is input push pull*/

/**********************************************************
    Description:       This function is used to init a GPIO pin

    Input parameters:  A valid pointer of gpioCfg_t (Not NULL) with valid values for port, pin, mode, and speed

    Return:            Returns GPIO_ErrorStatus_t
                       - gpio_retInvalidPort (if the port i invalid, valid ports are gpioPortA, gpioPortB, gpioPortC, 
                         gpioPortD, gpioPortE, gpioPortH)
                       - gpio_retInvalidPin (if the pin is invalid, valid pins are from PIN0 to PIN16)
                       - gpio_retInvalidSpeed (if the speed is invalid, valid speeds are speedLow, speedMedium
                         , speedHigh, speedVeryHigh)
                       - gpio_retInvalidMode (if the mode is invalid, valid modes are: modeInpFloat, modeInpPullUp
                         , modeInpPullDown, modeOutPP, modeOutPP_PU, modeOutPP_PD, 
                         modeOutOD, modeOutOD_PU, modeOutOD_PD, modeAF_PP, modeAF_PP_PU, 
                         modeAF_PP_PD, modeAF_OD, modeAF_OD_PU, modeAF_OD_PD, modeAnalogInOut)
                       - gpio_retOk (if the initialization of pin is done successfully)
                       - gpio_retNotOk (if couldn't init a pin)
***********************************************************/
GPIO_ErrorStatus_t gpio_initPin(gpioCfg_t* pinCfg);




/**********************************************************
    Description:       This function is used to write 0 or 1 on a pin

    Input parameters:  Valid inputs for gpioPort are: gpioPortA, gpioPortB, gpioPortC, gpioPortD, gpioPortE, gpioPortH
                       Valid inputs for pin are from PIN0 to PIN15
                       Valid inputs for value are gpioVal_SET or HIGH and gpioVal_RESET or LOW, or 0 and 1

    Return:            Returns GPIO_ErrorStatus_t
                       - gpio_retInvalidPort (if got invalid value for gpioPort)
                       - gpio_retInvalidPin (if got invalid value for pin)
                       - gpio_retNotOk (if got invalid value for value or couldn't write to the pin)
                       - gpio_retOk (if value is written to pin successfully)
***********************************************************/
GPIO_ErrorStatus_t gpio_writePinValue(u32 gpioPort, u8 pin, u8 value);




/**********************************************************
    Description:       This function is used to get the state of pin if 0 or 1

    Input parameters:  Valid inputs for gpioPort are: gpioPortA, gpioPortB, gpioPortC, gpioPortD, gpioPortE, gpioPortH
                       Valid inputs for pin are from PIN0 to PIN15
                       A valid pointer (Not NULL) to store value back in it

    Return:            Returns GPIO_ErrorStatus_t
                       - gpio_retInvalidPort (if got invalid value for gpioPort)
                       - gpio_retInvalidPin (if got invalid value for pin)
                       - gpio_retNullPointer (if got a NULL pointer)
                       - gpio_retOk (if got the value of pin successfully)
                       - gpio_retNotOk (if couldn't get pin value)
                       - return value in pointer may be (HIGH or LOW) or (gpioVal_SET or gpioVal_RESET) or (0 or 1)
***********************************************************/
GPIO_ErrorStatus_t gpio_getPinValue(u32 gpioPort, u8 pin, pu8 value);




/**********************************************************
    Description:       This function is used to select an alternate function for a pin

    Input parameters:  Valid inputs for gpioPort are: gpioPortA, gpioPortB, gpioPortC, gpioPortD, gpioPortE, gpioPortH
                       Valid inputs for pin are from PIN0 to PIN15
                       Valid inputs for aFunction are from aFunction0 to aFunction15

    Return:            Returns GPIO_ErrorStatus_t
                       - gpio_retInvalidPort (if got invalid value for gpioPort)
                       - gpio_retInvalidPin  (if got invalid value for pin)
                       - gpio_retInvalidAF   (if got invalid value for aFunction)
                       - gpio_retOk (if alternate function is selected successfully)
                       - gpio_retNotOk (if couldn't select alternate function)
***********************************************************/
GPIO_ErrorStatus_t gpio_selectAlternativeFunction(u32 gpioPort, u32 pin, u32 aFunction);




/**********************************************************
    Description:       This function is used to write a value on the port

    Input parameters:  Valid inputs for gpioPort are: gpioPortA, gpioPortB, gpioPortC, gpioPortD, gpioPortE, gpioPortH
                       Valid inputs for value are from 0 to 65535 (0xFFFF)

    Return:            Returns GPIO_ErrorStatus_t
                       - gpio_retInvalidPort (if got invalid value for gpioPort)
                       - gpio_retOk (if value is written to the port successfully)
                       - gpio_retNotOk (if couldn't write to the port)        
***********************************************************/
GPIO_ErrorStatus_t gpio_writePort(u32 gpioPort, u16 value);




/**********************************************************
    Description:       This function is used to read a port

    Input parameters:  Valid inputs for gpioPort are: gpioPortA, gpioPortB, gpioPortC, gpioPortD, gpioPortE, gpioPortH
                       A valid pointer (Not NULL) to write the value back in it

    Return:            Returns GPIO_ErrorStatus_t
                       - gpio_retInvalidPort (if got invalid value for gpioPort)
                       - gpio_retNullPointer (if got a NULL pointer)
                       - gpio_retOk (if value is written to the pointer successfully)
                       - gpio_retNotOk (if couldn't read the port)            
***********************************************************/
GPIO_ErrorStatus_t gpio_readPort(u32 gpioPort, pu16 value);




/**********************************************************
    Description:       This function is used to toggle pin

    Input parameters:  Valid inputs for gpioPort are: gpioPortA, gpioPortB, gpioPortC, gpioPortD, gpioPortE, gpioPortH
                       Valid inputs for pin are from PIN0 to PIN15

    Return:            Returns GPIO_ErrorStatus_t
                       - gpio_retInvalidPort (if got invalid value for gpioPort)
                       - gpio_retInvalidPin (if got invalid value for pin)
                       - gpio_retOk (if rhe pin is toggled successfully)
                       - gpio_retNotOk (if couldn't toggle the pin)
***********************************************************/
GPIO_ErrorStatus_t gpio_togglePin(u32 gpioPort, u8 pin);

#endif
