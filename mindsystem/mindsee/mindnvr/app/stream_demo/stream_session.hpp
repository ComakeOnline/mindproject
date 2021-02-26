#pragma once

#include <iostream>
#include <string>
#include <cstring>

#include "stream.hpp"
#include "isession.hpp"
#include "sysadapter.h"
#include "pcm2aac.h"

class CStreamSession : public ISession
{
private:
    Stream m_stream;
public:
    std::function<void(std::shared_ptr<CISysVideoDecoder>)> m_cbAfterConnect = [](std::shared_ptr<CISysVideoDecoder> videoDecoder){};
    std::shared_ptr<CISysVideoDecoder> m_videoDecoder;
    std::shared_ptr<CISysAdec> m_adec;
    PcmToAac *m_pPcmToAac = nullptr;
    int m_talkIndex = -1;
public:
    CStreamSession(/* args */){
        m_stream.SetCtrlCb([](std::string _cmd, void* userdata) {return; });
        m_stream.SetFrameCb([this](Stream::sFrame_t& frame, void* userdata) {
            if (m_videoDecoder == nullptr)
            {
                if(frame.codec_type == priprot::CODEC_TYPE::VIDEO_CODEC_H264)
                {
                    auto ptr = CSysAdapter::GetVideoDecoder(CISysVideoDecoder::E_VIDEO_CODEC_TYPE_H264);
                    m_videoDecoder.swap(ptr);
                    m_cbAfterConnect(m_videoDecoder);
                }
                if(frame.codec_type == priprot::CODEC_TYPE::VIDEO_CODEC_H265)
                {
                    auto ptr = CSysAdapter::GetVideoDecoder(CISysVideoDecoder::E_VIDEO_CODEC_TYPE_H265);
                    m_videoDecoder.swap(ptr);
                    m_cbAfterConnect(m_videoDecoder);
                }
            }
            if (m_adec != nullptr)
            {
                if(frame.codec_type == priprot::CODEC_TYPE::AUDIO_CODEC_AAC)
                {
                    auto ptr = std::make_shared<priprot::Frame_st>();
                    ptr->data = frame.data;
                    m_adec->SendStream(ptr);
                }
            }
            if ( frame.codec_type == priprot::CODEC_TYPE::VIDEO_CODEC_H264 || frame.codec_type == priprot::CODEC_TYPE::VIDEO_CODEC_H265) 
            {
                auto ptr = std::make_shared<priprot::Frame_st>();
                ptr->data = frame.data;
                m_videoDecoder->SendStream(ptr);
            }
        });
    }
    ~CStreamSession(){}
    //目前URL为ip地址 端口9102
    int Start(const std::string &priUrl) override{
        return m_stream.Start(priUrl);
    }
    int Stop() override{
        return m_stream.Stop();
    }

    bool Status() override{
        return m_stream.Status();
    }

    void SetCbAfterConnect(std::function<void(std::shared_ptr<CISysVideoDecoder>)> _cb) override
    {
        m_cbAfterConnect.swap(_cb);
        if(m_videoDecoder != nullptr)
        {
            LOGGER_DEBUG("DO %d",m_videoDecoder->GetChn());
            m_cbAfterConnect(m_videoDecoder);
        }
    }

    int PlayAudio() override{
        if(m_adec == nullptr)
        {
            m_adec = CSysAdapter::GetAdec(CISysAdec::ECodeType::E_AUDIO_CODEC_TYPE_AAC);
            return 0;
        }
        return -1;
    }
    int StopAudio() override{
        m_adec = nullptr;
        return 0;
    }
    int PlayTalk() override{
        m_stream.PlayTalk();
        m_pPcmToAac = new PcmToAac{};
        auto pAi = CSysAdapter::GetAi();
        m_talkIndex = pAi->Register(0,[this](CISysAo::AudioFrame_st _aframe){
            auto ts = _aframe.timestamp_us;
            m_pPcmToAac->encode((unsigned char*)(_aframe.data[0]),_aframe.len,[this,ts](unsigned char *buf, int len){
                Stream::sFrame_t frame;
                frame.channel_type = Stream::CHANNEL_AUDIO;
                frame.codec_type = Stream::AUDIO_CODEC_AAC;
                frame.timestamp = ts;
                frame.data = std::string{(char*)buf,len};
                m_stream.AddAacFrame(frame);
            });
        });
        if(m_talkIndex == -1){return -1;}
        return 0;
    }
    int StopTalk() override{
        m_stream.StopTalk();
        auto pAi = CSysAdapter::GetAi();
        if(m_talkIndex != -1)
        {
            pAi->CancelRegister(m_talkIndex);
        }
        m_talkIndex = -1;
        delete m_pPcmToAac;
    }

    void OpenNetInformation() override {}
    void CloseNetInformation() override {}

    int SendCmd(const std::string& _cmd) override
    {
        return m_stream.SendCmd(_cmd);
    }
};
