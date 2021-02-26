#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "sys.hpp"
#include "isysdisplay.h"
#include "sysbind.hpp"

class CSysDispPort
{
private:
    static int s_portIndex;
    int m_myPortIndex;
    int m_myLayer;
public:
    CSysDispPort(int _DispLayer,uint16_t x,uint16_t y,uint16_t width,uint16_t height);
    ~CSysDispPort();
    DISABLE_COPY(CSysDispPort);
    DISABLE_EQUAL(CSysDispPort);
};


class CSysDispLayer
{
private:
    static int s_layerIndex;
    int m_myLayerIndex;
    std::vector<std::shared_ptr<CSysDispPort>> m_vecPort;
public:
    CSysDispLayer(/* args */);
    ~CSysDispLayer();
    DISABLE_COPY(CSysDispLayer);
    DISABLE_EQUAL(CSysDispLayer);
};



class CSysDisplay : public CSys , public CISysDisplay
{
private:
    CSysDisplay(/* args */);
    ~CSysDisplay();

    void Enable();
    void Disable();

    void ChangeFbResolution(int width, int height);
public:
    DISABLE_COPY(CSysDisplay);
    DISABLE_EQUAL(CSysDisplay);

    static CSysDisplay* GetInstance();
    int FbInit() override;
    int FbShow(bool _is_show) override;
    void Restart() override;

    void Refresh() override;

    int SetInput(uint32_t _srcVDecChn,uint32_t _dstPort) override;

    int CancelInput(uint32_t _srcVDecChn,uint32_t _dstPort) override;

    int GetFbFd(){
        return m_fbFd;
    }

    char* GetFbBuffer() override{
        return m_frameBuffer;
    }

private:
    int m_fbFd;
    char *m_frameBuffer;
    int m_screensize;
    std::vector<std::shared_ptr<CSysDispLayer>> m_vecLayer;

    //sysbind
    std::vector<std::shared_ptr<CSysBind>> m_venc_divp_bindlist;
};