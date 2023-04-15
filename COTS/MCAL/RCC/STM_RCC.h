/*******************************************************************
*   File name:    STM_RCC.h
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs needed to deal with RCC peripheral
*                 features in STM32F401
*   Version: v1.0
*******************************************************************/
#ifndef STM_RCC_H
#define STM_RCC_H

#include "../../LIB/Std_types.h"

#define systemClock_HSI         0x10000001
#define systemClock_HSE         0x10010000
#define systemClock_PLL         0x11000000
#define pllSource_HSI           0x20000000
#define pllSource_HSE           0x20400000
#define peripheralBus_AHB1      0x30000001
#define peripheralBus_AHB2      0x30000002
#define peripheralBus_APB1      0x30000003
#define peripheralBus_APB2      0x30000004

#define prescalerBus_APB1       0x3A
#define prescalerBus_APB2       0x3D
#define prescalerBus_AHB        0x34

#define busPrescaler_NoPresc    0xF0
#define busPrescaler_Per2       0xF4
#define busPrescaler_Per4       0xF5
#define busPrescaler_Per8       0xF6
#define busPrescaler_Per16      0xF7
#define busPrescaler_Per64      0xF8
#define busPrescaler_Per128     0xF9
#define busPrescaler_Per256     0xFA
#define busPrescaler_Per512     0xFB

#define rtcPrescaler_NoClock    0x0200
#define rtcPrescaler_HSEPer2    0x0202
#define rtcPrescaler_HSEPer3    0x0203
#define rtcPrescaler_HSEPer4    0x0204
#define rtcPrescaler_HSEPer5    0x0205
#define rtcPrescaler_HSEPer6    0x0206
#define rtcPrescaler_HSEPer7    0x0207
#define rtcPrescaler_HSEPer8    0x0208
#define rtcPrescaler_HSEPer9    0x0209
#define rtcPrescaler_HSEPer10   0x020A
#define rtcPrescaler_HSEPer11   0x020B
#define rtcPrescaler_HSEPer12   0x020C
#define rtcPrescaler_HSEPer13   0x020D
#define rtcPrescaler_HSEPer14   0x020E
#define rtcPrescaler_HSEPer15   0x020F
#define rtcPrescaler_HSEPer16   0x0210
#define rtcPrescaler_HSEPer17   0x0211
#define rtcPrescaler_HSEPer18   0x0212
#define rtcPrescaler_HSEPer19   0x0213
#define rtcPrescaler_HSEPer20   0x0214
#define rtcPrescaler_HSEPer21   0x0215
#define rtcPrescaler_HSEPer22   0x0216
#define rtcPrescaler_HSEPer23   0x0217
#define rtcPrescaler_HSEPer24   0x0218
#define rtcPrescaler_HSEPer25   0x0219
#define rtcPrescaler_HSEPer26   0x021A
#define rtcPrescaler_HSEPer27   0x021B
#define rtcPrescaler_HSEPer28   0x021C
#define rtcPrescaler_HSEPer29   0x021D
#define rtcPrescaler_HSEPer30   0x021E
#define rtcPrescaler_HSEPer31   0x021F

/* AHB1 peripherals */
#define peripheralGPIOA         0x40000001
#define peripheralGPIOB         0x40000002
#define peripheralGPIOC         0x40000004
#define peripheralGPIOD         0x40000008
#define peripheralGPIOE         0x40000010
#define peripheralGPIOH         0x40000080
#define peripheralDMA1          0x40200000
#define peripheralDMA2          0x40400000
#define peripheralCRC           0x40001000
/* AHB2 peripherals*/
#define peripheralOTGFS         0x50000080
/* APB1 peripherals*/
#define peripheralTIM2          0x80000001
#define peripheralTIM3          0x80000002
#define peripheralTIM4          0x80000004
#define peripheralTIM5          0x80000008
#define peripheralWWDG          0x80000800
#define peripheralSPI2          0x80004000
#define peripheralSPI3          0x80008000
#define peripheralUSART2        0x80020000
#define peripheralI2C1          0x80200000
#define peripheralI2C2          0x80400000
#define peripheralPWR           0x90000000
/* APB2 peripherals*/
#define peripheralTIM1          0x70000001
#define peripheralUSART1        0x70000010
#define peripheralUSART6        0x70000020
#define peripheralADC1          0x70000100
#define peripheralSDIO          0x70000800
#define peripheralSPI1          0x70001000
#define peripheralSPI4          0x70002000
#define peripheralSYSCFG        0x70004000
#define peripheralTIM9          0x70010000
#define peripheralTIM10         0x70020000
#define peripheralTIM11         0x70040000

