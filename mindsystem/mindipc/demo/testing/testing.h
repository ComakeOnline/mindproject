/*---------------------------------------------------------------- 
sigma star版权所有。
作者：lison.guo
时间：2020.7.10
----------------------------------------------------------------*/

#ifndef __TESTING_H__
#define __TESTING_H__

#define USE_AI 1
#define USE_AO 1
#define USE_OSD 0
#define USE_VENC 0
#define USE_WIFILINK 0
#define USE_FAAC_FAAD 1
#define USE_MINDSDK_AUDIO 1
#define USE_MINDSDK_VIDEO 1

extern volatile bool g_bRunning;

void *routeAi(void *arg);
void *routeAo(void *arg);
void *routeVideo(void *arg);
void *routeOsd(void *arg);
void recvAudio(unsigned char* buf, unsigned short len);

void startVideo(unsigned char chn);
void stopVideo(unsigned char chn);
void getNextViFrame(unsigned char chn);
void changeBit(unsigned int bitrate, unsigned char);

#endif
