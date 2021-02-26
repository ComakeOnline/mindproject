/*
单画面，显示一个IPC 图像。
*/

#include "minigui_singleframe.hpp"
#include "minigui_entry.hpp"
#include "minigui_resource.hpp"
#include "minigui_talkingsetting.hpp"
#include "nvr_etc.hpp"
#include "router.hpp"
#include "message.hpp"
#include "appinterface.h"
#include "wifilink.h"
#include "ipcmanager.hpp"

#include <stdlib.h>
#include <unistd.h>

static bool UI_UpdateNetInfo(CMessage &msg, void *userdata);
static void UI_ProcStaticNotif(HWND hWnd, LINT id, int nc, DWORD add_data);
#ifndef MG4_0_4
static LRESULT UI_ProcSigleFrame(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
static LRESULT UI_DlgTalkingSettingProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_ProcSigleFrame(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT UI_DlgTalkingSettingProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif
static HWND UI_ShowRssi(HWND hParent, unsigned int id, PLOGFONT pLogFont, DWORD index);
static HWND UI_ShowNetStatus(HWND hParent, unsigned int id, PLOGFONT pLogFont, DWORD rgbColor);

// 图标坐标
const static int iconSY = 450;
const static int iconW = 100;
const static int iconH = iconW;
const static int iconIntervalX = 1024 / 5;

// 通话，设置，返回按钮坐标
const static int settingSX = RESOLUTION_WIDTH / 2 - iconW / 2;
const static int callSX = settingSX - iconIntervalX;
const static int backSX = settingSX + iconIntervalX;

// "有人活动" 坐标
const static int OSDSX = 800;
const static int OSDSY = 100;
const static int OSDW = 200;
const static int OSDH = 80;

// 网络状态 坐标
const static int netStatusSX = RESOLUTION_WIDTH / 2 + 50;
const static int netStatusSY = 20;
const static int netStatusW = RESOLUTION_WIDTH - netStatusSX;
const static int netStatusH = 40;
const static int rssiSX = netStatusSX;
const static int rssiSY = netStatusSY + netStatusH + 5;
const static int rssiW = netStatusW;
const static int rssiH = netStatusH * 2;

// 控件ID
const static int ID_CALL = 100;
const static int ID_SETTING = 101;
const static int ID_BACK = 102;
const static int ID_OSD = 103;
const static int ID_NETSTATUS = 104;
const static int ID_RSSI = 104;

// 资源文件
static BITMAP bitmapCall = {0};
PBITMAP const pBitmapCall = &bitmapCall;
const char *bitmapCallPath = "./res/call.png";

static BITMAP bitmapHang = {0};
PBITMAP const pBitmapHang = &bitmapHang;
const char *bitmapHangPath = "./res/hang.png";

static BITMAP bitmapSetting = {0};
PBITMAP const pBitmapSetting = &bitmapSetting;
const char *bitmapSettingPath = "./res/setting.png";

static BITMAP bitmapBackIcon = {0};
PBITMAP const pBitmapBackIcon = &bitmapBackIcon;
const char *bitmapBackIconPath = "./res/backicon.png";

// 通话标志位
bool isTalking = false;
extern bool inSingleFrame;
extern bool inFourFrame;
extern bool inFilting;
extern bool inOSD;

// 控件句柄
static HWND hRssi = HWND_INVALID;
static HWND hNetStatus = HWND_INVALID;

/*-----------------------------------------------------------------------------
描   述：显示单个画面IPC 图像。
参	数：index 为IPC 标号。
返回值：返回0
注	意：
-----------------------------------------------------------------------------*/
int UI_CreateSingleFrameWnd(int index)
{
	MSG msg = {0};

	MAINWINCREATE wndCreate = {
		WS_VISIBLE, 								//仅可见，不要边框和标题栏
		WS_EX_NONE, 								//无扩展风格
		"Used for single frame.",					//标题栏。此处不显示
		0,											//主窗口菜单的句柄。此处无意义
		GetSystemCursor(0), 0,						//使用默认光标，无图标。
		HWND_DESKTOP,								//托管窗口设为DESKTOP
		UI_ProcSigleFrame,							//窗口过程函数
		0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT,	//lx, ty, rx, by
		(int)(gal_pixel)COLOR_blue,							//蓝色背景
		(DWORD)index, 0,									//额外的私有数据为IPC 的标号。
	};

	hFrameWnd = CreateMainWindow(&wndCreate);
	if(HWND_INVALID == hFrameWnd)
	{
		UI_perror("UI_MainScreen", "fail to call CreateMainWindow.\n");
		return 0;
	}

	ShowWindow(hFrameWnd, true);
	while(GetMessage(&msg, hMainScreen))				//主窗口进入消息循环模式
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

/*-----------------------------------------------------------------------------
描   述：窗口过程函数
参	数：
返回值：
注	意：
-----------------------------------------------------------------------------*/
#ifndef MG4_0_4
static LRESULT UI_ProcSigleFrame(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_ProcSigleFrame(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{
	static int ret = -1;
	static HDC hDC = HDC_INVALID;
	static HWND hOSD = HWND_INVALID;
	static HWND hBack = HWND_INVALID;
	static HWND hCall = HWND_INVALID;
	static HWND hSetting = HWND_INVALID;
	
	switch(message)
	{
		case MSG_CREATE:
			hFrameWnd = hWnd;
			inFilting = true;
			
			// 注册网络信息的获取函数。
			{
				static bool subscribeOnce = true;
				if(subscribeOnce)
				{
					// UI_print("Call R->Use(MSG_TYPE, LAMBDA).\n");
					subscribeOnce = false;
					auto R = Router::GetInstance();
					R->Use(CMessage::E_MSG_VIDEO_NET_MESSAGE, UI_UpdateNetInfo);
				}
			}

			UI_print("Call LoadBitmapFromFile().\n");
			//连续加载资源文件
			if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapCall, bitmapCallPath))
			{
				UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile().\n");
			}

			if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapHang, bitmapHangPath))
			{
				UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile().\n");
			}

			if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapSetting, bitmapSettingPath))
			{
				UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile().\n");
			}

			if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapBackIcon, bitmapBackIconPath))
			{
				UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile().\n");
			}

			//创建3个静态框放图片文件。
			hCall = CreateWindowEx(CTRL_STATIC, "h", WS_VISIBLE | WS_CHILD | SS_NOTIFY | SS_BITMAP, WS_EX_TRANSPARENT, \
				ID_CALL, callSX, iconSY, iconW, iconH, hFrameWnd, (DWORD)pBitmapCall);
			SetNotificationCallback(hCall, UI_ProcStaticNotif);

			hSetting = CreateWindowEx(CTRL_STATIC, "h", WS_VISIBLE | WS_CHILD | SS_NOTIFY | SS_BITMAP, WS_EX_TRANSPARENT, \
				ID_SETTING, settingSX, iconSY, iconW, iconH, hFrameWnd, (DWORD)pBitmapSetting);
			SetNotificationCallback(hSetting, UI_ProcStaticNotif);

			hBack = CreateWindowEx(CTRL_STATIC, "h", WS_VISIBLE | WS_CHILD | SS_NOTIFY | SS_BITMAP, WS_EX_TRANSPARENT, \
				ID_BACK, backSX, iconSY, iconW, iconH, hFrameWnd, (DWORD)pBitmapBackIcon);
			SetNotificationCallback(hBack, UI_ProcStaticNotif);
			
			if(UI_NetStatusDispEnabled())
			{
				hNetStatus = UI_ShowNetStatus(hWnd, ID_NETSTATUS, pLogFont25, GetWindowAdditionalData(hWnd));
				hRssi = UI_ShowRssi(hWnd, ID_RSSI, pLogFont25, GetWindowAdditionalData(hWnd));
			}

			// 如果是检测到人形后的OSD 弹窗，则执行如下部分。
			if(inOSD)
			{
				hOSD = CreateWindowEx(CTRL_STATIC, "有人活动", WS_VISIBLE | SS_SIMPLE | SS_CENTER,	WS_EX_TRANSPARENT, \
					ID_OSD, OSDSX, OSDSY, OSDW, OSDH, hWnd, 0);
				SetWindowElementAttr(GetDlgItem(hWnd, ID_OSD), WE_FGC_WINDOW, (DWORD)0x0000ff);
				SetWindowFont(GetDlgItem(hWnd, ID_OSD), pLogFont50);
			}
			
			break;
		case MSG_CLOSE:
		{			
			//销毁资源文件
			UI_print("Call UnloadBitmap().\n");
			UnloadBitmap(pBitmapCall);
			UnloadBitmap(pBitmapHang);
			UnloadBitmap(pBitmapSetting);
			UnloadBitmap(pBitmapBackIcon);

			//销毁句柄
			DestroyWindow (hOSD);
			hOSD = HWND_INVALID;
			hCall = HWND_INVALID;
			hBack = HWND_INVALID;
			hSetting = HWND_INVALID;
			hFrameWnd = HWND_INVALID;

			// 销毁主窗口
			DestroyMainWindow(hWnd);
			inFilting = true;
			inSingleFrame = false;
			inFourFrame = true;
			inOSD = false;
			
			break;
		}
		case MSG_COMMAND:
			switch(wParam)
			{
				case ID_BACK:
				break;
			}
			break;
		case TBN_CHANGE:
			break;
		case MSG_ERASEBKGND:
			break;
		case MSG_LBUTTONDOWN:
			break;
		case MSG_LBUTTONUP:
			break;
		case MSG_IDLE:
		{
			break;
		}
		default:
			break;
	}

	return DefaultWindowProc(hWnd, message, wParam, lParam);
}

