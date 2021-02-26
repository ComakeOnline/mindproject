/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.10
----------------------------------------------------------------*/

#include "iostream"

#include "sys.h"
#include "sensor.hpp"
#include "vif.h"
#include "vpe.h"
#include "venc.h"
#include "ai.hpp"
#include "ao.hpp"
#include "aac.h"
#include "aad.h"
#include "rgn.h"
#include "wifi.h"
#include "mindSdk.h"
#include "testing.h"
#include "ircut.h"

using namespace std;

void sigHandler(int sig);

int main(int argc, const char *argv[])
{
	/*
		统一采用C++单例设计模式，getInstance() 为各模块实例的统一入口函数。
		单例模式不存在重复初始化的问题，在调用之初执行构造，后续其它地方的调用时间开销小。
		风格尽量趋近于C, 避免C++11 及之后的高级用法。
		库函数尽可能使用linux 标准库函数，高效，可调试性高。暂不考虑linux->otherOS 的移植问题。
	*/

	signal(SIGINT, sigHandler);
	
	IrCut::getInstance()->resetFilter();	// ircut滤波片复位

	// 系统初始化
	Sys *pSys = Sys::getInstance();
	// 出图模块初始化。数据流向：sensor -> vif -> vpe -> venc -> 应用处理。
	Sensor *pSensor = Sensor::getInstance();// sensor 初始化
	pSensor->setFps(25);
	Vif *pVif = Vif::getInstance();			// VIF 初始化
	Vpe *pVpe = Vpe::getInstance();			// VPE 初始化
	pVpe->createMainPort(Vpe::vpeMainPort);	// 创建VPE 主码流
	pVpe->createSubPort(Vpe::vpeSubPort);	// 创建VPE 子码流
	//pVpe->createJpegPort(Vpe::vpeJpegPort);	// 创建VPE JPEG码流
	
	Venc *pVenc = Venc::getInstance();		// VENC 初始化
	pVenc->createMainStream(Venc::vencMainChn, NULL);	// 创建VENC主码流
	pVenc->createSubStream(Venc::vencSubChn, NULL);		// 创建VENC子码流
	//pVenc->createJpegStream(Venc::vencJpegChn, NULL);	// 创建VENC-JPEG码流
	
	// 绑定VIF -> VPE. 只需要绑定一次，用REALTIME
	pSys->bindVif2Vpe(Vif::vifPort, Vpe::vpeInputPort, 30, 30, E_MI_SYS_BIND_TYPE_REALTIME, 0);
	// 绑定VPE -> VENC, 如果有多路码流，则需要绑定多次。
	pSys->bindVpe2Venc(Vpe::vpeMainPort, Venc::vencMainChn, 30, 30, E_MI_SYS_BIND_TYPE_REALTIME, 0);
	pSys->bindVpe2Venc(Vpe::vpeSubPort, Venc::vencSubChn, 30, 30, E_MI_SYS_BIND_TYPE_REALTIME, 0);
	//pSys->bindVpe2Venc(Vpe::vpeJpegPort, Venc::vencJpegChn, 30, 30, E_MI_SYS_BIND_TYPE_FRAME_BASE, 0);

	// 初始化OSD
	#if (1 == (USE_OSD))
	Rgn *pRgn = Rgn::getInstance();
	#endif
	
	// AI 和AO 属于两个模块，分别初始化。
	#if (1 == (USE_AI))
	audioIn *pAudioIn = audioIn::getInstance();
	#endif
	
	#if (1 == (USE_AO))
	audioOut *pAudioOut = audioOut::getInstance();
	#endif
	
	#if (1 == (USE_FAAC_FAAD))
	// 音频编/解码初始化。aac = audio coder; aad = audio decoder.
	Aac *pAac = Aac::getInstance();
	Aad *pAad = Aad::getInstance();
	#endif

	#if (1 == (USE_WIFILINK))
	Wifi *pWifi = Wifi::getInstance();
	pWifi->enable();
	#endif

	/*
		至此，SENSOR, VIF, VPE, VENC, AI, AO, OSD 均已初始化完成。
		以下代码的功能为通过音视频传输协议将数据传输给另一台设备。
	*/
	
	#if (1 == (USE_MINDSDK_AUDIO))
	// MindSDK 传输协议。不开源，文档由jerry 提供。
	Mindsdk_Audio& audio = *Mindsdk_Audio::getInstance();
	audio.cb_recvAudio = recvAudio;
	audio.start();
	#endif

	#if (1 == (USE_MINDSDK_VIDEO))
	// MindSDK 传输协议。不开源，文档由jerry 提供。
	Mindsdk_Video &Video = *Mindsdk_Video::getInstance();
	Video.cb_startVideo = startVideo;
	Video.cb_stopVideo = stopVideo;
	Video.cb_getNextViFrame = getNextViFrame;
	Video.cb_changeBit = changeBit;
	Video.start();
	#endif

	// 创建3个线程，分别用于测试AI, AO, 出图。
	int ret = 0;
	#if (1 == (USE_AI))
	pthread_t tidAi = -1;
	ret = pthread_create(&tidAi, NULL, routeAi, NULL);
	if(0 != ret)
	{
		cerr << "Fail to call pthread_create(3) for AI, " << strerror(errno) << endl;
	}
	#endif

	#if (1 == (USE_AO))
	pthread_t tidAo = -1;
	// AO. 参数为本地音频文件的路径。写死的，16位宽 16000采样率
	ret = pthread_create(&tidAo, NULL, routeAo, (void *)"pcm_16000_16bit.pcm");
	if(0 != ret)
	{
		cerr << "Fail to call pthread_create(3) for AO, " << strerror(errno) << endl;
	}
	#endif

	#if(1 == (USE_OSD))
	// OSD 功能
	pthread_t tidOsd = -1;
	ret = pthread_create(&tidOsd, NULL, routeOsd, NULL);
	if(0 != ret)
	{
		cerr << "Fail to call pthread_create(3) for OSD, " << strerror(errno) << endl;
	}
	#endif

	#if (1 == (USE_VENC))
	pthread_t tidVideo = -1;
	cout << "routeVideo" << endl;
	cout << "routeVideo" << endl;
	// 测试出图的线程，将VENC 的数据保存到本地。参数为VENC 的通道号，支持主码流和子码流。
	ret = pthread_create(&tidVideo, NULL, routeVideo, (void *)Venc::vencJpegChn);
	if(0 != ret)
	{
		cerr << "Fail to call pthread_create(3) for VENC, " << strerror(errno) << endl;
	}
	cout << "routeVideo" << endl;
	cout << "routeVideo" << endl;
	#endif

	sleep(2);

	g_bRunning = true;		// sigHandler() 对其取反。
	while(g_bRunning)
	{
		sleep(1);
	}

	#if (1 == (USE_AI))
	pthread_join(tidAi, NULL);
	#endif
	
	#if (1 == (USE_AO))
	pthread_join(tidAo, NULL);
	#endif
	
	#if (1 == (USE_VENC))
	pthread_join(tidVideo, NULL);
	#endif

	#if (1 == (USE_OSD))
	pthread_join(tidOsd, NULL);
	#endif

	cout << "Success to exit programe! Bye bye ~" << endl;
	return 0;
}

void sigHandler(int sig)
{
    switch (sig)
    {
    case SIGINT:
    	cerr << "Receive SIGINT!!!" << endl;
	    g_bRunning = false;
	    exit(0);
        break;
    default:
        g_bRunning = false;
        break;
    }
}

