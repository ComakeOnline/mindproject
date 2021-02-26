#include "sysadec.h"

#include "config.hpp"
#include "mi_vdec.h"
#include "mi_common.h"
#include "sysadapter.h"
#include "sysao.h"


using namespace std;

std::unordered_map<uint32_t,bool> CSysAdec::m_decoderPool;
std::mutex CSysAdec::mtx;

CSysAdec::CSysAdec(const ECodeType& _codec_type)
{
    std::lock_guard<std::mutex> _Lock_1{mtx};
    m_myChn = 0xffffffff;
    auto max_adec_num = config::Config::GetInstance()->GetProduct("/audio/max_adec_number",1);
    for(int i=0;i< max_adec_num;i++)
    {
        cout<<"i="<<i<<endl;
        auto iter = m_decoderPool.find(i);
        if(iter == m_decoderPool.end() || iter->second == false)
        {
            m_myChn = i;
            m_decoderPool[i] = true;
            break;
        }
    }
    if(m_myChn == 0xffffffff)
    {
        LOGGER_ERROR("ADEC init failed!");
        abort();
    }
    m_running = true;
    m_pth = new thread{[this](){
        while(m_running)
        {
            m_que_mtx.lock();
            if(!m_audio_que.empty())
            {
                auto ptr = m_audio_que.front();
                m_audio_que.pop();
                m_que_mtx.unlock();
                m_aac.decode((uint8_t*)ptr->data.c_str(),ptr->data.size(),[](unsigned char* pcmData, int pcmLen){
                    CISysAo::AudioFrame_st frame;
                    frame.data[0] = (char*)pcmData;
                    frame.data[1] = nullptr;
                    frame.len = pcmLen;
                    auto ret = CSysAo::GetInstance()->SendFrame(frame);
                    if(ret != 0)
                    {
                        LOGGER_ERROR("AO - SendFrame error:%d",ret);
                    }
                });
            }
            else
            {
                m_que_mtx.unlock();
                this_thread::sleep_for(chrono::milliseconds(1));
            }
            
        }
    }};
}

CSysAdec::~CSysAdec()
{
    std::lock_guard<std::mutex> _Lock_1{mtx};
    m_decoderPool[m_myChn] = false;

    m_running = false;
    if(m_pth != nullptr)
    {
        m_pth->join();
        delete m_pth;
        m_pth = nullptr;
    }
    
}


int CSysAdec::SendStream(std::shared_ptr<priprot::Frame_st> _aframe)
{
    lock_guard<std::mutex> __lock231{m_que_mtx};
    if(m_audio_que.size() > 2)
    {
        LOGGER_WARNING("Adec que size > 2, stop recv audio frame");
        return -1;
    }
    m_audio_que.push(_aframe);
    return 0;
}
