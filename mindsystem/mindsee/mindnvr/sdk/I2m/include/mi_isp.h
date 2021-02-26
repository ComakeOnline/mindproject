/* Copyright (c) 2018-2019 Sigmastar Technology Corp.
 All rights reserved.

  Unless otherwise stipulated in writing, any and all information contained
 herein regardless in any format shall remain the sole proprietary of
 Sigmastar Technology Corp. and be kept in strict confidence
 (��Sigmastar Confidential Information��) by the recipient.
 Any unauthorized act including without limitation unauthorized disclosure,
 copying, use, reproduction, sale, distribution, modification, disassembling,
 reverse engineering and compiling of the contents of Sigmastar Confidential
 Information is unlawful and strictly prohibited. Sigmastar hereby reserves the
 rights to any and all damages, losses, costs and expenses resulting therefrom.
*/

/*
*   mi_isp.h
*
*   Created on: June 27, 2018
*       Author: Jeffrey Chou
*/

#ifndef _MI_ISP_H_
#define _MI_ISP_H_
//#include <pthread.h>

#include "mi_common.h"
#include "mi_isp_datatype.h"
#include "mi_vpe_datatype.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef LINUX_FLOW_ON_DUAL_OS
#define DUAL_OS_RUN_ON_LINUX 1
#define DUAL_OS_RUN_ON_RTOS 0
#else
#ifdef __RTOS__
#define DUAL_OS_RUN_ON_LINUX 0
#define DUAL_OS_RUN_ON_RTOS 1
#else
#define DUAL_OS_RUN_ON_LINUX 0
#define DUAL_OS_RUN_ON_RTOS 0
#endif
#endif

#define MI_ISP_OK                       (0)
#define MI_ISP_NOT_SUPPORT              (1)
#define MI_ISP_FAILURE                  (-1)
#define MI_ISP_API_MAX_PARM_NUMBRE      (6)

extern MI_S32 MI_VPE_GetIspApiData(MI_VPE_IspApiHeader_t *pstIspDataHeader, void *pVirData);
extern MI_S32 MI_VPE_SetIspApiData(MI_VPE_IspApiHeader_t *pstIspDataHeader, void *pVirData);

#if (DUAL_OS_RUN_ON_LINUX)
#define MI_GETAPI(Channel, APIFuncID, PARAM_t, PtrData) \
    MI_S32 s32Ret = MI_ISP_OK;                          \
    MI_VPE_IspApiHeader_t stIspApiHeader;               \
    PARAM_t  *pIspBuffer = NULL;\
    MI_VPE_Alloc_IspDataBuf(sizeof(PARAM_t), &pIspBuffer);\
    memcpy(pIspBuffer, PtrData, sizeof(PARAM_t));\
    stIspApiHeader.u32HeadSize  = sizeof(MI_VPE_IspApiHeader_t);\
    stIspApiHeader.u32DataLen   = sizeof(PARAM_t);\
    stIspApiHeader.u32CtrlID    = APIFuncID;\
    stIspApiHeader.u32Channel   = Channel;\
    stIspApiHeader.s32Ret       = 0;\
    MI_SYS_FlushInvCache((void *)pIspBuffer, sizeof(PARAM_t));\
    s32Ret = stIspApiHeader.s32Ret  = MI_VPE_GetIspApiData(&stIspApiHeader, pIspBuffer);\
    memcpy(PtrData, pIspBuffer, sizeof(PARAM_t));\
    MI_VPE_Free_IspDataBuf(pIspBuffer);\
    return s32Ret;

#define MI_SETAPI(Channel, APIFuncID, PARAM_t, PtrData) \
    MI_S32 s32Ret = MI_ISP_OK;                          \
    MI_VPE_IspApiHeader_t stIspApiHeader;               \
    PARAM_t  *pIspBuffer = NULL;\
    MI_VPE_Alloc_IspDataBuf(sizeof(PARAM_t), &pIspBuffer);\
    stIspApiHeader.u32HeadSize  = sizeof(MI_VPE_IspApiHeader_t);\
    stIspApiHeader.u32DataLen   = sizeof(PARAM_t);\
    stIspApiHeader.u32CtrlID    = APIFuncID;\
    stIspApiHeader.u32Channel   = Channel;\
    stIspApiHeader.s32Ret       = 0;\
    memcpy(pIspBuffer, PtrData, sizeof(PARAM_t));\
    MI_SYS_FlushInvCache((void *)pIspBuffer, sizeof(PARAM_t));\
    s32Ret = stIspApiHeader.s32Ret = MI_VPE_SetIspApiData(&stIspApiHeader, pIspBuffer);\
    MI_VPE_Free_IspDataBuf(pIspBuffer);\
    return s32Ret;

