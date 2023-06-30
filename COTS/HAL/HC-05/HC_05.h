/**
 * @file HC_05.h
 * @author Ibrahim Saad
 * @brief This file contains all the APIs needed to deal with hc05 bluetooth 
          module (right now, driver doesn't support to program the module 
          using AT commands)
 * @version 0.1
 * @date 2023-06-16
 * @copyright Copyright (c) 2023
 */

#ifndef HC_05_H
#define HC_05_H

#include "HC_05_Cfg.h"

typedef enum
{
    hc05_retNotOk,
    hc05_retOk,
    hc05_retNullPointer,
    hc05_retTimeout,
    hc05_retInvalidUsartId,
    hc05_retInvalidModuleName,
    hc05_retInvalidUsartDirection,
    hc05_retModuleNotPaired,
}HC05_ErrorStatus_t;

HC05_ErrorStatus_t hc05_init(void);
HC05_ErrorStatus_t hc05_recieveByteSync(Hc05Info_t module, pu8 ch);
HC05_ErrorStatus_t hc05_sendByteSync(Hc05Info_t module, u8 ch);
HC05_ErrorStatus_t hc05_recieveBufferSyncZeroCopy(Hc05Info_t module, pu8 buffer, u16 bufferSize);
HC05_ErrorStatus_t hc05_sendBufferSyncZeroCopy(Hc05Info_t module, pu8 buffer, u16 bufferSize);

#endif      /* HC_05_H */
