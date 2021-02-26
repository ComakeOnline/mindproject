/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.15
----------------------------------------------------------------*/

/*
	本文件为测试用例，仅用于验证硬件完好、音视频通讯正常，不能用于客户开发上层应用。
	客户在熟悉接口之后，请删除本文件，根据自身需求开发上层应用。
*/

#include "testing.h"
#include "iostream"
#include "string.h"

#include "venc.h"
#include "ai.hpp"
#include "ao.hpp"
#include "aac.h"
#include "aad.h"
#include "rgn.h"
#include "mindSdk.h"
#include "wifi.h"

using namespace std;

volatile bool g_bRunning = true;

/*-----------------------------------------------------------------------------
描--述：开启视频流。
参--数：chn 通道号，0 表示主码流，1 表示子码流。
返回值：无
注--意：非必要接口，设计原意是在此调用MI 接口初始化VENC. 实际情况是，软件运行一开始就已经对VENC 做了初始化。
	个人认为，"音视频传输协议"只需要考虑数据的稳定传输，不需要考虑开启或者停止MI 底层相关的东西。
	最好的方法是去掉这个接口，在命令控制的回调函数中给一个开启的消息即可，用户根据消息去做对应的行为。
-----------------------------------------------------------------------------*/
void startVideo(unsigned char chn)
{
	cout << "Call MindSdk::startVideo(), chn = " << (MI_U32)chn << endl;
	cout << "Call MindSdk::startVideo() end." << endl;
	return;
}

/*-----------------------------------------------------------------------------
描--述：停止视频流。
参--数：chn 通道号，0 表示主码流，1 表示子码流。
返回值：无
注--意：非必要接口，设计原意是在此调用MI 接口禁用VENC. 
	但个人认为，"音视频传输协议"只需要考虑数据的稳定传输，不需要考虑开启或者停止MI 底层相关的东西。
	最好的方法是：去掉这个接口，在命令控制的回调函数中给一个停止的消息即可，用户根据消息去做对应的行为。
-----------------------------------------------------------------------------*/
void stopVideo(unsigned char chn)
{
	cout << "Call MindSdk::stopVideo(), chn = " << (MI_U32)chn << endl;
	cout << "Call MindSdk::stopVideo() end." << endl;
	return;
}

/*-----------------------------------------------------------------------------
描--述：改变码流速率，MindSDK 所需的回调函数。
参--数：bitrate 比特率，单位是bits.
返回值：无
注--意：非必要接口，如果不想改变码流速率，可以不调用。详情咨询jerry.
-----------------------------------------------------------------------------*/
void changeBit(unsigned int bitrate, unsigned char) 	// bit 2000 * 1000, 1200 * 1000, 600 * 1000, 300 * 1000.
{
	cout << "Call MindSdk::changeBit(), bitrate = " << bitrate << endl;

	MI_S32 s32Ret = 0;
	Venc *pVenc = Venc::getInstance();
	s32Ret = pVenc->changeBitrate(0, bitrate);
	if(0 != s32Ret)
	{
		cerr << "Fail to call pVenc->changeBitrate(), errno = " << s32Ret << endl;
	}

	cout << "Call MindSdk::changeBit() end." << endl;	
	return;
}

#if (1 == (USE_MINDSDK_VIDEO))
/*-----------------------------------------------------------------------------
描--述：获取下一帧数据
参--数：chn 通道号，0 表示主码流，1 表示子码流。
返回值：无
注--意：必要接口！
	必须通过该接口获取MI 的数据，紧接着在该函数内部调用Mindsdk_Video::insertViFrame() 将数据喂给传输协议。
	异常用法：
	有用户存在这样的错误用法：没有设置cb_getNextViFrame() 这个回调函数，
	然后从VENC 中recStream()并立刻调用insertViFrame() 喂数据给音视频传输协议。
	这样做会导致网络环境较差时出现花屏现象，因此，务必设置此回调，在此回调中传输数据。
-----------------------------------------------------------------------------*/
void getNextViFrame(unsigned char chn)
{
	//cout << "Call MindSdk::getNextViFrame(), chn = " << (MI_U32)chn << endl;

	if(Venc::vencMainChn != (MI_U32)chn && Venc::vencSubChn != (MI_U32)chn)
	{
		cerr << "Fail to call getNextViFrame(), invalid venc chanel!" << endl;
		return;
	}
	
	int ret = 0;
	Venc::StreamPack_t streamPack;
	memset(&streamPack, 0, sizeof(Venc::StreamPack_t));
	Venc *pVenc = Venc::getInstance();
	ret = pVenc->rcvStream((MI_U32)chn, &streamPack);
	if(0 != ret)
	{
		cerr << "Fail to call pVenc->rcvStream(), ret = " << ret << endl;
		return;
	}

	#if 0	//debug
	cout << "streamPack.u32Len = " << streamPack.u32Len << endl;
	#endif

	bool bIframe = true;
	Mindsdk_Video *pMindsdk_video = Mindsdk_Video::getInstance();
	pMindsdk_video->insertViFrame((void *)streamPack.u8Pack, streamPack.u32Len, bIframe, streamPack.u64PTS, chn);
	
	//cout << "Call MindSdk::getNextViFrame() end." << endl;
	return;
}
#endif

