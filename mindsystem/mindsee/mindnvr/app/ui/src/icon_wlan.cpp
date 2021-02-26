#include "icon_wlan.hpp"
#include "ui_wlankeyboard.hpp"
#include "ui_stringmanager.hpp"

#include "ipcmanager.hpp"

// 公共部分，WIFI 设置页面的函数声明和变量声明。
/***
?*??.--,???????.--,
?*?(?(??\.---./??)?)
?*??'.__/o???o\__.'
?*?????{=??^??=}
?*??????>??-??<
?*?????/???????\
?*????//???????\\
?*???//|???.???|\\
?*???"'\???????/'"_.-~^`'-.
?*??????\??_??/--'?????????`
?*????___)(?)(___
?*???(((__)?(__)))????用于间隔程序模块
?*/

EM_WLANSTATUS_T emWifiStatus = EM_WLANOFF;
EM_INPUTFOCUS_T emInputFocus = EM_FOCUS_MAX;

// 资源文件
static BITMAP bitmapWlanOn = {0};
static PBITMAP const pBitmapWlanOn = &bitmapWlanOn;
static const char *bitmapWlanOnPath = "./res/icon_on.png";
static BITMAP bitmapWlanOff = {0};
static PBITMAP const pBitmapWlanOff = &bitmapWlanOff;
static const char *bitmapWlanOffPath = "./res/icon_off.png";
static BITMAP bitmapRefresh = {0};
static PBITMAP const pBitmapRefresh = &bitmapRefresh;
static const char *bitmapRefreshPath = "./res/refresh.png";
static BITMAP bitmapWLAN = {0};
static PBITMAP const pBitmapWLAN = &bitmapWLAN;
static const char *bitmapWLANPath = "./res/wifi.png";

// 控件句柄
static HWND hKeyboard = HWND_INVALID;

// WLAN连接对话框的宽和高
const static int DIALOG_CONNECT_WIDTH = DIALOG_WIDTH - 100;
const static int DIALOG_CONNECT_HEIGHT = DIALOG_HEIGHT - 50;
const static int DIALOG_CONNECT_SX = DIALOG_STARTX + (DIALOG_WIDTH - DIALOG_CONNECT_WIDTH) / 2;
const static int DIALOG_CONNECT_SY = DIALOG_STARTY + (DIALOG_HEIGHT - DIALOG_CONNECT_HEIGHT) / 2;

// “WLAN状态” 的文字和图片
const static int WifiStatusTextSX = 60;
const static int WifiStatusTextSY = 80;
const static int WifiStatusTextW = 100;
const static int WifiStatusTextH = 50;
const static int WifiStatusBitmapSX = WifiStatusTextSX + WifiStatusTextW + 40;
const static int WifiStatusBitmapSY = WifiStatusTextSY - 5;
const static int WifiStatusBitmapW = 130;
const static int WifiStatusBitmapH = WifiStatusTextH - 15;

// 离开按钮 坐标
const static int backSX = WifiStatusTextSX + 70;
const static int backSY = 285;
const static int backW = 100;
const static int backH = 50;

// 控件ID
const static int ID_BACK = 100;
const static int ID_WIFISTATUSTEXT = 101;
const static int ID_WIFISTATUSBITMAP = 102;
// 键盘ID, 该ID 之后的KeyNum 个值不应该再用于其它控件，详见:UI_CreateKeyboardFrame()
const static int ID_KEYBOARD = 140;

// 对话框模板
extern DLGTEMPLATE DlgTmplWifiOn;
extern DLGTEMPLATE DlgTmplWifiOff;
extern DLGTEMPLATE DlgTmplWifiConnect;

