#pragma once
#include <cstdint>
#include <memory>
#include "global.hpp"
#include "isysao.h"
#include "priproto.hpp"

class CISysAdec
{
public:
    enum ECodeType : int {
        E_AUDIO_CODEC_TYPE_PCM,
        E_AUDIO_CODEC_TYPE_AAC
    };
    
private:
    /* data */
public:
    DISABLE_COPY(CISysAdec);
    DISABLE_EQUAL(CISysAdec);
    CISysAdec() {}
    virtual ~CISysAdec() {}

    //virtual uint32_t GetChn() = 0;

    virtual int SendStream(std::shared_ptr<priprot::Frame_st> _aframe) = 0;
};