#define MI_CALI_SETAPI(Channel, APIFuncID, PARAM_t, DataSize, PtrData) \
    MI_VPE_IspApiHeader_t stIspApiHeader;                           \
    PARAM_t  *pIspBuffer = NULL;\
    MI_VPE_Alloc_IspDataBuf(sizeof(PARAM_t), &pIspBuffer);\
    stIspApiHeader.u32HeadSize  = sizeof(MI_VPE_IspApiHeader_t);\
    stIspApiHeader.u32DataLen   = DataSize * sizeof(PARAM_t);\
    stIspApiHeader.u32CtrlID    = APIFuncID;\
    stIspApiHeader.u32Channel   = Channel;\
    stIspApiHeader.s32Ret       = 0;\
    memcpy(pIspBuffer, PtrData, sizeof(PARAM_t));\
    MI_SYS_FlushInvCache((void *)pIspBuffer, sizeof(PARAM_t));\
    stIspApiHeader.s32Ret = MI_VPE_SetIspApiData(&stIspApiHeader, pIspBuffer);\
    MI_VPE_Free_IspDataBuf(pIspBuffer);\
    MI_ISP_DMSG("[%s] - (Channel,CtrlID, DataLenght, HeadSize) = (%d,%d,%d,%d)\n", __FUNCTION__, stIspApiHeader->u32Channel, stIspApiHeader->u32CtrlID, stIspApiHeader->u32DataLen, stIspApiHeader->u32HeadSize);\
    MI_ISP_DMSG("[%s] - Return = %d\n", __FUNCTION__, stIspApiData->stHeader.s32Ret);\

#else
#define MI_GETAPI(Channel, APIFuncID, PARAM_t, PtrData) \
    MI_S32 s32Ret = MI_ISP_OK;                          \
	MI_VPE_IspApiHeader_t stIspApiHeader;               \
    stIspApiHeader.u32HeadSize  = sizeof(MI_VPE_IspApiHeader_t);\
    stIspApiHeader.u32DataLen   = sizeof(PARAM_t);\
    stIspApiHeader.u32CtrlID    = APIFuncID;\
    stIspApiHeader.u32Channel   = Channel;\
    stIspApiHeader.s32Ret       = 0;\
    /*s32Ret = stIspApiHeader.s32Ret = MI_VPE_GetIspApiData(&stIspApiHeader, PtrData);*/\
    s32Ret = stIspApiHeader.s32Ret = _MI_ISP_GetIspApiData(&stIspApiHeader, PtrData);\
    MI_ISP_DMSG("[%s] - (Channel,CtrlID, DataLenght, HeadSize) = (%d,%d,%d,%d)\n", __FUNCTION__, stIspApiHeader.u32Channel, stIspApiHeader.u32CtrlID, stIspApiHeader.u32DataLen, stIspApiHeader.u32HeadSize);\
    return s32Ret;

#define MI_SETAPI(Channel, APIFuncID, PARAM_t, PtrData) \
    MI_S32 s32Ret = MI_ISP_OK;                          \
    MI_VPE_IspApiHeader_t stIspApiHeader;               \
    stIspApiHeader.u32HeadSize  = sizeof(MI_VPE_IspApiHeader_t);\
    stIspApiHeader.u32DataLen   = sizeof(PARAM_t);\
    stIspApiHeader.u32CtrlID    = APIFuncID;\
    stIspApiHeader.u32Channel   = Channel;\
    stIspApiHeader.s32Ret       = 0;\
    /*s32Ret = stIspApiHeader.s32Ret = MI_VPE_SetIspApiData(&stIspApiHeader, PtrData);*/\
    s32Ret = stIspApiHeader.s32Ret = _MI_ISP_SetIspApiData(&stIspApiHeader, PtrData);\
    MI_ISP_DMSG("[%s] - (Channel,CtrlID, DataLenght, HeadSize) = (%d,%d,%d,%d)\n", __FUNCTION__, stIspApiHeader.u32Channel, stIspApiHeader.u32CtrlID, stIspApiHeader.u32DataLen, stIspApiHeader.u32HeadSize);\
    return s32Ret;

