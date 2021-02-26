/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.10
----------------------------------------------------------------*/

/*
MI_S32 MI_VENC_GetChnDevid(MI_VENC_CHN vencChn, MI_U32 *pu32Devid);
MI_S32 MI_VENC_CreateChn(MI_VENC_CHN vencChn, MI_VENC_ChnAttr_t *pstChnAttr);
MI_S32 MI_VENC_DestroyChn(MI_VENC_CHN vencChn);
MI_S32 MI_VENC_ResetChn(MI_VENC_CHN vencChn);
MI_S32 MI_VENC_StartRecvPic(MI_VENC_CHN vencChn);
MI_S32 MI_VENC_StopRecvPic(MI_VENC_CHN vencChn);
MI_S32 MI_VENC_Query(MI_VENC_CHN vencChn, MI_VENC_ChnStat_t *pstStat);
MI_S32 MI_VENC_SetChnAttr(MI_VENC_CHN vencChn, MI_VENC_ChnAttr_t *pstChnAttr);
MI_S32 MI_VENC_GetChnAttr(MI_VENC_CHN vencChn, MI_VENC_ChnAttr_t *pstChnAttr);
MI_S32 MI_VENC_GetStream(MI_VENC_CHN vencChn, MI_VENC_Stream_t *pstStream, MI_S32 s32MilliSec);
MI_S32 MI_VENC_ReleaseStream(MI_VENC_CHN vencChn, MI_VENC_Stream_t *pstStream);
MI_S32 MI_VENC_InsertUserData(MI_VENC_CHN vencChn, MI_U8 *pu8Data, MI_U32 u32Len);
MI_S32 MI_VENC_SetMaxStreamCnt(MI_VENC_CHN vencChn, MI_U32 u32MaxStrmCnt);
MI_S32 MI_VENC_GetMaxStreamCnt(MI_VENC_CHN vencChn, MI_U32 *pu32MaxStrmCnt);
MI_S32 MI_VENC_GetFd(MI_VENC_CHN vencChn);
MI_S32 MI_VENC_CloseFd(MI_VENC_CHN vencChn);
MI_S32 MI_VENC_SetJpegParam(MI_VENC_CHN vencChn, MI_VENC_ParamJpeg_t *pstJpegParam);
MI_S32 MI_VENC_GetJpegParam(MI_VENC_CHN vencChn, MI_VENC_ParamJpeg_t *pstJpegParam);
MI_S32 MI_VENC_SetCrop(MI_VENC_CHN vencChn, MI_VENC_CropCfg_t *pstCropCfg);
MI_S32 MI_VENC_GetCrop(MI_VENC_CHN vencChn, MI_VENC_CropCfg_t *pstCropCfg);
MI_S32 MI_VENC_DupChn(MI_VENC_CHN vencChn);
MI_S32 MI_VENC_SetInputSourceConfig(MI_VENC_CHN vencChn, MI_VENC_InputSourceConfig_t *pstInputSourceConfig);
MI_S32 MI_VENC_InitDev(MI_VENC_InitParam_t *pstInitParam);
MI_S32 MI_VENC_DeInitDev(void);
MI_S32 MI_VENC_SetH265SliceSplit(MI_VENC_CHN VeChn, MI_VENC_ParamH265SliceSplit_t *pstSliceSplit);
*/

#ifndef __VENC_H__
#define __VENC_H__

#include "mi_venc.h"
#include "semaphore.h"

#define __USE_SLICE_MODE__ 0

class Venc{
public:	
	// wiki 资料：1080P的YUV约3M, 经过H.265	编码协议压缩后，I帧约30K, P帧约7K
	// BUG: 在图像质量较好时，I 帧的大小会超过32K, 从而溢出缓存区导致非法内存引用。		//2020.8.7
	// 修复: 将缓存区设为 128k.				// 2020.8.8
	const static MI_U32 u32PackLen = 128 * 1024;	// N * 1024 = N kb.
	typedef struct{
		MI_U8 u8Pack[u32PackLen];	// H.26X编码数据。
		MI_U32 u32Len;				// H.26X编码数据长度。
		MI_U64 u64PTS;				// 时间戳。
	}StreamPack_t;

