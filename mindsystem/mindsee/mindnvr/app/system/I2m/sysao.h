#pragma once

#include <thread>

#include "sys.hpp"
#include "isysao.h"
#include "mi_common_datatype.h"

class CSysAo : public CSys , public CISysAo
{
private:
    bool m_running = false;
    MI_AUDIO_DEV m_AoDevId = 0; // default set device 0
	MI_S32 m_AoChn = 0;
private:
    CSysAo(/* args */);
    ~CSysAo();
public:
    DISABLE_COPY(CSysAo);
    DISABLE_EQUAL(CSysAo);

    static CSysAo* GetInstance(){
        static CSysAo ins;
        return &ins;
    }
    void Init();
    void DeInit();

    void Refresh() override;

    //设置音量
    void SetVolume(int _db) override;

    //送出音频pcm数据
    int SendFrame(AudioFrame_st _frame) override;
};

