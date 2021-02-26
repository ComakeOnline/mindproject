/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.10
----------------------------------------------------------------*/

#include "vif.h"
#include "iostream"
#include "string.h"
#include "sensor.hpp"

using namespace std;

Vif::Vif()
{
	u32VifDev = 0;
	u32VifChn = 0;
	bEnable = false;
	
	enable();
}

Vif::~Vif()
{
	disable();
	
	bEnable = false;	
	u32VifChn = 0;
	u32VifDev = 0;
}

/*-----------------------------------------------------------------------------
描--述：VIF 模块获取实例的唯一入口函数。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
Vif* Vif::getInstance()
{
	static Vif vif;
	return &vif;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Vif::enable()
{
	cout << "Call Vif::enable()." << endl;
	
	if(bEnable)
	{
		cerr << "Fail to call Vif::enable(). Modules is already been enabled!" << endl;
		return 0;
	}

	setDevAttr();
	enableDev();
	setChnPortAttr(vifPort);
	enableChnPort(vifPort);

	bEnable = true;

	cout << "Call Vif::enable() end." << endl;
	return 0;	
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Vif::disable()
{
	cout << "Call Vif::disable()." << endl;

	disableChnPort(1);
	disableChnPort(0);
	disableDev();
	bEnable = false;

	cout << "Call Vif::disable() end." << endl;
	return 0;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：首选获取sensor 模块的pad、plane 信息，然后引用相关数据设置VIF 设备属性。
-----------------------------------------------------------------------------*/
MI_S32 Vif::setDevAttr()
{
	cout << "Call Vif::setDevAttr()." << endl;
	
	MI_S32 s32Ret = 0;
	
	// step1: 读取sensor 信息。
	MI_SNR_PADInfo_t stPadInfo;
	memset(&stPadInfo, 0, sizeof(MI_SNR_PADInfo_t));
	
	Sensor *pSensor = Sensor::getInstance();
	s32Ret = pSensor->getPadInfo(&stPadInfo);
	if(0 != s32Ret)
	{
		cerr << "Fail to call pSensor->getPadInfo()." << endl;
		return s32Ret;
	}
	
	// step2: 填充VIF 设备属性结构体。
	//MI_S32 MI_VIF_SetDevAttr(MI_VIF_DEV u32VifDev, MI_VIF_DevAttr_t *pstDevAttr);
	MI_VIF_DevAttr_t stDevAttr;
	memset(&stDevAttr, 0, sizeof(MI_VIF_DevAttr_t));

	stDevAttr.eIntfMode = stPadInfo.eIntfMode;
	stDevAttr.eHDRType = E_MI_VIF_HDR_TYPE_OFF;				// 参考mi_demo/jedi/rtsp/st_main_rtsp.c
	stDevAttr.eBitOrder = E_MI_VIF_BITORDER_NORMAL;
	stDevAttr.eWorkMode = E_MI_VIF_WORK_MODE_RGB_REALTIME;	// 参考mi_demo/jedi/internal/vif/st_vif.c
	
	if(E_MI_VIF_MODE_BT656 == stDevAttr.eIntfMode)
	{
		stDevAttr.eClkEdge = stPadInfo.unIntfAttr.stBt656Attr.eClkEdge;
	}
	else
	{
		stDevAttr.eClkEdge = E_MI_VIF_CLK_EDGE_DOUBLE;
	}
	
	if(E_MI_VIF_MODE_MIPI == stDevAttr.eIntfMode)
	{
		stDevAttr.eDataSeq = stPadInfo.unIntfAttr.stMipiAttr.eDataYUVOrder;
	}
	else
	{
		stDevAttr.eDataSeq = E_MI_VIF_INPUT_DATA_YUYV;
	}

	if(E_MI_VIF_MODE_BT656 == stDevAttr.eIntfMode)
	{
		memcpy(&stDevAttr.stSyncAttr, &stPadInfo.unIntfAttr.stBt656Attr.stSyncAttr, sizeof(MI_VIF_SyncAttr_t));
	}

	// step3: 设置VIF 设备属性结构体。
	s32Ret = MI_VIF_SetDevAttr(u32VifDev, &stDevAttr);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_VIF_SetDevAttr(), errno = " << s32Ret << endl;
	}

	cout << "Call Vif::setDevAttr() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Vif::getDevAttr(MI_VIF_DevAttr_t *pstDevAttr)
{
	cout << "Call Vif::getDevAttr()." << endl;

	if(NULL == pstDevAttr)
	{
		cerr << "Fail to call Vif::getDevAttr(), argument has null value." << endl;
		return 0;
	}
	
	//MI_S32 MI_VIF_GetDevAttr(MI_VIF_DEV u32VifDev, MI_VIF_DevAttr_t *pstDevAttr);
	MI_S32 s32Ret = 0;

	memset(pstDevAttr, 0, sizeof(MI_VIF_DevAttr_t));
	s32Ret = MI_VIF_GetDevAttr(u32VifDev, pstDevAttr);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_VIF_GetDevAttr(), errno = " << s32Ret << endl;
	}

	cout << "Call Vif::getDevAttr() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Vif::enableDev()
{
	cout << "Call Vif::enableDev()." << endl;
	//MI_S32 MI_VIF_EnableDev(MI_VIF_DEV u32VifDev);

	MI_S32 s32Ret = 0;
	s32Ret = MI_VIF_EnableDev(u32VifDev);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_VIF_EnableDev(), errno = " << s32Ret << endl;
	}

	cout << "Call Vif::enableDev() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Vif::disableDev()
{
	cout << "Call Vif::disableDev()." << endl;

	// MI_S32 MI_VIF_DisableDev(MI_VIF_DEV u32VifDev);
	MI_S32 s32Ret = 0;

	
	s32Ret = MI_VIF_DisableDev(u32VifDev);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_VIF_DisableDev(), errno = " << s32Ret << endl;
	}

	cout << "Call Vif::disableDev() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Vif::setChnPortAttr(MI_VIF_PORT u32ChnPort)
{
	cout << "Call Vif::setChnPortAttr()." << endl;

	//MI_S32 MI_VIF_SetChnPortAttr(MI_VIF_CHN u32VifChn, MI_VIF_PORT u32ChnPort, MI_VIF_ChnPortAttr_t *pstAttr);
	MI_S32 s32Ret = 0;

	// step1: 获取sensor 模块plane 信息。
	MI_SNR_PlaneInfo_t stPlaneInfo;
	memset(&stPlaneInfo, 0, sizeof(MI_SNR_PlaneInfo_t));
	
	Sensor *pSensor = Sensor::getInstance();
	s32Ret = pSensor->getPlaneInfo(&stPlaneInfo);
	if(0 != s32Ret)
	{
		cerr << "Fail to call pSensor->getPlaneInfo()." << endl;
		return s32Ret;
	}

	// step2: 获取VIF 模块属性信息。
	MI_VIF_DevAttr_t stDevAttr;
	memset(&stDevAttr, 0, sizeof(MI_VIF_DevAttr_t));

	s32Ret = getDevAttr(&stDevAttr);
	if(0 != s32Ret)
	{
		cerr << "Fail to call Vif::getDevAttr()." << endl;
		return s32Ret;
	}

	// step3: 根据sensor 和VIF 信息填充VIF 频道端口属性结构体。
	MI_VIF_ChnPortAttr_t stChnPortAttr;
	memset(&stChnPortAttr, 0, sizeof(MI_VIF_ChnPortAttr_t));
	
	stChnPortAttr.stCapRect.u16X = stPlaneInfo.stCapRect.u16X;
	stChnPortAttr.stCapRect.u16Y = stPlaneInfo.stCapRect.u16Y;
	stChnPortAttr.stCapRect.u16Width = stPlaneInfo.stCapRect.u16Width;
	stChnPortAttr.stCapRect.u16Height = stPlaneInfo.stCapRect.u16Height;
	stChnPortAttr.stDestSize.u16Width = stPlaneInfo.stCapRect.u16Width;
	stChnPortAttr.stDestSize.u16Height = stPlaneInfo.stCapRect.u16Height;
	MI_SYS_PixelFormat_e ePixFormat = (MI_SYS_PixelFormat_e)RGB_BAYER_PIXEL(stPlaneInfo.ePixPrecision, stPlaneInfo.eBayerId);
	stChnPortAttr.ePixFormat = ePixFormat;
	
	if(E_MI_VIF_MODE_BT656 == stDevAttr.eIntfMode)
	{
		stChnPortAttr.eFrameRate = E_MI_VIF_FRAMERATE_FULL;
		stChnPortAttr.eCapSel = E_MI_SYS_FIELDTYPE_BOTH;
		stChnPortAttr.eScanMode = E_MI_SYS_FRAME_SCAN_MODE_PROGRESSIVE;
	}

	// step4: 设置VIF 频道端口属性。
	s32Ret = MI_VIF_SetChnPortAttr(u32VifChn, u32ChnPort, &stChnPortAttr);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_VIF_SetChnPortAttr(), errno = " << s32Ret << endl;
	}
	
	cout << "Call Vif::setChnPortAttr() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Vif::getChnPortAttr(MI_VIF_PORT u32ChnPort)
{
	cout << "Call Vif::getChnPortAttr()." << endl;

	MI_S32 s32Ret = 0;
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_VIF_SetDevAttr(), errno = " << s32Ret << endl;
	}

	cout << "Call Vif::getChnPortAttr() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Vif::enableChnPort(MI_VIF_PORT u32ChnPort)
{
	cout << "Call Vif::enableChnPort()." << endl;

	//MI_S32 MI_VIF_EnableChnPort(MI_VIF_CHN u32VifChn, MI_VIF_PORT u32ChnPort);
	MI_S32 s32Ret = 0;

	s32Ret = MI_VIF_EnableChnPort(u32VifChn, u32ChnPort);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_VIF_EnableChnPort(), errno = " << s32Ret << endl;
	}

	cout << "Call Vif::enableChnPort() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Vif::disableChnPort(MI_VIF_PORT u32ChnPort)
{
	cout << "Call Vif::disableChnPort()." << endl;

	// MI_S32 MI_VIF_DisableChnPort(MI_VIF_CHN u32VifChn, MI_VIF_PORT u32ChnPort);
	MI_S32 s32Ret = 0;

	s32Ret = MI_VIF_DisableChnPort(u32VifChn, u32ChnPort);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_VIF_DisableChnPort(), errno = " << s32Ret << endl;
	}

	cout << "Call Vif::disableChnPort() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Vif::query()
{
	cout << "Call Vif::query()." << endl;

	MI_S32 s32Ret = 0;
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_VIF_SetDevAttr(), errno = " << s32Ret << endl;
	}

	cout << "Call Vif::query() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Vif::setDev2SnrPadMux()
{
	cout << "Call Vif::setDev2SnrPadMux()." << endl;

	MI_S32 s32Ret = 0;
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_VIF_SetDevAttr(), errno = " << s32Ret << endl;
	}
	
	cout << "Call Vif::setDev2SnrPadMux() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Vif::initDev()
{
	cout << "Call Vif::initDev()." << endl;

	MI_S32 s32Ret = 0;
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_VIF_SetDevAttr(), errno = " << s32Ret << endl;
	}

	cout << "Call Vif::initDev() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
MI_S32 Vif::deInitDev()
{
	cout << "Call Vif::deInitDev()." << endl;

	MI_S32 s32Ret = 0;
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_VIF_SetDevAttr(), errno = " << s32Ret << endl;
	}

	cout << "Call Vif::deInitDev() end." << endl;
	return s32Ret;
}

