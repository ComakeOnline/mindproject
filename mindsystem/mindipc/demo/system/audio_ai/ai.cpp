/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.10
----------------------------------------------------------------*/

#include "ai.hpp"
#include "string.h"
#include "iostream"
#include "errno.h"
#include "mi_sys.h"
#include "unistd.h"
#include "stdio.h"

using namespace std;

audioIn::audioIn()
{
	audioDev = 0;			// 默认设备号为0
	audioChn = 0;			// 默认chanel 号为0
	streamRouteTid = -1;	// 未建立线程时tid 置-1.
	streamRouteRunning = false;
	bFrameBufBusy = false;

	enable();
}

audioIn::~audioIn()
{
	disable();
	
	audioDev = -1;
	audioChn = -1;
	streamRouteTid = -1;
	streamRouteRunning = false;
	bFrameBufBusy = false;
}

int audioIn::enable()
{
	setPubAttr(E_MI_AUDIO_BIT_WIDTH_16, E_MI_AUDIO_SAMPLE_RATE_16000);
	enableDev();
	enableChanel();
	setChnOutputPortDepth();
	enableAed();
	setVqeVolume(10);
	streamRouteCreate();

	return 0;
}

int audioIn::disable()
{
	streamRouteDestroy();
	disableAed();
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
描--述：设置AI 公有属性。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioIn::setPubAttr(MI_AUDIO_BitWidth_e eBitWidth, MI_AUDIO_SampleRate_e eSample)
{
	MI_AUDIO_Attr_t stAttr;
	memset(&stAttr, 0, sizeof(MI_AUDIO_Attr_t));

	MI_U32 u32FrameNum = 6;
	MI_U32 u32ChnCnt = 1;
	MI_U32 u32PtNumPerFrm = 160;	// 每一帧的采样点数，可以不必128 字对齐

	stAttr.eWorkmode = E_MI_AUDIO_MODE_I2S_MASTER;	// demo
	//stAttr.eWorkmode = E_MI_AUDIO_MODE_I2S_SLAVE;	// MI_API
	stAttr.WorkModeSetting.stI2sConfig.eFmt = E_MI_AUDIO_I2S_FMT_I2S_MSB;
	stAttr.WorkModeSetting.stI2sConfig.eMclk = E_MI_AUDIO_I2S_MCLK_0;
	stAttr.eBitwidth = eBitWidth;
	stAttr.u32ChnCnt = u32ChnCnt;
	stAttr.eSamplerate = eSample;
	//stAttr.u32PtNumPerFrm = u32PtNumPerFrm;			// for aec	// MI_API
	//stAttr.u32PtNumPerFrm = stAttr.eSamplerate / 16;	// for aec	// demo
	stAttr.u32PtNumPerFrm = 1024;	// for aec	// demo
	if(1 == u32ChnCnt)
	{
		stAttr.eSoundmode = E_MI_AUDIO_SOUND_MODE_MONO;
	}
	else if(2 == u32ChnCnt)
	{
		stAttr.eSoundmode = E_MI_AUDIO_SOUND_MODE_STEREO;
	}

	// MI_S32 MI_AI_SetPubAttr(MI_AUDIO_DEV AIDevId, MI_AUDIO_Attr_t *pstAttr);
	MI_S32 s32Ret = 0;
	s32Ret = MI_AI_SetPubAttr(audioDev, &stAttr);  
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AI_SetPubAttr(). s32Ret = 0x" << hex << s32Ret << endl;
	}
	cout << "Success to call MI_AI_SetPubAttr()." << endl;
	
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：设置MI 系统通道输出端口的深度。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioIn::setChnOutputPortDepth()
{
	MI_SYS_ChnPort_t stChnOutputPort;
	memset(&stChnOutputPort, 0, sizeof(MI_SYS_ChnPort_t));

	
	stChnOutputPort.eModId = E_MI_MODULE_ID_AI;
	stChnOutputPort.u32DevId = audioDev;
	stChnOutputPort.u32ChnId = audioChn;
	stChnOutputPort.u32PortId = 0;

	MI_S32 s32Ret = 0;
	MI_U32 u32UserFrameDepth = 1;
	MI_U32 u32BufQueueDepth = 4;
	// MI_S32 MI_SYS_SetChnOutputPortDepth(MI_SYS_ChnPort_t *pstChnPort , MI_U32 u32UserFrameDepth , MI_U32 u32BufQueueDepth);
	s32Ret = MI_SYS_SetChnOutputPortDepth(&stChnOutputPort, u32UserFrameDepth, u32BufQueueDepth);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_SYS_SetChnOutputPortDepth(). s32Ret = 0x" << hex << s32Ret << endl;
	}
	
	return s32Ret;
}


