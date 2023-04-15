/*******************************************************************************************
*   File name:    STM_NVIC.c
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs definitions to deal with NVIC peripheral
*                 features in STM32F401
*   Version: v1.0
********************************************************************************************/

#include "STM_NVIC.h"

/* SCB registers */
#define SCB_VTOR                *((volatile u32* const) 0xE000ED08)
#define SCB_AIRCR               *((volatile u32* const) 0xE000ED0C)
#define SCB_ICSR                *((volatile u32* const) 0xE000ED04)

/* bits of ICSR */
#define ICSR_NMIPENDSET         31
#define ICSR_PENDSVSET          28
#define ICSR_PENDSVCLR          27
#define ICSR_PENDSTSET          26
#define ICSR_PENDSTCLR          25
/* Pending isr number from bit 21 to 12 */
/* Current running ISR [9:0] */
/* In AIRCR priority group bits [10:8] */

#define NORMAL_REG_WIDTH        32U
#define BITS_IMPLMENTED_NO      4U
#define MAX_PRIORITY_LEVEL      16U
#define M4_VOTR_SHIFT           9U
#define M4_VOTR_MSK             0xC00001FF
#define AIRCR_ACCSESS_KEY       0x05FA0000
#define PRIORITY_GROUP_SHIFT    8
#define GET_PRIORITY_GROUPING   0xAF

#define MSK_CHECK_VALID_PRIORITY_GROUP      0xF0
#define MSK_VALID_PRIORITY_GROUP            0xA0
#define MSK_CHECK_VALID_PRIORITY_GROUP_CLR  0x0F

#define MSK_READ_VECTACTIVE                 0x000001FF
#define MSK_PRIORITY_GROUP_CLR              0xFFFFF8FF
#define MSK_CLR_ACCESS_KEY                  0x0000FFFF
#define MSK_CLR_STIR                        0xFFFFFE00

typedef struct
{
    u32 ISER[8];
    u32 __reserved1[24];
    u32 ICER[8];
    u32 __reserved2[24];
    u32 ISPR[8];
    u32 __reserved3[24];
    u32 ICPR[8];
    u32 __reserved4[24];
    u32 IABR[8];
    u32 __reserved5[56];
    u8 IP[240];
    u32 __reserved6[644];
    u32 STIR;
}NVICRegisters_t;

static volatile NVICRegisters_t* const nvicRegs = (volatile NVICRegisters_t* const) (0xE000E100);

static NVIC_ErrorStatus_t getPriorityGrouping(pu8 priorityGroup);

NVIC_ErrorStatus_t nvic_enableIRQ(IRQ_Type_t nvic_IRQ)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retNotOk;
    if (nvic_IRQ >= nvic_IRQ0 && nvic_IRQ <= nvic_IRQ239)
    {
        u8 regNo = nvic_IRQ / NORMAL_REG_WIDTH;
        u8 bitNo = nvic_IRQ % NORMAL_REG_WIDTH;
        nvicRegs->ISER[regNo] |= 1 << bitNo;
        errorStatus = nvic_retOk;
    }
    else
    {
        errorStatus = nvic_retInvalidIRQNumber;
    }
    return errorStatus;
}

NVIC_ErrorStatus_t nvic_disableIRQ(IRQ_Type_t nvic_IRQ)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retNotOk;
    if (nvic_IRQ >= nvic_IRQ0 && nvic_IRQ <= nvic_IRQ239)
    {
        u8 regNo = nvic_IRQ / NORMAL_REG_WIDTH;
        u8 bitNo = nvic_IRQ % NORMAL_REG_WIDTH;
        nvicRegs->ICER[regNo] |= 1 << bitNo;
        errorStatus = nvic_retOk;
    }
    else
    {
        errorStatus = nvic_retInvalidIRQNumber;
    }
    return errorStatus;
}

NVIC_ErrorStatus_t nvic_setPendingIRQ(IRQ_Type_t nvic_IRQ)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retNotOk;
    if (nvic_IRQ >= nvic_IRQ0 && nvic_IRQ <= nvic_IRQ239)
    {
        u8 regNo = nvic_IRQ / NORMAL_REG_WIDTH;
        u8 bitNo = nvic_IRQ % NORMAL_REG_WIDTH;
        nvicRegs->ISPR[regNo] |= 1 << bitNo;
        errorStatus = nvic_retOk;
    }
    else
    {
        errorStatus = nvic_retInvalidIRQNumber;
    }
    return errorStatus;
}

