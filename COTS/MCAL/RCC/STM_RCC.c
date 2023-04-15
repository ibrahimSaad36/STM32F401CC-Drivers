/*******************************************************************
*   File name:    STM_RCC.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs definitions to deal with RCC peripheral
*                 features in STM32F401
*   Version: v1.0
*******************************************************************/
#include "STM_RCC.h"

#define TIME_OUT                      ((u32) 20000)

#define MSK_CHECK_VALID_SYS_CLK       0xF0000000
#define MSK_VALID_SYS_CLK             0x10000000
#define MSK_CHECL_VALID_SYS_CLK_CLR   0xEFFFFFFF
#define MSK_CHECK_PLL_SRC_CLR		  0x0FFFFFFF
#define MSK_CHECK_VALID_RTC_PRESC     0x0F00
#define MSK_VALID_RTC_PRESC           0x0200
#define MSK_CHECK_VALID_RTC_PRE_CLR   0x00FF
#define MSK_CHECK_VALID_BUS_PRESC     0xF0
#define MSK_VALID_BUS_PRESC           0x30
#define MSK_CHECK_VALID_BUS_PRE_CLR   0x0F
#define MSK_CHECK_VALID_PRESC         0xF0
#define MSK_VALID_PRESC               0xF0
#define MSK_CHECK_VALID_PRESC_CLR     0x0F

#define MSK_CLR_ABP1_PRESC            0xFFFF7CFF
#define MSK_CLR_ABP2_PRESC            0xFFFF1FFF
#define MSK_CLR_AHB_PRESC             0xFFFFFF0F
#define MSK_CLR_RTC_PRESC             0xFFF7FFFF

#define RTC_PRESC_SHIFT               16

#define MSK_HSI_RDY                   0x00000002
#define MSK_HSE_RDY                   0x00020000
#define MSK_PLL_RDY                   0x02000000
#define MSK_CHECK_CLK                 0x0000000C
#define MSK_SWS_HSI                   0x00000000
#define MSK_SWS_HSE                   0x00000004
#define MSK_SWS_PLL                   0x00000008
#define MSK_SW_HSI                    0x00000000
#define MSK_SW_HSE                    0x00000001
#define MSK_SW_PLL                    0x00000002
#define MSK_CLR_SW					  0xFFFFFFFC
#define MSK_CSS_EN                    0x00000008
#define MSK_CSS_DIS                   0xFFFFFFF7

#define MSK_VALID_PLL_SRC             0x20000000
#define MSK_CHECK_VALID_PLL_SRC       0xF0000000
#define MSK_CHECK_VALID_PLL_SRC_CLR   0xDFFFFFFF
#define MSK_READ_PLL_SRC              0x00400000

#define MSK_CHECK_VALID_BUS           0xF0000000
#define MSK_VALID_BUS                 0x30000000
#define MSK_CHECK_VALID_BUS_CLR       0xCFFFFFFF

#define MSK_CHECK_VALID_AHB1_PERI     0xF0000000
#define MSK_VALID_AHB1_PRI            0x40000000
#define MSK_AHB1_CLR                  0x01000000
#define MSK_CHECK_VALID_AHB1_PERI_CLR 0xBFFFFFFF

#define MSK_CHECK_VALID_AHB2_PERI     0xF0000000
#define MSK_VALID_AHB2_PRI            0x50000000
#define MSK_CHECK_VALID_AHB2_PERI_CLR 0xAFFFFFFF

#define MSK_CHECK_VALID_APB1_PERI     0xF0000000
#define MSK_VALID_APB1_PRI            0x80000000
#define MSK_CHECK_VALID_APB1_PERI_CLR 0x7FFFFFFF

#define MSK_CHECK_VALID_APB2_PERI     0xF0000000
#define MSK_VALID_APB2_PRI            0x70000000
#define MSK_CHECK_VALID_APB2_PERI_CLR 0x8FFFFFFF

