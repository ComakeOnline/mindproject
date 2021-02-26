#include "sysdisplay.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <errno.h>
#include <cstring>

#include "mi_disp.h"
#include "mi_divp.h"
#include "mstarFb.h"
#include "mi_panel.h"
#include "syshdmi.h"
#include "SAT070CP50_1024x600.h"

using namespace std;


#define MAKE_YUYV_VALUE(y,u,v) ((y) << 24) | ((u) << 16) | ((y) << 8) | (v)
#define YUYV_BLACK MAKE_YUYV_VALUE(0,128,128)
#define YUYV_WHITE MAKE_YUYV_VALUE(255,128,128)
#define YUYV_RED MAKE_YUYV_VALUE(76,84,255)
#define YUYV_GREEN MAKE_YUYV_VALUE(149,43,21)
#define YUYV_BLUE MAKE_YUYV_VALUE(29,225,107)

//CSysDispPort

int CSysDispPort::s_portIndex = 0;

CSysDispPort::CSysDispPort(int _DispLayer,uint16_t x,uint16_t y,uint16_t width,uint16_t height)
{
    if(s_portIndex > 15)
    {
        runtime_error("create too mach DispLayer");
    }
    m_myPortIndex = s_portIndex;
    m_myLayer = _DispLayer;
    s_portIndex++;

#if 1
    //create DIVP
    MI_DIVP_ChnAttr_t stDivpChnAttr;
    memset(&stDivpChnAttr, 0, sizeof(MI_DIVP_ChnAttr_t));
    stDivpChnAttr.bHorMirror            = FALSE;
    stDivpChnAttr.bVerMirror            = FALSE;
    stDivpChnAttr.eDiType               = E_MI_DIVP_DI_TYPE_OFF;
    stDivpChnAttr.eRotateType           = E_MI_SYS_ROTATE_NONE;
    stDivpChnAttr.eTnrLevel             = E_MI_DIVP_TNR_LEVEL_OFF;
    stDivpChnAttr.stCropRect.u16X       = 0;
    stDivpChnAttr.stCropRect.u16Y       = 0;
    stDivpChnAttr.stCropRect.u16Width   = config::Config::GetInstance()->m_allcfg.resolution_width;
    stDivpChnAttr.stCropRect.u16Height  = config::Config::GetInstance()->m_allcfg.resolution_height;
    stDivpChnAttr.u32MaxWidth           = config::Config::GetInstance()->m_allcfg.resolution_width;
    stDivpChnAttr.u32MaxHeight          = config::Config::GetInstance()->m_allcfg.resolution_height;

    CHECKRESULT(MI_DIVP_CreateChn(m_myPortIndex, &stDivpChnAttr), MI_SUCCESS);
    CHECKRESULT(MI_DIVP_StartChn(m_myPortIndex), MI_SUCCESS);

    MI_DIVP_OutputPortAttr_t stOutputPortAttr;
    memset(&stOutputPortAttr, 0, sizeof(stOutputPortAttr));
    stOutputPortAttr.eCompMode          = E_MI_SYS_COMPRESS_MODE_NONE;
    stOutputPortAttr.ePixelFormat       = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
    stOutputPortAttr.u32Width           = width;
    stOutputPortAttr.u32Height          = height;

    CHECKRESULT(MI_DIVP_SetOutputPortAttr(m_myPortIndex, &stOutputPortAttr),MI_SUCCESS);

#endif
    //create port

    MI_DISP_InputPortAttr_t stInputPortAttr;
    CHECKRESULT(MI_DISP_GetInputPortAttr(m_myLayer, m_myPortIndex, &stInputPortAttr), MI_SUCCESS);
    cout<<"###########"<<x<<" "<<y<< " "<< width <<" "<<height<<endl;
    stInputPortAttr.u16SrcWidth = width;
    stInputPortAttr.u16SrcHeight = height;
    stInputPortAttr.stDispWin.u16X = x;
    stInputPortAttr.stDispWin.u16Y = y;
    stInputPortAttr.stDispWin.u16Width = width;
    stInputPortAttr.stDispWin.u16Height = height;
    CHECKRESULT(MI_DISP_SetInputPortAttr(m_myLayer, m_myPortIndex, &stInputPortAttr), MI_SUCCESS);
    
    // MI_DISP_VidWinRect_t stWinRect;
    // stWinRect.u16X = x;
    // MI_DISP_SetZoomInWindow(m_myLayer, m_myPortIndex, &stWinRect);


    CHECKRESULT(MI_DISP_EnableInputPort(m_myLayer, m_myPortIndex), MI_SUCCESS);
    CHECKRESULT(MI_DISP_SetInputPortSyncMode(m_myLayer, m_myPortIndex, E_MI_DISP_SYNC_MODE_FREE_RUN), MI_SUCCESS);

#if 1
    //bind
    MI_SYS_ChnPort_t stSrcChnPort;
    memset(&stSrcChnPort, 0, sizeof(stSrcChnPort));
    MI_SYS_ChnPort_t stDstChnPort;
    memset(&stDstChnPort, 0, sizeof(stDstChnPort));
    stSrcChnPort.eModId = E_MI_MODULE_ID_DIVP;
    stSrcChnPort.u32DevId = 0;
    stSrcChnPort.u32ChnId = m_myPortIndex;
    stSrcChnPort.u32PortId = 0;

    stDstChnPort.eModId = E_MI_MODULE_ID_DISP;
    stDstChnPort.u32DevId = 0;
    stDstChnPort.u32ChnId = m_myPortIndex;
    stDstChnPort.u32PortId = 0;

    MI_U32 u32SrcFrmrate = 0;
    MI_U32 u32DstFrmrate = 0;

    LOGGER_INFO("bind src:%d,%d,%d,%d - dst:%d,%d,%d,%d",stSrcChnPort.eModId,
        stSrcChnPort.u32ChnId,stSrcChnPort.u32DevId,stSrcChnPort.u32PortId,
        stDstChnPort.eModId,stDstChnPort.u32ChnId,stDstChnPort.u32DevId,stDstChnPort.u32PortId);
    CHECKRESULT(MI_SYS_BindChnPort(&stSrcChnPort, &stDstChnPort, \
        u32SrcFrmrate, u32DstFrmrate), MI_SUCCESS);

#endif
}