/*-----------------------------------------------------------------------------
描--述：mindSDK 获取音频传输数据的回掉函数。
参--数：buf 音频帧缓冲区；len 音频帧长度。
返回值：无
注--意：1.音频数据为AAC 格式，需要通过faad 进行解码。
		2.该回调函数不“重入”，一次调用结束，才会进行二次调用。详情咨询jerry.
-----------------------------------------------------------------------------*/
void recvAudio(unsigned char *buf, unsigned short len)
{
	//cout << "Call recvAudio(), len = " << len << endl;

	void *pcmBuf = NULL;
	Aad *pAad = pAad->getInstance();
	NeAACDecFrameInfo decoderFrameInfo;
	memset(&decoderFrameInfo, 0, sizeof(NeAACDecFrameInfo));

	// step1: 解码一帧数据。
	pcmBuf = pAad->enDecoder(&decoderFrameInfo, buf, len);
	if(NULL == pcmBuf)
	{
		cerr << "Fail to call pAad->enDecoder(), return null value!" << endl;
		return;
	}
	else if(decoderFrameInfo.error > 0 || decoderFrameInfo.samples <= 0)
	{
		cerr << "Fail to call pAad->enDecoder(): " << endl;
		return;
	}

	#if 0	// debug
	printf("frame info: bytesconsumed %ld, channels %d, samples %ld, samplerate %ld\n", \
		decoderFrameInfo.bytesconsumed, decoderFrameInfo.channels, decoderFrameInfo.samples, decoderFrameInfo.samplerate);
	#endif

	// step2: 立体声转单声道，并用转换后的数据填充AO 结构体。
	audioOut::AudioOutFrame_t stAOframe;
	memset(&stAOframe, 0, sizeof(audioOut::AudioOutFrame_t));

	pAad->stereo2mono((unsigned char*)pcmBuf, decoderFrameInfo.samples * 2, stAOframe.audioBuf, NULL);
	stAOframe.stAudioFrame.u32Len = decoderFrameInfo.samples;

	// step3: 将AO 数据塞给AO 模块播放。
	audioOut::getInstance()->sendStream(&stAOframe);
	//usleep(1 * 1000);		// 若不阻塞，存在AO 拿数据不及时，踩内存的现象。2020.7.15 15:00
							// 2020.7.15 23:00 更新AO 模块，修复该问题。	
	return;
}