// 对话框过程函数声明
#ifndef MG4_0_4
static LRESULT UI_PorcWifiOn(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
static LRESULT UI_PorcWifiOff(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
static LRESULT UI_PorcWifiConnect(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_PorcWifiOn(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT UI_PorcWifiOff(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT UI_PorcWifiConnect(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

// 控件消息的回调函数声明
static void UI_NotifProcWifiOn(HWND hwnd, LINT id, int nc, DWORD add_data);
static void UI_NotifProcWifiOff(HWND hwnd, LINT id, int nc, DWORD add_data);
static void UI_NotifProcWifiConnect(HWND hwnd, LINT id, int nc, DWORD add_data);

// UI资源函数声明
static int UI_LoadResourceInWlan(void);
static int UI_UnloadResourceInWlan(void);

// wifi 相关函数声明
static int UI_UpdateSsidPasswd(char ch);
static int UI_ShowSsidPasswd(HWND hParent);
static int UI_SndSsidPasswd2IPC(int ipcIndex, const char *ssid, const char *passwd);

/*-----------------------------------------------------------------------------
描   述：WLAN 模块总入口
返回值：无
-----------------------------------------------------------------------------*/
int UI_OnIconWlan(int val)
{
	printf("val = %d\n", val);
	printf("emWifiStatus = %d\n", emWifiStatus);

	while(1)
	{
		static bool isFirstIn = true;
		if(isFirstIn)
		{
			isFirstIn = false;
			UI_LoadResourceInWlan();
		}
	
		if(EM_WLANOFF == emWifiStatus)			//wifi off
		{
			DialogBoxIndirectParam(&DlgTmplWifiOff, hIconScreen, UI_PorcWifiOff, 0);
		}
		else if(EM_WLANON == emWifiStatus)		// wifi on
		{
			DialogBoxIndirectParam(&DlgTmplWifiOn, hIconScreen, UI_PorcWifiOn, 0);	
		}
		else if(EM_WLANCONNECT == emWifiStatus)	// wifi connect
		{
			
		}
		else if(EM_WLANEXIT == emWifiStatus)
		{
			isFirstIn = true;
			emWifiStatus = EM_WLANOFF;
			UI_UnloadResourceInWlan();
			break;
		}
		else
		{
			UI_print("emWifiStatus has invalid value.\n");
		}
	}
	
	return 0;
}

// 第一部分，wifi OFF
/***
?*??.--,???????.--,
?*?(?(??\.---./??)?)
?*??'.__/o???o\__.'
?*?????{=??^??=}
?*??????>??-??<
?*?????/???????\
?*????//???????\\
?*???//|???.???|\\
?*???"'\???????/'"_.-~^`'-.
?*??????\??_??/--'?????????`
?*????___)(?)(___
?*???(((__)?(__)))????用于间隔程序模块
?*/

// 控件列表 WLAN OFF
static CTRLDATA CtrlArrWifiOff[] = {
	// 离开
	{
		CTRL_BUTTON,
		WS_VISIBLE | BS_NOBORDER | BS_TEXT,
		backSX, backSY, backW, backH,
		ID_BACK,
		"离开",
		0,
		 WS_EX_TRANSPARENT,
	},
	// wifi 状态 文字
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY,
		WifiStatusTextSX, WifiStatusTextSY, WifiStatusTextW, WifiStatusTextH,
		ID_WIFISTATUSTEXT,
		"WIFI状态",
		0,
		 WS_EX_TRANSPARENT,
	},
	// wifi 状态 图片
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY | SS_BITMAP,
		WifiStatusBitmapSX, WifiStatusBitmapSY, WifiStatusBitmapW, WifiStatusBitmapH,
		ID_WIFISTATUSBITMAP,
		"点我开",
		(DWORD)pBitmapWlanOff,
		WS_EX_NONE,
	},
};

// 对话框模板 WLAN OFF
DLGTEMPLATE DlgTmplWifiOff = {
	WS_VISIBLE,
	WS_EX_NONE,
	DIALOG_STARTX, DIALOG_STARTY, DIALOG_WIDTH, DIALOG_HEIGHT,
	"",
	0, 0,
	TABLESIZE(CtrlArrWifiOff), CtrlArrWifiOff,
};

/*-----------------------------------------------------------------------------
描   述：WLAN OFF 时的窗口过程函数
返回值：无
-----------------------------------------------------------------------------*/
#ifndef MG4_0_4
static LRESULT UI_PorcWifiOff(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_PorcWifiOff(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{
	static HWND hButton[3] = {0};
	
	switch(message)
	{
		case MSG_INITDIALOG:
			SetNotificationCallback(GetDlgItem(hWnd, ID_WIFISTATUSTEXT), UI_NotifProcWifiOff);
			SetNotificationCallback(GetDlgItem(hWnd, ID_WIFISTATUSBITMAP), UI_NotifProcWifiOff);

			SetWindowBkColor(hWnd, PIXEL_green0);
			SetWindowBkColor(GetDlgItem(hWnd, ID_BACK), PIXEL_green0);
			break;
		case MSG_COMMAND:
			switch(wParam)
			{
				case ID_BACK:
					UI_print("Click ID_BACK.\n");
					emWifiStatus = EM_WLANEXIT;
					SendMessage(hWnd, MSG_CLOSE, 0, 0);
					break;
				default:
					break;
			}
			break;
		case MSG_SETTEXT:
			break;
		case MSG_GETTEXT:
			break;
		case MSG_FONTCHANGED:
			break;
		case MSG_PAINT:
			break;
		case MSG_ERASEBKGND:
			break;
		case MSG_CLOSE:
			EndDialog(hWnd, 0);
			return 0;
		case MSG_KEYDOWN:
			break;
		case MSG_KEYUP:
			break;
		case MSG_LBUTTONDOWN:
			break;
		case MSG_LBUTTONUP:
			break;
		default:
			break;
	}
	
	return DefaultDialogProc(hWnd, message, wParam, lParam);
}

/*-----------------------------------------------------------------------------
描   述：WLAN OFF 时的控件消息回调函数。
返回值：无
-----------------------------------------------------------------------------*/
static void UI_NotifProcWifiOff(HWND hWnd, LINT id, int nc, DWORD add_data)
{
	if(STN_CLICKED != nc)
	{
		UI_print("Message is not STN_CLICKED, return.\n");
		return;
	}

	switch(id)
	{
		case ID_WIFISTATUSTEXT:
			UI_print("Click ID_WIFISTATUSTEXT.\n");
			break;
		case ID_WIFISTATUSBITMAP:
			UI_print("Click ID_WIFISTATUSBITMAP.\n");
			SendMessage(GetParent(hWnd), MSG_CLOSE, 0, 0);
			emWifiStatus = EM_WLANON;
			break;
		default:
			UI_print("Click nothing.\n");
			break;
	}
}


// 第二部分，wifi ON
/***
?*??.--,???????.--,
?*?(?(??\.---./??)?)
?*??'.__/o???o\__.'
?*?????{=??^??=}
?*??????>??-??<
?*?????/???????\
?*????//???????\\
?*???//|???.???|\\
?*???"'\???????/'"_.-~^`'-.
?*??????\??_??/--'?????????`
?*????___)(?)(___
?*???(((__)?(__)))????用于间隔程序模块
?*/


// 上一页 刷新 下一页 的坐标
const static int PageUpSX = 400;
const static int PageUpSY = 320;
const static int PageUpW = 60;
const static int PageUpH = 50;
const static int RefreshSX = 500;
const static int RefreshSY = PageUpSY - 10;
const static int RefreshW = PageUpW;
const static int RefreshH = PageUpH;
const static int PageDownSX = 600;
const static int PageDownSY = PageUpSY;
const static int PageDownW = PageUpW;
const static int PageDownH = PageUpH;

// WLAN 列表 坐标
const static int WlanListSX = 400;
const static int WlanListSY = 80;
const static int WlanListW = 250;
const static int WlanListH = 200;

// “点我连接” 坐标
const static int ConnectSX = 130;
const static int ConnectSY = 225;
const static int ConnectW = 100;
const static int ConnectH = 50;

// WLAN 图标
const static int WLANSX = ConnectSX + 5;
const static int WLANSY = 125;
const static int WLANW = 85;
const static int WLANH = WLANW;

// 控件ID
const static int ID_PAGEUP = 120;
const static int ID_REFRESH = 121;
const static int ID_PAGEDOWN = 122;
const static int ID_WLANLIST = 123;
const static int ID_CONNECT = 124;
const static int ID_WLAN = 125;

// 控件列表 WLAN ON
static CTRLDATA CtrlArrWifiOn[] = {
	// 离开
	{
		CTRL_BUTTON,
		WS_VISIBLE | BS_NOBORDER | BS_TEXT,
		backSX, backSY, backW, backH,
		ID_BACK,
		"离开",
		0,
		WS_EX_NONE,
	},
	// wifi 状态 文字
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY,
		WifiStatusTextSX, WifiStatusTextSY, WifiStatusTextW, WifiStatusTextH,
		ID_WIFISTATUSTEXT,
		"WIFI状态",
		0,
		WS_EX_TRANSPARENT,
	},
	// wifi 状态 图片
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY | SS_BITMAP,
		WifiStatusBitmapSX, WifiStatusBitmapSY, WifiStatusBitmapW, WifiStatusBitmapH,
		ID_WIFISTATUSBITMAP,
		"点我关",
		(DWORD)pBitmapWlanOn,
		WS_EX_NONE,
	},
	// 上一页
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY | SS_BITMAP,
		PageUpSX, PageUpSY, PageUpW, PageUpH,
		ID_PAGEUP,
		"上一页",
		0,
		WS_EX_TRANSPARENT,
	},
	// 刷新
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY | SS_BITMAP,
		RefreshSX, RefreshSY, RefreshW, RefreshH,
		ID_REFRESH,
		"刷新",
		(DWORD)pBitmapRefresh,
		WS_EX_TRANSPARENT,
	},
	// 下一页
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY | SS_BITMAP,
		PageDownSX, PageDownSY, PageDownW, PageDownH,
		ID_PAGEDOWN,
		"下一页",
		0,
		WS_EX_TRANSPARENT,
	},
	// WLAN 列表
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_BORDER | SS_CENTER | SS_NOTIFY,
		WlanListSX, WlanListSY, WlanListW, WlanListH,
		ID_WLANLIST,
		"为WLAN列表预留",
		0,
		WS_EX_TRANSPARENT,
	},
	// “点我连接”
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY | SS_BITMAP,
		ConnectSX, ConnectSY, ConnectW, ConnectH,
		ID_CONNECT,
		"点我连接",
		0,
		WS_EX_TRANSPARENT,
	},
	// WLAN
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY | SS_BITMAP,
		WLANSX, WLANSY, WLANW, WLANH,
		ID_WLAN,
		"点我连接",
		(DWORD)pBitmapWLAN,
		WS_EX_TRANSPARENT,
	},
};