typedef enum
{
    rcc_retNotOk = 0,
    rcc_retOk,
    rcc_retInvalidSystemClock,
    rcc_retSelectSystemClockTimeOut,
    rcc_retClockAlreadySelected,
    rcc_retNullPointer,
    rcc_retPLLLocked,
    rcc_retInvalidPLLSource,
    rcc_retInvalidBus,
    rcc_retPeripheralDoesNotBelongToBus,
    rcc_retClockReady,
    rcc_retClockNotReady,
    rcc_retConfigError,
    rcc_retInvalidPrescaler,
}RCC_ErrorStatus_t;

/**********************************************************
    Description:       - This function is used to select system clock

    Input parameters:  Accepted inputs (systemClock_HSI, systemClock_HSE
                       or systemClock_PLL)

    Return:           Retutns: RCC_ErrorStatus_t
                      - rcc_retOk (if clock selected successfully as system clock)
                      - rcc_retInvalidSystemClock (for invalid input from mentioned above)
                      - rcc_retNotOk (if couldn't select clock as system clock)
                      - rcc_retSelectSystemClockTimeOut (if clock didn't be ready within timeout, so can't be selected as system clock)
***********************************************************/
RCC_ErrorStatus_t rcc_selectSystemClock(u32 systemClock);




/**********************************************************
    Description:       This function is used to get system clock

    Input parameters:  A valid pointer (Not NULL) to store the result back in it

    Return:            Retutns: RCC_ErrorStatus_t
                       - rcc_retOk (if the pointer is valid and the system clock stored in it)
                       - rcc_retNullPointer (if the pointer equal to NULL)
                       - rcc_retNotOk (if couldn't get system clock)
***********************************************************/
RCC_ErrorStatus_t rcc_getRunningClock(pu32 runningClock);




/**********************************************************
    Description:       This function is used to check if a clock is ready or not

    Input parameters:  Accepted inputs (systemClock_HSI, systemClock_HSE
                       or systemClock_PLL)

    Return:            Returns RCC_ErrorStatus_t
                       - rcc_retInvalidSystemClock (if got invalid value for clock)
                       - rcc_retClockNotReady (if the clock is not ready, can't be then selected as system clock)
                       - rcc_retClockReady (if the clock is ready, can be then selected as system clock)
                       - rcc_retNotOk (if couldn't check if the clock is ready or not)
***********************************************************/
RCC_ErrorStatus_t rcc_isClockReady(u32 clock);





/**********************************************************
    Description:       This function is used to configure PLL

    Input parameters:  Valid inputs for pllSource are: pllSource_HSI, pllSource_HSE
                       Valid inputs for QVal are from 2 to 15
                       Valid inputs for PVal are from 2 to 8 and must be even
                       Valid inputs for NVal are from 192 to 432
                       Valid inputs for MVal are from 2 to 63

    Return:           Returns RCC_ErrorStatus_t
                      - rcc_retPLLLocked (if pll is running, then it can't be configured, switch to another clock and try again)
                      - rcc_retConfigError (if one or more of config values of Q, P, N, M are different from valid values mentioned above)
                      - rcc_retInvalidPLLSource (if pllSource is different from the valid values mentioned above)
                      - rcc_retOk (if pll is configured successfully based on the passed values to the function)
                      - rcc_retNotOk (if couldn't configure PLL)
***********************************************************/
RCC_ErrorStatus_t rcc_configurePLL(u32 pllSource, u16 QVal, u16 PVal, u16 NVal, u16 MVal);





