#pragma once

#include <thread>
#include <map>
#include <mutex>

#include "sys.hpp"
#include "isysai.h"
#include "mi_common_datatype.h"

class CSysAi : public CSys , public CISysAi
{
private:
    bool m_running = false;
    std::thread *m_pth = nullptr;
    std::map<int,std::function<void(CISysAo::AudioFrame_st)>> m_map_cb;
    std::mutex m_cb_mtx;


    MI_AUDIO_DEV m_AiDevId = 0; // default set device 0
	MI_S32 m_AiChn = 0;
private:
    CSysAi(/* args */);
    ~CSysAi();
public:
    DISABLE_COPY(CSysAi);
    DISABLE_EQUAL(CSysAi);

    static CSysAi* GetInstance(){
        static CSysAi ins;
        return &ins;
    }
    void Init();
    void DeInit();

    void Refresh() override;

    //设置音量
    //void SetVolume(int _db) override;

    int Register(int _aechn,const std::function<void(CISysAo::AudioFrame_st)> &_cb) override;
    int CancelRegister(int _id) override;
};