CSysDispPort::~CSysDispPort()
{
#if 1
    //unbind
    MI_SYS_ChnPort_t stSrcChnPort;
    memset(&stSrcChnPort, 0, sizeof(stSrcChnPort));
    MI_SYS_ChnPort_t stDstChnPort;
    memset(&stDstChnPort, 0, sizeof(stDstChnPort));
    stSrcChnPort.eModId = E_MI_MODULE_ID_DIVP;
    stSrcChnPort.u32DevId = 0;
    stSrcChnPort.u32ChnId = m_myPortIndex;
    stSrcChnPort.u32PortId = 0;

    stDstChnPort.eModId = E_MI_MODULE_ID_DISP;
    stDstChnPort.u32DevId = 0;
    stDstChnPort.u32ChnId = m_myPortIndex;
    stDstChnPort.u32PortId = 0;
    CHECKRESULT(MI_SYS_UnBindChnPort(&stSrcChnPort, &stDstChnPort), MI_SUCCESS);

    //divp
    CHECKRESULT(MI_DIVP_StopChn(m_myPortIndex), MI_SUCCESS);
    CHECKRESULT(MI_DIVP_DestroyChn(m_myPortIndex), MI_SUCCESS);

#endif
    CHECKRESULT(MI_DISP_DisableInputPort(m_myLayer, m_myPortIndex), MI_SUCCESS);
    s_portIndex--;
}

//CSysDispLayer

int CSysDispLayer::s_layerIndex = 0;

