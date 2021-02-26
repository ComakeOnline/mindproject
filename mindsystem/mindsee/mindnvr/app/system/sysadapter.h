#pragma once

#include <memory>

#include "isysdisplay.h"
#include "isyshdmi.h"
#include "isysvideodecoder.h"
#include "isysmouse.h"
#include "isysao.h"
#ifdef ENABLE_TOUCH
#include "isystouch.h"
#endif
#include "isysadec.h"
#include "isysai.h"

class CSysAdapter
{
private:
    /* data */
    CSysAdapter(/* args */);
    ~CSysAdapter();
public:
    static CISysDisplay* GetSysDisplay();
#ifndef DISABLE_HDMI
    static CISysHdmi* GetSysHdmi();
#endif
    static std::shared_ptr<CISysVideoDecoder> GetVideoDecoder(const CISysVideoDecoder::ECodeType& _codec_type);
    static CISysMouse* GetMouse();
#ifdef ENABLE_TOUCH
    static CISysTouch* GetTouch();
#endif
    static CISysAo* GetAo();
    static std::shared_ptr<CISysAdec> GetAdec(const CISysAdec::ECodeType& _codec_type);
    static CISysAi* GetAi();
};