/*******************************************************************************************
*   File name:    STM_GPIO.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs definitions to deal with GPIO peripherals
*                 features in STM32F401
*   Version: v1.0
********************************************************************************************/

#include "STM_GPIO.h"

#define CAST_GPIO_REGS(reg)         ((volatile GPIOxRegisters_t* const) reg)

#define GET_MODE_PINS(MODE)         (MODE & 0b00000011)
#define GET_OTYPE_PIN(MODE)         ((MODE & 0b00000100) >> 2)
#define GET_PUPD_PINS(MODE)         ((MODE & 0b00011000) >> 3)

#define MSK_CHECK_VALID_PORT        0x000000FF
#define MSK_VALID_PORT              0x000000CC
#define MSK_CLR_CHECK_VALID_PORT    0xFFFFFF00

#define MSK_CHECK_VALID_PIN         0xF0
#define MSK_VALID_PIN               0xE0
#define MSK_CLR_CHECK_VALID_PIN     0x0F

#define MSK_CHECK_VALID_SPEED       0xF0
#define MSK_VALID_SPEED             0xA0
#define MSK_CLR_CHECK_VALID_SPEED   0x0F

#define MSK_CHECK_VALID_MODE        0b11100000
#define MSK_VALID_MODE              0b11100000

#define MSK_CHECK_VALID_AF          0xF0
#define MSK_VALID_AF                0xF0
#define MSK_CLR_CHECK_VALID_AF      0x0F

#define MSK_MODE_CLR                0b11
#define MSK_OTYPE_CLR               0b1
#define MSK_PUPD_CLR                0b11
#define MSK_SPEED_CLR               0b11
#define MSK_AF_CLR                  0b1111

#define REG_ONES                    0xFFFFFFFF

#define MODE_OUTPUT                 1
#define RESET_BIT_SHIFT             16

#define PINx_SHIFT_2BITS            2U
#define PINx_SHIFT_4BITS            4U

typedef struct
{
    /* GPIOA Base Address: 0x40020000 */
    /* GPIOB Base Address: 0x40020400 */
    /* GPIOC Base Address: 0x40020800 */
    /* GPIOD Base Address: 0x40020C00 */
    /* GPIOE Base Address: 0x40021000 */
    /* GPIOH Base Address: 0x40021C00 */
    u32 GPIOx_MODER;
    u32 GPIOx_OTYPER;
    u32 GPIOx_OSPEEDR;
    u32 GPIOx_PUPDR;
    u32 GPIOx_IDR;
    u32 GPIOx_ODR;
    u32 GPIOx_BSRR;
    u32 GPIOx_LCKR;
    u32 GPIOx_AFRL;
    u32 GPIOx_AFRH;
}GPIOxRegisters_t;