CSysDispLayer::CSysDispLayer(/* args */)
{
    MI_DISP_DEV DispDev = 0;
    if(s_layerIndex > 1)
    {
        LOGGER_ERROR("create too mach DispLayer");
        assert(0);
    }
    m_myLayerIndex = s_layerIndex;
    s_layerIndex++;
    MI_DISP_LAYER DispLayer = m_myLayerIndex;
    //获取当前配置
    auto vecLw = config::Config::GetInstance()->ConvertLayerCfg();
    //检查是否有该Layer的配置
    try
    {
        auto lw = vecLw.at(m_myLayerIndex);
#if 0
        MI_DISP_VideoLayerAttr_t stLayerAttr;
        memset(&stLayerAttr, 0, sizeof(stLayerAttr));
        stLayerAttr.stVidLayerSize.u16Width  = lw.vsize_width;
        stLayerAttr.stVidLayerSize.u16Height = lw.vsize_height;

        stLayerAttr.ePixFormat = E_MI_SYS_PIXEL_FRAME_YUV_MST_420;
        stLayerAttr.stVidLayerDispWin.u16X      = lw.x;
        stLayerAttr.stVidLayerDispWin.u16Y      = lw.y;
        stLayerAttr.stVidLayerDispWin.u16Width  = lw.width;
        stLayerAttr.stVidLayerDispWin.u16Height = lw.height;
#endif
        CHECKRESULT(MI_DISP_BindVideoLayer(DispLayer, DispDev), MI_SUCCESS);
#if 0
        CHECKRESULT(MI_DISP_SetVideoLayerAttr(DispLayer, &stLayerAttr), MI_SUCCESS);
        CHECKRESULT(MI_DISP_GetVideoLayerAttr(DispLayer, &stLayerAttr), MI_SUCCESS);
        printf("\n");
        printf("[%s %d]Get Video Layer Size [%d, %d] !!!\n", __FUNCTION__, __LINE__, stLayerAttr.stVidLayerSize.u16Width,
            stLayerAttr.stVidLayerSize.u16Height);
        printf("[%s %d]Get Video Layer DispWin [%d, %d, %d, %d] !!!\n", __FUNCTION__, __LINE__,\
            stLayerAttr.stVidLayerDispWin.u16X, stLayerAttr.stVidLayerDispWin.u16Y,
            stLayerAttr.stVidLayerDispWin.u16Width, stLayerAttr.stVidLayerDispWin.u16Height);
        printf("\n");
#endif
        CHECKRESULT(MI_DISP_EnableVideoLayer(DispLayer), MI_SUCCESS);

        //创建Port
        for (auto &&i : lw.vecPort)
        {
            auto port_ptr = make_shared<CSysDispPort>(DispLayer,i.x,i.y,i.width,i.hetght);
            m_vecPort.emplace_back(port_ptr);
        }
        
    }
    catch(const std::exception& e)
    {
        LOGGER_ERROR("ERROR Layer Create!");
        //std::runtime_error{"ERROR Layer Create!"};
        assert(0);
    }
}

CSysDispLayer::~CSysDispLayer()
{
    MI_DISP_DEV DispDev = 0;
    MI_DISP_LAYER DispLayer = m_myLayerIndex;
    s_layerIndex--;

    m_vecPort.clear();
    CHECKRESULT(MI_DISP_DisableVideoLayer(DispLayer), MI_SUCCESS);
    CHECKRESULT(MI_DISP_UnBindVideoLayer(DispLayer, DispDev), MI_SUCCESS);
}


CSysDisplay::CSysDisplay(/* args */)
:m_fbFd{-1},
m_frameBuffer{nullptr}
{
    this->Enable();
}

CSysDisplay::~CSysDisplay()
{
    this->Disable();
}