	// 主码流和子码流的通道号。
	const static MI_VENC_CHN vencMainChn = 0;
	const static MI_VENC_CHN vencSubChn = 1;
	const static MI_VENC_CHN vencJpegChn = 2;

	static Venc *getInstance();
	MI_S32 enable();
	MI_S32 disable();

	int rcvStream(MI_VENC_CHN vencChn, StreamPack_t *pStreamPack);

	MI_S32 getChnAttr(MI_VENC_CHN VeChn, MI_VENC_ChnAttr_t *pstChnAttr);
	MI_S32 setChnAttr(MI_VENC_CHN VeChn, MI_VENC_ChnAttr_t *pstChnAttr);
	MI_S32 changeBitrate(MI_VENC_CHN VeChn, MI_U32 u32Bitrate);

	MI_S32 createMainStream(MI_VENC_CHN vencChn, MI_VENC_ChnAttr_t *pstChnAttr);
	MI_S32 createSubStream(MI_VENC_CHN vencChn, MI_VENC_ChnAttr_t *pstChnAttr);
	MI_S32 createJpegStream(MI_VENC_CHN vencChn, MI_VENC_ChnAttr_t *pstChnAttr);
	
	MI_S32 getChnDevid(MI_VENC_CHN vencChn, MI_U32 *pu32Devid);
	MI_S32 createChn(MI_VENC_CHN vencChn, MI_VENC_ChnAttr_t *pstChnAttr);
	MI_S32 destroyChn(MI_VENC_CHN vencChn);
	MI_S32 resetChn(MI_VENC_CHN vencChn);
	MI_S32 startRecvPic(MI_VENC_CHN vencChn);
	MI_S32 stopRecvPic(MI_VENC_CHN vencChn);
	MI_S32 query(MI_VENC_CHN vencChn, MI_VENC_ChnStat_t *pstChnStat);
	MI_S32 getStream(MI_VENC_CHN vencChn, MI_VENC_Stream_t *pstStream, MI_S32 s32MilliSec);
	MI_S32 releaseStream(MI_VENC_CHN vencChn, MI_VENC_Stream_t *pstStream);
	MI_S32 insertUserData(MI_VENC_CHN vencChn, MI_U8 *pu8Data, MI_U32 u32Len);
	MI_S32 setMaxStreamCnt(MI_VENC_CHN vencChn, MI_U32 u32MaxStrmCnt);
	MI_S32 getMaxStreamCnt(MI_VENC_CHN vencChn, MI_U32 *pu32MaxStrmCnt);
	MI_S32 getFd(MI_VENC_CHN vencChn);
	MI_S32 closeFd(MI_VENC_CHN vencChn);
	MI_S32 setJpegParam(MI_VENC_CHN vencChn, MI_VENC_ParamJpeg_t *pstJpegParam);
	MI_S32 getJpegParam(MI_VENC_CHN vencChn, MI_VENC_ParamJpeg_t *pstJpegParam);
	MI_S32 setCrop(MI_VENC_CHN vencChn, MI_VENC_CropCfg_t *pstCropCfg);
	MI_S32 getCrop(MI_VENC_CHN vencChn, MI_VENC_CropCfg_t *pstCropCfg);
	MI_S32 dupChn(MI_VENC_CHN vencChn);
	//MI_S32 initDev(MI_VENC_InitParam_t *pstInitParam);		// 不同平台接口有差异，暂且注释之。
	MI_S32 deInitDev(void);
	MI_S32 setH265SliceSplit(MI_VENC_CHN VeChn, MI_VENC_ParamH265SliceSplit_t *pstSliceSplit);
	
private:
	Venc();
	~Venc();
	Venc(const Venc&);
	Venc& operator=(const Venc&);

	bool bEnable;
};

#endif

