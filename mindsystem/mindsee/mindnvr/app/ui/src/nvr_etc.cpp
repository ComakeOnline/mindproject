#include "nvr_etc.hpp"

static bool enHcDetect = true;
static bool enHcOSD = false;
static bool enASR = true;
static bool enNetStatusDisp = true;

int UI_ETC()
{
	return 0;
}

/*-----------------------------------------------------------------------------
描   述：如下函数返回功能开关的状态，开关包括：
		人形检测开关、人形检测OSD弹窗的开关、语音识别开关、网络状态显示开关
参	数：
返回值：返回功能开关的状态，bool 类型。
注	意：
-----------------------------------------------------------------------------*/
bool UI_HcDetectEnabled()
{
	return enHcDetect;
}

bool UI_HcOSDEnabled()
{
	return enHcOSD;
}

bool UI_ASREnabled()
{
	return enASR;
}

bool UI_NetStatusDispEnabled()
{
	return enNetStatusDisp;
}

/*-----------------------------------------------------------------------------
描   述：如下函数禁用功能开关，开关包括：
		人形检测开关、人形检测OSD弹窗的开关、语音识别开关、网络状态显示开关
参	数：
返回值：返回设置后的功能开关的状态，bool 类型。
注	意：
-----------------------------------------------------------------------------*/
bool UI_DisableHcDetect()
{
	return enHcDetect = false;
}

bool UI_DisableHcOSD()
{
	return enHcOSD = false;
}

bool UI_DisableASR()
{
	return enASR = false;
}

bool UI_DisableNetStatusDisp()
{
	return enNetStatusDisp = false;
}

/*-----------------------------------------------------------------------------
描   述：如下函数启用功能开关，开关包括：
		人形检测开关、人形检测OSD弹窗的开关、语音识别开关、网络状态显示开关
参	数：
返回值：返回功能开关的状态，bool 类型。
注	意：
-----------------------------------------------------------------------------*/
bool UI_EnableHcDetect()
{
	return enHcDetect = true;
}

bool UI_EnableHcOSD()
{
	return enHcOSD = true;
}

bool UI_EnableASR()
{
	return enASR = true;
}

bool UI_EnableNetStatusDisp()
{
	return enNetStatusDisp = true;
}

/*-----------------------------------------------------------------------------
描   述：如下函数将功能开关的状态取反，开关包括：
		人形检测开关、人形检测OSD弹窗的开关、语音识别开关、网络状态显示开关
参	数：
返回值：返回功能开关的状态，bool 类型。
注	意：
-----------------------------------------------------------------------------*/
bool UI_InvertHcDetect()
{
	return enHcDetect = !enHcDetect;
}

bool UI_InvertHcOSD()
{
	return enHcOSD = !enHcOSD;
}

bool UI_InvertASR()
{
	return enASR = !enASR;
}

bool UI_InvertNetStatusDisp()
{
	return enNetStatusDisp = !enNetStatusDisp;
}