void CSysDisplay::Enable()
{
    //临时设置
    MI_DISP_DEV DispDev = 0;
    MI_DISP_PubAttr_t stPubAttr;
    memset(&stPubAttr, 0, sizeof(stPubAttr));
    stPubAttr.u32BgColor = YUYV_BLACK;
    switch (config::Config::GetInstance()->m_allcfg.eTiming)
    {
    case config::E_CONFIG_TIMING_720_60P:
        stPubAttr.eIntfSync = E_MI_DISP_OUTPUT_720P60;
        stPubAttr.eIntfType = E_MI_DISP_INTF_HDMI;
        break;
    case config::E_CONFIG_TIMING_1080_60P:
        stPubAttr.eIntfSync = E_MI_DISP_OUTPUT_1080P60;
        stPubAttr.eIntfType = E_MI_DISP_INTF_HDMI;
        break;
    case config::E_CONFIG_TIMING_1024_600_LCD:
        stPubAttr.stSyncInfo.u16Vact = stPanelParam.u16Height;
        stPubAttr.stSyncInfo.u16Vbb = stPanelParam.u16VSyncBackPorch;
        stPubAttr.stSyncInfo.u16Vfb = stPanelParam.u16VTotal - (stPanelParam.u16VSyncWidth + stPanelParam.u16Height + stPanelParam.u16VSyncBackPorch);
        stPubAttr.stSyncInfo.u16Hact = stPanelParam.u16Width;
        stPubAttr.stSyncInfo.u16Hbb = stPanelParam.u16HSyncBackPorch;
        stPubAttr.stSyncInfo.u16Hfb = stPanelParam.u16HTotal - (stPanelParam.u16HSyncWidth + stPanelParam.u16Width + stPanelParam.u16HSyncBackPorch);
        stPubAttr.stSyncInfo.u16Bvact = 0;
        stPubAttr.stSyncInfo.u16Bvbb = 0;
        stPubAttr.stSyncInfo.u16Bvfb = 0;
        stPubAttr.stSyncInfo.u16Hpw = stPanelParam.u16HSyncWidth;
        stPubAttr.stSyncInfo.u16Vpw = stPanelParam.u16VSyncWidth;
        stPubAttr.stSyncInfo.u32FrameRate = stPanelParam.u16DCLK*1000000/(stPanelParam.u16HTotal*stPanelParam.u16VTotal);
        stPubAttr.eIntfSync = E_MI_DISP_OUTPUT_USER;
        stPubAttr.eIntfType = E_MI_DISP_INTF_LCD;
        break;
    
    default:
        stPubAttr.eIntfSync = E_MI_DISP_OUTPUT_1080P60;
        stPubAttr.eIntfType = E_MI_DISP_INTF_HDMI;
        break;
    }
    
    CHECKRESULT(MI_DISP_SetPubAttr(DispDev, &stPubAttr), MI_SUCCESS);
    CHECKRESULT(MI_DISP_Enable(DispDev), MI_SUCCESS);

    auto vecLayer = config::Config::GetInstance()->ConvertLayerCfg();
    for (auto &&i : vecLayer)
    {
        auto layer_ptr = make_shared<CSysDispLayer>();
        m_vecLayer.emplace_back(layer_ptr);
    }

    //Init panel
    MI_PANEL_Init(stPanelParam.eLinkType);
    MI_PANEL_SetPanelParam(&stPanelParam);
    //调整屏幕参数
    system("echo csc 0 4 60 80 45 60 0 0 > proc/mi_modules/mi_disp/mi_disp0");
}
void CSysDisplay::Disable()
{
    MI_DISP_DEV DispDev = 0;
    m_vecLayer.clear();
    CHECKRESULT(MI_DISP_Disable(DispDev), MI_SUCCESS);

    if (m_frameBuffer != nullptr)
    {
        munmap (m_frameBuffer, m_screensize);
        m_frameBuffer = nullptr;
    }

    if(m_fbFd > 0)
    {
        close(m_fbFd);
    }
    MI_PANEL_DeInit();
}

