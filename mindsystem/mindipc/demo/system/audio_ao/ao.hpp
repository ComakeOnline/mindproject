/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.10
----------------------------------------------------------------*/

#ifndef __AO_HPP__
#define __AO_HPP__

#include "mi_ao.h"
#include "pthread.h"
#include "semaphore.h"

class audioOut{
public:
	static const unsigned audioBufLen = 4 * 1024;	// N * 1024 = N kb
	typedef struct
	{
		MI_AUDIO_Frame_t stAudioFrame;
		unsigned char audioBuf[audioBufLen];
	}AudioOutFrame_t;

	AudioOutFrame_t stAudioOutFrame;

	int setPubAttr(MI_AUDIO_BitWidth_e eBitWidth, MI_AUDIO_SampleRate_e eSample);
	int enable();
	int disable();	
	int sendStream(AudioOutFrame_t *pstAudioOutFrame);
	int setVolume(int val);
	
	static audioOut *getInstance(){
		static audioOut audioOut;
		return &audioOut;
	};
	
private:
	sem_t sem0;
	sem_t sem1;
	MI_AUDIO_DEV audioDev;			// AO 设备号
	MI_AO_CHN audioChn;				// 使能设备下的chanel
	pthread_t streamRouteTid;		// 播放音频流线程的tid.
	bool streamRouteRunning;		// stream 线程的运行状态。
	// MI_AUDIO_Frame_t *pFrameData;	// 指向音频流的指针，会被MI_AO_SendFrame() 用到

	// 单例模式需要将如下4个函数声明为private 的。
	audioOut();
	~audioOut();
	audioOut(const audioOut&);
	audioOut& operator=(const audioOut&);

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
