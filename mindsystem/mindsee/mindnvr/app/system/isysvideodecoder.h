#pragma once
#include <cstdint>
#include "global.hpp"
#include "priproto.hpp"

class CISysVideoDecoder
{
public:
    enum ECodeType : int {
        E_VIDEO_CODEC_TYPE_H264,
        E_VIDEO_CODEC_TYPE_H265
    };
    
private:
    /* data */
public:
    DISABLE_COPY(CISysVideoDecoder);
    DISABLE_EQUAL(CISysVideoDecoder);
    CISysVideoDecoder() {}
    virtual ~CISysVideoDecoder() {}

    virtual uint32_t GetChn() = 0;

    virtual int SendStream(std::shared_ptr<priprot::Frame_st> _aframe) = 0;
};