void CSysDisplay::ChangeFbResolution(int width, int height)
{
    if(m_fbFd < 0)
    {
        return;
    }
    printf("%s %d, width:%d,height:%d\n", __func__, __LINE__, width, height);
    MI_FB_DisplayLayerAttr_t stDisplayAttr;
    memset(&stDisplayAttr, 0, sizeof(MI_FB_DisplayLayerAttr_t));
    stDisplayAttr.u32SetAttrMask =  E_MI_FB_DISPLAYLAYER_ATTR_MASK_SCREEN_SIZE |
        E_MI_FB_DISPLAYLAYER_ATTR_MASK_BUFFER_SIZE | E_MI_FB_DISPLAYLAYER_ATTR_MASK_DISP_SIZE
        | E_MI_FB_DISPLAYLAYER_ATTR_MASK_DISP_POS | E_MI_FB_DISPLAYLAYER_ATTR_MASK_COLOR_FMB;
    stDisplayAttr.u32Xpos = 0;
    stDisplayAttr.u32YPos = 0;
    stDisplayAttr.u32dstWidth = width;
    stDisplayAttr.u32dstHeight = height;
    stDisplayAttr.u32DisplayWidth = (width > 1920) ? 1920 : width;
    stDisplayAttr.u32DisplayHeight = (height > 1080) ? 1080 : height;
    stDisplayAttr.u32ScreenWidth = width;
    stDisplayAttr.u32ScreenHeight = height;
    stDisplayAttr.eFbColorFmt = E_MI_FB_COLOR_FMT_ARGB8888;
    //E_MI_FB_DISPLAYLAYER_ATTR_MASK_BUFFER_SIZE operaton will change
    //var info and fix.line_lingth, so Need Retrive fixinfo and varinfo
    if (ioctl(m_fbFd, FBIOSET_DISPLAYLAYER_ATTRIBUTES, &stDisplayAttr))
    {
        perror("Error: failed to FBIOSET_DISPLAYLAYER_ATTRIBUTES");
        exit(0);
        return;
    }

    fb_var_screeninfo vinfo;
    fb_fix_screeninfo finfo;
    // get fb_fix_screeninfo
    if (ioctl(m_fbFd, FBIOGET_FSCREENINFO, &finfo) == -1)
    {
        perror("Error reading fixed information");
        exit(0);
    }
    // printFixedInfo();

    // get fb_var_screeninfo
    if (ioctl(m_fbFd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        perror("Error reading variable information");
        exit(0);
    }
    // printVariableInfo();

    // clear framebuffer
    //drawRect(0, 0, vinfo.xres, vinfo.yres, ARGB888_BLUE);
    // memset(frameBuffer, 0x0, finfo.smem_len);
    //darw
    const int bytesPerPixel = 4;
    int stride = 0;
    int *dest = NULL;
    int x = 0, y = 0;

    stride = finfo.line_length / bytesPerPixel;
    dest = (int *) (m_frameBuffer)
        + (0 + vinfo.yoffset) * stride + (0 + vinfo.xoffset);

    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            dest[x] = 0x00000000;
        }
        dest += stride;
    }

    //Pandisplay
    if (ioctl(m_fbFd, FBIOPAN_DISPLAY, &vinfo) == -1)
    {
        perror("Error reading variable information");
        exit(0);
    }

    //
    // MI_FB_ColorKey_t colorKeyInfo;
    // if (ioctl(m_fbFd, FBIOGET_COLORKEY,&colorKeyInfo) < 0) {
    //     perror("Error: failed to FBIOGET_COLORKEY\n");
    //     exit(0);
    // }

    // colorKeyInfo.bKeyEnable = TRUE;
    // colorKeyInfo.u8Red = 0;
    // colorKeyInfo.u8Green = 0;
    // colorKeyInfo.u8Blue = 0xff;

    // //convertColorKeyByFmt(&colorKeyInfo);
    // if (ioctl(m_fbFd, FBIOSET_COLORKEY, &colorKeyInfo) < 0) {
    //     perror("Error: failed to FBIOGET_COLORKEY");
    //     exit(0);
    // }

    MI_FB_GlobalAlpha_t stAlphaInfo;
    if (ioctl(m_fbFd, FBIOGET_GLOBAL_ALPHA, &stAlphaInfo) < 0)
    {
        perror("Error: failed to FBIOGET_GLOBAL_ALPHA");
        exit(0);
    }
    stAlphaInfo.bAlphaEnable = TRUE;
    stAlphaInfo.bAlphaChannel= TRUE;
    stAlphaInfo.u8GlobalAlpha = 0xff;
    if (ioctl(m_fbFd, FBIOSET_GLOBAL_ALPHA, &stAlphaInfo) < 0)
    {
        perror("Error: failed to FBIOGET_GLOBAL_ALPHA");
        exit(0);
    }

    
}


CSysDisplay* CSysDisplay::GetInstance()
{
    static CSysDisplay ins;
    return &ins;
}

