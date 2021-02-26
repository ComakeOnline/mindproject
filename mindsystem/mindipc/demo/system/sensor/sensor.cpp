/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.10
----------------------------------------------------------------*/

#include "sensor.hpp"
#include "iostream"
#include "string.h"

using namespace std;

Sensor::Sensor()
{
	ePADId = E_MI_SNR_PAD_ID_0;
	u32PlaneID = 0;
	u32ResCnt = 0;
	bEnable = false;
	
	enable();	
}

Sensor::~Sensor()
{
	disable();
	
	bEnable = false;	
	u32ResCnt = 0;
	u32PlaneID = 0;
	ePADId = E_MI_SNR_PAD_ID_NA;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::getFps(MI_U32 *pFps)
{
	cout << "Call Sensor::getFps()." << endl;
	//MI_S32 MI_SNR_GetFps(MI_SNR_PAD_ID_e	  ePADId, MI_U32 *pFps);
	
	MI_S32 s32Ret = 0;
	s32Ret = MI_SNR_GetFps(ePADId, pFps);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_SNR_GetFps(), errno = " << s32Ret << endl;
	}
	cout << "FPS = " << *pFps << endl;

	cout << "Call Sensor::getFps() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::setFps(MI_U32 u32Fps)
{
	cout << "Call Sensor::setFps()." << endl;
	
	//MI_S32 MI_SNR_SetFps(MI_SNR_PAD_ID_e	  ePADId, MI_U32  u32Fps);
	MI_S32 s32Ret = 0;
	s32Ret = MI_SNR_SetFps(ePADId, u32Fps);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_SNR_SetFps(), errno = " << s32Ret << endl;
	}

	cout << "Call Sensor::setFps() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::queryResCount(MI_U32 *pu32ResCount)
{
	cout << "Call Sensor::queryResCount()." << endl;
	
	//MI_S32 MI_SNR_QueryResCount(MI_SNR_PAD_ID_e ePADId, MI_U32 *pu32ResCount);
	MI_S32 s32Ret = 0;
	s32Ret = MI_SNR_QueryResCount(ePADId, pu32ResCount);
	if(0 != s32Ret)
	{
		cerr << "Fail to call queryResCount(), errno = " << s32Ret << endl;
	}
	u32ResCnt = *pu32ResCount;

	cout << "Call Sensor::queryResCount() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::getRes()
{
	cout << "Call Sensor::getRes()." << endl;
	
	MI_U32 u32ResCount = 0;
	queryResCount(&u32ResCount);

	//MI_S32 MI_SNR_GetRes(MI_SNR_PAD_ID_e ePADId, MI_U8 u8ResIdx, MI_SNR_Res_t *pstRes);
	int i = 0;
	for(i = 0; i < u32ResCount; ++i)
	{
		MI_S32 s32Ret = 0;
		MI_SNR_Res_t stRet;

		memset(&stRet, 0, sizeof(MI_SNR_Res_t));
		s32Ret = MI_SNR_GetRes(ePADId, i, &stRet);
		if(0 != s32Ret)
		{
			cerr << "Fail to call MI_SNR_SetPlaneMode(), errno = " << s32Ret << endl;
			continue;
		}

		cout << "Sensor res index: " << i << endl;
		cout << "WindowOutputSize[w, h]:" << stRet.stOutputSize.u16Width << ", " << stRet.stOutputSize.u16Height << endl;
		cout << "WindowCropRect[x, y, w, h]: " << stRet.stCropRect.u16X << ", " << stRet.stCropRect.u16Y << ", "
			<< stRet.stCropRect.u16Width << ", " << stRet.stCropRect.u16Height << endl;
		cout << "FPS[min, max]: " << stRet.u32MinFps << ", " << stRet.u32MaxFps << endl;
		cout << "strResDesc[32]: " << stRet.strResDesc << endl;
	}

	cout << "Call Sensor::getRes() end." << endl;
	return 0;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::setRes(MI_U8 u8ResIdx)
{
	cout << "Call Sensor::setRes()." << endl;

	
	//MI_S32 MI_SNR_SetRes(MI_SNR_PAD_ID_e ePADId, MI_U8 u8ResIdx);
	MI_S32 s32Ret = 0;
	s32Ret = MI_SNR_SetRes(ePADId, u8ResIdx);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_SNR_SetPlaneMode(), errno = " << s32Ret << endl;
	}

	cout << "Call Sensor::setRes() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::sensorEnable()
{

	cout << "Call Sensor::sensorEnable()." << endl;
	
	if(bEnable)
	{
		return 0;
	}
	
	//MI_S32 MI_SNR_Enable(MI_SNR_PAD_ID_e     ePADId);
	MI_S32 s32Ret = 0;
	s32Ret = MI_SNR_Enable(ePADId);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_SNR_Enable(), errno = " << s32Ret << endl;
	}

	cout << "Call Sensor::sensorEnable() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::sensorDisable()
{
	cout << "Call Sensor::sensorDisable()." << endl;
	
	if(!bEnable)
	{
		return 0;
	}
	
	//MI_S32 MI_SNR_Disable(MI_SNR_PAD_ID_e     ePADId);
	MI_S32 s32Ret = 0;
	s32Ret = MI_SNR_Disable(ePADId);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_SNR_Disable(), errno = " << s32Ret << endl;
	}

	cout << "Call Sensor::sensorDisable() end." << endl;
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::enable(MI_U32 u32Fps)
{
	cout << "Call Sensor::enable()." << endl;

	if(bEnable)
	{
		cerr << "Fail to call Sensor::enable(). Modules has already been enabled!" << endl;
		return 0;
	}
	
	setPlaneMode();
	getRes();
	setRes(0);
	setFps(u32Fps);
	sensorEnable();	
	bEnable = true;

	cout << "Call Sensor::enable() end." << endl;
	return 0;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::disable()
{
	cout << "Call Sensor::disable()." << endl;
	
	sensorDisable();
	bEnable = false;

	cout << "Call Sensor::disable() end." << endl;
	return 0;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::getPadInfo(MI_SNR_PADInfo_t *pstPadInfo)
{
	cout << "Call Sensor::getPadInfo()." << endl;
	
	if(NULL == pstPadInfo)
	{
		cerr << "Fail to call getPadInfo(), argument has null value!"<< endl;
		return -1;
	}

	// MI_S32 MI_SNR_GetPadInfo(MI_SNR_PAD_ID_e ePADId, MI_SNR_PADInfo_t *pstPadInfo);
	MI_S32 s32Ret = 0;
	memset(pstPadInfo, 0, sizeof(MI_SNR_PADInfo_t));
	
	s32Ret = MI_SNR_GetPadInfo(ePADId, pstPadInfo);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_VIF_SetDevAttr(), errno = " << s32Ret << endl;
	}

	cout << "Call Sensor::getPadInfo() end." << endl;
	return s32Ret;	
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::setPadInfo()
{
	cout << "Call Sensor::getPadInfo()." << endl;

	cout << "Call Sensor::getPadInfo() end." << endl;
	return 0;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::getPlaneInfo(MI_SNR_PlaneInfo_t *pstPlaneInfo)
{
	cout << "Call Sensor::getPlaneInfo()." << endl;
	
	//MI_S32 MI_SNR_GetPlaneInfo(MI_SNR_PAD_ID_e ePADId, MI_U32 u32PlaneID, MI_SNR_PlaneInfo_t *pstPlaneInfo);
	MI_S32 s32Ret = 0;

	memset(pstPlaneInfo, 0, sizeof(MI_SNR_PlaneInfo_t));
	s32Ret = MI_SNR_GetPlaneInfo(ePADId, u32PlaneID, pstPlaneInfo);
	if(0 != s32Ret)
	{
		cerr << "Fail to call getPlaneInfo(), errno = " << s32Ret << endl;
	}

	cout << "Call Sensor::getPlaneInfo() end." << endl;
	return s32Ret;	
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::getPlaneMode()
{
	cout << "Call Sensor::getPlaneMode()." << endl;
	
	cout << "Call Sensor::getPlaneMode() end." << endl;
	return 0;
}

/*-----------------------------------------------------------------------------
描--述：
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int Sensor::setPlaneMode()
{
	cout << "Call Sensor::setPlaneMode()." << endl;
	
	//MI_S32 MI_SNR_SetPlaneMode(MI_SNR_PAD_ID_e ePADId, MI_BOOL bEnable);
	MI_S32 s32Ret = 0;
	MI_BOOL bEnableHDR = false;
	
	s32Ret = MI_SNR_SetPlaneMode(ePADId, bEnableHDR);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_SNR_SetPlaneMode(), errno = " << s32Ret << endl;
	}

	cout << "Call Sensor::setPlaneMode() end." << endl;
	return s32Ret;
}