// 对话框模板 WLAN ON
DLGTEMPLATE DlgTmplWifiOn = {
	WS_VISIBLE,
	WS_EX_NONE,
	DIALOG_STARTX, DIALOG_STARTY, DIALOG_WIDTH, DIALOG_HEIGHT,
	"",
	0, 0,
	TABLESIZE(CtrlArrWifiOn), CtrlArrWifiOn,
};

/*-----------------------------------------------------------------------------
描   述：WLAN ON 时的窗口过程函数
返回值：无
-----------------------------------------------------------------------------*/
#ifndef MG4_0_4
static LRESULT UI_PorcWifiOn(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_PorcWifiOn(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{
	static HWND hButton[3] = {0};
	
	switch(message)
	{
		case MSG_INITDIALOG:
			// 为控件设置回调函数。
			//SetNotificationCallback(GetDlgItem(hWnd, ID_WIFISTATUSTEXT), UI_NotifProcWifiOn);
			SetNotificationCallback(GetDlgItem(hWnd, ID_WIFISTATUSBITMAP), UI_NotifProcWifiOn);
			SetNotificationCallback(GetDlgItem(hWnd, ID_PAGEUP), UI_NotifProcWifiOn);
			SetNotificationCallback(GetDlgItem(hWnd, ID_PAGEDOWN), UI_NotifProcWifiOn);
			SetNotificationCallback(GetDlgItem(hWnd, ID_REFRESH), UI_NotifProcWifiOn);
			SetNotificationCallback(GetDlgItem(hWnd, ID_CONNECT), UI_NotifProcWifiOn);
			SetNotificationCallback(GetDlgItem(hWnd, ID_WLAN), UI_NotifProcWifiOn);
			SetNotificationCallback(GetDlgItem(hWnd, ID_WLANLIST), UI_NotifProcWifiOn);

			SetWindowBkColor(hWnd, PIXEL_green0);
			SetWindowBkColor(GetDlgItem(hWnd, ID_BACK), PIXEL_green0);
			break;
		case MSG_COMMAND:
			switch(wParam)
			{
				case ID_BACK:
					UI_print("Click ID_BACK.\n");
					emWifiStatus = EM_WLANEXIT;
					SendMessage(hWnd, MSG_CLOSE, 0, 0);
					break;
				default:
					break;
			}
			break;
		case MSG_SETTEXT:
			break;
		case MSG_GETTEXT:
			break;
		case MSG_FONTCHANGED:
			break;
		case MSG_PAINT:
			break;
		case MSG_ERASEBKGND:
			break;
		case MSG_CLOSE:
			EndDialog(hWnd, 0);
			break;
		case MSG_KEYDOWN:
			break;
		case MSG_KEYUP:
			break;
		case MSG_LBUTTONDOWN:
			break;
		case MSG_LBUTTONUP:
			break;
		default:
			break;
	}
	
	return DefaultDialogProc(hWnd, message, wParam, lParam);
}

/*-----------------------------------------------------------------------------
描   述：WLAN ON 时的控件消息回调函数。
返回值：无
-----------------------------------------------------------------------------*/
static void UI_NotifProcWifiOn(HWND hWnd, LINT id, int nc, DWORD add_data)
{
	if(STN_CLICKED != nc)
	{
		UI_print("Message is not STN_CLICKED, return.\n");
		return;
	}

	switch(id)
	{
		case ID_WIFISTATUSTEXT:
			UI_print("Click ID_WIFISTATUSTEXT.\n");
			break;
		case ID_WIFISTATUSBITMAP:
			UI_print("Click ID_WIFISTATUSBITMAP.\n");
			emWifiStatus = EM_WLANOFF;
			SendMessage(GetParent(hWnd), MSG_CLOSE, 0, 0);
			break;
		case ID_WLAN:
		case ID_CONNECT:
			UI_print("Click ID_CONNECT.\n");
			DialogBoxIndirectParam(&DlgTmplWifiConnect, GetParent(hWnd), UI_PorcWifiConnect, 0);
			break;
		case ID_PAGEUP:
			UI_print("Click ID_PAGEUP.\n");
			break;
		case ID_PAGEDOWN:
			UI_print("Click ID_PAGEDOWN.\n");
			break;
		case ID_REFRESH:
			UI_print("Click ID_REFRESH.\n");
			break;
		default:
			UI_print("Click nothing.\n");
			break;
	}
}

// 第三部分，WLAN CONNECT
/***
?*??.--,???????.--,
?*?(?(??\.---./??)?)
?*??'.__/o???o\__.'
?*?????{=??^??=}
?*??????>??-??<
?*?????/???????\
?*????//???????\\
?*???//|???.???|\\
?*???"'\???????/'"_.-~^`'-.
?*??????\??_??/--'?????????`
?*????___)(?)(___
?*???(((__)?(__)))????用于间隔程序模块
?*/


// “完成” 坐标
const static int EnsureSX = 450;
const static int EnsureSY = 30;
const static int EnsureW = 80;
const static int EnsureH = EnsureW;

// SSID 和密码 提示框坐标
const static int SsidTextSX = 40;
const static int SsidTextSY = 20;
const static int SsidTextW = 70;
const static int SsidTextH = 40;
const static int PasswdTextSX = SsidTextSX;
const static int PasswdTextSY = 80;
const static int PasswdTextW = SsidTextW;
const static int PasswdTextH = SsidTextH;

// SSID 和密码对应的输入框坐标
const static int SsidInputSX = 150;
const static int SsidInputSY = SsidTextSY;
const static int SsidInputW = 250;
const static int SsidInputH = SsidTextH;
const static int PasswdInputSX = SsidInputSX;
const static int PasswdInputSY = PasswdTextSY;
const static int PasswdInputW = SsidInputW;
const static int PasswdInputH = SsidTextH;

// 输入框的提示文字
const static int TIPLEN = 16;
const char ssidTip[TIPLEN] = {"请键入SSID"};
const char passwdTip[TIPLEN] = {"请键入密码"};

// 键盘的宽和高
const static int keyboardW = DIALOG_CONNECT_WIDTH - 50;
const static int keyboardH = 150;
const static int keyboardSX = (DIALOG_CONNECT_WIDTH - keyboardW) / 2;
const static int keyboardSY = (DIALOG_CONNECT_HEIGHT - keyboardH) - 25;

// 控件ID
const static int ID_ENSURE = 131;
const static int ID_SSIDTEXT = 131;
const static int ID_PASSWDTEXT = 132;
const static int ID_SSIDINPUT = 133;
const static int ID_PASSWDINPUT = 134;

// SSID 和PASSWD 的字符串
const static size_t STR_SIZE = 32;
static char strSsid[STR_SIZE] = {0};
static char strPasswd[STR_SIZE] = {0};

// 控件列表 WLAN CONNECT
static CTRLDATA CtrlArrWifiConnect[] = {
	// 完成并返回
	{
		CTRL_BUTTON,
		WS_VISIBLE | BS_NOBORDER | BS_TEXT,
		EnsureSX, EnsureSY, EnsureW, EnsureH,
		ID_ENSURE,
		"完成",
		0,
		WS_EX_TRANSPARENT,
	},
	// SSID TEXT
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY,
		SsidTextSX, SsidTextSY, SsidTextW, SsidTextH,
		ID_SSIDTEXT,
		"SSID",
		0,
		WS_EX_TRANSPARENT,
	},
	// PASSWD TEXT
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY,
		PasswdTextSX, PasswdTextSY, PasswdTextW, PasswdTextH,
		ID_PASSWDTEXT,
		"密码",
		0,
		WS_EX_TRANSPARENT,
	},
	// SSID INPUT
	{
		CTRL_SLEDIT,
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_TIP,
		SsidInputSX, SsidInputSY, SsidInputW, SsidInputH,
		ID_SSIDINPUT,
		"",
		0,
		WS_EX_NONE,
	},
	// PASSWD INPUT
	{
		CTRL_SLEDIT,
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD | ES_TIP,
		PasswdInputSX, PasswdInputSY, PasswdInputW, PasswdInputH,
		ID_PASSWDINPUT,
		"",
		0,
		WS_EX_NONE,
	},
};

