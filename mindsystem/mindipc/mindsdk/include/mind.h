#pragma once
#include "mind_package.h"
#include "public.h"

#ifndef DE_MindSdk_NoResend_VERSION//版本信息
#define DE_MindSdk_NoResend_VERSION "2020-06-16-12-18"
#endif // !DE_MindSdk_NoResend_VERSION

typedef void (*Function_Cb)(char* cmd, int len);

class Mind_Sdk
{
    Mind_Sdk();
    static void recv_cb(void* data, int len);
    static bool sendingVideo;
    static bool sendingAudio;
public:
    static Mind_Sdk* getInstance();
    static void setServerAddr(const char* ip, int port);//设置NVR端的ip和端口
    static void checkIn(const char* deviceId);//向nvr报告ipc设备的存在，deviceId为ipc唯一标识码，可以为网卡mac地址，芯片uuid等
    static void sendViFrame(void* data, int dataLen, unsigned short stream_num, unsigned char sliceID, DATA_TYPE dataType);//调用此函数插入vi数据
    static void sendAudioFrame(void* data, int dataLen, unsigned short stream_num);
    static void sendCmd(const char* cmd);//发送控制命令，命令必须为字符串
    static Function_Cb cmd_cb;//处理用户自定义命令的回调函数，用户要实现此回调函数
    static Function_Cb getAudio_cb;//处理接收到的音频数据的回调函数
};