#include "syshdmi.h"


#include "mi_hdmi.h"

static MI_S32 Hdmi_callback_impl(MI_HDMI_DeviceId_e eHdmi, MI_HDMI_EventType_e Event, void *pEventParam, void *pUsrParam)
{
    switch (Event)
    {
        case E_MI_HDMI_EVENT_HOTPLUG:
            printf("E_MI_HDMI_EVENT_HOTPLUG.\n");
            CHECKRESULT(MI_HDMI_Start(eHdmi), MI_SUCCESS);
            break;
        case E_MI_HDMI_EVENT_NO_PLUG:
            printf("E_MI_HDMI_EVENT_NO_PLUG.\n");
            CHECKRESULT(MI_HDMI_Stop(eHdmi), MI_SUCCESS);
            break;
        default:
            printf("Unsupport event.\n");
            break;
    }

    return MI_SUCCESS;
}

CSysHdmi::CSysHdmi()
:m_hdmi_status{false}
{
    //临时
    MI_HDMI_InitParam_t stInitParam;
    MI_HDMI_DeviceId_e eHdmi = E_MI_HDMI_ID_0;

    stInitParam.pCallBackArgs = NULL;
    stInitParam.pfnHdmiEventCallback = Hdmi_callback_impl;

    CHECKRESULT(MI_HDMI_Init(&stInitParam), MI_SUCCESS);
    CHECKRESULT(MI_HDMI_Open(eHdmi), MI_SUCCESS);
    this->SetAttr();

}

CSysHdmi::~CSysHdmi()
{
    if(m_hdmi_status)
    {
        this->Stop();
    }
    MI_HDMI_DeviceId_e eHdmi = E_MI_HDMI_ID_0;
    CHECKRESULT(MI_HDMI_Close(eHdmi), MI_SUCCESS);
    CHECKRESULT(MI_HDMI_DeInit(), MI_SUCCESS);
}

CSysHdmi* CSysHdmi::GetInstance()
{
    static CSysHdmi ins;
    return &ins;
}

int CSysHdmi::Start()
{
    MI_HDMI_DeviceId_e eHdmi = E_MI_HDMI_ID_0;
    this->SetAttr();
    CHECKRESULT(MI_HDMI_Start(eHdmi), MI_SUCCESS);
    m_hdmi_status = true;
    return 0;
}

int CSysHdmi::Stop()
{
    MI_HDMI_DeviceId_e eHdmi = E_MI_HDMI_ID_0;
    CHECKRESULT(MI_HDMI_Stop(eHdmi), MI_SUCCESS);
    m_hdmi_status = false;
    return 0;
}

void CSysHdmi::SetAttr()
{
    MI_HDMI_DeviceId_e eHdmi = E_MI_HDMI_ID_0;
    MI_HDMI_Attr_t stAttr;

    memset(&stAttr, 0, sizeof(MI_HDMI_Attr_t));
    stAttr.stEnInfoFrame.bEnableAudInfoFrame  = FALSE;
    stAttr.stEnInfoFrame.bEnableAviInfoFrame  = FALSE;
    stAttr.stEnInfoFrame.bEnableSpdInfoFrame  = FALSE;
    stAttr.stAudioAttr.bEnableAudio = TRUE;
    stAttr.stAudioAttr.bIsMultiChannel = 0;
    stAttr.stAudioAttr.eBitDepth = E_MI_HDMI_BIT_DEPTH_16;
    stAttr.stAudioAttr.eCodeType = E_MI_HDMI_ACODE_PCM;
    stAttr.stAudioAttr.eSampleRate = E_MI_HDMI_AUDIO_SAMPLERATE_48K;
    stAttr.stVideoAttr.bEnableVideo = TRUE;
    stAttr.stVideoAttr.eColorType = E_MI_HDMI_COLOR_TYPE_RGB444;//default color type
    stAttr.stVideoAttr.eDeepColorMode = E_MI_HDMI_DEEP_COLOR_24BIT;
    switch (config::Config::GetInstance()->m_allcfg.eTiming)
    {
    case config::E_CONFIG_TIMING_720_60P:
        stAttr.stVideoAttr.eTimingType = E_MI_HDMI_TIMING_720_60P;
        break;
    case config::E_CONFIG_TIMING_1080_60P:
        stAttr.stVideoAttr.eTimingType = E_MI_HDMI_TIMING_1080_60P;
        break;
    
    default:
        stAttr.stVideoAttr.eTimingType = E_MI_HDMI_TIMING_1080_60P;
        break;
    }
    
    stAttr.stVideoAttr.eOutputMode = E_MI_HDMI_OUTPUT_MODE_HDMI;
    CHECKRESULT(MI_HDMI_SetAttr(eHdmi, &stAttr), MI_SUCCESS);
}