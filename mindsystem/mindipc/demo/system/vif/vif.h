/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.10
----------------------------------------------------------------*/

/*
MI_S32 MI_VIF_SetDevAttr(MI_VIF_DEV u32VifDev, MI_VIF_DevAttr_t *pstDevAttr);
MI_S32 MI_VIF_GetDevAttr(MI_VIF_DEV u32VifDev, MI_VIF_DevAttr_t *pstDevAttr);
MI_S32 MI_VIF_EnableDev(MI_VIF_DEV u32VifDev);
MI_S32 MI_VIF_DisableDev(MI_VIF_DEV u32VifDev);
MI_S32 MI_VIF_SetChnPortAttr(MI_VIF_CHN u32VifChn, MI_VIF_PORT u32ChnPort, MI_VIF_ChnPortAttr_t *pstAttr);
MI_S32 MI_VIF_GetChnPortAttr(MI_VIF_CHN u32VifChn, MI_VIF_PORT u32ChnPort, MI_VIF_ChnPortAttr_t *pstAttr);
MI_S32 MI_VIF_EnableChnPort(MI_VIF_CHN u32VifChn, MI_VIF_PORT u32ChnPort);
MI_S32 MI_VIF_DisableChnPort(MI_VIF_CHN u32VifChn, MI_VIF_PORT u32ChnPort);
MI_S32 MI_VIF_Query(MI_VIF_CHN u32VifChn, MI_VIF_PORT u32ChnPort, MI_VIF_ChnPortStat_t *pstStat);
MI_S32 MI_VIF_SetDev2SnrPadMux(MI_VIF_Dev2SnrPadMuxCfg_t *pstVifDevMap, MI_U8 u8Length);
MI_S32 MI_VIF_InitDev(MI_VIF_InitParam_t *pstInitParam);
MI_S32 MI_VIF_DeInitDev(void);
*/
	
#ifndef __VIF_H__
#define __VIF_H__
	
#include "mi_vif.h"

class Vif{
public:
	const static MI_VIF_PORT vifPort = 0;
	
	static Vif* getInstance();
	MI_S32 enable();
	MI_S32 disable();
	
	MI_S32 setDevAttr();
	MI_S32 getDevAttr(MI_VIF_DevAttr_t *pstDevAttr);
	MI_S32 enableDev();
	MI_S32 disableDev();
	MI_S32 setChnPortAttr(MI_VIF_PORT u32ChnPort);
	MI_S32 getChnPortAttr(MI_VIF_PORT u32ChnPort);
	MI_S32 enableChnPort(MI_VIF_PORT u32ChnPort);
	MI_S32 disableChnPort(MI_VIF_PORT u32ChnPort);
	MI_S32 query();
	MI_S32 setDev2SnrPadMux();
	MI_S32 initDev();
	MI_S32 deInitDev();

private:
	Vif();
	~Vif();
	Vif(const Vif&);
	Vif& operator=(const Vif&);

	bool bEnable;
	MI_VIF_DEV u32VifDev;
	MI_VIF_CHN u32VifChn;
};

#endif
