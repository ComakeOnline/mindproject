#pragma once

#include <unordered_map>
#include <mutex>

#include "sys.hpp"
#include "isysvideodecoder.h"


class CSysVideoDecoder : public CSys , public CISysVideoDecoder
{
private:
    static std::unordered_map<uint32_t,bool> m_decoderPool;
    static std::mutex mtx;

    uint32_t m_myChn;

    //decodec thread
    std::thread *m_pth = nullptr;
    bool m_running = false;
    std::queue<std::shared_ptr<priprot::Frame_st>> m_video_que;
    std::mutex m_que_mtx;
public:
    CSysVideoDecoder(const ECodeType& _codec_type);
    ~CSysVideoDecoder();

    uint32_t GetChn() override;

    int SendStream(std::shared_ptr<priprot::Frame_st> _aframe) override;
};