#define MI_CALI_SETAPI(Channel, APIFuncID, PARAM_t, DataSize, PtrData) \
    MI_VPE_IspApiHeader_t stIspApiHeader;                           \
    stIspApiHeader.u32HeadSize  = sizeof(MI_VPE_IspApiHeader_t);\
    stIspApiHeader.u32DataLen   = DataSize * sizeof(PARAM_t);\
    stIspApiHeader.u32CtrlID    = APIFuncID;\
    stIspApiHeader.u32Channel   = Channel;\
    stIspApiHeader.s32Ret       = 0;\
    stIspApiHeader.s32Ret = MI_VPE_SetIspApiData(&stIspApiHeader, PtrData);\
    MI_ISP_DMSG("[%s] - (Channel,CtrlID, DataLenght, HeadSize) = (%d,%d,%d,%d)\n", __FUNCTION__, stIspApiHeader->u32Channel, stIspApiHeader->u32CtrlID, stIspApiHeader->u32DataLen, stIspApiHeader->u32HeadSize);\
    MI_ISP_DMSG("[%s] - Return = %d\n", __FUNCTION__, stIspApiData->stHeader.s32Ret);\

#endif

#define MI_CALL_SETAPI(Channel, APIFunc, PARAM_t) \
    PARAM_t *stParam = MI_ISP_CALLOC(1, sizeof(PARAM_t));\
    memcpy(stParam, param_ary[0], sizeof(PARAM_t));\
    s32Ret = APIFunc(Channel, stParam);\
    MI_ISP_DMSG("[%s][%s] param_num = %d\n", __FILE__, __FUNCTION__, param_num);\
    MI_ISP_FREE(stParam);\

#define MI_RESET_API(Channel, APIFunc, PARAM_t) \
    {\
        PARAM_t *stParam = MI_ISP_CALLOC(1, sizeof(PARAM_t));\
        memset(stParam, 0x00, sizeof(PARAM_t));\
        s32Ret = APIFunc(Channel, stParam);\
        MI_ISP_DMSG("[%s][%s] param_num = %d\n", __FILE__, __FUNCTION__, param_num);\
        MI_ISP_FREE(stParam);\
    }\

