/*******************************************************************************************
*   File name:    STM_NVIC.h
*   Author:       Ibrahim Saad
*   Description:  This file contains all APIs prototype to deal with NVIC peripheral
*                 features in STM32F401
*   Version: v1.0
********************************************************************************************/

#ifndef STM_NVIC_H
#define STM_NVIC_H

#include "../../LIB/Std_types.h"

#define priorityGroup_0Bit      0xA3
#define priorityGroup_1Bit      0xA4
#define priorityGroup_2Bits     0xA5
#define priorityGroup_3Bits     0xA6
#define priorityGroup_4Bits     0xA7

typedef enum
{
    nvic_retNotOk = 0,
    nvic_retOk,
    nvic_retInvalidIRQNumber,
    nvic_retNullPointer,
    nvic_retInvalidPriorityLevel,
    nvic_retInvalidPriorityGroup,
    nvic_retInvalidPrioritySetting,
    nvic_retInvalidPreemptionSetting,
}NVIC_ErrorStatus_t;

typedef enum
{
    nvic_IRQ0 = 0,
    nvic_IRQ1,
    nvic_IRQ2,
    nvic_IRQ3,
    nvic_IRQ4,
    nvic_IRQ5,
    nvic_IRQ6,
    nvic_IRQ7,
    nvic_IRQ8,
    nvic_IRQ9,
    nvic_IRQ10,
    nvic_IRQ11,
    nvic_IRQ12,
    nvic_IRQ13,
    nvic_IRQ14,
    nvic_IRQ15,
    nvic_IRQ16,
    nvic_IRQ17,
    nvic_IRQ18,
    nvic_IRQ19,
    nvic_IRQ20,
    nvic_IRQ21,
    nvic_IRQ22,
    nvic_IRQ23,
    nvic_IRQ24,
    nvic_IRQ25,
    nvic_IRQ26,
    nvic_IRQ27,
    nvic_IRQ28,
    nvic_IRQ29,
    nvic_IRQ30,
    nvic_IRQ31,
    nvic_IRQ32,
    nvic_IRQ33,
    nvic_IRQ34,
    nvic_IRQ35,
    nvic_IRQ36,
    nvic_IRQ37,
    nvic_IRQ38,
    nvic_IRQ39,
    nvic_IRQ40,
    nvic_IRQ41,
    nvic_IRQ42,
    nvic_IRQ43,
    nvic_IRQ44,
    nvic_IRQ45,
    nvic_IRQ46,
    nvic_IRQ47,
    nvic_IRQ48,
    nvic_IRQ49,
    nvic_IRQ50,
    nvic_IRQ51,
    nvic_IRQ52,
    nvic_IRQ53,
    nvic_IRQ54,
    nvic_IRQ55,
    nvic_IRQ56,
    nvic_IRQ57,
    nvic_IRQ58,
    nvic_IRQ59,
    nvic_IRQ60,
    nvic_IRQ61,
    nvic_IRQ62,
    nvic_IRQ63,
    nvic_IRQ64,
    nvic_IRQ65,
    nvic_IRQ66,
    nvic_IRQ67,
    nvic_IRQ68,
    nvic_IRQ69,
    nvic_IRQ70,
    nvic_IRQ71,
    nvic_IRQ72,
    nvic_IRQ73,
    nvic_IRQ74,
    nvic_IRQ75,
    nvic_IRQ76,
    nvic_IRQ77,
    nvic_IRQ78,
    nvic_IRQ79,
    nvic_IRQ80,
    nvic_IRQ81,
    nvic_IRQ82,
    nvic_IRQ83,
    nvic_IRQ84,
    nvic_IRQ85,
    nvic_IRQ86,
    nvic_IRQ87,
    nvic_IRQ88,
    nvic_IRQ89,
    nvic_IRQ90,
    nvic_IRQ91,
    nvic_IRQ92,
    nvic_IRQ93,
    nvic_IRQ94,
    nvic_IRQ95,
    nvic_IRQ96,
    nvic_IRQ97,
    nvic_IRQ98,
    nvic_IRQ99,
    nvic_IRQ100,
    nvic_IRQ101,
    nvic_IRQ102,
    nvic_IRQ103,
    nvic_IRQ104,
    nvic_IRQ105,
    nvic_IRQ106,
    nvic_IRQ107,
    nvic_IRQ108,
    nvic_IRQ109,
    nvic_IRQ110,
    nvic_IRQ111,
    nvic_IRQ112,
    nvic_IRQ113,
    nvic_IRQ114,
    nvic_IRQ115,
    nvic_IRQ116,
    nvic_IRQ117,
    nvic_IRQ118,
    nvic_IRQ119,
    nvic_IRQ120,
    nvic_IRQ121,
    nvic_IRQ122,
    nvic_IRQ123,
    nvic_IRQ124,
    nvic_IRQ125,
    nvic_IRQ126,
    nvic_IRQ127,
    nvic_IRQ128,
    nvic_IRQ129,
    nvic_IRQ130,
    nvic_IRQ131,
    nvic_IRQ132,
    nvic_IRQ133,
    nvic_IRQ134,
    nvic_IRQ135,
    nvic_IRQ136,
    nvic_IRQ137,
    nvic_IRQ138,
    nvic_IRQ139,
    nvic_IRQ140,
    nvic_IRQ141,
    nvic_IRQ142,
    nvic_IRQ143,
    nvic_IRQ144,
    nvic_IRQ145,
    nvic_IRQ146,
    nvic_IRQ147,
    nvic_IRQ148,
    nvic_IRQ149,
    nvic_IRQ150,
    nvic_IRQ151,
    nvic_IRQ152,
    nvic_IRQ153,
    nvic_IRQ154,
    nvic_IRQ155,
    nvic_IRQ156,
    nvic_IRQ157,
    nvic_IRQ158,
    nvic_IRQ159,
    nvic_IRQ160,
    nvic_IRQ161,
    nvic_IRQ162,
    nvic_IRQ163,
    nvic_IRQ164,
    nvic_IRQ165,
    nvic_IRQ166,
    nvic_IRQ167,
    nvic_IRQ168,
    nvic_IRQ169,
    nvic_IRQ170,
    nvic_IRQ171,
    nvic_IRQ172,
    nvic_IRQ173,
    nvic_IRQ174,
    nvic_IRQ175,
    nvic_IRQ176,
    nvic_IRQ177,
    nvic_IRQ178,
    nvic_IRQ179,
    nvic_IRQ180,
    nvic_IRQ181,
    nvic_IRQ182,
    nvic_IRQ183,
    nvic_IRQ184,
    nvic_IRQ185,
    nvic_IRQ186,
    nvic_IRQ187,
    nvic_IRQ188,
    nvic_IRQ189,
    nvic_IRQ190,
    nvic_IRQ191,
    nvic_IRQ192,
    nvic_IRQ193,
    nvic_IRQ194,
    nvic_IRQ195,
    nvic_IRQ196,
    nvic_IRQ197,
    nvic_IRQ198,
    nvic_IRQ199,
    nvic_IRQ200,
    nvic_IRQ201,
    nvic_IRQ202,
    nvic_IRQ203,
    nvic_IRQ204,
    nvic_IRQ205,
    nvic_IRQ206,
    nvic_IRQ207,
    nvic_IRQ208,
    nvic_IRQ209,
    nvic_IRQ210,
    nvic_IRQ211,
    nvic_IRQ212,
    nvic_IRQ213,
    nvic_IRQ214,
    nvic_IRQ215,
    nvic_IRQ216,
    nvic_IRQ217,
    nvic_IRQ218,
    nvic_IRQ219,
    nvic_IRQ220,
    nvic_IRQ221,
    nvic_IRQ222,
    nvic_IRQ223,
    nvic_IRQ224,
    nvic_IRQ225,
    nvic_IRQ226,
    nvic_IRQ227,
    nvic_IRQ228,
    nvic_IRQ229,
    nvic_IRQ230,
    nvic_IRQ231,
    nvic_IRQ232,
    nvic_IRQ233,
    nvic_IRQ234,
    nvic_IRQ235,
    nvic_IRQ236,
    nvic_IRQ237,
    nvic_IRQ238,
    nvic_IRQ239,
}IRQ_Type_t;

