#pragma once

#include "global.hpp"


class CISysAo
{
private:
    /* data */
public:
    enum EAudioBitWidth {
        E_AUDIO_BIT_WIDTH_16 = 16,
        E_AUDIO_BIT_WIDTH_24 = 24,
    };
    enum EAudioSoundMode {
        E_AUDIO_SOUND_MODE_MONO =0, /* mono */
        E_AUDIO_SOUND_MODE_STEREO =1, /* stereo */
        E_AUDIO_SOUND_MODE_QUEUE =2, /*all data in One chn */
    };
    struct AudioFrame_st
    {
        EAudioBitWidth bitwidth;
        EAudioSoundMode soundmode;
        char *data[2];
        uint64_t timestamp_us;
        uint32_t len;
    };
    
public:
    CISysAo(/* args */) {}
    ~CISysAo() {}

    //开启VO
    virtual void Refresh() = 0;

    //设置音量
    virtual void SetVolume(int _db) = 0;

    //送出音频pcm数据
    virtual int SendFrame(AudioFrame_st _frame) = 0;
};