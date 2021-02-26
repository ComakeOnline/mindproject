#ifndef __NVR_ETC_HPP__
#define __NVR_ETC_HPP__

extern int UI_ETC();

bool UI_HcDetectEnabled();
bool UI_HcOSDEnabled();
bool UI_ASREnabled();
bool UI_NetStatusDispEnabled();

bool UI_DisableHcDetect();
bool UI_DisableHcOSD();
bool UI_DisableASR();
bool UI_DisableNetStatusDisp();

bool UI_EnableHcDetect();
bool UI_EnableHcOSD();
bool UI_EnableASR();
bool UI_EnableNetStatusDisp();

bool UI_InvertHcDetect();
bool UI_InvertHcOSD();
bool UI_InvertASR();
bool UI_InvertNetStatusDisp();

#endif