typedef MI_S32 (*MI_ISP_ApiAgent_fp)(MI_VPE_IspApiHeader_t *pstData, void* pData);

    /************************************* IQ  API START *************************************/
    MI_S32 MI_ISP_IQ_GetVersionInfo(MI_U32 Channel, MI_ISP_IQ_VERSION_INFO_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetVersionInfo(MI_U32 Channel, MI_ISP_IQ_VERSION_INFO_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetParaInitStatus(MI_U32 Channel, MI_ISP_IQ_PARAM_INIT_INFO_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetFastMode(MI_U32 Channel, MI_ISP_IQ_FAST_MODE_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetFastMode(MI_U32 Channel, MI_ISP_IQ_FAST_MODE_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetColorToGray(MI_U32 Channel, MI_ISP_IQ_COLORTOGRAY_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetColorToGray(MI_U32 Channel, MI_ISP_IQ_COLORTOGRAY_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetContrast(MI_U32 Channel, MI_ISP_IQ_CONTRAST_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetContrast(MI_U32 Channel, MI_ISP_IQ_CONTRAST_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetBrightness(MI_U32 Channel, MI_ISP_IQ_BRIGHTNESS_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetBrightness(MI_U32 Channel, MI_ISP_IQ_BRIGHTNESS_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetLightness(MI_U32 Channel, MI_ISP_IQ_LIGHTNESS_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetLightness(MI_U32 Channel, MI_ISP_IQ_LIGHTNESS_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetRGBGamma(MI_U32 Channel, MI_ISP_IQ_RGBGAMMA_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetRGBGamma(MI_U32 Channel, MI_ISP_IQ_RGBGAMMA_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetYUVGamma(MI_U32 Channel, MI_ISP_IQ_YUVGAMMA_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetYUVGamma(MI_U32 Channel, MI_ISP_IQ_YUVGAMMA_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetSaturation(MI_U32 Channel, MI_ISP_IQ_SATURATION_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetSaturation(MI_U32 Channel, MI_ISP_IQ_SATURATION_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetDefog(MI_U32 Channel, MI_ISP_IQ_DEFOG_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetDefog(MI_U32 Channel, MI_ISP_IQ_DEFOG_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetRGBMatrix(MI_U32 Channel, MI_ISP_IQ_RGBMATRIX_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetRGBMatrix(MI_U32 Channel, MI_ISP_IQ_RGBMATRIX_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetFalseColor(MI_U32 Channel, MI_ISP_IQ_FALSECOLOR_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetFalseColor(MI_U32 Channel, MI_ISP_IQ_FALSECOLOR_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetNR3D(MI_U32 Channel, MI_ISP_IQ_NR3D_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetNR3D(MI_U32 Channel, MI_ISP_IQ_NR3D_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetNR3D_P1(MI_U32 Channel, MI_ISP_IQ_NR3D_P1_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetNR3D_P1(MI_U32 Channel, MI_ISP_IQ_NR3D_P1_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetNRDeSpike(MI_U32 Channel, MI_ISP_IQ_NRDESPIKE_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetNRDeSpike(MI_U32 Channel, MI_ISP_IQ_NRDESPIKE_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetNRLuma(MI_U32 Channel, MI_ISP_IQ_NRLUMA_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetNRLuma(MI_U32 Channel, MI_ISP_IQ_NRLUMA_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetNRChroma(MI_U32 Channel, MI_ISP_IQ_NRChroma_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetNRChroma(MI_U32 Channel, MI_ISP_IQ_NRChroma_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetSharpness(MI_U32 Channel, MI_ISP_IQ_SHARPNESS_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetSharpness(MI_U32 Channel, MI_ISP_IQ_SHARPNESS_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetCrossTalk(MI_U32 Channel, MI_ISP_IQ_CROSSTALK_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetCrossTalk(MI_U32 Channel, MI_ISP_IQ_CROSSTALK_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetOBC(MI_U32 Channel, MI_ISP_IQ_OBC_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetOBC(MI_U32 Channel, MI_ISP_IQ_OBC_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetOBC_P1(MI_U32 Channel, MI_ISP_IQ_OBC_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetOBC_P1(MI_U32 Channel, MI_ISP_IQ_OBC_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetWDR(MI_U32 Channel, MI_ISP_IQ_WDR_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetWDR(MI_U32 Channel, MI_ISP_IQ_WDR_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetWDRCurveAdv(MI_U32 Channel, MI_ISP_IQ_WDRCurveAdv_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetWDRCurveAdv(MI_U32 Channel, MI_ISP_IQ_WDRCurveAdv_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetWDRCurveFull(MI_U32 Channel, MI_ISP_IQ_WDRCurveFull_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetWDRCurveFull(MI_U32 Channel, MI_ISP_IQ_WDRCurveFull_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetDynamicDP(MI_U32 Channel, MI_ISP_IQ_DYNAMIC_DP_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetDynamicDP(MI_U32 Channel, MI_ISP_IQ_DYNAMIC_DP_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetDynamicDP_CLUSTER(MI_U32 Channel, MI_ISP_IQ_DYNAMIC_DP_CLUSTER_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetDynamicDP_CLUSTER(MI_U32 Channel, MI_ISP_IQ_DYNAMIC_DP_CLUSTER_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetHSV(MI_U32 Channel, MI_ISP_IQ_HSV_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetHSV(MI_U32 Channel, MI_ISP_IQ_HSV_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetRGBIR(MI_U32 Channel, MI_ISP_IQ_RGBIR_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetRGBIR(MI_U32 Channel, MI_ISP_IQ_RGBIR_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetFPN(MI_U32 Channel, MI_ISP_IQ_FPN_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetFPN(MI_U32 Channel, MI_ISP_IQ_FPN_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetPFC(MI_U32 Channel, MI_ISP_IQ_PFC_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetPFC(MI_U32 Channel, MI_ISP_IQ_PFC_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetDEMOSAIC(MI_U32 Channel, MI_ISP_IQ_DEMOSAIC_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetDEMOSAIC(MI_U32 Channel, MI_ISP_IQ_DEMOSAIC_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetR2Y(MI_U32 Channel, MI_ISP_IQ_R2Y_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetR2Y(MI_U32 Channel, MI_ISP_IQ_R2Y_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetCOLORTRANS(MI_U32 Channel, MI_ISP_IQ_COLORTRANS_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetCOLORTRANS(MI_U32 Channel, MI_ISP_IQ_COLORTRANS_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetHDR(MI_U32 Channel, MI_ISP_IQ_HDR_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetHDR(MI_U32 Channel, MI_ISP_IQ_HDR_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetEffect(MI_U32 Channel, MI_ISP_IQ_EFFECT_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetEffect(MI_U32 Channel, MI_ISP_IQ_EFFECT_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetEnSysMcnrMemory(MI_U32 Channel, MI_ISP_IQ_SYS_MCNR_MEMORY_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetEnSysMcnrMemory(MI_U32 Channel, MI_ISP_IQ_SYS_MCNR_MEMORY_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetLSC(MI_U32 Channel, MI_ISP_IQ_LSC_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetLSC(MI_U32 Channel, MI_ISP_IQ_LSC_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetLSC_CTRL(MI_U32 Channel, MI_ISP_IQ_LSC_CTRL_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetLSC_CTRL(MI_U32 Channel, MI_ISP_IQ_LSC_CTRL_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetALSC(MI_U32 Channel, MI_ISP_IQ_ALSC_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetALSC(MI_U32 Channel, MI_ISP_IQ_ALSC_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetALSC_CTRL(MI_U32 Channel, MI_ISP_IQ_ALSC_CTRL_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetALSC_CTRL(MI_U32 Channel, MI_ISP_IQ_ALSC_CTRL_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetNRLuma_Adv(MI_U32 Channel, MI_ISP_IQ_NRLuma_Adv_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetNRLuma_Adv(MI_U32 Channel, MI_ISP_IQ_NRLuma_Adv_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetNRChroma_Adv(MI_U32 Channel, MI_ISP_IQ_NRChroma_Adv_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetNRChroma_Adv(MI_U32 Channel, MI_ISP_IQ_NRChroma_Adv_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetPFC_EX(MI_U32 Channel, MI_ISP_IQ_PFC_EX_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetPFC_EX(MI_U32 Channel, MI_ISP_IQ_PFC_EX_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetHDR_EX(MI_U32 Channel, MI_ISP_IQ_HDR_EX_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetHDR_EX(MI_U32 Channel, MI_ISP_IQ_HDR_EX_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetSHP_EX(MI_U32 Channel, MI_ISP_IQ_SHARPNESS_EX_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetSHP_EX(MI_U32 Channel, MI_ISP_IQ_SHARPNESS_EX_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetNR3D_EX(MI_U32 Channel, MI_ISP_IQ_NR3D_EX_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetNR3D_EX(MI_U32 Channel, MI_ISP_IQ_NR3D_EX_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetDUMMY(MI_U32 Channel, MI_ISP_IQ_DUMMY_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetDUMMY(MI_U32 Channel, MI_ISP_IQ_DUMMY_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetDUMMY_EX(MI_U32 Channel, MI_ISP_IQ_DUMMY_EX_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetDUMMY_EX(MI_U32 Channel, MI_ISP_IQ_DUMMY_EX_TYPE_t *data);
    MI_S32 MI_ISP_IQ_SetIQMode(MI_U32 Channel, MI_ISP_IQ_PARAM_MODE_e *data);
    MI_S32 MI_ISP_IQ_GetIQMode(MI_U32 Channel, MI_ISP_IQ_PARAM_MODE_e *data);
//    MI_S32 MI_ISP_IQ_GetLinearityLUT(MI_U32 Channel, MS_CAM_GAMMA_LUT_TYPE* GamaLutType);
//    MI_S32 MI_ISP_IQ_GetOBCCALIB(MI_U32 Channel, MS_CAM_OBC4Ch_TYPE *sOBC);
    MI_S32 MI_ISP_IQ_GetIQind(MI_U32 Channel, MI_ISP_IQ_INDEX_e *data);
    MI_S32 MI_ISP_IQ_SetAPIBypassMode(MI_U32 Channel, MI_ISP_API_BYPASS_TYPE_t *data);
    MI_S32 MI_ISP_IQ_GetAPIBypassMode(MI_U32 Channel, MI_ISP_API_BYPASS_TYPE_t *data);
    MI_S32 MI_ISP_IQ_QueryCCMInfo(MI_U32 Channel, MI_ISP_IQ_CCM_INFO_TYPE_t *data);
    /************************************* IQ  API END   *************************************/

    /************************************* AE  API START *************************************/
    MI_S32 MI_ISP_AE_GetHistoWghtY(MI_U32 Channel, MI_ISP_AE_HIST_WEIGHT_Y_TYPE_t *data);
    MI_S32 MI_ISP_AE_QueryExposureInfo(MI_U32 Channel, MI_ISP_AE_EXPO_INFO_TYPE_t *data);
    MI_S32 MI_ISP_AE_SetEVComp(MI_U32 Channel, MI_ISP_AE_EV_COMP_TYPE_t *data);
    MI_S32 MI_ISP_AE_GetEVComp(MI_U32 Channel, MI_ISP_AE_EV_COMP_TYPE_t *data);
    MI_S32 MI_ISP_AE_SetExpoMode(MI_U32 Channel, MI_ISP_AE_MODE_TYPE_e *data);
    MI_S32 MI_ISP_AE_GetExpoMode(MI_U32 Channel, MI_ISP_AE_MODE_TYPE_e *data);
    MI_S32 MI_ISP_AE_SetManualExpo(MI_U32 Channel, MI_ISP_AE_EXPO_VALUE_TYPE_t *data);
    MI_S32 MI_ISP_AE_GetManualExpo(MI_U32 Channel, MI_ISP_AE_EXPO_VALUE_TYPE_t *data);
    MI_S32 MI_ISP_AE_SetState(MI_U32 Channel, MI_ISP_SM_STATE_TYPE_e *data);  // Pause, Resume
    MI_S32 MI_ISP_AE_GetState(MI_U32 Channel, MI_ISP_SM_STATE_TYPE_e *data);
    MI_S32 MI_ISP_AE_SetTarget(MI_U32 Channel, MI_ISP_AE_INTP_LUT_TYPE_t *data);
    MI_S32 MI_ISP_AE_GetTarget(MI_U32 Channel, MI_ISP_AE_INTP_LUT_TYPE_t *data);
    MI_S32 MI_ISP_AE_SetConverge(MI_U32 Channel, MI_ISP_AE_CONV_CONDITON_TYPE_t *data);
    MI_S32 MI_ISP_AE_GetConverge(MI_U32 Channel, MI_ISP_AE_CONV_CONDITON_TYPE_t *data);
    MI_S32 MI_ISP_AE_SetExposureLimit(MI_U32 Channel, MI_ISP_AE_EXPO_LIMIT_TYPE_t *data);
    MI_S32 MI_ISP_AE_GetExposureLimit(MI_U32 Channel, MI_ISP_AE_EXPO_LIMIT_TYPE_t *data);
    MI_S32 MI_ISP_AE_SetPlainLongExpoTable(MI_U32 Channel, MI_ISP_AE_EXPO_TABLE_TYPE_t *data);
    MI_S32 MI_ISP_AE_GetPlainLongExpoTable(MI_U32 Channel, MI_ISP_AE_EXPO_TABLE_TYPE_t *data);
    MI_S32 MI_ISP_AE_SetPlainShortExpoTable(MI_U32 Channel, MI_ISP_AE_EXPO_TABLE_TYPE_t *data);
    MI_S32 MI_ISP_AE_GetPlainShortExpoTable(MI_U32 Channel, MI_ISP_AE_EXPO_TABLE_TYPE_t *data);
    MI_S32 MI_ISP_AE_SetWinWgtType(MI_U32 Channel, MI_ISP_AE_WIN_WEIGHT_MODE_TYPE_e *data);
    MI_S32 MI_ISP_AE_GetWinWgtType(MI_U32 Channel, MI_ISP_AE_WIN_WEIGHT_MODE_TYPE_e *data);
    MI_S32 MI_ISP_AE_SetWinWgt(MI_U32 Channel, MI_ISP_AE_WIN_WEIGHT_TYPE_t *data);
    MI_S32 MI_ISP_AE_GetWinWgt(MI_U32 Channel, MI_ISP_AE_WIN_WEIGHT_TYPE_t *data);
    MI_S32 MI_ISP_AE_SetFlicker(MI_U32 Channel, MI_ISP_AE_FLICKER_TYPE_e *data);
    MI_S32 MI_ISP_AE_GetFlicker(MI_U32 Channel, MI_ISP_AE_FLICKER_TYPE_e *data);
    MI_S32 MI_ISP_AE_SetStrategy(MI_U32 Channel, MI_ISP_AE_STRATEGY_TYPE_t *data);
    MI_S32 MI_ISP_AE_GetStrategy(MI_U32 Channel, MI_ISP_AE_STRATEGY_TYPE_t *data);
    MI_S32 MI_ISP_AE_SetRGBIRAE(MI_U32 Channel, MI_ISP_AE_RGBIR_AE_TYPE_t *data);
    MI_S32 MI_ISP_AE_GetRGBIRAE(MI_U32 Channel, MI_ISP_AE_RGBIR_AE_TYPE_t *data);
    MI_S32 MI_ISP_AE_SetHDR(MI_U32 Channel, MI_ISP_AE_HDR_TYPE_t *data);
    MI_S32 MI_ISP_AE_GetHDR(MI_U32 Channel, MI_ISP_AE_HDR_TYPE_t *data);
    /************************************* AE  API END   *************************************/

    /************************************* AWB API START *************************************/
    MI_S32 MI_ISP_AWB_QueryInfo(MI_U32 Channel, MI_ISP_AWB_QUERY_INFO_TYPE_t *data);
    MI_S32 MI_ISP_AWB_SetAttr(MI_U32 Channel, MI_ISP_AWB_ATTR_TYPE_t *data);
    MI_S32 MI_ISP_AWB_GetAttr(MI_U32 Channel, MI_ISP_AWB_ATTR_TYPE_t *data);
    MI_S32 MI_ISP_AWB_SetAttrEx(MI_U32 Channel, MI_ISP_AWB_ATTR_EX_TYPE_t *data);
    MI_S32 MI_ISP_AWB_GetAttrEx(MI_U32 Channel, MI_ISP_AWB_ATTR_EX_TYPE_t *data);
    MI_S32 MI_ISP_AWB_SetMultiLSAttr(MI_U32 Channel, MI_ISP_AWB_MULTILS_LS_TYPE_t *data);
    MI_S32 MI_ISP_AWB_GetMultiLSAttr(MI_U32 Channel, MI_ISP_AWB_MULTILS_LS_TYPE_t *data);
    MI_S32 MI_ISP_AWB_SetCTWeight(MI_U32 Channel, MI_ISP_AWB_CT_WEIGHT_TYPE_t *data);
    MI_S32 MI_ISP_AWB_GetCTWeight(MI_U32 Channel, MI_ISP_AWB_CT_WEIGHT_TYPE_t *data);

    MI_S32 MI_ISP_AWB_SetCTCaliAttr(MI_U32 Channel, MI_ISP_AWB_CT_CALI_TYPE_t *data);
    MI_S32 MI_ISP_AWB_GetCTCaliAttr(MI_U32 Channel, MI_ISP_AWB_CT_CALI_TYPE_t *data);
    MI_S32 MI_ISP_AWB_GetCTStats(MI_U32 Channel, MI_ISP_AWB_CT_STATISTICS_TYPE_t *data);
    MI_S32 MI_ISP_AWB_GetHWStats(MI_U32 Channel, MI_ISP_AWB_HW_STATISTICS_TYPE_t *data);
    /************************************* AWB API END   *************************************/

    /************************************* AF  API START *************************************/
    /************************************* AF  API END   *************************************/
    /************************************* Customer 3A API ***********************************/
    MI_S32 MI_ISP_AE_GetAeHwAvgStats(MI_U32 Channel, MI_ISP_AE_HW_STATISTICS_t *data);
    MI_S32 MI_ISP_AWB_GetAwbHwAvgStats(MI_U32 Channel, MI_ISP_AWB_HW_STATISTICS_t *data);
    MI_S32 MI_ISP_AWB_GetAwbHwAvgStatsShort(MI_U32 Channel, MI_ISP_AWB_HW_STATISTICS_t *data);
    MI_S32 MI_ISP_AE_GetHisto0HwStats(MI_U32 Channel, MI_ISP_HISTO_HW_STATISTICS_t *data);
    MI_S32 MI_ISP_AE_GetHisto1HwStats(MI_U32 Channel, MI_ISP_HISTO_HW_STATISTICS_t *data);
    MI_S32 MI_ISP_CUS3A_Enable(MI_U32 Channel, Cus3AEnable_t *data);
    MI_S32 MI_ISP_CUS3A_GetAeInitStatus(MI_U32 Channel, CusAEInitParam_t *data);
    MI_S32 MI_ISP_CUS3A_GetAeStatus(MI_U32 Channel, CusAEInfo_t *data);
    MI_S32 MI_ISP_CUS3A_SetAeParam(MI_U32 Channel, CusAEResult_t *data);
    MI_S32 MI_ISP_CUS3A_GetAwbStatus(MI_U32 Channel, CusAWBInfo_t *data);
    MI_S32 MI_ISP_CUS3A_SetAwbParam(MI_U32 Channel, CusAWBResult_t *data);
    MI_S32 MI_ISP_CUS3A_SetAEWindowBlockNumber(MI_U32 Channel, MS_CUST_AE_WIN_BLOCK_NUM_TYPE_e *data);
    MI_S32 MI_ISP_CUS3A_SetAEHistogramWindow(MI_U32 Channel, CusAEHistWin_t*data);
    MI_S32 MI_ISP_CUS3A_SetAWBSampling(MI_U32 Channel, CusAWBSample_t *data);

    MI_S32 MI_ISP_CUS3A_GetAFStats(MI_U32 Channel, CusAFStats_t *data);
    MI_S32 MI_ISP_CUS3A_SetAFWindow(MI_U32 Channel, CusAFWin_t *data);
    MI_S32 MI_ISP_CUS3A_GetAFWindow(MI_U32 Channel, CusAFWin_t *data);
    MI_S32 MI_ISP_CUS3A_SetAFFilter(MI_U32 Channel, CusAFFilter_t *data);
    MI_S32 MI_ISP_CUS3A_GetAFFilter(MI_U32 Channel, CusAFFilter_t *data);
    MI_S32 MI_ISP_CUS3A_SetAFFilterSq(MI_U32 Channel, CusAFFilterSq_t *data);
    MI_S32 MI_ISP_CUS3A_GetAFFilterSq(MI_U32 Channel, CusAFFilterSq_t *data);
    MI_S32 MI_ISP_CUS3A_SetAFRoiMode(MI_U32 Channel, CusAFRoiMode_t *data);
    MI_S32 MI_ISP_CUS3A_GetAFRoiMode(MI_U32 Channel, CusAFRoiMode_t *data);

    MI_S32 MI_ISP_CUS3A_GetImageResolution(MI_U32 Channel, CusImageResolution_t *data);
    MI_S32 MI_ISP_CUS3A_EnableISPOutImage(MI_U32 Channel, CusISPOutImage_t *data);
    MI_S32 MI_ISP_CUS3A_GetISPOutImageCount(MI_U32 Channel, MI_U32 *data);
    MI_S32 MI_ISP_CUS3A_CaptureHdrRawImage(MI_U32 Channel, CusHdrRawImage_t *data);
    MI_S32 MI_ISP_GetFrameMetaInfo(MI_U32 Channel, IspFrameMetaInfo_t *data);
    MI_S32 MI_ISP_ReadSensorData(MI_U32 Channel, CusSensorI2cParam_t *data);
    MI_S32 MI_ISP_WriteSensorData(MI_U32 Channel, CusSensorI2cParam_t *data);
    /************************************* Customer 3A API END********************************/
    /************************************* Debug  API ***********************************/
    MI_S32 MI_ISP_SetDebugLevel_AE(MI_U32 Channel, MI_S32 *data);
    MI_S32 MI_ISP_GetDebugLevel_AE(MI_U32 Channel, MI_S32 *data);
    MI_S32 MI_ISP_SetDebugLevel_AWB(MI_U32 Channel, MI_S32 *data);
    MI_S32 MI_ISP_GetDebugLevel_AWB(MI_U32 Channel, MI_S32 *data);
    /************************************* Debug  API END********************************/

    MI_S32 call_MI_ISP_AE_SetEVComp(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetExpoMode(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetManualExpo(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetManualShortExpo(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetState(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetTarget(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetConverge(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetExposureLimit(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetPlainLongExpoTable(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetPlainShortExpoTable(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
//    MI_S32 call_MI_ISP_AE_SetWinWgtType(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetWinWgt(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetFlicker(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetStrategy(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetRGBIRAE(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AE_SetAEHDR(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);

    MI_S32 call_MI_ISP_AWB_SetAttr(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AWB_SetAttrEx(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AWB_SetMultiLSAttr(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_AWB_SetCTCaliAttr(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);

    MI_S32 call_MI_ISP_IQ_SetFPN(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetDynamicDP(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetDynamicDP_CLUSTER(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetCrossTalk(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetFalseColor(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetOBC(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetOBC_P1(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetNRDeSpike(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetNRLuma(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetNRChroma(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetNR3D(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetNR3D_P1(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetRGBMatrix(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetHSV(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetRGBGamma(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetBrightness(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetLightness(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetContrast(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetColorToGray(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetSharpness(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetSaturation(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetPFC(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetYUVGamma(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetWDR(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetHDR(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetCOLORTRANS(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetRGBIR(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetLSC(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetLSC_CTRL(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetALSC(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetALSC_CTRL(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetDefog(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetDeMosaic(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetWDRCurveAdv(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetR2Y(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetNRLuma_Adv(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetNRChroma_Adv(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetPFC_EX(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetHDR_EX(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetSHP_EX(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetNR3D_EX(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetDUMMY(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);
    MI_S32 call_MI_ISP_IQ_SetDUMMY_EX(MI_U32 Channel, MI_U8 *param_ary[], MI_U8 param_num);

    MI_S32 call_MI_ISP_API_DisableAllApi(MI_U32 Channel);
    MI_S32 call_MI_ISP_API_Reset(MI_U32 Channel, MI_U16 APIID);
    MI_S32 call_MI_ISP_API_CmdLoadBinFile(MI_U32 Channel, MI_U8 *bindata_buf, MI_U32 user_key);
    MI_S32 MI_ISP_API_CmdLoadBinFile(MI_U32 Channel, char* filepath, MI_U32 user_key);
    MI_S32 MI_ISP_API_CmdLoadCaliData(MI_U32 Channel, MI_ISP_CALI_ITEM_e eCaliItem, char* filepath);

    /*Api agent for SigmaStar user space 3A library*/
    MI_S32 MI_ISP_RegisterIspApiAgent(MI_U32 Channel, MI_ISP_ApiAgent_fp fpApiSet, MI_ISP_ApiAgent_fp fpApiGet);
#if !defined(DUAL_OS) && !defined(__RTOS__) //for pure linux
    MI_S32 MI_ISP_EnableUserspace3A(MI_U32 Channel,MI_ISP_API_USERSPACE3A_ATTR_t *pAttr);
    MI_S32 MI_ISP_DisableUserspace3A(MI_U32 Channel);
#else //for DualOS-Linux , RTK
    inline MI_S32 MI_ISP_EnableUserspace3A(MI_U32 Channel,MI_ISP_API_USERSPACE3A_ATTR_t *pAttr) {return 0;}
    inline MI_S32 MI_ISP_DisableUserspace3A(MI_U32 Channel) {return 0;}
#endif

#ifdef __cplusplus
}   //end of extern C
#endif

#endif  //_MI_ISP_H_