// 对话框模板 WLAN CONNECT
DLGTEMPLATE DlgTmplWifiConnect = {
	WS_VISIBLE | WS_BORDER,
	WS_EX_TRANSPARENT,
	DIALOG_CONNECT_SX, DIALOG_CONNECT_SY, DIALOG_CONNECT_WIDTH, DIALOG_CONNECT_HEIGHT,
	"",
	0, 0,
	TABLESIZE(CtrlArrWifiConnect), CtrlArrWifiConnect,
};

/*-----------------------------------------------------------------------------
描   述：WLAN CONNECT 时的窗口过程函数
返回值：无
-----------------------------------------------------------------------------*/
#ifndef MG4_0_4
	static LRESULT UI_PorcWifiConnect(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
	static LRESULT UI_PorcWifiConnect(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{
	static HWND hButton[3] = {0};
	
	switch(message)
	{
		case MSG_INITDIALOG:
			SetWindowBkColor(hWnd, PIXEL_green0);
		
			// 为输入框设置提示文字。
			SendMessage(GetDlgItem(hWnd, ID_SSIDINPUT), EM_SETTIPTEXT, TIPLEN, (LPARAM)ssidTip);
			SendMessage(GetDlgItem(hWnd, ID_PASSWDINPUT), EM_SETTIPTEXT, TIPLEN, (LPARAM)passwdTip);
			
			// 为控件设置回调函数。
			SetNotificationCallback(GetDlgItem(hWnd, ID_SSIDINPUT), UI_NotifProcWifiConnect);
			SetNotificationCallback(GetDlgItem(hWnd, ID_PASSWDINPUT), UI_NotifProcWifiConnect);
			
			// 创建键盘。
			hKeyboard = UI_CreateKeyboardFrame(hWnd, ID_KEYBOARD, keyboardSX, keyboardSY, keyboardW, keyboardH);
			if(HWND_INVALID == hKeyboard)
			{
				UI_perror("UI_PorcWifiConnect", "Fail to call UI_CreateKeyboardFrame().\n");
				return -1;
			}
			UI_SetInputMethod(hKeyboard, EM_METHOD_LOWER);
			break;
		case MSG_COMMAND:
			if(ID_ENSURE == wParam)
			{
				UI_print("Click ID_BACK.\n");
				UI_SndSsidPasswd2IPC(0, strSsid, strPasswd);
				emWifiStatus = EM_WLANON;
				SendMessage(hWnd, MSG_CLOSE, 0, 0);
				break;
			}
			else if(isKeyID(wParam))
			{
				int ch = 0;
				int keyID = 0;

				keyID = wParam;
				ch = UI_GetCharFromKeyID(hKeyboard, keyID);
				//printf("key val = %d\n", ch);

				UI_UpdateSsidPasswd(ch);
				UI_ShowSsidPasswd(hWnd);

				break;
			}
			else
			{

			}
			break;
		case MSG_SETTEXT:
			break;
		case MSG_GETTEXT:
			break;
		case MSG_FONTCHANGED:
			break;
		case MSG_PAINT:
			break;
		case MSG_ERASEBKGND:
			break;
		case MSG_CLOSE:
			EndDialog(hWnd, 0);
			break;
		case MSG_KEYDOWN:
			break;
		case MSG_KEYUP:
			break;
		case MSG_LBUTTONDOWN:
			break;
		case MSG_LBUTTONUP:
			break;
		default:
			break;
	}
	
	return DefaultDialogProc(hWnd, message, wParam, lParam);
}

/*-----------------------------------------------------------------------------
描   述：WLAN CONNECT 时的控件消息回调函数。
返回值：无
-----------------------------------------------------------------------------*/
static void UI_NotifProcWifiConnect(HWND hWnd, LINT id, int nc, DWORD add_data)
{
	// printf("nc = %d\n", nc);
	// 很奇怪，对于CTRL_SLEDIT 控件来说，单击也被识别为双击。
	// 切换CTRL_SLEDIT 控件的焦点时，nc 依次取值512, 256, 1
	if(STN_DBLCLK != nc && STN_CLICKED != nc)
	{
		UI_print("Message is not STN_DBLCLK, return.\n");
		return;
	}

	switch(id)
	{
		case ID_SSIDINPUT:
			UI_print("Click ID_SSIDINPUT.\n");
			emInputFocus = EM_FOCUS_SSID;
			break;
		case ID_PASSWDINPUT:
			UI_print("Click ID_PASSWDINPUT.\n");
			emInputFocus = EM_FOCUS_PASSWD;
			break;
		default:
			UI_print("Click nothing.\n");
			break;
	}
}

/*-----------------------------------------------------------------------------
描   述：更新ssid 和passwd
参	数：ch  为ssid 或passwd 新追加的字符。
返回值：返回0
注	意：
-----------------------------------------------------------------------------*/
static int UI_UpdateSsidPasswd(char ch)
{
	switch(ch)
	{
		case EM_ASCII_CAPSLOCK:
			UI_print("Click ID_KEY_CAPSLOCK.\n");
			UI_SetInputMethod(hKeyboard, EM_METHOD_CAPSLOCK);
			return 0;
			break;
		case EM_ASCII_SYMBOOL:
			UI_print("Click ID_KEY_SYMBOOL.\n");
			UI_SetInputMethod(hKeyboard, EM_METHOD_SYMBOOL);
			return 0;
			break;
		case EM_ASCII_MAX:
			UI_print("Click EM_ASCII_MAX.\n");
			return 0;
			break;
		default:
			printf("keyVal = %d, ASCII = %c\n", ch, ch);
			break;
	}

	char *p = NULL;
	if(EM_FOCUS_PASSWD == emInputFocus)
	{
		//UI_print("EM_FOCUS_PASSWD == emInputFocus\n");
		p = strPasswd;
	}
	else if(EM_FOCUS_SSID == emInputFocus)
	{
		//UI_print("EM_FOCUS_SSID == emInputFocus\n");
		p = strSsid;
	}

	UI_AppendChar2String(p, STR_SIZE, ch);
	printf("After call UI_AppendChar2String(), str = %s\n", p);
	
	return 0;
}

/*-----------------------------------------------------------------------------
描   述：在单行编辑框中显示ssid 和passwd
参	数：hparent 编辑框所在的父窗口句柄。
返回值：返回0
注	意：
-----------------------------------------------------------------------------*/
static int UI_ShowSsidPasswd(HWND hParent)
{
	if(EM_FOCUS_SSID ==  emInputFocus)
	{
		SetWindowText(GetDlgItem(hParent, ID_SSIDINPUT), strSsid);
	}
	else if(EM_FOCUS_PASSWD ==  emInputFocus)
	{
		SetWindowText(GetDlgItem(hParent, ID_PASSWDINPUT), strPasswd);
	}

	return 0;
}

/*-----------------------------------------------------------------------------
描   述：在Wlan 设置界面加载资源文件
参	数：无
返回值：返回0
注	意：
-----------------------------------------------------------------------------*/
static int UI_LoadResourceInWlan(void)
{
	if(ERR_BMP_OK != LoadBitmapFromFile(HDC_SCREEN, pBitmapWlanOn, bitmapWlanOnPath))
	{
		UI_perror("UI_LoadResourceInBoot", "Fail to call LoadBitmapFromFile()");
	}

	if(ERR_BMP_OK != LoadBitmapFromFile(HDC_SCREEN, pBitmapWlanOff, bitmapWlanOffPath))
	{
		UI_perror("UI_LoadResourceInBoot", "Fail to call LoadBitmapFromFile()");
	}

	if(ERR_BMP_OK != LoadBitmapFromFile(HDC_SCREEN, pBitmapRefresh, bitmapRefreshPath))
	{
		UI_perror("UI_LoadResourceInBoot", "Fail to call LoadBitmapFromFile()");
	}

	if(ERR_BMP_OK != LoadBitmapFromFile(HDC_SCREEN, pBitmapWLAN, bitmapWLANPath))
	{
		UI_perror("UI_LoadResourceInBoot", "Fail to call LoadBitmapFromFile()");
	}
	
	return 0;
}

/*-----------------------------------------------------------------------------
描   述：在Wlan 设置界面卸载资源文件
参	数：无
返回值：返回0
注	意：
-----------------------------------------------------------------------------*/
static int UI_UnloadResourceInWlan(void)
{
	UnloadBitmap(pBitmapWlanOn);
	UnloadBitmap(pBitmapWlanOff);
	UnloadBitmap(pBitmapRefresh);
	UnloadBitmap(pBitmapWLAN);
	
	return 0;
}

/*-----------------------------------------------------------------------------
描   述：将SSID 和PASSWD 发送给IPC
参	数：ipcIndex 目标IPC 的坐标。
返回值：成功，返回调用结果，含义未知；失败，返回，-1
注	意：
-----------------------------------------------------------------------------*/
static int UI_SndSsidPasswd2IPC(int ipcIndex, const char *ssid, const char *passwd)
{	
	if(NULL == ssid || NULL == passwd)
	{
		UI_perror("UI_SndSsidPasswd2IPC", "invalid argument, pointer has null value!.\n");
		return -1;
	}

	int bufSize = 128;
	char buf[bufSize] = {0};
	const char *format = "SetWifiLink SSID_%s PWD_%s";		// 消息格式由yume 制定的协议规定。

	// 打包命令缓冲区
	sprintf(buf, format, ssid, passwd);
	printf("_cmd = %s\n", buf);

	int ret = 0;
	auto pM = CIpcManager::GetInstance();
	ret = pM->SendCmdToIpc(ipcIndex, "SetWifiLink SSID_xxx PWD_yyy");

	return ret;
}

