/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.10
----------------------------------------------------------------*/

#include "ao.hpp"
#include "string.h"
#include "iostream"
#include "errno.h"
//#include "unistd.h"

using namespace std;

audioOut::audioOut()
{
	audioDev = 0;	// 默认设备号为0
	audioChn = 0;			// 默认chanel 号为0
	//pFrameData = NULL;
	streamRouteTid = -1;			// 未建立线程时tid 置-1.
	streamRouteRunning = false;

	enable();
}

audioOut::~audioOut()
{
	disable();

	audioDev = -1;
	audioChn = -1;
	//pFrameData = NULL;
	streamRouteTid = -1;
	streamRouteRunning = false;
}

int audioOut::enable()
{
	setPubAttr(E_MI_AUDIO_BIT_WIDTH_16, E_MI_AUDIO_SAMPLE_RATE_16000);
	enableDev();
	enableChanel();
	streamRouteCreate();

	return 0;
}

int audioOut::disable()
{
	streamRouteDestroy();
	disableChanel();
	disableDev();

	return 0;
}


/*
eSamplerate	音频采样率。
静态属性。
eBitwidth	音频采样精度（从模式下，此参数必须和音频AD/DA 的采样精度匹配）。
静态属性。
eWorkmode	音频输入输出工作模式。
静态属性。
eSoundmode	音频声道模式。
静态属性。
u32FrmNum	缓存帧数目。
保留，未使用。
u32PtNumPerFrm	每帧的采样点个数。
取值范围为：128, 128*2,…,128*N。
静态属性。
u32CodecChnCnt	支持的codec通道数目。
保留，未使用。
u32ChnCnt	支持的通道数目，实际可使能的最大通道数。取值：1、2、4、8、16。
			（输入最多支持MI_AUDIO_MAX_CHN_NUM 个通道，输出最多支持2 个通道）
MI_AUDIO_I2sConfig_t stI2sConfig;	设置I2S 工作属性
*/

