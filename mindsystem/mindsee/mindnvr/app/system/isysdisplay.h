#pragma once

#include <cstdint>

#include "global.hpp"

class CISysDisplay
{
public:
    //static CISysDisplay* GetInstance();
    DISABLE_COPY(CISysDisplay);
    DISABLE_EQUAL(CISysDisplay);

    virtual ~CISysDisplay(){};
    CISysDisplay(/* args */){};

public:
    virtual int FbInit() = 0;
    virtual int FbShow(bool _is_show) = 0;

    //重启设备 用于修改配置时
    virtual void Restart() = 0;

    //刷新画面，不重启设备
    virtual void Refresh() = 0;

    //设置解码器输入输出端口
    virtual int SetInput(uint32_t _srcVDecChn,uint32_t _dstPort) = 0;

    //取消解码器输入输出端口
    virtual int CancelInput(uint32_t _srcVDecChn,uint32_t _dstPort) = 0;

    virtual char* GetFbBuffer() = 0;
};


