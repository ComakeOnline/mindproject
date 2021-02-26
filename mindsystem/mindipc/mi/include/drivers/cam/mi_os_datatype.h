/* SigmaStar trade secret */
/* Copyright (c) [2019~2020] SigmaStar Technology.
All rights reserved.

Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
SigmaStar and be kept in strict confidence
(SigmaStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of SigmaStar Confidential
Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
*/
///////////////////////////////////////////////////////////////////////////////
/// @file      mi_os_datatype.h
///////////////////////////////////////////////////////////////////////////////

#ifndef _MI_OS_DATATYPE_H_
#define _MI_OS_DATATYPE_H_

#include "mi_common_datatype.h"

typedef struct MI_OS_Mutex_s
{
    MI_U8 au8Priv[44];
} MI_OS_Mutex_t;

typedef struct MI_OS_Semaphore_s
{
    MI_U8 au8Priv[80];
} MI_OS_Semaphore_t;

typedef struct MI_OS_WaitCond_s
{
    MI_U8 au8Priv[80];
} MI_OS_WaitCond_t;

typedef struct MI_OS_TimeSpec_s
{
    MI_U32 u32Sec;
    MI_U32 u32NanoSec;
} MI_OS_TimeSpec_t;

typedef struct MI_OS_ThreadAttr_s
{
    MI_U16 u16Priority;      /* From 1(lowest) to 99(highest), use OS default priority if set 0 */
    MI_U16 u16StackSize;     /* If nStackSize is zero, use OS default value */
} MI_OS_ThreadAttr_t;

typedef struct MI_OS_Atomic_s
{
    union {
        MI_U32 u32Counter;
        MI_S32 s32Counter;
    };
} MI_OS_Atomic_t;

typedef void * MI_OS_Thread_t;

#define MI_OS_ERR_FAILED       (MI_DEF_ERR( E_MI_MODULE_ID_OS, E_MI_ERR_LEVEL_ERROR, E_MI_ERR_FAILED))
#define MI_OS_ERR_ALLOC       (MI_DEF_ERR( E_MI_MODULE_ID_OS, E_MI_ERR_LEVEL_ERROR, E_MI_ERR_NOBUF))
#define MI_OS_ERR_TIMEOUT       (MI_DEF_ERR( E_MI_MODULE_ID_OS, E_MI_ERR_LEVEL_ERROR, E_MI_ERR_SYS_TIMEOUT))

#endif /* _MI_OS_DATATYPE_H_ */