/*-----------------------------------------------------------------------------
描   述：退出单画面模式
参	数：
返回值：
注	意：
-----------------------------------------------------------------------------*/
int UI_DestroySingleFrameWnd()
{
	// 关闭网络状态更新。
	//AppInterface *pAppInterface = AppInterface::GetInstance();
	//pAppInterface->CloseNetInformation(pAppInterface->GetIPCIndex());

	//刷新为小画面
	AppInterface::GetInstance()->RefreshToMainPage();
	
	//如果正在通话，则停止通话（停止已经停止的通话，似乎会导致程序奔溃）
	if(isTalking)
	{
		isTalking = false;
		AppInterface::GetInstance()->StopAllTalk();
		AppInterface::GetInstance()->StopAllAudio();
	}
	//inSingleFrame = false;

	return 0;
}

/*-----------------------------------------------------------------------------
描   述：控件的消息回调函数
参	数：
返回值：
注	意：
-----------------------------------------------------------------------------*/
static void UI_ProcStaticNotif(HWND hWnd, LINT id, int nc, DWORD add_data)
{
	HDC hDC = HDC_INVALID;
	
	UI_print("ID_STATIC clicked.\n");

	//只处理单击动作，其它消息一律返回。
	if(nc != STN_CLICKED)
	{
		UI_print("nc != STN_CLICKED.\n");
		return;
	}

	if(ID_CALL == id)
	{
		UI_print("id == id_call.\n");
		AppInterface *pAppInterface = AppInterface::GetInstance();

		if(inOSD)
		{
			inOSD = false;
		}
		
		if(isTalking)		// 如果正在通话，则显示通话图标，关闭通话。
		{
			hDC = GetDC(hWnd);
			FillBoxWithBitmap(hDC, 0, 0, iconW, iconH, pBitmapCall);
			ReleaseDC(hDC);
			hDC = HDC_INVALID;
			UI_print("Call StopAllTalk().\n");
			pAppInterface->StopAllTalk();
			pAppInterface->StopAllAudio();
			UI_print("End Call StopAllTalk().\n");
		}
		else					// 否则如果没有通话，则显示通话图标，关闭通话。
		{
			hDC = GetDC(hWnd);
			FillBoxWithBitmap(hDC, 0, 0, iconW, iconH, pBitmapHang);
			ReleaseDC(hDC);
			hDC = HDC_INVALID;
			
			UI_print("Ready to call PlayTalk().\n");
			int index = 0;
			index = pAppInterface->GetIPCIndex();		
			if(INDEX_INVALID == index)
			{
				UI_perror("UI_ProcCallNotif()", "Fail to call UI_ProcCallNotif(), invalid result.\n");
			}
			else
			{
				pAppInterface->PlayTalk(index);
				pAppInterface->PlayAudio(index);
				UI_print("End to call PlayTalk().\n");
			}
		}

		isTalking = !isTalking;
	}
	else if(ID_BACK == id)
	{
		UI_print("id == ID_BACK.\n");
		// 退出单画面，销毁窗口（先后顺序不能错）
		UI_DestroySingleFrameWnd();
		PostMessage(hFrameWnd, MSG_CLOSE, 0, 0);
	}
	else if(ID_SETTING == id)
	{
		UI_print("id == ID_SETTING.\n");
		UI_SettingInTalking(GetParent(hWnd));
		if(inOSD)
		{
			inOSD = false;
		}
	}
	else
	{
		UI_print("id == others.\n");
	}

	return;
}

