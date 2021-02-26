#include "sysao.h"

#include "mi_common_datatype.h"
#include "mi_ao.h"

CSysAo::CSysAo()
{
}

CSysAo::~CSysAo()
{
    DeInit();
}

void CSysAo::Init()
{
    m_running = true;
    MI_AUDIO_Attr_t stSetAttr;
    memset(&stSetAttr, 0, sizeof(MI_AUDIO_Attr_t));
	stSetAttr.eBitwidth = E_MI_AUDIO_BIT_WIDTH_16;
	stSetAttr.eWorkmode = E_MI_AUDIO_MODE_I2S_MASTER;
	stSetAttr.u32FrmNum = 16;
	stSetAttr.u32PtNumPerFrm = 128;
	stSetAttr.u32ChnCnt = 1;
	stSetAttr.eSoundmode = E_MI_AUDIO_SOUND_MODE_MONO;
	stSetAttr.eSamplerate = E_MI_AUDIO_SAMPLE_RATE_16000;
    /* set ao public attr*/
    CHECKRESULT(MI_AO_SetPubAttr(m_AoDevId, &stSetAttr), MI_SUCCESS);
    CHECKRESULT(MI_AO_Enable(m_AoDevId), MI_SUCCESS);
    CHECKRESULT(MI_AO_SetVolume(0, 10), MI_SUCCESS);
    CHECKRESULT(MI_AO_EnableChn(m_AoDevId, m_AoChn), MI_SUCCESS);
    //CHECKRESULT(MI_AO_SetVolume(0, 30), MI_SUCCESS);
}
void CSysAo::DeInit()
{
    if(!m_running)
    {
        return;
    }
    CHECKRESULT(MI_AO_DisableChn(m_AoDevId, m_AoChn), MI_SUCCESS);
	/* disable ao device */
	CHECKRESULT(MI_AO_Disable(m_AoDevId), MI_SUCCESS);
    m_running = false;
}

void CSysAo::Refresh()
{
    this->DeInit();
    this->Init();
}

//设置音量
void CSysAo::SetVolume(int _db)
{
    if(!m_running)
    {
        return;
    }
    CHECKRESULT(MI_AO_SetVolume(0, _db), MI_SUCCESS);
}

//送出音频pcm数据
int CSysAo::SendFrame(AudioFrame_st _frame)
{
    if(!m_running)
    {
        return -1;
    }
    MI_AUDIO_Frame_t stAoSendFrame;
    //read data and send to AO module
    stAoSendFrame.u32Len = _frame.len;
    stAoSendFrame.apVirAddr[0] = _frame.data[0];
    stAoSendFrame.apVirAddr[1] = _frame.data[1];
    MI_S32 s32RetSendStatus = 0;
    do
    {
        s32RetSendStatus = MI_AO_SendFrame(m_AoDevId, m_AoChn, &stAoSendFrame, 128);
    } while (s32RetSendStatus == MI_AO_ERR_NOBUF);
    return 0;
}