/*-----------------------------------------------------------------------------
描--述：设置AO 公有属性。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioOut::setPubAttr(MI_AUDIO_BitWidth_e eBitWidth, MI_AUDIO_SampleRate_e eSample)
{
	MI_AUDIO_Attr_t stAttr;
	memset(&stAttr, 0, sizeof(MI_AUDIO_Attr_t));

	unsigned u32ChnCnt = 1;
	unsigned u32PtNumPerFrm = 128 * 4;		// 每帧的采样点个数。取值范围为：128, 128*2,…,128*

	stAttr.eWorkmode = E_MI_AUDIO_MODE_I2S_MASTER;
	stAttr.WorkModeSetting.stI2sConfig.bSyncClock = FALSE;
	stAttr.WorkModeSetting.stI2sConfig.eFmt = E_MI_AUDIO_I2S_FMT_I2S_MSB;
	stAttr.WorkModeSetting.stI2sConfig.eMclk = E_MI_AUDIO_I2S_MCLK_0;
	stAttr.eBitwidth = eBitWidth;
	stAttr.u32PtNumPerFrm = u32PtNumPerFrm;
	stAttr.u32ChnCnt = u32ChnCnt;
	stAttr.eSamplerate = eSample;
	if(1 == u32ChnCnt)
	{
		stAttr.eSoundmode = E_MI_AUDIO_SOUND_MODE_MONO;
	}
	else if(2 == u32ChnCnt)
	{
		stAttr.eSoundmode = E_MI_AUDIO_SOUND_MODE_STEREO;
	}

	// MI_S32 MI_AO_SetPubAttr(MI_AUDIO_DEV AoDevId, MI_AUDIO_Attr_t *pstAttr);
	MI_S32 s32Ret = 0;
	s32Ret = MI_AO_SetPubAttr(audioDev, &stAttr);  
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AO_SetPubAttr(). s32Ret = 0x" << hex << s32Ret << endl;
	}
	cout << "Success to call MI_AO_SetPubAttr()." << endl;
	
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：使能AO 设备
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioOut::enableDev()
{	
	// MI_S32 MI_AO_Enable(MI_AUDIO_DEV AoDevId);
	MI_S32 s32Ret = 0;
	s32Ret = MI_AO_Enable(audioDev);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AO_Enable(). s32Ret = 0x" << hex << s32Ret << endl;
	}
	cout << "Success to call MI_AO_Enable()." << endl;
	
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：禁用AO 设备
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioOut::disableDev()
{
	// MI_S32 MI_AO_Disable(MI_AUDIO_DEV AoDevId);
	MI_S32 s32Ret = 0;
	s32Ret = MI_AO_Disable(audioDev);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AO_Disable(). s32Ret = 0x" << hex << s32Ret << endl;
	}
	cout << "Success to call MI_AO_Disable()." << endl;

	return s32Ret;
}


/*-----------------------------------------------------------------------------
描--述：使能AO 设备下的chanel
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioOut::enableChanel()
{
	// MI_S32 MI_AO_EnableChn(MI_AUDIO_DEV AoDevId, MI_AO_CHN AoChn);
	MI_S32 s32Ret = 0;
	s32Ret = MI_AO_EnableChn(audioDev, audioChn);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AO_EnableChn(). s32Ret = 0x" << hex << s32Ret << endl;
	}
	cout << "Success to call MI_AO_EnableChn()." << endl;

	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：禁用AO 设备下的chanel
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioOut::disableChanel()
{
	// MI_S32 MI_AO_EnableChn(MI_AUDIO_DEV AoDevId, MI_AO_CHN AoChn);
	MI_S32 s32Ret = 0;
	s32Ret = MI_AO_DisableChn(audioDev, audioChn);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AO_DisableChn(). s32Ret = 0x" << hex << s32Ret << endl;
	}
	cout << "Success to call MI_AO_DisableChn()." << endl;

	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：处理音频流的线程函数。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
void *audioOut::__route(void *arg)
{
	pthread_detach(pthread_self());

	audioOut *pThis = (audioOut *)arg;
	return pThis->route(NULL);
}
void *audioOut::route(void *arg)
{
	while(streamRouteTid)
	{
		sem_wait(&sem0);
		
		// MI_S32 MI_AO_SendFrame(MI_AUDIO_DEV AoDevId, MI_AO_CHN AoChn, MI_AUDIO_Frame_t *pstData, MI_S32 s32MilliSec);
		MI_S32 s32Ret = -1;
		s32Ret = MI_AO_SendFrame(audioDev, audioChn, &stAudioOutFrame.stAudioFrame, -1);
		if(0 != s32Ret)
		{
			cerr << "Fail to call MI_AO_SendFrame(). s32Ret = 0x" << hex << s32Ret << endl;
		}
		memset(&stAudioOutFrame, 0, sizeof(AudioOutFrame_t));
		sem_post(&sem1);
	}
	
	return NULL;
}

/*-----------------------------------------------------------------------------
描--述：建立stream流线程。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioOut::streamRouteCreate()
{	
	int ret = 0;
	int initVal = 0;
	bool bShared = false;
	
	ret = sem_init(&sem0, bShared, initVal);
	if(0 != ret)
	{
		cerr << "Fail to call sem_init(3), ret = " << ret << ". " << strerror(errno) << endl;
		return ret;
	}
	cout << "Success to call sem_init(3)." << endl;

	initVal = 1;
	ret = sem_init(&sem1, bShared, initVal);
	if(0 != ret)
	{
		cerr << "Fail to call sem_init(3), ret = " << ret << ". " << strerror(errno) << endl;
		return ret;
	}
	cout << "Success to call sem_init(3)." << endl;
	
	ret = pthread_create(&streamRouteTid, NULL, __route, this);
	if(0 != ret)
	{
		cerr << "Fail to call pthread_create(3), ret = " << ret << ". " << strerror(errno) << endl;
		return ret;
	}
	cout << "Success to call pthread_create(3)." << endl;
	
	streamRouteTid = true;
	return -1;
}

/*-----------------------------------------------------------------------------
描--述：结束stream流线程。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioOut::streamRouteDestroy()
{
	streamRouteTid = false;

	if(-1 != streamRouteTid)
	{
		streamRouteTid = -1;
	}

	int ret = 0;
	ret = sem_destroy(&sem0);
	if(0 != ret)
	{
		cerr << "Fail to call sem_destroy(3), ret = " << ret << ". " << strerror(errno) << endl;
	}
	cout << "Success to call sem_destroy(3)." << endl;

	ret = sem_destroy(&sem1);
	if(0 != ret)
	{
		cerr << "Fail to call sem_destroy(3), ret = " << ret << ". " << strerror(errno) << endl;
	}
	cout << "Success to call sem_destroy(3)." << endl;
	
	return 0;
}

/*-----------------------------------------------------------------------------
描--述：结束stream流线程。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioOut::sendStream(AudioOutFrame_t *pstAudioOutFrame)
{
	if(NULL == pstAudioOutFrame)
	{
		cerr << "Fail to call sendStream(), argument has NULL value!" << endl;
		return -1;
	}

	sem_wait(&sem1);
	memcpy(&stAudioOutFrame, pstAudioOutFrame, sizeof(AudioOutFrame_t));
	stAudioOutFrame.stAudioFrame.apVirAddr[0] = stAudioOutFrame.audioBuf;
	stAudioOutFrame.stAudioFrame.apVirAddr[1] = NULL;
	sem_post(&sem0);

	return 0;
}

/*-----------------------------------------------------------------------------
描--述：设置音量
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioOut::setVolume(int val)
{
	int valMin = -60;
	int valMax = 30;
	
	if(val < valMin || val > valMax)
	{
		cerr << "Fail to call setVolume(), bad argument!" << endl;
		cerr << "Volume range: [" << valMin << ", " << valMax << "]" << endl;
	}

	MI_S32 s32Ret = 0;
	s32Ret = MI_AO_SetVolume(audioDev, val);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AO_SetVolume()." << endl;
	}

	return s32Ret;
}