GPIO_ErrorStatus_t gpio_initPin(gpioCfg_t* pinCfg)
{
    GPIO_ErrorStatus_t errorStatus = gpio_retNotOk;
    if((pinCfg->port & MSK_CHECK_VALID_PORT) == MSK_VALID_PORT)
    {
        if((pinCfg->pin & MSK_CHECK_VALID_PIN) == MSK_VALID_PIN)
        {
            if((pinCfg->speed & MSK_CHECK_VALID_SPEED) == MSK_VALID_SPEED)
            {
                if((pinCfg->mode & MSK_CHECK_VALID_MODE) == MSK_VALID_MODE)
                {
                    u32 temp, port;
                    u8 speed, pin, mode, otype, pupd;
                    speed = pinCfg->speed & MSK_CLR_CHECK_VALID_SPEED;
                    pin = pinCfg->pin & MSK_CLR_CHECK_VALID_PIN;
                    port = pinCfg->port & MSK_CLR_CHECK_VALID_PORT;
                    temp = CAST_GPIO_REGS(port)->GPIOx_MODER;
                    temp &= ~(MSK_MODE_CLR << (pin * PINx_SHIFT_2BITS));
                    mode = GET_MODE_PINS(pinCfg->mode);
                    temp |= (mode << (pin * PINx_SHIFT_2BITS));
                    CAST_GPIO_REGS(port)->GPIOx_MODER = temp;
                    temp = CAST_GPIO_REGS(port)->GPIOx_OTYPER;
                    temp &= ~(MSK_OTYPE_CLR << pin);
                    otype = GET_OTYPE_PIN(pinCfg->mode);
                    temp |= (otype << pin);
                    CAST_GPIO_REGS(port)->GPIOx_OTYPER = temp;
                    if(mode == MODE_OUTPUT)
                    {
                        temp = CAST_GPIO_REGS(port)->GPIOx_OSPEEDR;
                        temp &= ~(MSK_SPEED_CLR << (pin * PINx_SHIFT_2BITS));
                        temp |= (speed << (pin * PINx_SHIFT_2BITS));
                        CAST_GPIO_REGS(port)->GPIOx_OSPEEDR = temp;
                    }
                    temp = CAST_GPIO_REGS(port)->GPIOx_PUPDR;
                    temp &= ~((MSK_PUPD_CLR << (pin * PINx_SHIFT_2BITS)));
                    pupd = GET_PUPD_PINS(pinCfg->mode);
                    temp |= (pupd << (pin * PINx_SHIFT_2BITS));
                    CAST_GPIO_REGS(port)->GPIOx_PUPDR = temp;
                    errorStatus = gpio_retOk;
                }
                else
                {
                    errorStatus = gpio_retInvalidMode;
                }
            }
            else
            {
                errorStatus = gpio_retInvalidSpeed;
            }
        }
        else
        {
            errorStatus = gpio_retInvalidPin;
        }
    }
    else
    {
        errorStatus = gpio_retInvalidPort;
    }
    return errorStatus;
}

GPIO_ErrorStatus_t gpio_writePinValue(u32 gpioPort, u8 pin, u8 value)
{
    GPIO_ErrorStatus_t errorStatus = gpio_retNotOk;
    if((gpioPort & MSK_CHECK_VALID_PORT) == MSK_VALID_PORT)
    {
        if((pin & MSK_CHECK_VALID_PIN) == MSK_VALID_PIN)
        {
            u32 port = gpioPort & MSK_CLR_CHECK_VALID_PORT;
            pin &= MSK_CLR_CHECK_VALID_PIN;
            if(value == gpioVal_SET || value == HIGH)
            {
                CAST_GPIO_REGS(port)->GPIOx_BSRR |= (1 << pin);
                errorStatus = gpio_retOk;
            }
            else if(value == gpioVal_RESET || value == LOW)
            {
                CAST_GPIO_REGS(port)->GPIOx_BSRR |= (1 << (pin + RESET_BIT_SHIFT));
                errorStatus = gpio_retOk;
            }
            else
            {
                errorStatus = gpio_retNotOk;
            }
        }
        else
        {
            errorStatus = gpio_retInvalidPin;
        }
    }
    else
    {
        errorStatus = gpio_retInvalidPort;
    }
    return errorStatus;
}

GPIO_ErrorStatus_t gpio_getPinValue(u32 gpioPort, u8 pin, pu8 value)
{
    GPIO_ErrorStatus_t errorStatus = gpio_retNotOk;
    u8 retValue;
    if((gpioPort & MSK_CHECK_VALID_PORT) == MSK_VALID_PORT)
    {
        if((pin & MSK_CHECK_VALID_PIN) == MSK_VALID_PIN)
        {
            if(value)
            {
                u32 port = gpioPort & MSK_CLR_CHECK_VALID_PORT;
                pin &= MSK_CLR_CHECK_VALID_PIN;
                retValue = ((CAST_GPIO_REGS(port)->GPIOx_IDR & (1 << pin)) >> pin);
                *value = retValue;
                errorStatus = gpio_retOk;
            }
            else
            {
                errorStatus = gpio_retNullPointer;
            }
        }
        else
        {
            errorStatus = gpio_retInvalidPin;
        }
    }
    else
    {
        errorStatus = gpio_retInvalidPort;
    }
    return errorStatus;
}

