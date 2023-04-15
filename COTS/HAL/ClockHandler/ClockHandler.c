/*******************************************************************
*   File name:    ClockHandler.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs definitions needed to deal with clock handler module
*   Version: v1.0
*******************************************************************/

#include "ClockHandler.h"

void ClockHandler_getClockCallback(clkHandlercbf_t cbf)
{
    if (cbf != NULL)
    {
        u32 clock, retClock, pllSource;
        u16 m, n, q, p;
        RCC_ErrorStatus_t rccErrorStatus;
        rccErrorStatus = rcc_getRunningClock(&clock);
        if (rccErrorStatus == rcc_retOk)
        {
            switch (clock)
            {
                case systemClock_HSI:
                    cbf(HSI_CLOCK);
                    break;
                case systemClock_HSE:
                    cbf(HSE_CLOCK);
                    break;
                case systemClock_PLL:
                    rccErrorStatus = rcc_getPllSource(&pllSource);
                    rccErrorStatus = rcc_getPllMValue(&m);
                    rccErrorStatus = rcc_getPllNValue(&n);
                    rccErrorStatus = rcc_getPllQValue(&q);
                    rccErrorStatus = rcc_getPllPValue(&p);
                    if (rccErrorStatus == rcc_retOk)
                    {
                        if (pllSource == pllSource_HSI)
                        {
                            f32 fvco = HSI_CLOCK * ((n * 1.0) / m);
                            retClock = fvco / p;
                        }
                        else
                        {
                            f32 fvco = HSE_CLOCK * ((n * 1.0) / m);
                            retClock = fvco / p;
                        }
                        cbf(retClock);
                    }
                    break;
            }
        }
    }
}