// PLL Config [Q]
#define PLL_MIN_QVAL                2
#define PLL_MAX_QVAL                15
#define PLL_QVAL_SHIFT              24
#define MSK_PLL_CLR_Q               0xF0FFFFFF
#define MSK_READ_Q_VAL              0x0F000000
// PLL Config [P]
#define PLL_MIN_PVAL                2
#define PLL_MAX_PVAL                8
#define PLL_PVAL_SHIFT              16
#define MSK_PLL_CLR_P               0xFFFCFFFF
#define MSK_READ_P_VAL              0x00030000
// PLL Config [N]
#define PLL_MIN_NVAL                192
#define PLL_MAX_NVAL                432
#define PLL_NVAL_SHIFT              6
#define MSK_PLL_CLR_N               0xFFFF803F
#define MSK_READ_N_VAL              0x00007FC0
// PLL Config [M]
#define PLL_MIN_MVAL                2
#define PLL_MAX_MVAL                63
#define PLL_MVAL_SHIFT              0
#define MSK_PLL_CLR_M               0xFFFFFFC0
#define MSK_READ_M_VAL              0x0000003F

/* AHB prescaler add value */
#define AHB_PRESC_ADD               4

/* RCC Clock Control register bits*/
#define RCC_CR_HSION        0
#define RCC_CR_HSIRDY       1
#define RCC_CR_HSEON        16
#define RCC_CR_HSERDY       17
#define RCC_CR_HSEBYP       18
#define RCC_CR_CSSON        19
#define RCC_CR_PLLON        24
#define RCC_CR_PLLRDY       25
#define RCC_CR_PLLI2SON     26
#define RCC_CR_PLLI2SRDY    27

/* HSION bit config */
#define HSION_ON            1
#define HSION_OFF           0
/* HSIRDY bit config */
#define HSIRDY_NOT_RDY      0
#define HSIRDY_RDY          1
/* HSEON bit config */
#define HSEON_ON            1
#define HSEON_OFF           0
/* HSERDY bit config */
#define HSERDY_NOT_RDY      0
#define HSERDY_RDY          1
/* HSEBYP vit config */
#define HSEBYP_NOT_BYP      0
#define HSEBYP_BYP          1
/* CSSON bit config */
#define CSSON_ON            1
#define CSSON_OFF           0
/* PLLON bit config */
#define PLLON_ON            1
#define PLLON_OFF           0
/* PLLRDY bit config */
#define PLLRDY_UNLOCKED     0
#define PLLRDY_LOCKED       1
/* PLLI2SON bit config */
#define PLLI2SON_ON         1
#define PLLI2SON_OFF        0
/* PLLI2SRDY bit config */
#define PLLI2SRDY_UNLOCKED  0
#define PLLI2SRDY_LOCKED    1

typedef struct
{
    /* Base address of RCC: 0x40023800*/
    u32 RCC_CR;
    u32 RCC_PLLCFGR;
    u32 RCC_CFGR;
    u32 RCC_CIR;
    u32 RCC_AHB1RSTR;
    u32 RCC_AHB2RSTR;
    u32 __reserved1;
    u32 __reserved2;
    u32 RCC_APB1RSTR;
    u32 RCC_APB2RSTR;
    u32 __reserved3;
    u32 __reserved4;
    u32 RCC_AHB1ENR;
    u32 RCC_AHB2ENR;
    u32 __reserved5;
    u32 __reserved6;
    u32 RCC_APB1ENR;
    u32 RCC_APB2ENR;
    u32 __reserved7;
    u32 __reserved8;
    u32 RCC_AHB1LPENR;
    u32 RCC_AHB2LPENR;
    u32 __reserved9;
    u32 __reserved10;
    u32 RCC_APB1LPENR;
    u32 RCC_APB2LPENR;
    u32 __reserved11;
    u32 __reserved12;
    u32 RCC_BDCR;
    u32 RCC_CSR;
    u32 __reserved13;
    u32 __reserved14;
    u32 RCC_SSCGR;
    u32 RCC_PLLI2SCFGR;
    u32 __reserved15;
    u32 RCC_DCKCFGR;
}rccRegisters_t;