/**********************************************************
    Description:       This function is used to enable a peripheral

    Input parameters:  Valid inputs for peripheralBus are:
                                                            peripheralBus_AHB1
                                                            peripheralBus_AHB2
                                                            peripheralBus_APB1
                                                            peripheralBus_APB2
                       Valid inputs for peripheral are:
                                                        For AHB1
                                                            peripheralGPIOA 
                                                            peripheralGPIOB 
                                                            peripheralGPIOC 
                                                            peripheralGPIOD 
                                                            peripheralGPIOE 
                                                            peripheralGPIOH 
                                                            peripheralDMA1  
                                                            peripheralDMA2  
                                                            peripheralCRC  
                                                        For AHB2
                                                            peripheralOTGFS
                                                        For APB1 
                                                            peripheralTIM2  
                                                            peripheralTIM3  
                                                            peripheralTIM4  
                                                            peripheralTIM5  
                                                            peripheralWWDG  
                                                            peripheralSPI2  
                                                            peripheralSPI3  
                                                            peripheralUSART2
                                                            peripheralI2C1  
                                                            peripheralI2C2  
                                                            peripheralPWR   
                                                        For APB2
                                                            peripheralTIM1  
                                                            peripheralUSART1
                                                            peripheralUSART6
                                                            peripheralADC1  
                                                            peripheralSDIO  
                                                            peripheralSPI1  
                                                            peripheralSPI4  
                                                            peripheralSYSCFG
                                                            peripheralTIM9  
                                                            peripheralTIM10 
                                                            peripheralTIM11 

    Return:        Returns RCC_ErrorStatus_t   
                   - rcc_retInvalidBus (if got a peripheralBus different from the accepted inputs mentioned above)
                   - rcc_retPeripheralDoesNotBelongToBus (if got a peripheral different from the accepted inputs mentioned above)
                   - rcc_retOk (if peripheral is enabled successfully)
                   - rcc_retNotOk (if coudln't enable peripheral)      
***********************************************************/
RCC_ErrorStatus_t rcc_enablePeripheral(u32 peripheralBus, u32 peripheral);





/**********************************************************
    Description:       This function is used to disable a peripheral

    Input parameters:  It follows the same inputs restrictions mentioned for rcc_enablePeripheral function
    
    Return:            It follows the same return values mentioned for rcc_enablePeripheral function
***********************************************************/
RCC_ErrorStatus_t rcc_disablePeripheral(u32 peripheralBus, u32 peripheral);





/**********************************************************
    Description:       This function is used to enable CSS (Clock Security System)

    Input parameters:  No inputs

    Return:            Returns RCC_ErrorStatus_t
                       - rcc_retOk (if CSS is enabled successfully)
                       - rcc_retNotOk (if cou\dn't enable CSS)
***********************************************************/
RCC_ErrorStatus_t rcc_enableCSS(void);




/**********************************************************
    Description:       This function is used to disable CSS (Clock Security System)

    Input parameters:  No inputs

    Return:            Returns RCC_ErrorStatus_t
                       - rcc_retOk (if CSS is disabled successfully)
                       - rcc_retNotOk (if cou\dn't disable CSS)       
***********************************************************/
RCC_ErrorStatus_t rcc_disableCSS(void);




/**********************************************************
    Description:       This function is used to reset a peripheral

    Input parameters:  It follows the same inputs restrictions mentioned for rcc_enablePeripheral function
    
    Return:            It follows the same return values mentioned for rcc_enablePeripheral function
***********************************************************/
RCC_ErrorStatus_t rcc_resetPeripheral(u32 peripheralBus, u32 peripheral);