NVIC_ErrorStatus_t nvic_clearPendingIRQ(IRQ_Type_t nvic_IRQ)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retNotOk;
    if (nvic_IRQ >= nvic_IRQ0 && nvic_IRQ <= nvic_IRQ239)
    {
        u8 regNo = nvic_IRQ / NORMAL_REG_WIDTH;
        u8 bitNo = nvic_IRQ % NORMAL_REG_WIDTH;
        nvicRegs->ICPR[regNo] |= 1 << bitNo;
        errorStatus = nvic_retOk;
    }
    else
    {
        errorStatus = nvic_retInvalidIRQNumber;
    }
    return errorStatus;
}

NVIC_ErrorStatus_t nvic_getPendingState(IRQ_Type_t nvic_IRQ, pu8 pendingState)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retNotOk;
    if (nvic_IRQ >= nvic_IRQ0 && nvic_IRQ <= nvic_IRQ239)
    {
        if (pendingState == NULL)
        {
            errorStatus = nvic_retNullPointer;
        }
        else
        {
            u8 regNo = nvic_IRQ / NORMAL_REG_WIDTH;
            u8 bitNo = nvic_IRQ % NORMAL_REG_WIDTH;
            *pendingState = (u8) ((nvicRegs->ICPR[regNo] & (1 << bitNo)) >> bitNo);
            errorStatus = nvic_retOk;
        }
    }
    else
    {
        errorStatus = nvic_retInvalidIRQNumber;
    }
    return errorStatus;
}

NVIC_ErrorStatus_t nvic_getActiveState(IRQ_Type_t nvic_IRQ, pu8 activateState)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retNotOk;
    if (nvic_IRQ >= nvic_IRQ0 && nvic_IRQ <= nvic_IRQ239)
    {
        if (activateState == NULL)
        {
            errorStatus = nvic_retNullPointer;
        }
        else
        {
            u8 regNo = nvic_IRQ / NORMAL_REG_WIDTH;
            u8 bitNo = nvic_IRQ % NORMAL_REG_WIDTH;
            *activateState = (u8) ((nvicRegs->IABR[regNo] & (1 << bitNo)) >> bitNo);
            errorStatus = nvic_retOk;
        }
    }
    else
    {
        errorStatus = nvic_retInvalidIRQNumber;
    }
    return errorStatus;
}

NVIC_ErrorStatus_t nvic_setPreemption(IRQ_Type_t nvic_IRQ, u8 preemption)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retNotOk;
    if (nvic_IRQ >= nvic_IRQ0 && nvic_IRQ <= nvic_IRQ239)
    {
        if (preemption > MAX_PRIORITY_LEVEL)
        {
            errorStatus = nvic_retInvalidPriorityLevel;
        }
        else
        {
            u8 group;
            errorStatus = getPriorityGrouping(&group);
            if (errorStatus == nvic_retOk)
            {
                u8 regNo = nvic_IRQ / NORMAL_REG_WIDTH;
                u8 temp = nvicRegs->IP[regNo];
                switch (group)
                {
                    case priorityGroup_0Bit:
                        temp &= 0x0F;
                        temp |= (preemption << 4);
                        nvicRegs->IP[regNo] = temp;
                        errorStatus = nvic_retOk;
                        break;
                    case priorityGroup_1Bit:
                        temp &= 0x1F;
                        temp |= (preemption << 5);
                        nvicRegs->IP[regNo] = temp;
                        errorStatus = nvic_retOk;
                        break;
                    case priorityGroup_2Bits:
                        temp &= 0x3F;
                        temp |= (preemption << 6);
                        nvicRegs->IP[regNo] = temp;
                        errorStatus = nvic_retOk;
                        break;
                    case priorityGroup_3Bits:
                        temp &= 0x7F;
                        temp |= (preemption << 7);
                        nvicRegs->IP[regNo] = temp;
                        errorStatus = nvic_retOk;
                        break;
                    case priorityGroup_4Bits:
                        errorStatus = nvic_retInvalidPreemptionSetting;
                        break;
                }
                
            }
        }
    }
    else
    {
        errorStatus = nvic_retInvalidIRQNumber;
    }
    return errorStatus;
}