/*-----------------------------------------------------------------------------
描--述：AI 线程。不断地获取音频原始数据，编码后塞给mindSDK 传输。
参--数：无
返回值：无
注--意：AI 处理流程：获取PCM流 -> 编码为AAC流 -> mindSDK 传输。
		注意循环结束后，释放自由空间的内存。
-----------------------------------------------------------------------------*/
void *routeAi(void *arg)
{
	unsigned char *pcmBuf = NULL;
	unsigned char *aacBuf = NULL;
	Aac *pAac = Aac::getInstance();

	pcmBuf = (unsigned char*)malloc(pAac->inputSamples);
	if(NULL == pcmBuf)
	{
		cerr << "Fail to call malloc(3)." << endl;	
	}

	aacBuf = (unsigned char*)malloc(pAac->maxOutputBytes);
	if(NULL == pcmBuf)
	{
		cerr << "Fail to call malloc(3)." << endl;	
	}

	while(g_bRunning)
	{
		MI_S32 s32Ret = 0;
		audioIn::AudioInFrame_t stAudioFrame;
		memset(&stAudioFrame, 0, sizeof(stAudioFrame));
		
		s32Ret = audioIn::getInstance()->rcvStream(&stAudioFrame);
		if(0 != s32Ret)
		{
			cerr << "Fail to call pAudioIn->rcvStream(). s32Ret = " << s32Ret << endl;
			continue;
		}

		#if 0	// debug
		cout << "[AI]" << stAudioFrame.u32Len << endl;
		cout << "[AI bLoudSoundDetected]" << (int)stAudioFrame.bLoudSoundDetected << endl;
		#endif

		int ret = 0;
		Aac *pAac = Aac::getInstance();
		ret = pAac->enEncoder((int32_t *)stAudioFrame.apFrameBuf, stAudioFrame.u32Len / 2, aacBuf, pAac->maxOutputBytes);
		if(-1 == ret || 0 == ret)
		{
			cerr << "Fail to call pAac->enEncode(), ret = " << ret << endl;
			continue;
		}

		#if 0	//debug
		cout << "Success to call pAac->enEncoder(), ret = " << ret << endl;
		#endif
		
		#if (1 == (USE_MINDSDK_AUDIO))
		Mindsdk_Audio::getInstance()->sendAudio(aacBuf, ret);
		#endif
	}

	free(aacBuf);
	aacBuf = NULL;
	free(pcmBuf);
	pcmBuf = NULL;

	return NULL;
}

/*-----------------------------------------------------------------------------
描--述：AO 线程。从本地PCM 文件中读取数据，塞给AO 模块进行播放。
参--数：arg 本地文件的路径
返回值：无
注--意：
-----------------------------------------------------------------------------*/
void *routeAo(void *arg)
{
	if(NULL == arg)
    {   
        cerr << "Fail to call readPcmRoute(), argument has null value!" << endl;
        return NULL;
    }

    const char *filePath = (char *)arg;    
    int fd = -1; 
    fd = open(filePath, O_RDONLY);
    if(-1 == fd) 
    {   
        cerr << "Fail to call open(2), " << strerror(errno) << endl;
        return NULL;
    }   

    do{ 
        int ret = 0;
		audioOut::AudioOutFrame_t stAOframe;
		memset(&stAOframe, 0, sizeof(audioOut::AudioOutFrame_t));

        ret = read(fd, stAOframe.audioBuf, audioOut::audioBufLen);
        if(-1 == ret)
        {   
            cerr << "Fail to call read(2), " << strerror(errno) << endl;
            break;
        }   
        else if(0 == ret)
        {   
            cout << "Read file over!" << endl;
            break;
        }
        
        stAOframe.stAudioFrame.u32Len = ret;
        audioOut::getInstance()->sendStream(&stAOframe);
        usleep(62 * 1000);  // 注意休眠时间，等待上一帧PCM 播放结束，再塞下一帧。
    }while(g_bRunning);

	if(-1 != fd)
	{
	    close(fd);
    	fd = -1;            
    }

	return NULL;
}

/*-----------------------------------------------------------------------------
描--述：VIDEO 线程：从VENC 中获取若干帧数据保存到本地。
参--数：arg, VENC 通道号。
返回值：
注--意：数据保存的路径为/customer/video.h265
-----------------------------------------------------------------------------*/
void *routeVideo(void *arg)
{
	if(NULL == arg)
	{
		cerr << "Fail to call routeVideo(), argument has null value!" << endl;
        return NULL;
	}

	MI_U32 u32VencChn = (MI_U32)arg;
	if(Venc::vencMainChn != u32VencChn && Venc::vencSubChn != u32VencChn && Venc::vencJpegChn != u32VencChn)
	{
		cerr << "Fail to call routeVideo(), invalid chanel!" << endl;
        return NULL;
	}

	int fd = 0;
	const char *filePath = "/customer/video.test";
	fd = open(filePath, O_WRONLY | O_CREAT | O_TRUNC);		// 只写 | 如果不存在则创建 | 如果存在则清空
	if(-1 == fd)
	{
		cerr << "Fail to open " << filePath << ", " << strerror(errno) << endl;
		return NULL;
	}

	unsigned int uFrameCnt = 10;				// 写入文件的帧数。1 秒大约30帧，视VENC 的FPS 而定。
	MI_VENC_CHN vencChn = Venc::vencMainChn;	// 将主码流/子码流写入文件。

	while(g_bRunning && uFrameCnt--)
	{		
		MI_S32 s32Ret = 0;
		Venc::StreamPack_t stStreamPack;
		Venc *pVenc = Venc::getInstance();
		memset(&stStreamPack, 0, sizeof(Venc::StreamPack_t));
		s32Ret = pVenc->rcvStream(vencChn, &stStreamPack);
		if(0 != s32Ret)
		{
			cerr << "Fail to call pVenc->rcvStream(). s32Ret = " << s32Ret << endl;
			continue;
		}
		
		#if 1	//debug
		{
			cout << "stStreamPack.u32Len = " << stStreamPack.u32Len <<endl;
			cout << "stStreamPack.u64PTS = " << stStreamPack.u64PTS << endl;
			cout << endl;
		}
		#endif
		
		int ret = 0;
		ret = write(fd, stStreamPack.u8Pack, stStreamPack.u32Len);
		if(-1 == ret)
		{
			cerr << "Fail to call write(2), " << strerror(errno) << endl;
			continue;
		}
	}

	if(-1 != fd)
	{
		close(fd);
		fd = -1;
	}

	return NULL;
}

