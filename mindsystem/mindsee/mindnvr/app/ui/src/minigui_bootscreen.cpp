/*
开机界面
*/

#include "minigui_bootscreen.hpp"

// 开机logo 坐标
static int bootLogoW = RESOLUTION_WIDTH;
static int bootLogoH = RESOLUTION_HEIGHT;
static int bootLogoSX = 0;
static int bootLogoSY = 0;
// 倒计时坐标
static int countDownW = 100;
static int countDownH = 100;
static int countDownSX = RESOLUTION_WIDTH / 2 - countDownW / 2;
static int countDownSY = 365;

// 资源文件
static BITMAP bitmapLogo = {0};
PBITMAP const pBitmapLogo = &bitmapLogo;
const char *bitmapLogoPath = "./res/bootlogo.png";

// 控件ID
static const int ID_BOOTLOGO = 31;
static const int ID_COUNTDOWN = 32;

// 函数声明
static int UI_LoadResourceInBoot(void);
static int UI_UnloadResourceInBoot(void);
static int UI_ShowTimeInBoot(HWND hParent, HWND hCountDown);
#ifndef MG4_0_4
static LRESULT UI_ProcBootScreen(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_ProcBootScreen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

/*-----------------------------------------------------------------------------
描   述：主窗口的过程函数。处理触摸消息，短按切换IPC 画面，长按打开UI 滑动窗口。
返回值：
-----------------------------------------------------------------------------*/
#ifndef MG4_0_4
static LRESULT UI_ProcBootScreen(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_ProcBootScreen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{	
	switch(message)
	{
		case MSG_CREATE:
			UI_LoadResourceInBoot();

			// logo 图标
			static HWND hStaticBoot = HWND_INVALID;
			hStaticBoot = CreateWindow(CTRL_STATIC, "", WS_VISIBLE | SS_BITMAP, ID_BOOTLOGO, \
				bootLogoSX, bootLogoSY, bootLogoW, bootLogoH, hWnd, (DWORD)pBitmapLogo);

			// 倒数计时图标
			static HWND hCountDown = HWND_INVALID;
			hCountDown = CreateWindowEx(CTRL_STATIC, "10", WS_VISIBLE | SS_CENTER, WS_EX_TRANSPARENT, \
				ID_COUNTDOWN, countDownSX, countDownSY, countDownW, countDownH, hWnd, 0);
			SetWindowFont(hCountDown, pLogFont50);
			SetWindowElementAttr(hCountDown, WE_FGC_WINDOW, (DWORD)UI_PIXEL2RGB(PIXEL_lightwhite));
				
			break;
		case MSG_CLOSE:
			UI_print("MSG_CLOSE.\n");
			UI_UnloadResourceInBoot();
			DestroyMainWindow(hWnd);
			PostQuitMessage(hWnd);
			break;
		case MSG_COMMAND:
			switch(wParam)
			{
				break;
			}
			break;
		case MSG_ERASEBKGND:
			break;
		case MSG_LBUTTONDOWN:
			break;
		case MSG_LBUTTONUP:
			if(UI_GetTouchX(lParam) < 100 && UI_GetTouchY(lParam) < 100)
			{
				PostMessage(hWnd, MSG_CLOSE, 0, 0);
			}
			break;
		case MSG_IDLE:
			UI_ShowTimeInBoot(hWnd, hCountDown);
			break;
		default:
			break;
	}

	return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

/*-----------------------------------------------------------------------------
描   述：对外接口函数，调用后显示开机画面。
返回值：返回0.
-----------------------------------------------------------------------------*/
int UI_BootScreen()
{
	MSG msg;

	MAINWINCREATE mainWinInf = {
		WS_VISIBLE,									//仅可见，不要边框和标题栏
		WS_EX_NONE, 								//无扩展风格
		"This main window is used for boot screen.",	//标题栏。此处不显示
		0,											//主窗口菜单的句柄。此处无意义
		GetSystemCursor(0), 0,						//使用默认光标，无图标。
		HWND_DESKTOP,								//托管窗口设为DESKTOP
		UI_ProcBootScreen,							//窗口过程函数
		0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT,	//lx, ty, rx, by
		(int)(gal_pixel)COLOR_lightgray,						//蓝色背景
		0, 0,										//无额外数据
	};

	HWND hBootScreen = HWND_INVALID;
	hBootScreen = CreateMainWindow(&mainWinInf);	//创建主窗口，返回窗口句柄
	if(HWND_INVALID == hBootScreen)
	{
		UI_perror("UI_MainScreen", "fail to call CreateMainWindow.\n");
		return 0;
	}

	ShowWindow(hBootScreen, SW_SHOWNORMAL);

	while(GetMessage(&msg, hBootScreen))			//主窗口进入消息循环模式
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}	
	MainWindowCleanup(hBootScreen);
	hBootScreen = HWND_INVALID;

	return 0;
}

/*-----------------------------------------------------------------------------
描   述：加载资源文件。
返回值：返回0.
-----------------------------------------------------------------------------*/
static int UI_LoadResourceInBoot(void)
{
	if(ERR_BMP_OK != LoadBitmapFromFile(HDC_SCREEN, pBitmapLogo, bitmapLogoPath))
	{
		UI_perror("UI_LoadResourceInBoot", "Fail to call LoadBitmapFromFile()");
	}
	
	return 0;
}

/*-----------------------------------------------------------------------------
描   述：卸载资源文件。
返回值：返回0.
-----------------------------------------------------------------------------*/
static int UI_UnloadResourceInBoot(void)
{
	UnloadBitmap(pBitmapLogo);

	return 0;
}

/*-----------------------------------------------------------------------------
描   述：在bootscreen 界面显示倒数计时。
返回值：返回0.
-----------------------------------------------------------------------------*/
static int UI_ShowTimeInBoot(HWND hParent, HWND hCountDown)
{
	static int cnt = 0;
	static int countDown = 2;	// 倒数计时的秒数。

	//UI_print("Call UI_ShowTimeInBoot().\n");

	// 每隔一秒，倒数计时-1.
	if(++cnt == (int)timeCnt1000ms)
	{
		cnt = 0;

		const unsigned bufSize = 8;
		char buf[bufSize] = {0};
		sprintf(buf, "%2d", --countDown);
		
		SetWindowText(hCountDown, buf);
	}

	// 倒数计时结束后发送MSG_CLOSE 消息，关闭logoscreen 界面。
	if(0 == countDown)
	{
		PostMessage(hParent, MSG_CLOSE, 0, 0);
	}

	return 0;
}