NVIC_ErrorStatus_t nvic_getPreemption(IRQ_Type_t nvic_IRQ, pu8 preemption)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retNotOk;
    if (nvic_IRQ >= nvic_IRQ0 && nvic_IRQ <= nvic_IRQ239)
    {
        if (preemption == NULL)
        {
            errorStatus = nvic_retNullPointer;
        }
        else
        {
            u8 group;
            errorStatus = getPriorityGrouping(&group);
            if (errorStatus == nvic_retOk)
            {
                u8 regNo = nvic_IRQ / NORMAL_REG_WIDTH;
                u8 temp;
                switch (group)
                {
                    case priorityGroup_0Bit:
                        temp = ((nvicRegs->IP[regNo] & 0xF0) >> 4);
                        break;
                    case priorityGroup_1Bit:
                        temp = ((nvicRegs->IP[regNo] & 0xE0) >> 5);
                        break;
                    case priorityGroup_2Bits:
                        temp = ((nvicRegs->IP[regNo] & 0xC0) >> 6);
                        break;
                    case priorityGroup_3Bits:
                        temp = ((nvicRegs->IP[regNo] & 0x80) >> 7);
                        break;
                    case priorityGroup_4Bits:
                        temp = 0;
                        break;
                }
                *preemption = temp;
                errorStatus = nvic_retOk;
            }
        }
    }
    else
    {
        errorStatus = nvic_retInvalidIRQNumber;
    }
    return errorStatus;
}

NVIC_ErrorStatus_t nvic_setPriority(IRQ_Type_t nvic_IRQ, u8 priority)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retNotOk;
    if (nvic_IRQ >= nvic_IRQ0 && nvic_IRQ <= nvic_IRQ239)
    {
        u8 group;
        errorStatus = getPriorityGrouping(&group);
        if (errorStatus == nvic_retOk)
        {
            u8 regNo = nvic_IRQ / NORMAL_REG_WIDTH;
            u8 temp = nvicRegs->IP[regNo];
            switch (group)
            {
                case priorityGroup_0Bit:
                    errorStatus = nvic_retInvalidPrioritySetting;
                    break;
                case priorityGroup_1Bit:
                    temp &= (priority & 0x10);
                    nvicRegs->IP[regNo] = temp;
                    errorStatus = nvic_retOk;
                    break;
                case priorityGroup_2Bits:
                    temp &= (priority & 0x30);
                    nvicRegs->IP[regNo] = temp;
                    errorStatus = nvic_retOk;
                    break;
                case priorityGroup_3Bits:
                    temp &= (priority & 0x70);
                    nvicRegs->IP[regNo] = temp;
                    errorStatus = nvic_retOk;
                    break;
                case priorityGroup_4Bits:
                    temp &= (priority & 0xF0);
                    nvicRegs->IP[regNo] = temp;
                    errorStatus = nvic_retOk;
                    break;
            }
        }
    }
    else
    {
        errorStatus = nvic_retInvalidIRQNumber;
    }
    return errorStatus;
}

NVIC_ErrorStatus_t nvic_getPriority(IRQ_Type_t nvic_IRQ, pu8 priority)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retNotOk;
    if (nvic_IRQ >= nvic_IRQ0 && nvic_IRQ <= nvic_IRQ239)
    {
        if (priority == NULL)
        {
            errorStatus = nvic_retNullPointer;
        }
        else
        {
            u8 group;
            errorStatus = getPriorityGrouping(&group);
            if (errorStatus == nvic_retOk)
            {
                u8 regNo = nvic_IRQ / NORMAL_REG_WIDTH;
                u8 temp;
                switch (group)
                {
                    case priorityGroup_0Bit:
                        temp = 0;
                        break;
                    case priorityGroup_1Bit:
                        temp = ((nvicRegs->IP[regNo] & 0x10) >> 5);
                        break;
                    case priorityGroup_2Bits:
                        temp = ((nvicRegs->IP[regNo] & 0x30) >> 6);
                        break;
                    case priorityGroup_3Bits:
                        temp = ((nvicRegs->IP[regNo] & 0x70) >> 7);
                        break;
                    case priorityGroup_4Bits:
                        temp = ((nvicRegs->IP[regNo] & 0xF0) >> 7);
                        break;
                }
                *priority = temp;
                errorStatus = nvic_retOk;
            }
        }
    }
    else
    {
        errorStatus = nvic_retInvalidIRQNumber;
    }
    return errorStatus;
}

