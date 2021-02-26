#include "sysai.h"

#include <thread>


#include "mi_common_datatype.h"
#include "mi_ai.h"

using namespace std;


CSysAi::CSysAi()
{
    m_pth = nullptr;
}

CSysAi::~CSysAi()
{
    this->DeInit();
}

void CSysAi::Init()
{
    MI_AUDIO_Attr_t stAiSetAttr;
    memset(&stAiSetAttr, 0, sizeof(MI_AUDIO_Attr_t));
	stAiSetAttr.eBitwidth = E_MI_AUDIO_BIT_WIDTH_16;
	stAiSetAttr.eSamplerate = E_MI_AUDIO_SAMPLE_RATE_16000;
	stAiSetAttr.eWorkmode = E_MI_AUDIO_MODE_I2S_MASTER;
	stAiSetAttr.u32FrmNum = 6;
	stAiSetAttr.u32PtNumPerFrm = 2048;// (MI_U32)1600 / 16;//MI_AUDIO_SAMPLE_PER_FRAME;
	stAiSetAttr.eSoundmode = E_MI_AUDIO_SOUND_MODE_MONO;
	stAiSetAttr.u32ChnCnt = 1;
    /* set ai public attr*/
    CHECKRESULT(MI_AI_SetPubAttr(m_AiDevId, &stAiSetAttr), MI_SUCCESS);
    /* enable ai device */
	CHECKRESULT(MI_AI_Enable(m_AiDevId), MI_SUCCESS);
    MI_S32 s32Volume = 16;
    CHECKRESULT(MI_AI_SetVqeVolume(m_AiDevId, m_AiChn, s32Volume), MI_SUCCESS);

    MI_SYS_ChnPort_t astAiChnOutputPort0;
	memset(&astAiChnOutputPort0, 0x0, sizeof(MI_SYS_ChnPort_t));
	astAiChnOutputPort0.eModId = E_MI_MODULE_ID_AI;
	astAiChnOutputPort0.u32DevId = m_AiDevId;
	astAiChnOutputPort0.u32ChnId = m_AiChn;
	astAiChnOutputPort0.u32PortId = 0;
	CHECKRESULT(MI_SYS_SetChnOutputPortDepth(&astAiChnOutputPort0, 4, 8), MI_SUCCESS);
	CHECKRESULT(MI_AI_EnableChn(m_AiDevId, m_AiChn), MI_SUCCESS);

    //open aec
	MI_AI_VqeConfig_t stAiSetVqeConfig;
	memset(&stAiSetVqeConfig, 0x0, sizeof(MI_AI_VqeConfig_t));
	stAiSetVqeConfig.u32ChnNum = 1;
	stAiSetVqeConfig.bAecOpen = TRUE;
	stAiSetVqeConfig.bAgcOpen = FALSE;
	stAiSetVqeConfig.bAnrOpen = FALSE;
	stAiSetVqeConfig.bEqOpen = FALSE;
	stAiSetVqeConfig.bHpfOpen = FALSE;
	stAiSetVqeConfig.s32FrameSample = 128;
	stAiSetVqeConfig.s32WorkSampleRate = E_MI_AUDIO_SAMPLE_RATE_16000;

	MI_AI_AecConfig_t stAecCfg = { FALSE, 0,{ 4, 6, 36, 49, 50, 51}, {5, 4, 4, 5, 10, 10, 10} };
	// AEC
	memcpy(&stAiSetVqeConfig.stAecCfg, &stAecCfg, sizeof(MI_AI_AecConfig_t));

	CHECKRESULT(MI_AI_SetVqeAttr(m_AiDevId, m_AiChn, 0, 0, &stAiSetVqeConfig), MI_SUCCESS);
	//ExecFunc(MI_AI_GetVqeAttr(AiDevId, AiChn, &stAiSetVqeConfig), MI_SUCCESS);
	CHECKRESULT(MI_AI_EnableVqe(m_AiDevId, m_AiChn), MI_SUCCESS);
    
    //get audio
    m_running = true;
    
    m_pth = new std::thread{[this](){
        MI_AUDIO_Frame_t stAiChFrame;
		MI_AUDIO_AecFrame_t stAecFrame;
        CISysAo::AudioFrame_st aframe;
        while (m_running)
        {
            memset(&stAiChFrame,0,sizeof(stAiChFrame));
            memset(&stAecFrame,0,sizeof(stAecFrame));
            if(MI_SUCCESS == MI_AI_GetFrame(m_AiDevId, m_AiChn, &stAiChFrame, nullptr, -1))
            {
                //bitwidth
                if(stAiChFrame.eBitwidth == E_MI_AUDIO_BIT_WIDTH_16)
                {
                    aframe.bitwidth = CISysAo::EAudioBitWidth::E_AUDIO_BIT_WIDTH_16;
                }
                else if(stAiChFrame.eBitwidth == E_MI_AUDIO_BIT_WIDTH_24)
                {
                    aframe.bitwidth = CISysAo::EAudioBitWidth::E_AUDIO_BIT_WIDTH_24;
                }
                //soundmode
                if(stAiChFrame.eSoundmode == E_MI_AUDIO_SOUND_MODE_MONO)
                {
                    aframe.soundmode = CISysAo::EAudioSoundMode::E_AUDIO_SOUND_MODE_MONO;
                }
                else if(stAiChFrame.eSoundmode == E_MI_AUDIO_SOUND_MODE_QUEUE)
                {
                    aframe.soundmode = CISysAo::EAudioSoundMode::E_AUDIO_SOUND_MODE_QUEUE;
                }
                else if(stAiChFrame.eSoundmode == E_MI_AUDIO_SOUND_MODE_STEREO)
                {
                    aframe.soundmode = CISysAo::EAudioSoundMode::E_AUDIO_SOUND_MODE_STEREO;
                }
                //data
                aframe.data[0] = (char*)stAiChFrame.apVirAddr[0];
                aframe.data[1] = (char*)stAiChFrame.apVirAddr[1];
                aframe.len = stAiChFrame.u32Len;
                aframe.timestamp_us = stAiChFrame.u64TimeStamp;
                std::lock_guard<std::mutex> __lock21{m_cb_mtx};
                for (auto &&each : m_map_cb)
                {
                    each.second(aframe);
                }
                MI_AI_ReleaseFrame(m_AiDevId, m_AiChn, &stAiChFrame, nullptr);
            }
        }
    }};
}
void CSysAi::DeInit()
{
    if(!m_running)
    {
        return;
    }
    m_running = false;
    if(m_pth != nullptr)
    {
        m_pth->join();
        delete m_pth;
        m_pth = nullptr;
    }
    CHECKRESULT(MI_AI_DisableVqe(m_AiDevId, m_AiChn), MI_SUCCESS);
    CHECKRESULT(MI_AI_DisableChn(m_AiDevId, m_AiChn), MI_SUCCESS);
    CHECKRESULT(MI_AI_Disable(m_AiDevId), MI_SUCCESS);
}

void CSysAi::Refresh()
{
    this->DeInit();
    this->Init();
}

//设置音量
//void SetVolume(int _db) override;

int CSysAi::Register(int _aechn,const std::function<void(CISysAo::AudioFrame_st)> &_cb)
{
    std::lock_guard<std::mutex> __lock1{m_cb_mtx};
    for (int i = 0; i < 64; i++)
    {
        auto iter = m_map_cb.find(i);
        if(iter == m_map_cb.end())
        {
            m_map_cb[i] = _cb;
            LOGGER_INFO("Register AI[%d] OK,index = %d",_aechn,i);
            return i;
        }
    }
    LOGGER_ERROR("Register AI[%d] ERROR",_aechn);
    return -1;
}
int CSysAi::CancelRegister(int _id)
{
    std::lock_guard<std::mutex> __lock1{m_cb_mtx};
    LOGGER_INFO("CancelRegister AI index = %d",_id);
    m_map_cb.erase(_id);
    return 0;
}