int CSysDisplay::FbInit()
{
    const char *devfile = "/dev/fb0";

    /* Open the file for reading and writing */
    m_fbFd = open (devfile, O_RDWR);
    cout<<"fbFD="<<m_fbFd<<endl;
    if (m_fbFd == -1)
    {
        perror("Error: cannot open framebuffer device");
        exit(0);
    }
    fb_var_screeninfo vinfo;
    fb_fix_screeninfo finfo;

    //get fb_fix_screeninfo
    if (ioctl(m_fbFd, FBIOGET_FSCREENINFO, &finfo) == -1)
    {
        perror("Error reading fixed information");
        exit(0);
    }
    //printFixedInfo();

    //get fb_var_screeninfo
    if (ioctl(m_fbFd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        perror("Error reading variable information");
        exit(0);
    }
    //printVariableInfo();

    /* Figure out the size of the screen in bytes */
    m_screensize = finfo.smem_len;

    /* Map the device to memory */
    m_frameBuffer =
        (char *) mmap(0, m_screensize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fbFd, 0);
    if (m_frameBuffer == MAP_FAILED)
    {
        perror("Error: Failed to map framebuffer device to memory");
        exit(0);
    }

    this->ChangeFbResolution(config::Config::GetInstance()->m_allcfg.resolution_width,
        config::Config::GetInstance()->m_allcfg.resolution_height);
    return 0;
    // clear framebuffer
    //drawRect(0, 0, vinfo.xres, vinfo.yres, ARGB888_BLUE);
    // memset(frameBuffer, ARGB1555_RED, g_screensize);
}

int CSysDisplay::FbShow(bool _is_show)
{
    if (m_fbFd < 0)
    {
        return -1;
    }

    fb_var_screeninfo vinfo;
    if (ioctl(m_fbFd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        perror("Error reading variable information");
        exit(0);
    }
    if (ioctl(m_fbFd, FBIOPAN_DISPLAY, &vinfo) == -1)
    {
        perror("Error: failed to FBIOPAN_DISPLAY");
        exit(0);
    }

    if (ioctl(m_fbFd, FBIOSET_SHOW, &_is_show)<0)
    {
        perror("Error: failed to FBIOSET_SHOW");
    }

    printf("%s fb\n", _is_show ? "show" : "hide");
    return 0;
}

void CSysDisplay::Restart()
{
    //CSysHdmi::GetInstance()->Stop();
    this->Disable();
    this->Enable();
    //CSysHdmi::GetInstance()->Start();
    FbInit();
    FbShow(true);
    this->ChangeFbResolution(config::Config::GetInstance()->m_allcfg.resolution_width,
        config::Config::GetInstance()->m_allcfg.resolution_height);
}

void CSysDisplay::Refresh()
{
    m_venc_divp_bindlist.clear();
    m_vecLayer.clear();
    auto vecLayer = config::Config::GetInstance()->ConvertLayerCfg();
    for (auto &&i : vecLayer)
    {
        auto layer_ptr = make_shared<CSysDispLayer>();
        m_vecLayer.emplace_back(layer_ptr);
    }
}

int CSysDisplay::SetInput(uint32_t _srcVDecChn,uint32_t _dstPort)
{
    this->CancelInput(_srcVDecChn,_dstPort);
    MI_SYS_ChnPort_t stSrcChnPort;
    MI_SYS_ChnPort_t stDstChnPort;
    // bind VDEC->DIVP
    stSrcChnPort.eModId = E_MI_MODULE_ID_VDEC;
    stSrcChnPort.u32DevId = 0;
    stSrcChnPort.u32ChnId = _srcVDecChn;
    stSrcChnPort.u32PortId = 0;

    stDstChnPort.eModId = E_MI_MODULE_ID_DIVP;
    stDstChnPort.u32DevId = 0;
    stDstChnPort.u32ChnId = _dstPort;
    stDstChnPort.u32PortId = 0;

    auto bind_ptr = make_shared<CSysBind>(stSrcChnPort,stDstChnPort,0,0);
    m_venc_divp_bindlist.emplace_back(bind_ptr);
    return 0;
}

int CSysDisplay::CancelInput(uint32_t _srcVDecChn,uint32_t _dstPort)
{
    for (auto iter = m_venc_divp_bindlist.begin();iter!= m_venc_divp_bindlist.end();)
    {
        if((*iter) == nullptr)
        {
            iter++;
        }
        if ((*iter)->stSrcChnPort.u32ChnId == _srcVDecChn)
        {
            iter = m_venc_divp_bindlist.erase(iter);
        }
        else if((*iter)->stDstChnPort.u32ChnId == _dstPort)
        {
            iter = m_venc_divp_bindlist.erase(iter);
        }
        else
        {
            iter++;
        }
        
    }
    return 0;
}