NVIC_ErrorStatus_t nvic_triggerSoftwareInterrupt(IRQ_Type_t nvic_IRQ)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retNotOk;
    if (nvic_IRQ >= nvic_IRQ0 && nvic_IRQ <= nvic_IRQ239)
    {
        u32 temp = nvicRegs->STIR;
        temp &= MSK_CLR_STIR;
        temp |= nvic_IRQ;
        nvicRegs->STIR = temp;
        errorStatus = nvic_retOk;
    }
    else
    {
        errorStatus = nvic_retInvalidIRQNumber;
    }
}

NVIC_ErrorStatus_t nvic_setPriorityGrouping(u8 priorityGroup)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retOk;
    if ((priorityGroup & MSK_CHECK_VALID_PRIORITY_GROUP) == MSK_VALID_PRIORITY_GROUP)
    {
        u32 temp = SCB_AIRCR;
        temp &= MSK_CLR_ACCESS_KEY;
        temp |= AIRCR_ACCSESS_KEY;
        SCB_AIRCR = temp;
        temp = SCB_AIRCR;
        priorityGroup &= MSK_CHECK_VALID_PRIORITY_GROUP_CLR;
        temp &= MSK_PRIORITY_GROUP_CLR;
        temp |= (priorityGroup << PRIORITY_GROUP_SHIFT);
        SCB_AIRCR = temp;
        errorStatus = nvic_retOk;
    }
    else
    {
        errorStatus = nvic_retInvalidPriorityGroup;
    }
    return errorStatus;
}

NVIC_ErrorStatus_t nvic_getPriorityGrouping(pu8 priorityGroup)
{
    return getPriorityGrouping(priorityGroup);
}

NVIC_ErrorStatus_t nvic_reallocateVectorTable(u32 vectAddress)
{
    SCB_VTOR &= (vectAddress << M4_VOTR_SHIFT) | M4_VOTR_MSK;
    return nvic_retOk;
}

NVIC_ErrorStatus_t nvic_getRunningISR(pu16 runningISR)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retNotOk;
    if (runningISR == NULL)
    {
        errorStatus = nvic_retNullPointer;
    }
    else
    {
        *runningISR = SCB_ICSR & MSK_READ_VECTACTIVE;
        errorStatus = nvic_retOk;
    }
    return errorStatus;
}

NVIC_ErrorStatus_t nvic_setPRIMASK(void)
{
    __asm("CPSID I");
    return nvic_retOk;
}

NVIC_ErrorStatus_t nvic_clearPRIMASK(void)
{
    __asm("CPSIE I");
    return nvic_retOk;
}

NVIC_ErrorStatus_t nvic_setFAULTMASK(void)
{
    __asm("CPSID F");
    return nvic_retOk;
}

NVIC_ErrorStatus_t nvic_clearFAULTMASK(void)
{
    __asm("CPSIE F");
    return nvic_retOk;
}

NVIC_ErrorStatus_t nvic_setBASEPRI(u32 basePri)
{
    __asm("MSR basepri, %0"::"r"(basePri):"memory");
    return nvic_retOk;
}

static NVIC_ErrorStatus_t getPriorityGrouping(pu8 priorityGroup)
{
    NVIC_ErrorStatus_t errorStatus = nvic_retNotOk;
    if (priorityGroup == NULL)
    {
        errorStatus = nvic_retNullPointer;
    }
    else
    {
        u8 group;
        u32 temp = SCB_AIRCR;
        temp &= MSK_CLR_ACCESS_KEY;
        temp |= AIRCR_ACCSESS_KEY;
        SCB_AIRCR = temp;
        group = ((SCB_AIRCR & (~MSK_PRIORITY_GROUP_CLR)) >> PRIORITY_GROUP_SHIFT);
        group &= GET_PRIORITY_GROUPING;
        *priorityGroup = group;
        errorStatus = nvic_retOk;
    }
    return errorStatus;
}
