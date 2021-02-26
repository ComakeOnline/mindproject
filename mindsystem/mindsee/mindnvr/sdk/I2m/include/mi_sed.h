/*
* mi_sed.h- Sigmastar
*
* Copyright (C) 2018 Sigmastar Technology Corp.
*
* Author: XXXX <XXXX@sigmastar.com.cn>
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
*/
#ifndef _MI_SED_H_
#define _MI_SED_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "mi_sed_datatype.h"

MI_S32 MI_SED_CreateChn(MI_SED_CHN SedChn, MI_SED_DetectorAttr_t* pstAttr);
MI_S32 MI_SED_DestroyChn(MI_SED_CHN SedChn);
MI_S32 MI_SED_StartDetector(MI_SED_CHN SedChn);
MI_S32 MI_SED_StopDetector(MI_SED_CHN SedChn);
MI_S32 MI_SED_AttachToChn(MI_SED_CHN SedChn, MI_SED_TARGET_CHN TargetChn);
MI_S32 MI_SED_DetachFromChn(MI_SED_CHN SedChn, MI_SED_TARGET_CHN TargetChn);
#ifdef __cplusplus
}
#endif

#endif
