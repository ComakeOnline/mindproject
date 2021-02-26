#pragma once

#include <unordered_map>
#include <queue>
#include <mutex>
#include <thread>

#include "sys.hpp"
#include "isysadec.h"
#include "aac2pcm.h"


class CSysAdec : public CSys , public CISysAdec
{
private:
    static std::unordered_map<uint32_t,bool> m_decoderPool;
    static std::mutex mtx;

    uint32_t m_myChn;
    AacToPcm m_aac;

    //decodec thread
    std::thread *m_pth = nullptr;
    bool m_running = false;
    std::queue<std::shared_ptr<priprot::Frame_st>> m_audio_que;
    std::mutex m_que_mtx;
public:
    CSysAdec(const ECodeType& _codec_type);
    ~CSysAdec();

    //uint32_t GetChn() override;

    int SendStream(std::shared_ptr<priprot::Frame_st> _aframe) override;
};

