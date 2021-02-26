/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.10
----------------------------------------------------------------*/

#ifndef __VPE_H__
#define __VPE_H__

#include "mi_vpe.h"

/*
VPE 流程：VIF -> VPE(1 个chanel) -> set port mode(创建多个输出port) 
		-> enable port -> start port.
*/

/*
本示例默认创建两个port, port 1作主码流，port 2作子码流。
该特性在MI 手册中没有提到，由benis.chen, zb.lin 提到。
*/

class Vpe{
public:
	const static MI_VPE_PORT vpeInputPort = 0;
	const static MI_VPE_PORT vpeMainPort = 1;
	const static MI_VPE_PORT vpeSubPort = 2;
	const static MI_VPE_PORT vpeJpegPort = 3;

// ===========================================================
	static Vpe* getInstance();
	MI_S32 enable();
	MI_S32 disable();

// ===========================================================
	MI_S32 createMainPort(MI_VPE_PORT vpePort);
	MI_S32 createSubPort(MI_VPE_PORT vpePort);
	MI_S32 createJpegPort(MI_VPE_PORT vpePort);
	
// ===========================================================
	MI_S32 setChnOutputPortDepth(MI_VPE_PORT vpePort, MI_U32 u32UserFrameDepth, MI_U32 u32BufQueueDepth);

	MI_S32 createChannel();
	MI_S32 destroyChannel();
	
	MI_S32 getChannelAttr(MI_VPE_ChannelAttr_t *pstVpeChAttr);
	MI_S32 setChannelAttr(MI_VPE_ChannelAttr_t *pstVpeChAttr);
	
	MI_S32 startChannel();
	MI_S32 stopChannel();
	
	MI_S32 setChannelParam();
	MI_S32 getChannelParam(MI_VPE_ChannelPara_t *pstVpeParam);
	
	MI_S32 setChannelCrop();
	MI_S32 getChannelCrop(MI_SYS_WindowRect_t *pstCropInfo);
	
	MI_S32 getChannelRegionLuma(MI_VPE_RegionInfo_t *pstRegionInfo, MI_U32 *pu32LumaData,MI_S32 s32MilliSec);
	MI_S32 setChannelRotation(MI_SYS_Rotate_e eType);
	
	MI_S32 getChannelRotation(MI_SYS_Rotate_e *pType);
	
	MI_S32 enablePort(MI_VPE_PORT vpePort);
	MI_S32 disablePort(MI_VPE_PORT vpePort);
	
	MI_S32 setPortMode(MI_VPE_PORT vpePort, MI_VPE_PortMode_t *pstVpeMode);
	MI_S32 getPortMode(MI_VPE_PORT vpePort, MI_VPE_PortMode_t *pstVpeMode);
	
	MI_S32 setPortCrop(MI_VPE_PORT vpePort, MI_SYS_WindowRect_t *pstOutCropInfo);
	MI_S32 getPortCrop(MI_VPE_PORT vpePort, MI_SYS_WindowRect_t *pstOutCropInfo);
	
	MI_S32 setPortShowPosition(MI_SYS_WindowRect_t *pstPortPositionInfo);
	MI_S32 getPortShowPosition(MI_SYS_WindowRect_t *pstPortPositionInfo);
	
	MI_S32 skipFrame(MI_VPE_PORT vpePort, MI_U32 u32FrameNum);
	
	MI_S32 LDCBegViewConfig();
	MI_S32 LDCEndViewConfig();
	MI_S32 LDCSetViewConfig(void *pConfigAddr, MI_U32 u32ConfigSize);
	
	MI_S32 alloc_IspDataBuf(MI_U32 u32Size,void **pUserVirAddr);
	MI_S32 free_IspDataBuf(void *pUserBuf);

	// 参考MI demo, 发现并没有VPE 设备初始化的相关调用。
	//MI_S32 initDev(MI_VPE_InitParam_t *pstInitParam);
	//MI_S32 deInitDev(void);
	
private:
	Vpe();
	~Vpe();
	Vpe(const Vpe&);
	Vpe& operator=(const Vpe&);

	bool bEnable;
	const static MI_U32 u32MaxVpePortID = 3;
	
	MI_VPE_CHANNEL vpeCh;
};

#endif

