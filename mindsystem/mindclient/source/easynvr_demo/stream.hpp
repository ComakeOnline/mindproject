#pragma once

#include <string>
#include <functional>


#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
class DLLEXPORT Stream
{
public:
    enum CODEC_TYPE:uint8_t {
        TYPE_ACK,
        VIDEO_CODEC_H264,
        VIDEO_CODEC_H265,
        AUDIO_CODEC_AAC,
        TYPE_CONTROL = 0x5,
        TYPE_DEFAULT = 0xff
    };

    enum CHANNEL_TYPE:uint8_t {
        CHANNEL_CONTROL = 0x0,
        CHANNEL_AUDIO = 0x20,
        CHANNEL_VIDEO = 0x30,
        CHANNEL_DEFAULT = 0xff
    };
    struct sFrame_t
    {
        CHANNEL_TYPE channel_type;
        CODEC_TYPE codec_type;
        uint32_t timestamp;
        std::string data;
    };
private:
    void *p = nullptr;
    void *userdata = nullptr;
    std::function<void(sFrame_t&, void*)> m_frame_cb;
    std::function<void(std::string, void*)> m_ctrl_cb;

public:
    Stream(/* args */);
    ~Stream();
    //开始连接
    int Start(const std::string &priUrl);
    //停止
    int Stop();
    //连接状态
    bool Status();
    
    //开始对讲
    int PlayTalk();
    int StopTalk();
    //发送指令
    int SendCmd(const std::string& _cmd);

    //设置*userdata 
    void SetUserdata(void *_ptr){this->userdata = _ptr;};

    //设置处理音视频数据的回调函数
    void SetFrameCb(std::function<void(sFrame_t&, void*)> _cb);

    //添加音频帧aac数据，对讲时使用
    int AddAacFrame(sFrame_t _audio_frame);

    //消息处理回调函数
    void SetCtrlCb(std::function<void(std::string, void*)> _cb);
};