/**********************************************************
    Description:       This function is used to set a bus prescaler

    Input parameters:  Valid inputs for peripheralBus are: prescalerBus_AHB, prescalerBus_APB1, and prescalerBus_APB2
                       Valid inputs for busPrescaler are:
                                                           busPrescaler_NoPresc
                                                           busPrescaler_Per2   
                                                           busPrescaler_Per4   
                                                           busPrescaler_Per8   
                                                           busPrescaler_Per16  
                                                           busPrescaler_Per64  
                                                           busPrescaler_Per128 
                                                           busPrescaler_Per256 
                                                           busPrescaler_Per512  

    Return:            Returns RCC_ErrorStatus_t
                       - rcc_retInvalidPrescaler (if got an input different from the accepted inputs mentioned above)
                       - rcc_retInvalidBus (if got an input  is different from the accepted inputs mentioned above)
                       - rcc_retOk (if the prescaler is selected successfully)
                       - rcc_retNotOk (if couldn't set prescaler)       
***********************************************************/
RCC_ErrorStatus_t rcc_setBusPrescaler(u8 prescalerBus, u8 busPrescaler);




/**********************************************************
    Description:       This function is used to set the RTC prescaler

    Input parameters:  Accepted inputs are: rtcPrescaler_NoClock or from rtcPrescaler_HSEPer2 to rtcPrescaler_HSEPer31

    Return:            Returns RCC_ErrorStatus_t
                       - rcc_retInvalidPrescaler (if got an input  is different from the accepted inputs mentioned above)
                       - rcc_retOk (if the prescaler is selected successfully)
                       - rcc_retNotOk (if couldn't set prescaler)
***********************************************************/
RCC_ErrorStatus_t rcc_setRTCPrescaler(u16 rtcPrescaler);




/**********************************************************
    Description:       This function is used to get M value of PLL configuration

    Input parameters:  A valid pointer (Not NULL) to store the value back in it

    Return:            Returns RCC_ErrorStatus_t
                       - rcc_retNullPointer (if the pointer is NULL)
                       - rcc_retOk (if the the value is returned to the pointer)
                       - rcc_retNotOk (if couldn't get value)
                       - the value stored in pointer contains M value of PLL configuration    
***********************************************************/
RCC_ErrorStatus_t rcc_getPllMValue(pu16 mVal);




/**********************************************************
    Description:       This function is used to get Q value of PLL configuration

    Input parameters:  A valid pointer (Not NULL) to store the value back in it

    Return:            Returns RCC_ErrorStatus_t
                       - rcc_retNullPointer (if the pointer is NULL)
                       - rcc_retOk (if the the value is returned to the pointer)
                       - rcc_retNotOk (if couldn't get value)
                       - the value stored in pointer contains Q value of PLL configuration     
***********************************************************/
RCC_ErrorStatus_t rcc_getPllQValue(pu16 qVal);





/**********************************************************
    Description:       This function is used to get P value of PLL configuration

    Input parameters:  A valid pointer (Not NULL) to store the value back in it

    Return:            Returns RCC_ErrorStatus_t
                       - rcc_retNullPointer (if the pointer is NULL)
                       - rcc_retOk (if the the value is returned to the pointer)
                       - rcc_retNotOk (if couldn't get value)
                       - the value stored in pointer contains P value of PLL configuration          
***********************************************************/
RCC_ErrorStatus_t rcc_getPllPValue(pu16 pVal);




/**********************************************************
    Description:       This function is used to get N value of PLL configuration

    Input parameters:  A valid pointer (Not NULL) to store the value back in it

    Return:            Returns RCC_ErrorStatus_t
                       - rcc_retNullPointer (if the pointer is NULL)
                       - rcc_retOk (if the the value is returned to the pointer)
                       - rcc_retNotOk (if couldn't get value)
                       - the value stored in pointer contains N value of PLL configuration
***********************************************************/
RCC_ErrorStatus_t rcc_getPllNValue(pu16 nVal);




/**********************************************************
    Description:       This function is used to get PLL source

    Input parameters:  A valid pointer (Not NULL) to store the value back in it

    Return:           Returns RCC_ErrorStatus_t
                      - rcc_retNullPointer (if the pointer is NULL)
                      - rcc_retOk (if the the clock source is returned to the pointer)
                      - rcc_retNotOk (if couldn't get pll source)
                      - the value stored in pointer can be: pllSource_HSE or pllSource_HSI
***********************************************************/
RCC_ErrorStatus_t rcc_getPllSource(pu32 pllSource);

#endif
