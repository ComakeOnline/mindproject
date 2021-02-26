#include "sysvideodecoder.h"

#include "config.hpp"
#include "mi_vdec.h"
#include "mi_common.h"
#include "sysadapter.h"

using namespace std;

std::unordered_map<uint32_t,bool> CSysVideoDecoder::m_decoderPool;
std::mutex CSysVideoDecoder::mtx;

CSysVideoDecoder::CSysVideoDecoder(const ECodeType& _codec_type)
{
    std::lock_guard<std::mutex> _Lock_1{mtx};
    m_myChn = 0xffffffff;
    for(int i=0;i< config::Config::GetInstance()->m_productCfg.MAX_VIDEO_DCODER_NUMBER;i++)
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
        LOGGER_ERROR("VENC OUT OF LIMIT");
        runtime_error{"VENC OUT OF LIMIT"};
    }

    MI_VDEC_ChnAttr_t stVdecChnAttr;
    // create VDEC
    memset(&stVdecChnAttr, 0, sizeof(MI_VDEC_ChnAttr_t));
    stVdecChnAttr.stVdecVideoAttr.u32RefFrameNum = 1;
    stVdecChnAttr.eVideoMode    = E_MI_VDEC_VIDEO_MODE_FRAME;
    stVdecChnAttr.u32BufSize    = 1 * 1024 * 1024;
    stVdecChnAttr.u32PicWidth   = config::Config::GetInstance()->m_productCfg.MAX_INPUT_VIDEO_WIDTH;
    stVdecChnAttr.u32PicHeight  = config::Config::GetInstance()->m_productCfg.MAX_INPUT_VIDEO_HEIGHT;
    stVdecChnAttr.u32Priority   = 0;
    switch (_codec_type)
    {
    case E_VIDEO_CODEC_TYPE_H264:
        stVdecChnAttr.eCodecType    = E_MI_VDEC_CODEC_TYPE_H264;
        break;
    case E_VIDEO_CODEC_TYPE_H265:
        stVdecChnAttr.eCodecType    = E_MI_VDEC_CODEC_TYPE_H265;
        break;
    
    default:
        stVdecChnAttr.eCodecType    = E_MI_VDEC_CODEC_TYPE_H264;
        break;
    }
    CHECKRESULT(MI_VDEC_CreateChn(m_myChn, &stVdecChnAttr), MI_SUCCESS);
    CHECKRESULT(MI_VDEC_StartChn(m_myChn), MI_SUCCESS);

    MI_VDEC_OutputPortAttr_t stOutputPortAttr;
    stOutputPortAttr.u16Width = 1024;
    stOutputPortAttr.u16Height = 600;
    MI_VDEC_SetOutputPortAttr(m_myChn, &stOutputPortAttr);

    MI_SYS_ChnPort_t stChnPort;
    memset(&stChnPort, 0x0, sizeof(MI_SYS_ChnPort_t));
    stChnPort.eModId = E_MI_MODULE_ID_VDEC;
    stChnPort.u32DevId = 0;
    stChnPort.u32ChnId = m_myChn;
    stChnPort.u32PortId = 0;

    CHECKRESULT(MI_SYS_SetChnOutputPortDepth(&stChnPort, 2, 5), MI_SUCCESS);

    m_running = true;
    m_pth = new thread{[this](){
        while(m_running)
        {
            m_que_mtx.lock();
            if(!m_video_que.empty())
            {
                auto ptr = m_video_que.front();
                m_video_que.pop();
                m_que_mtx.unlock();
                MI_S32 s32TimeOutMs = 20;
                MI_S32 s32Ret = MI_SUCCESS;

                MI_VDEC_VideoStream_t stVdecStream;
                stVdecStream.pu8Addr = (uint8_t*)ptr->data.c_str();
                stVdecStream.u32Len = ptr->data.size();
                stVdecStream.u64PTS = ptr->timestamp;
                stVdecStream.bEndOfFrame = 1;
                stVdecStream.bEndOfStream = 0;
                if (MI_SUCCESS != (s32Ret = MI_VDEC_SendStream(m_myChn, &stVdecStream, s32TimeOutMs)))
                {
                    LOGGER_ERROR("MI_VDEC_SendStream fail, chn:%d, 0x%X\n", m_myChn, s32Ret);
                    //return -1;
                }
            }
            else
            {
                m_que_mtx.unlock();
                this_thread::sleep_for(chrono::milliseconds(15));
            }
            
        }
    }};
}

CSysVideoDecoder::~CSysVideoDecoder()
{
    std::lock_guard<std::mutex> _Lock_1{mtx};
    auto pDisp = CSysAdapter::GetSysDisplay();
    pDisp->CancelInput(m_myChn,-1);
    CHECKRESULT(MI_VDEC_StopChn(m_myChn), MI_SUCCESS);
    CHECKRESULT(MI_VDEC_DestroyChn(m_myChn), MI_SUCCESS);

    m_decoderPool[m_myChn] = false;

    m_running = false;
    if(m_pth != nullptr)
    {
        m_pth->join();
        delete m_pth;
        m_pth = nullptr;
    }
    
}

uint32_t CSysVideoDecoder::GetChn()
{
    return m_myChn;
}

int CSysVideoDecoder::SendStream(std::shared_ptr<priprot::Frame_st> _aframe)
{
    lock_guard<std::mutex> __lock231{m_que_mtx};
    if(m_video_que.size() > 10)
    {
        LOGGER_WARNING("Vdec que size > 10, stop recv audio frame");
        return -1;
    }
    m_video_que.push(_aframe);
    return 0;
}
