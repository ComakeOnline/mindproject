#pragma once
#include <cstdint>
#include <memory>
#include <cstring>
#include <string>
#include <cstdio>
#include <queue>
#include <list>



namespace priprot
{

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

//frame 帧数据
struct Frame_st
{
    CHANNEL_TYPE channel_type;
    CODEC_TYPE codec_type;
    uint32_t timestamp;
    std::string data;
};


} // namespace priprot
