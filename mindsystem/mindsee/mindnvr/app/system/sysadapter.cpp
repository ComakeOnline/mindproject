#include "sysadapter.h"
#include "sysdisplay.h"
#include "syshdmi.h"
#include "sysvideodecoder.h"
#include "sysmouse.h"
#include "sysao.h"
#ifdef ENABLE_TOUCH
#include "systouch.h"
#endif

#include "sysadec.h"
#include "sysai.h"

CSysAdapter::CSysAdapter(/* args */)
{
}

CSysAdapter::~CSysAdapter()
{
}


CISysDisplay* CSysAdapter::GetSysDisplay()
{
    return CSysDisplay::GetInstance();
}

#ifndef DISABLE_HDMI
CISysHdmi* CSysAdapter::GetSysHdmi()
{
    return CSysHdmi::GetInstance();
}
#endif

std::shared_ptr<CISysVideoDecoder> CSysAdapter::GetVideoDecoder(const CISysVideoDecoder::ECodeType& _codec_type)
{
    auto ret = std::make_shared<CSysVideoDecoder>(_codec_type);
    return ret;
}

CISysMouse* CSysAdapter::GetMouse()
{
    return CSysMouse::GetInstance();
}

#ifdef ENABLE_TOUCH
CISysTouch* CSysAdapter::GetTouch()
{
    return CSysTouch::GetInstance();
}
#endif

CISysAo* CSysAdapter::GetAo()
{
    return CSysAo::GetInstance();
}

std::shared_ptr<CISysAdec> CSysAdapter::GetAdec(const CISysAdec::ECodeType& _codec_type)
{
    auto ret = std::make_shared<CSysAdec>(_codec_type);
    return ret;
}

CISysAi* CSysAdapter::GetAi()
{
    return CSysAi::GetInstance();
}