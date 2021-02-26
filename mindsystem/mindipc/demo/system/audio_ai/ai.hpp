/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.10
----------------------------------------------------------------*/

#ifndef __AI_HPP__
#define __AI_HPP__

#include "mi_ai.h"
#include "pthread.h"
#include "semaphore.h"

class audioIn{
public:
	static const unsigned int uFrameBufSize = 1024 * 2;	// buf size: 1024 * N = Nkb
	
	typedef struct
	{
		MI_AUDIO_BitWidth_e eBitWidth;
		MI_AUDIO_SoundMode_e eSoundmode;
		char apFrameBuf[uFrameBufSize];
		MI_U64 u64TimeStamp;
		MI_U32 u32Len;
		MI_BOOL bAcousticEventDetected;
		MI_BOOL bLoudSoundDetected;
	}AudioInFrame_t;
	
	static audioIn *getInstance(){
		static audioIn audioIn;
		return &audioIn;
	};
	
	int setPubAttr(MI_AUDIO_BitWidth_e eBitWidth, MI_AUDIO_SampleRate_e eSample);
	int setChnOutputPortDepth();
	int enable();
	int disable();	
	int rcvStream(AudioInFrame_t *pAudioFrame);
	int enableAed();
	int disableAed();
	int setVqeVolume(int val);
	
private:
	sem_t sem;
	bool bFrameBufBusy;				// 音频帧缓冲区有数据
	bool enAed;
	MI_AUDIO_DEV audioDev;			// AI 设备号
	MI_AI_CHN audioChn;				// 使能设备下的chanel
	pthread_t streamRouteTid;		// 播放音频流线程的tid.
	bool streamRouteRunning;		// stream 线程的运行状态。
	AudioInFrame_t stAudioInFrame;	// 音频流的数据，包含音频+AED结果。

	// 单例模式需要将如下4个函数声明为private 的。
	audioIn();
	~audioIn();
	audioIn(const audioIn&);
	audioIn& operator=(const audioIn&);

	// 类中创建线程，必须使用一个空格函数传递this 指针。
	void *route(void *arg);
	static void *__route(void *arg);
	
	int enableDev();
	int disableDev();
	int enableChanel();
	int disableChanel();
	int streamRouteCreate();
	int streamRouteDestroy();
};

#endif