/*-----------------------------------------------------------------------------
描   述：在单界面下显示网络状态信息
参	数：hParent 父窗口句柄；id 新增控件的ID; pLogFont 控件字体；index IPC的标号。
返回值：返回控件的句柄。
注	意：
-----------------------------------------------------------------------------*/
static HWND UI_ShowNetStatus(HWND hParent, unsigned int id, PLOGFONT pLogFont, DWORD index)
{
	if(HWND_INVALID == hParent || NULL == pLogFont)
	{
		UI_perror("UI_ShowNetStatus", "invalid argument!\n");
		return HWND_INVALID;
	}

	// UI_print("Call OpenNetInformation().\n");
	//AppInterface *pAppInterface = AppInterface::GetInstance();
	//pAppInterface->OpenNetInformation(index);

	HWND hStatic = HWND_INVALID;

	hStatic = CreateWindowEx(CTRL_STATIC, "码流:---kb/s  丢包率: --- %", WS_VISIBLE | SS_SIMPLE | SS_LEFT,	WS_EX_TRANSPARENT, \
		id, netStatusSX, netStatusSY, netStatusW, netStatusH, hParent, 0);
	SetWindowElementAttr(hStatic, WE_FGC_WINDOW, 0xe00000);			// 蓝色字体0xee0000
	SetWindowFont(hStatic, pLogFont25);
	
	return hStatic;
}