NVIC_ErrorStatus_t nvic_enableIRQ(IRQ_Type_t nvic_IRQ);

NVIC_ErrorStatus_t nvic_disableIRQ(IRQ_Type_t nvic_IRQ);

NVIC_ErrorStatus_t nvic_setPendingIRQ(IRQ_Type_t nvic_IRQ);

NVIC_ErrorStatus_t nvic_clearPendingIRQ(IRQ_Type_t nvic_IRQ);

NVIC_ErrorStatus_t nvic_getPendingState(IRQ_Type_t nvic_IRQ, pu8 pendingState);

NVIC_ErrorStatus_t nvic_getActiveState(IRQ_Type_t nvic_IRQ, pu8 activateState);

NVIC_ErrorStatus_t nvic_setPriority(IRQ_Type_t nvic_IRQ, u8 priority);

NVIC_ErrorStatus_t nvic_getPriority(IRQ_Type_t nvic_IRQ, pu8 priority);

NVIC_ErrorStatus_t nvic_setPreemption(IRQ_Type_t nvic_IRQ, u8 preemption);

NVIC_ErrorStatus_t nvic_getPreemption(IRQ_Type_t nvic_IRQ, pu8 preemption);

NVIC_ErrorStatus_t nvic_triggerSoftwareInterrupt(IRQ_Type_t nvic_IRQ);

NVIC_ErrorStatus_t nvic_setPriorityGrouping(u8 priorityGroup);

NVIC_ErrorStatus_t nvic_getPriorityGrouping(pu8 priorityGroup);

NVIC_ErrorStatus_t nvic_reallocateVectorTable(u32 vectAddress);

NVIC_ErrorStatus_t nvic_getRunningISR(pu16 runningISR);

NVIC_ErrorStatus_t nvic_setPRIMASK(void);

NVIC_ErrorStatus_t nvic_clearPRIMASK(void);

NVIC_ErrorStatus_t nvic_setFAULTMASK(void);

NVIC_ErrorStatus_t nvic_clearFAULTMASK(void);

NVIC_ErrorStatus_t nvic_setBASEPRI(u32 basePri);

#endif