/*-----------------------------------------------------------------------------
描--述：使能AI 设备
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioIn::enableDev()
{	
	// MI_S32 MI_AI_Enable(MI_AUDIO_DEV AIDevId);
	MI_S32 s32Ret = 0;
	s32Ret = MI_AI_Enable(audioDev);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AI_Enable(). s32Ret = 0x" << hex << s32Ret << endl;
	}
	cout << "Success to call MI_AI_Enable()." << endl;
	
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：禁用AI 设备
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioIn::disableDev()
{
	// MI_S32 MI_AI_Disable(MI_AUDIO_DEV AIDevId);
	MI_S32 s32Ret = 0;
	s32Ret = MI_AI_Disable(audioDev);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AI_Disable(). s32Ret = 0x" << hex << s32Ret << endl;
	}
	cout << "Success to call MI_AI_Disable()." << endl;

	return s32Ret;
}


/*-----------------------------------------------------------------------------
描--述：使能AI 设备下的chanel
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioIn::enableChanel()
{
	// MI_S32 MI_AI_EnableChn(MI_AUDIO_DEV AIDevId, MI_AI_CHN AIChn);
	MI_S32 s32Ret = 0;
	s32Ret = MI_AI_EnableChn(audioDev, audioChn);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AI_EnableChn(). s32Ret = 0x" << hex << s32Ret << endl;
	}
	cout << "Success to call MI_AI_EnableChn()." << endl;

	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：禁用AI 设备下的chanel
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioIn::disableChanel()
{
	// MI_S32 MI_AI_EnableChn(MI_AUDIO_DEV AIDevId, MI_AI_CHN AIChn);
	MI_S32 s32Ret = 0;
	s32Ret = MI_AI_DisableChn(audioDev, audioChn);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AI_DisableChn(). s32Ret = 0x" << hex << s32Ret << endl;
	}
	cout << "Success to call MI_AI_DisableChn()." << endl;

	return s32Ret;
}

/*
bEnableNr			是否启用婴儿哭声检测的降噪功能
eSensitivity		婴儿哭声检测的灵敏度
s32OperatingPoint	婴儿哭声检测的操作点。范围[-10, 10]，步长为1, 默认值为0
					注：提高操作点将会降低误报率，减小操作点将会降低漏测率
s32VadThresholdDb	婴儿哭声检测的Vad门槛值(dB), 范围[-80, 0]，步长为1, 默认值为-40
*/
/*-----------------------------------------------------------------------------
描--述：启用声音事件检测功能。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioIn::enableAed()
{
	MI_AI_AedConfig_t stAedConfig;
	memset(&stAedConfig, 0, sizeof(MI_AI_AedConfig_t));

	stAedConfig.bEnableNr = TRUE;
	stAedConfig.eSensitivity = E_MI_AUDIO_AED_SEN_HIGH;
	stAedConfig.s32OperatingPoint = -5;
	stAedConfig.s32VadThresholdDb = -40;
	stAedConfig.s32LsdThresholdDb = -15;
	
	// MI_S32 MI_AI_SetAedAttr(MI_AUDIO_DEV AiDevId, MI_AI_CHN AiChn, MI_AI_AedConfig_t *pstAedConfig);
	MI_S32 s32Ret = 0;
	s32Ret = MI_AI_SetAedAttr(audioDev, audioChn, &stAedConfig);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AI_SetAedAttr(). s32Ret = 0x" << hex << s32Ret << endl;
		return s32Ret;
	}

	// MI_S32 MI_AI_EnableAed(MI_AUDIO_DEV AiDevId, MI_AI_CHN AiChn);
	s32Ret = MI_AI_EnableAed(audioDev, audioChn);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AI_EnableAed(). s32Ret = 0x" << hex << s32Ret << endl;
		return s32Ret;
	}
	
	enAed = true;
	
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：禁用声音事件检测功能。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioIn::disableAed()
{
	enAed = false;

	// MI_S32 MI_AI_DisableAed(MI_AUDIO_DEV AiDevId, MI_AI_CHN AiChn);
	MI_S32 s32Ret = 0;
	s32Ret = MI_AI_DisableAed(audioDev, audioChn);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AI_EnableAed(). s32Ret = 0x" << hex << s32Ret << endl;
		return s32Ret;
	}
	
	return s32Ret;
}

/*-----------------------------------------------------------------------------
描--述：处理音频流的线程函数。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
void *audioIn::__route(void *arg)
{
	pthread_detach(pthread_self());

	audioIn *pThis = (audioIn *)arg;
	return pThis->route(NULL);
}

void *audioIn::route(void *arg)
{
	streamRouteRunning = true;
	while(streamRouteRunning)
	{
		MI_AUDIO_Frame_t stAiChFrame;
		memset(&stAiChFrame, 0, sizeof(MI_AUDIO_Frame_t));
		
		MI_AUDIO_AecFrame_t stAecFrame;
		memset(&stAecFrame, 0, sizeof(MI_AUDIO_AecFrame_t));

		// MI_S32 MI_AI_GetFrame(MI_AUDIO_DEV AiDevId, MI_AI_CHN AiChn, MI_AUDIO_Frame_t*pstFrm, MI_AUDIO_AecFrame_t *pstAecFrm , MI_S32 s32MilliSec);
		MI_S32 s32Ret = 0;
		s32Ret = MI_AI_GetFrame(audioDev, audioChn, &stAiChFrame, &stAecFrame, -1);
		if(0 != s32Ret)
		{
			cerr << "Fail to call MI_AI_GetFrame(). s32Ret = 0x" << hex << s32Ret << endl;
			continue;
		}

		// 获取AED声音事件检测结果。占用CPU 和FLASH 很高。
		// MI_S32 MI_AI_GetAedResult(MI_AUDIO_DEV AiDevId, MI_AI_CHN AiChn, MI_AI_AedResult_t *pstAedResult);
		MI_AI_AedResult_t stAedResult;
		memset(&stAedResult, 0, sizeof(stAedResult));
		
		s32Ret = MI_AI_GetAedResult(audioDev, audioChn, &stAedResult);
		if(0 != s32Ret)
		{
			cerr << "Fail to call MI_AI_GetAedResult(). s32Ret = 0x" << hex << s32Ret << endl;
		}
		//cout << "bAcousticEventDetected : bLoudSoundDetected = " << 
		//	(int)stAedResult.bAcousticEventDetected << " : " << (int)stAedResult.bLoudSoundDetected << endl;

		// 拷贝数据到用户层。
		memset(&stAudioInFrame, 0, sizeof(AudioInFrame_t));
		stAudioInFrame.eBitWidth = stAiChFrame.eBitwidth;
		stAudioInFrame.eSoundmode = stAiChFrame.eSoundmode;
		stAudioInFrame.u64TimeStamp = stAiChFrame.u64TimeStamp;
		stAudioInFrame.u32Len = stAiChFrame.u32SrcPcmLen;
		memcpy(stAudioInFrame.apFrameBuf, stAiChFrame.apSrcPcmVirAddr[0], stAiChFrame.u32SrcPcmLen);
		stAudioInFrame.bAcousticEventDetected = stAedResult.bAcousticEventDetected;
		stAudioInFrame.bLoudSoundDetected = stAedResult.bLoudSoundDetected;

		MI_AI_ReleaseFrame(audioDev, audioChn, &stAiChFrame, &stAecFrame);
		
		// 只有当缓冲区不忙碌时（说明用户调用rcvStream() 取走了数据），此时才执行sem_post()
		if(!bFrameBufBusy)
		{
			bFrameBufBusy = true;
			sem_post(&sem);
			// 跳转至 rcvStream() 查看处理音频流的代码。
		}
	}
	
	return NULL;
}

/*-----------------------------------------------------------------------------
描--述：建立stream流线程。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioIn::streamRouteCreate()
{	
	int ret = 0;
	int initVal = 0;
	bool bShared = false;
	
	ret = sem_init(&sem, bShared, initVal);
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
	
	return -1;
}

/*-----------------------------------------------------------------------------
描--述：结束stream流线程。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioIn::streamRouteDestroy()
{
	streamRouteRunning = false;

	if(-1 != streamRouteTid)
	{
		streamRouteTid = -1;
	}

	int ret = 0;
	ret = sem_destroy(&sem);
	if(0 != ret)
	{
		cerr << "Fail to call sem_destroy(3), ret = " << ret << ". " << strerror(errno) << endl;
	}
	cout << "Success to call sem_destroy(3)." << endl;

	return 0;
}

/*-----------------------------------------------------------------------------
描--述：接收音频数据到用户空间。
参--数：pAudioFrame 指向用户空间的音频数据指针。
返回值：
注--意：用户接收音频数据的唯一接口。
-----------------------------------------------------------------------------*/
int audioIn::rcvStream(AudioInFrame_t *pAudioFrame)
{
	if(NULL == pAudioFrame)
	{
		cerr << "Fail to call sendStream(), argument has NULL value!" << endl;
		return -1;
	}

	sem_wait(&sem);
	// 将audio 中的数据拷贝到用户空间。拷贝结束后切换缓冲区的标志位。
	memcpy(pAudioFrame, &stAudioInFrame, sizeof(AudioInFrame_t));
	bFrameBufBusy = false;

	return 0;
}

/*-----------------------------------------------------------------------------
描--述：结束stream流线程。
参--数：
返回值：
注--意：
-----------------------------------------------------------------------------*/
int audioIn::setVqeVolume(int val)
{	
	// MI_S32 MI_AI_SetVqeVolume(MI_AUDIO_DEV AiDevId, MI_AI_CHN AiChn, MI_S32 s32VolumeDb);
	MI_S32 s32Ret = 0;
	
	s32Ret = MI_AI_SetVqeVolume(audioDev, audioChn, val);
	if(0 != s32Ret)
	{
		cerr << "Fail to call MI_AI_SetVqeVolume(). s32Ret = 0x" << hex << s32Ret << endl;
	}

	return s32Ret;
}