volatile rccRegisters_t* const rccRegs = (volatile rccRegisters_t* const)  0x40023800;

static RCC_ErrorStatus_t rcc_getRunningSyetmClock(pu32 clock);

RCC_ErrorStatus_t rcc_selectSystemClock(u32 systemClock)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    u32 clock, temp;
    errorStatus = rcc_getRunningSyetmClock(&clock);
    if(errorStatus == rcc_retOk && clock == systemClock)
    {
        errorStatus = rcc_retClockAlreadySelected;
    }
    else{
        if((systemClock & MSK_CHECK_VALID_SYS_CLK) == MSK_VALID_SYS_CLK)
        {
            u32 timeOutCounter = TIME_OUT;
            rccRegs->RCC_CR |= (systemClock & MSK_CHECL_VALID_SYS_CLK_CLR);
            switch (systemClock)
            {
                case systemClock_HSI:
                    while (((rccRegs->RCC_CR & MSK_HSI_RDY) != MSK_HSI_RDY) && timeOutCounter > 0)
                    {
                        timeOutCounter--;
                    }
                    if((rccRegs->RCC_CR & MSK_HSI_RDY) == MSK_HSI_RDY)
                    {
                    	temp = rccRegs->RCC_CFGR;
                    	temp &= MSK_CLR_SW;
                    	temp |= MSK_SW_HSI;
                        rccRegs->RCC_CFGR = temp;
                        if((rccRegs->RCC_CFGR & MSK_CHECK_CLK) == MSK_SWS_HSI)
                        {
                            errorStatus = rcc_retOk;
                        }
                    }
                    else
                    {
                        errorStatus = rcc_retSelectSystemClockTimeOut;
                    }
                    break;
                case systemClock_HSE:
                    while (((rccRegs->RCC_CR & MSK_HSE_RDY) != MSK_HSE_RDY) && timeOutCounter > 0)
                    {
                        timeOutCounter--;
                    }
                    if((rccRegs->RCC_CR & MSK_HSE_RDY) == MSK_HSE_RDY)
                    {
                    	temp = rccRegs->RCC_CFGR;
                    	temp &= MSK_CLR_SW;
                    	temp |= MSK_SW_HSE;
                        rccRegs->RCC_CFGR = temp;
                        if((rccRegs->RCC_CFGR & MSK_CHECK_CLK) == MSK_SWS_HSE)
                        {
                            errorStatus = rcc_retOk;
                        }
                    }
                    else
                    {
                        errorStatus = rcc_retSelectSystemClockTimeOut;
                    }
                    break;
                case systemClock_PLL:
                    while (((rccRegs->RCC_CR & MSK_PLL_RDY) != MSK_PLL_RDY) && timeOutCounter > 0)
                    {
                        timeOutCounter--;
                    }
                    if((rccRegs->RCC_CR & MSK_PLL_RDY) == MSK_PLL_RDY)
                    {
                    	temp = rccRegs->RCC_CFGR;
                    	temp &= MSK_CLR_SW;
                    	temp |= MSK_SW_PLL;
                        rccRegs->RCC_CFGR = temp;
                        if((rccRegs->RCC_CFGR & MSK_CHECK_CLK) == MSK_SWS_PLL)
                        {
                            errorStatus = rcc_retOk;
                        }
                    }
                    else
                    {
                        errorStatus = rcc_retSelectSystemClockTimeOut;
                    }
                    break;
            }
        }
        else
        {
            errorStatus = rcc_retInvalidSystemClock;
        }
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_getRunningClock(pu32 runningClock)
{
    RCC_ErrorStatus_t errorStatus = rcc_getRunningSyetmClock(runningClock);
    return errorStatus;
}

RCC_ErrorStatus_t rcc_isClockReady(u32 clock)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    if((clock & MSK_CHECK_VALID_SYS_CLK) == MSK_VALID_SYS_CLK)
    {
        u32 timeOutCounter = TIME_OUT;
        clock &= MSK_CHECL_VALID_SYS_CLK_CLR;
        rccRegs->RCC_CR |= clock;
        while (((rccRegs->RCC_CR & MSK_HSI_RDY) != MSK_HSI_RDY) && timeOutCounter > 0)
        {
            timeOutCounter--;
        }
        if((rccRegs->RCC_CR & MSK_HSI_RDY) == MSK_HSI_RDY)
        {
            
            errorStatus = rcc_retClockReady;
        }
        else
        {
            errorStatus = rcc_retClockNotReady;
        }
    }
    else
    {
        errorStatus = rcc_retInvalidSystemClock;
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_configurePLL(u32 pllSource, u16 QVal, u16 PVal, u16 NVal, u16 MVal)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    u32 clock, temp;
    errorStatus = rcc_getRunningSyetmClock(&clock);
    if(errorStatus == rcc_retOk && clock == systemClock_PLL)
    {
        errorStatus = rcc_retPLLLocked;
    }
    else
    {
        if((pllSource & MSK_CHECK_VALID_PLL_SRC) == MSK_VALID_PLL_SRC)
        {
            if(QVal >= PLL_MIN_QVAL && QVal <= PLL_MAX_QVAL)
            {
                if((PVal >= PLL_MIN_PVAL && PVal <= PLL_MAX_PVAL) && !(PVal & 1) == 0)
                {
                    if(NVal >= PLL_MIN_NVAL && NVal <= PLL_MAX_NVAL)
                    {
                        if(MVal >= PLL_MIN_MVAL && MVal <= PLL_MAX_MVAL)
                        {
                            if(pllSource == pllSource_HSI)
                            {
                                rccRegs->RCC_PLLCFGR |= (pllSource_HSI & MSK_CHECK_PLL_SRC_CLR);
                            }
                            else
                            {
                                rccRegs->RCC_PLLCFGR |= (pllSource_HSE & MSK_CHECK_PLL_SRC_CLR);
                            }
                            // Q config
                            temp = rccRegs->RCC_PLLCFGR;
                            temp &= (u32) MSK_PLL_CLR_Q;
                            temp |= (u32) (QVal << PLL_QVAL_SHIFT);
                            rccRegs->RCC_PLLCFGR = temp;
                            // P config
                            temp = rccRegs->RCC_PLLCFGR;
                            temp &= (u32) MSK_PLL_CLR_P;
                            temp |= (u32) (PVal << PLL_PVAL_SHIFT);
                            rccRegs->RCC_PLLCFGR = temp;
                            // N config
                            temp = rccRegs->RCC_PLLCFGR;
                            temp &= (u32) MSK_PLL_CLR_N;
                            temp |= (u32) (NVal << PLL_NVAL_SHIFT);
                            rccRegs->RCC_PLLCFGR = temp;
                            // M config
                            temp = rccRegs->RCC_PLLCFGR;
                            temp &= (u32) MSK_PLL_CLR_M;
                            temp |= (u32) (MVal << PLL_MVAL_SHIFT);
                            rccRegs->RCC_PLLCFGR = temp;
                            errorStatus = rcc_retOk;
                        }
                        else
                        {
                            errorStatus = rcc_retConfigError;
                        }
                    }
                    else
                    {
                        errorStatus = rcc_retConfigError;
                    }
                }
                else
                {
                    errorStatus = rcc_retConfigError;
                }
            }
            else
            {
                errorStatus = rcc_retConfigError;
            }
        }
        else
        {
            errorStatus = rcc_retInvalidPLLSource;
        }
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_enablePeripheral(u32 peripheralBus, u32 peripheral)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    if((peripheralBus & MSK_CHECK_VALID_BUS) == MSK_VALID_BUS)
    {
        switch (peripheralBus)
        {
            case peripheralBus_AHB1:
                if((peripheral & MSK_CHECK_VALID_AHB1_PERI) == MSK_VALID_AHB1_PRI)
                {
                    peripheral &= MSK_CHECK_VALID_AHB1_PERI_CLR;
                    rccRegs->RCC_AHB1ENR |= peripheral;
                    errorStatus = rcc_retOk;
                }
                else
                {
                    errorStatus = rcc_retPeripheralDoesNotBelongToBus;
                }
                break;
            case peripheralBus_AHB2:
                if((peripheral & MSK_CHECK_VALID_AHB2_PERI) == MSK_VALID_AHB2_PRI)
                {
                    peripheral &= MSK_CHECK_VALID_AHB2_PERI_CLR;
                    rccRegs->RCC_AHB2ENR |= peripheral;
                    errorStatus = rcc_retOk;
                }
                else
                {
                    errorStatus = rcc_retPeripheralDoesNotBelongToBus;
                }
                break;
            case peripheralBus_APB1:
                if((peripheral & MSK_CHECK_VALID_APB1_PERI) == MSK_VALID_APB1_PRI)
                {
                    peripheral &= MSK_CHECK_VALID_APB1_PERI_CLR;
                    rccRegs->RCC_APB1ENR |= peripheral;
                    errorStatus = rcc_retOk;
                }
                else
                {
                    errorStatus = rcc_retPeripheralDoesNotBelongToBus;
                }
                break;
            case peripheralBus_APB2:
                if((peripheral & MSK_CHECK_VALID_APB2_PERI) == MSK_VALID_APB2_PRI)
                {
                    peripheral &= MSK_CHECK_VALID_APB2_PERI_CLR;
                    rccRegs->RCC_APB2ENR |= peripheral;
                    errorStatus = rcc_retOk;
                }
                else
                {
                    errorStatus = rcc_retPeripheralDoesNotBelongToBus;
                }
                break;
        }
    }
    else
    {
        errorStatus = rcc_retInvalidBus;
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_disablePeripheral(u32 peripheralBus, u32 peripheral)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    if((peripheralBus & MSK_CHECK_VALID_BUS) == MSK_VALID_BUS)
    {
        switch (peripheralBus)
        {
            case peripheralBus_AHB1:
                if((peripheral & MSK_CHECK_VALID_AHB1_PERI) == MSK_VALID_AHB1_PRI)
                {
                    peripheral &= MSK_CHECK_VALID_AHB1_PERI_CLR;
                    rccRegs->RCC_AHB1ENR &= (~peripheral) | MSK_AHB1_CLR;
                    errorStatus = rcc_retOk;
                }
                else
                {
                    errorStatus = rcc_retPeripheralDoesNotBelongToBus;
                }
                break;
            case peripheralBus_AHB2:
                if((peripheral & MSK_CHECK_VALID_AHB2_PERI) == MSK_VALID_AHB2_PRI)
                {
                    peripheral &= MSK_CHECK_VALID_AHB2_PERI_CLR;
                    rccRegs->RCC_AHB2ENR &= (~peripheral);
                    errorStatus = rcc_retOk;
                }
                else
                {
                    errorStatus = rcc_retPeripheralDoesNotBelongToBus;
                }
                break;
            case peripheralBus_APB1:
                if((peripheral & MSK_CHECK_VALID_APB1_PERI) == MSK_VALID_APB1_PRI)
                {
                    peripheral &= MSK_CHECK_VALID_APB1_PERI_CLR;
                    rccRegs->RCC_APB1ENR &= (~peripheral);
                    errorStatus = rcc_retOk;
                }
                else
                {
                    errorStatus = rcc_retPeripheralDoesNotBelongToBus;
                }
                break;
            case peripheralBus_APB2:
                if((peripheral & MSK_CHECK_VALID_APB2_PERI) == MSK_VALID_APB2_PRI)
                {
                    peripheral &= MSK_CHECK_VALID_APB2_PERI_CLR;
                    rccRegs->RCC_APB2ENR &= (~peripheral);
                    errorStatus = rcc_retOk;
                }
                else
                {
                    errorStatus = rcc_retPeripheralDoesNotBelongToBus;
                }
                break;
        }
    }
    else
    {
        errorStatus = rcc_retInvalidBus;
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_enableCSS(void)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    rccRegs->RCC_CR |= MSK_CSS_EN;
    if((rccRegs->RCC_CR & MSK_CSS_EN) == MSK_CSS_EN)
    {
        errorStatus = rcc_retOk;
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_disableCSS(void)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    rccRegs->RCC_CR &= MSK_CSS_DIS;
    if((rccRegs->RCC_CR & (~MSK_CSS_DIS)) == MSK_CSS_EN)
    {
        errorStatus = rcc_retOk;
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_resetPeripheral(u32 peripheralBus, u32 peripheral)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    if((peripheralBus & MSK_CHECK_VALID_BUS) == MSK_VALID_BUS)
    {
        switch (peripheralBus)
        {
            case peripheralBus_AHB1:
                if((peripheral & MSK_CHECK_VALID_AHB1_PERI) == MSK_VALID_AHB1_PRI)
                {
                    peripheral &= MSK_CHECK_VALID_AHB1_PERI_CLR;
                    rccRegs->RCC_AHB1RSTR |= peripheral;
                    errorStatus = rcc_retOk;
                }
                else
                {
                    errorStatus = rcc_retPeripheralDoesNotBelongToBus;
                }
                break;
            case peripheralBus_AHB2:
                if((peripheral & MSK_CHECK_VALID_AHB2_PERI) == MSK_VALID_AHB2_PRI)
                {
                    peripheral &= MSK_CHECK_VALID_AHB2_PERI_CLR;
                    rccRegs->RCC_AHB2RSTR |= peripheral;
                    errorStatus = rcc_retOk;
                }
                else
                {
                    errorStatus = rcc_retPeripheralDoesNotBelongToBus;
                }
                break;
            case peripheralBus_APB1:
                if((peripheral & MSK_CHECK_VALID_APB1_PERI) == MSK_VALID_APB1_PRI)
                {
                    peripheral &= MSK_CHECK_VALID_APB1_PERI_CLR;
                    rccRegs->RCC_APB1RSTR |= peripheral;
                    errorStatus = rcc_retOk;
                }
                else
                {
                    errorStatus = rcc_retPeripheralDoesNotBelongToBus;
                }
                break;
            case peripheralBus_APB2:
                if((peripheral & MSK_CHECK_VALID_APB2_PERI) == MSK_VALID_APB2_PRI)
                {
                    peripheral &= MSK_CHECK_VALID_APB2_PERI_CLR;
                    rccRegs->RCC_APB2RSTR |= peripheral;
                    errorStatus = rcc_retOk;
                }
                else
                {
                    errorStatus = rcc_retPeripheralDoesNotBelongToBus;
                }
                break;
        }
    }
    else
    {
        errorStatus = rcc_retInvalidBus;
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_setBusPrescaler(u8 prescalerBus, u8 busPrescaler)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    if ((prescalerBus & MSK_CHECK_VALID_BUS_PRESC) == MSK_VALID_BUS_PRESC)
    {
        if ((busPrescaler & MSK_CHECK_VALID_PRESC) == MSK_VALID_PRESC)
        {
            u32 temp = rccRegs->RCC_CFGR;
            busPrescaler &= MSK_CHECK_VALID_PRESC_CLR;
            switch (prescalerBus)
            {
                case prescalerBus_APB1:
                    temp &= MSK_CLR_ABP1_PRESC;
                    temp |= busPrescaler << (prescalerBus & MSK_CHECK_VALID_BUS_PRE_CLR);
                    break;
                case prescalerBus_APB2:
                    temp &= MSK_CLR_ABP2_PRESC;
                    temp |= busPrescaler << (prescalerBus & MSK_CHECK_VALID_BUS_PRE_CLR);
                    break;
                case prescalerBus_AHB:
                    temp &= MSK_CLR_AHB_PRESC;
                    busPrescaler += AHB_PRESC_ADD;
                    temp |= busPrescaler << (prescalerBus & MSK_CHECK_VALID_BUS_PRE_CLR);
                    break;
            }
            rccRegs->RCC_CFGR = temp;
            errorStatus = rcc_retOk;
        }
        else
        {
            errorStatus = rcc_retInvalidPrescaler;
        }
    }
    else
    {
        errorStatus = rcc_retInvalidBus;
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_setRTCPrescaler(u16 rtcPrescaler)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    if ((rtcPrescaler & MSK_CHECK_VALID_RTC_PRESC) == MSK_VALID_RTC_PRESC)
    {
        u32 temp = rccRegs->RCC_CFGR;
        rtcPrescaler &= MSK_CHECK_VALID_RTC_PRE_CLR;
        temp &= MSK_CLR_RTC_PRESC;
        temp |= rtcPrescaler << RTC_PRESC_SHIFT;
        rccRegs->RCC_CFGR = temp;
        errorStatus = rcc_retOk;
    }
    else
    {
        errorStatus = rcc_retInvalidPrescaler;
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_getPllMValue(pu16 mVal)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    if (mVal == NULL)
    {
        errorStatus = rcc_retNullPointer;
    }
    else
    {
        u16 tempMVal = rccRegs->RCC_PLLCFGR & MSK_READ_M_VAL;
        tempMVal >>= PLL_MVAL_SHIFT;
        *mVal = tempMVal;
        errorStatus = rcc_retOk;
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_getPllQValue(pu16 qVal)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    if (qVal == NULL)
    {
        errorStatus = rcc_retNullPointer;
    }
    else
    {
        u16 tempQVal = rccRegs->RCC_PLLCFGR & MSK_READ_Q_VAL;
        tempQVal >>= PLL_QVAL_SHIFT;
        *qVal = tempQVal;
        errorStatus = rcc_retOk;
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_getPllPValue(pu16 pVal)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    if (pVal == NULL)
    {
        errorStatus = rcc_retNullPointer;
    }
    else
    {
        u16 tempPVal = rccRegs->RCC_PLLCFGR & MSK_READ_P_VAL;
        tempPVal >>= PLL_PVAL_SHIFT;
        *pVal = tempPVal;
        errorStatus = rcc_retOk;
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_getPllNValue(pu16 nVal)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    if (nVal == NULL)
    {
        errorStatus = rcc_retNullPointer;
    }
    else
    {
        u16 tempNVal = rccRegs->RCC_PLLCFGR & MSK_READ_N_VAL;
        tempNVal >>= PLL_NVAL_SHIFT;
        *nVal = tempNVal;
        errorStatus = rcc_retOk;
    }
    return errorStatus;
}

RCC_ErrorStatus_t rcc_getPllSource(pu32 pllSource)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    if (pllSource == NULL)
    {
        errorStatus = rcc_retNullPointer;
    }
    else
    {
        if (rccRegs->RCC_PLLCFGR & MSK_READ_PLL_SRC)
        {
            *pllSource = pllSource_HSE;
            errorStatus = rcc_retOk;
        }
        else
        {
            *pllSource = pllSource_HSI;
             errorStatus = rcc_retOk;
        }
    }
    return errorStatus;
}

static RCC_ErrorStatus_t rcc_getRunningSyetmClock(pu32 clock)
{
    RCC_ErrorStatus_t errorStatus = rcc_retNotOk;
    if(clock == NULL)
    {
        errorStatus = rcc_retNullPointer;
    }
    else
    {
        if((rccRegs->RCC_CFGR & MSK_CHECK_CLK) == MSK_SWS_HSI)
        {
            *clock = systemClock_HSI;
            errorStatus = rcc_retOk;
        }
        if((rccRegs->RCC_CFGR & MSK_CHECK_CLK) == MSK_SWS_HSE)
        {
            *clock = systemClock_HSE;
            errorStatus = rcc_retOk;
        }
        if((rccRegs->RCC_CFGR & MSK_CHECK_CLK) == MSK_SWS_PLL)
        {
            *clock = systemClock_PLL;
            errorStatus = rcc_retOk;
        }
    }
    return errorStatus;
}