/*-----------------------------------------------------------------------------
描--述：OSD 线程：每秒更新网络状况信息
参--数：无
返回值：
注--意：
-----------------------------------------------------------------------------*/
#if (1 == (USE_MINDSDK_VIDEO))
extern std::atomic<int> wifiSignal;		// wifi信号强度
extern std::string preWifiMac;			// 上级wifi的mac地址
extern std::string localMac;			// 本地mac地址
extern std::atomic<int> currentBitRate;	// 当前码率
#else
int currentBitRate = 0;	// 当前码率
#endif
void *routeOsd(void *arg)
{
	while(g_bRunning)
	{
		const MI_U32 u32StrSize = 128;
		char str[u32StrSize] = {0};
		Rgn *pRgn = Rgn::getInstance();
		#if (1 == (USE_MINDSDK_VIDEO))
		Udper *pUdper = Udper::getInstance();
		#endif

		// 第一行，显示音视频码率
		MI_S32 s32AudioKbps = 0;
		MI_S32 s32VideoKbps = 0;
		#if (1 == (USE_MINDSDK_VIDEO))
		s32AudioKbps = Udper::getInstance()->audioBitRate * 8;
		s32VideoKbps = Udper::getInstance()->videoBitRate * 8;
		#endif
		snprintf(str, u32StrSize, "Audio:%2dkbps Video:%4dkbps", s32AudioKbps, s32VideoKbps);
		pRgn->setText(Rgn::rgnHandle0, str, I4_RED, DMF_Font_Size_32x32);

		// 第二行，显示wifi 信号强度
		MI_S32 s32WifiSignal = Wifi::getInstance()->getApRssi();
		snprintf(str, u32StrSize, "Wifi: %2d dB", s32WifiSignal);
		pRgn->setText(Rgn::rgnHandle1, str, I4_RED, DMF_Font_Size_32x32);

		// 第三/四行，显示本地和上级设备MAC
		const unsigned macBufSize = 32;
		char macBuf[macBufSize] = {0};
		Wifi::getInstance()->getLocalMac(macBuf, macBufSize, "wlan0");
		snprintf(str, u32StrSize, "Local MAC: %s", macBuf);
		pRgn->setText(Rgn::rgnHandle2, str, I4_RED, DMF_Font_Size_32x32);
		
		Wifi::getInstance()->getPreMac(macBuf, macBufSize);
		snprintf(str, u32StrSize, "AP MAC: %s", macBuf);
		pRgn->setText(Rgn::rgnHandle3, str, I4_RED, DMF_Font_Size_32x32);

		// 第五行，显示视频码率
		double dBitRate = (double)currentBitRate / (1000 * 1000);
		snprintf(str, u32StrSize, "Video bitrate: %3.2fMbps", dBitRate);
		pRgn->setText(Rgn::rgnHandle4, str, I4_RED, DMF_Font_Size_32x32);

		// 第六/七行，预留
		pRgn->setText(Rgn::rgnHandle5, "line 6: green, green, green", I4_GREEN, DMF_Font_Size_32x32);
		pRgn->setText(Rgn::rgnHandle5, "line 7: black, black, black", I4_BLACK, DMF_Font_Size_32x32);
	
		sleep(1);
	}

	return NULL;
}


