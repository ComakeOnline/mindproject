#pragma once


#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <ctime>

#ifndef COMPILE_STREAM_LIB
#include "sysadapter.h"
#else
class CISysVideoDecoder;
#endif


class ISession
{
private:
    // std::function<void(std::shared_ptr<CISysVideoDecoder>)> m_cbAfterConnect = [](std::shared_ptr<CISysVideoDecoder> videoDecoder){};
    // std::shared_ptr<CISysVideoDecoder> m_videoDecoder;
public:
    ISession(/* args */){};
    ~ISession(){};
    //目前URL为ip地址 端口9102
    virtual int Start(const std::string &Url) = 0;
    virtual int Stop() = 0;

    virtual bool Status() = 0;

    virtual void SetCbAfterConnect(std::function<void(std::shared_ptr<CISysVideoDecoder>)> _cb) = 0;
    // {
    //     m_cbAfterConnect.swap(_cb);
    //     if(m_videoDecoder != nullptr)
    //     {
    //         LOGGER_DEBUG("DO %d",m_videoDecoder->GetChn());
    //         m_cbAfterConnect(m_videoDecoder);
    //     }
    // }

    virtual int PlayAudio() = 0;
    virtual int StopAudio() = 0;
    virtual int PlayTalk() = 0;
    virtual int StopTalk() = 0;
    virtual void OpenNetInformation() = 0;
    virtual void CloseNetInformation() = 0;
    //
    virtual int SendCmd(const std::string& _cmd) = 0;
};