/*-----------------------------------------------------------------------------
描   述：在单界面下显示信号强度RSSI
参	数：hParent 父窗口句柄；id 新增控件的ID; pLogFont 控件字体；index IPC的标号。
返回值：返回控件的句柄。
注	意：
-----------------------------------------------------------------------------*/
static HWND UI_ShowRssi(HWND hParent, unsigned int id, PLOGFONT pLogFont, DWORD index)
{
	if(HWND_INVALID == hParent || NULL == pLogFont)
	{
		UI_perror("UI_ShowRssi", "invalid argument!\n");
		return HWND_INVALID;
	}

	HWND hStatic = HWND_INVALID;

	hStatic = CreateWindowEx(CTRL_STATIC, "信号强度：--dB (开发阶段)", WS_VISIBLE | SS_SIMPLE | SS_LEFT,	WS_EX_TRANSPARENT, \
		id, rssiSX, rssiSY, rssiW, rssiH, hParent, 0);
	SetWindowElementAttr(hStatic, WE_FGC_WINDOW, 0xe00000);			// 蓝色字体0xee0000
	SetWindowFont(hStatic, pLogFont25);
	
	return hStatic;
}

/*-----------------------------------------------------------------------------
描   述：更新网络状态信息
参	数：参考router.hpp 中的如下函数：
		Router::Use(CMessage::MSG_TYPE index, std::function<bool(CMessage&, void*)> func)
返回值：成功，返回true; 失败，返回false.
注	意：
-----------------------------------------------------------------------------*/
static bool UI_UpdateNetInfo(CMessage &msg, void *userdata)
{
	// msg的参数1 2 3 4分别带别"收到的包个数"、"未收到的包个数"、"收到的字节数"、"IPC 的通道号"

	// 如果UI中选中的IPC 和消息传来的IPC 通道号不一致，则直接返回。
	auto pAppInterface = AppInterface::GetInstance();
	if(pAppInterface->GetIPCIndex() != msg.param_4)
	{
		UI_perror("UI_UpdateNetInfo", "IPC index != CMessage::param_4.\n");
		return false;
	}

	// 网络信息E_MSG_VIDEO_NET_MESSAGE 每隔3 秒刷新一次。
	// 根据收到的"收到的包个数"、"未收到的包个数"、"收到的字节数" 来计算"码率"、"丢包率"
	double msgIntervalTime = 3.0;
	unsigned int rcvPacketNum = msg.param_1;
	unsigned int unrcvPacketNum = msg.param_2;
	unsigned int rcvBytes = msg.param_3;

	// 计算码率：收到的字节数 / 秒数 / 1024 = kb/s.
	// 计算丢包率：未收到的包 / (收到的包 + 未收到的包) * 100 = %.
	double netSpeed = 0.0;
	double packetLossRate = 0.0;
	netSpeed = rcvBytes / msgIntervalTime / 1024;
	if(0 != rcvPacketNum + unrcvPacketNum)	// 确保分母不为0
	{
		packetLossRate = (double)unrcvPacketNum / (rcvPacketNum + unrcvPacketNum) * 100;
	}
	else
	{
		UI_perror("UI_UpdateNetInfo", "Fatal: Denominator cannot be zero!");
	}
	printf("[UI] RcvBytes, RcvPacketNum, UnRcvPacketNum = %d, %d, %d\n", rcvBytes, rcvPacketNum, unrcvPacketNum);

	// 将计算结果转化为文本信息。
	const unsigned textLen = 128;		// 文本长度不会超过128 字节。
	char text[textLen] = {0};
	// (汉字5个 + 冒号2个 + 5位有效数字 * 2 + 小数点2个 + 空格1个 + 单位5) * UTF最大字节数
	// = 25 * 4 = 100
	const char textFormat[textLen] = "码流:%.2fkb/s 丢包率:%.2f%%";
	snprintf(text, textLen, textFormat, netSpeed, packetLossRate);

	// 更新网络状态控件
	if(HWND_INVALID != hNetStatus)
	{
		SetWindowText(hNetStatus, text);
	}

	// 信号强度RSSI
	WifiRssi wifiRssiData = {0};
	int ret = GetWifiRssi(&wifiRssiData);
	if(-1 == ret)
	{
		UI_perror("UI_UpdateNetInfo", "Fail to call GetWifiRssi().\n");
		return false;
	}
	printf("RSSI = %d\n", wifiRssiData.aprssi);

	int validRssiCnt = 0;		// 有效RSSI 数据的个数，RSSI != 0 则认为有效。
	for(int i = 0; i < 8; i++)
	{
		if(0 != wifiRssiData.starssi[i].wext_rssi)
		{
			validRssiCnt = i + 1;
		}
	
		printf("STASSI[%d] RSSI = %ddB", i, wifiRssiData.starssi[i].wext_rssi);
		const unsigned char *const p = wifiRssiData.starssi[i].wext_mac;
		printf(", ");
		printf("MAC = %X:%X:%X:%X:%X:%X\n", p[0], p[1], p[2], p[3], p[4], p[5]);
	}

	// 将多个ssid 值拼接为字符串格式。
	snprintf(text, textLen, "%s", "信号强度:");
	const unsigned numTextLen = 16;	// 字符串长度最大长度为8，"-100dB "
	char numText[numTextLen] = {0};
	for(int i = 0; i < validRssiCnt; i++)
	{
		snprintf(numText, numTextLen, "%ddB ", wifiRssiData.starssi[i].wext_rssi);
		strcat(text, numText);
	}
#if 0
	for(int i = 0; i < 4; i++)
	{
		snprintf(numText, numTextLen, "%ddB ", -53);
		strcat(text, numText);
	}
#endif
	
	strcat(text, "/b(开发阶段)");
	UI_print(text);

	// 前文设置textLen 为128 字节，要显示的字符串最大长度为："信号强度：-100dB -100dB -100dB -100dB"
	// 汉字5个 * 3字节 + 空格3个 * 1字节 + 数字字母字母24个 * 1字节 = 42字节。因此128 字节足够了。

	// 更新RSSI控件
	if(HWND_INVALID != hRssi)
	{
		SetWindowText(hRssi, text);
	}
	
	return false;
}