GPIO_ErrorStatus_t gpio_selectAlternativeFunction(u32 gpioPort, u32 pin, u32 aFunction)
{
    GPIO_ErrorStatus_t errorStatus = gpio_retNotOk;
    if((gpioPort & MSK_CHECK_VALID_PORT) == MSK_VALID_PORT)
    {
        if((pin & MSK_CHECK_VALID_PIN) == MSK_VALID_PIN)
        {
            if((aFunction & MSK_CHECK_VALID_AF) == MSK_VALID_AF)
            {
                u32 temp, port = gpioPort & MSK_CLR_CHECK_VALID_PORT;
                aFunction &= MSK_CLR_CHECK_VALID_AF;
                pin &= MSK_CLR_CHECK_VALID_PIN;
                if(pin >= 0 && pin <= 7)
                {
                    temp = CAST_GPIO_REGS(port)->GPIOx_AFRL;
                    temp &= ~(MSK_AF_CLR << (PINx_SHIFT_4BITS * pin));
                    temp |= aFunction << (PINx_SHIFT_4BITS * pin);
                    CAST_GPIO_REGS(port)->GPIOx_AFRL = temp;
                }
                else
                {
                    temp = CAST_GPIO_REGS(port)->GPIOx_AFRH;
                    temp &= ~(MSK_AF_CLR << (PINx_SHIFT_4BITS * (pin % 8)));
                    temp |= aFunction << (PINx_SHIFT_4BITS * (pin % 8));
                    CAST_GPIO_REGS(port)->GPIOx_AFRH = temp;
                }
                errorStatus = gpio_retOk;
            }
            else
            {
                errorStatus = gpio_retInvalidAF;
            }
        }
        else
        {
            errorStatus = gpio_retInvalidPin;
        }
    }
    else
    {
        errorStatus = gpio_retInvalidPort;
    }
    return errorStatus;
}

GPIO_ErrorStatus_t gpio_writePort(u32 gpioPort, u16 value)
{
    GPIO_ErrorStatus_t errorStatus = gpio_retNotOk;
    u32 assignValue = 0x0000FFFF & value;
    if((gpioPort & MSK_CHECK_VALID_PORT) == MSK_VALID_PORT)
    {
        u32 port = gpioPort & MSK_CLR_CHECK_VALID_PORT;
        CAST_GPIO_REGS(port)->GPIOx_ODR = assignValue;
        errorStatus = gpio_retOk;
    }
    else
    {
        errorStatus = gpio_retInvalidPort;
    }
    return errorStatus;
}

GPIO_ErrorStatus_t gpio_readPort(u32 gpioPort, pu16 value)
{
    GPIO_ErrorStatus_t errorStatus = gpio_retNotOk;
    if((gpioPort & MSK_CHECK_VALID_PORT) == MSK_VALID_PORT)
    {
        if(value)
        {
            u32 port = gpioPort & MSK_CLR_CHECK_VALID_PORT;
            *value = CAST_GPIO_REGS(port)->GPIOx_IDR;
            errorStatus = gpio_retOk;
        }
        else
        {
            errorStatus = gpio_retNullPointer;
        }
    }
    else
    {
        errorStatus = gpio_retInvalidPort;
    }
    return errorStatus;
}

GPIO_ErrorStatus_t gpio_togglePin(u32 gpioPort, u8 pin)
{
    GPIO_ErrorStatus_t errorStatus = gpio_retNotOk;
    if((gpioPort & MSK_CHECK_VALID_PORT) == MSK_VALID_PORT)
    {
        if((pin & MSK_CHECK_VALID_PIN) == MSK_VALID_PIN)
        {
            u32 port = gpioPort & MSK_CLR_CHECK_VALID_PORT;
            pin &= MSK_CLR_CHECK_VALID_PIN;
            CAST_GPIO_REGS(port)->GPIOx_ODR ^= (1 << pin);
            errorStatus = gpio_retOk;
        }
        else
        {
            errorStatus = gpio_retInvalidPin;
        }
    }
    else
    {
        errorStatus = gpio_retInvalidPort;
    }
    return errorStatus;
}
