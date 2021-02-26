#ifndef __ICON_WLAN_HPP__
#define __ICON_WLAN_HPP__

#include "minigui_entry.hpp"
#include "minigui_resource.hpp"

extern int UI_OnIconWlan(int val);

// WLAN 状态的枚举
typedef enum{
	EM_WLANON = 1,			// WLAN 状态为ON 时
	EM_WLANOFF,				// WLAN 状态为OFF 时
	EM_WLANCONNECT,			// WLAN 准备连接 时
	EM_WLANEXIT,			// WLAN 退出
	EM_WLANMAX,
}EM_WLANSTATUS_T;

typedef enum{
	EM_FOCUS_SSID = 1,
	EM_FOCUS_PASSWD,
	EM_FOCUS_MAX,
}EM_INPUTFOCUS_T;

#endif
