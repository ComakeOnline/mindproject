#include "icon_settime.hpp"
#include <stdlib.h>

//确认、返回按钮的坐标
const static int backSX = 620;
const static int backSY = 350;
const static int backW = 80;
const static int backH = 50;
const static int ensureSX = 0;
const static int ensureSY = backSY;
const static int ensureW = backW;
const static int ensureH = backH;

//背景图片坐标
const static int bitmapStaticW = DIALOG_WIDTH;
const static int bitmapStaticH = DIALOG_HEIGHT;
const static int bitmapStaticStartX = 0;
const static int bitmapStaticStartY = 0;

//控件ID
const static int ID_BACK = 100;
const static int ID_ENSURE = 101;
const static int ID_TITLESTATIC = 102;
const static int ID_DELSTATIC = 103;
const static int ID_SPACESTATIC = 104;
const static int ID_BITMAPSTATIC = 105;

//资源文件
static BITMAP bitmapIPCManager = {0};
PBITMAP const pBitmapIPCManager = &bitmapIPCManager;
const char *bitmapIPCManagerPath = "./res/IPCManager.png";

static int UI_LoadResourceInIpcManager(void);
static int UI_UnloadResourceInIpcManager(void);
#ifndef MG4_0_4
static LRESULT UI_PorcIconIPCManager(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_PorcIconIPCManager(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

//控件容器
static CTRLDATA CtrlArr[] = {
	//文本框：图片
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_BITMAP,
		bitmapStaticStartX, bitmapStaticStartY, bitmapStaticW, bitmapStaticH,
		ID_BITMAPSTATIC,
		"",
		(DWORD)pBitmapIPCManager,
		WS_EX_TRANSPARENT,
	},	
	//确定和返回
	{
		CTRL_BUTTON,
		WS_VISIBLE | WS_CHILD | BS_NOTIFY,
		ensureSX, ensureSY, ensureW, ensureH,
		ID_ENSURE,
		"重启",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE | WS_CHILD | BS_NOTIFY,
		backSX, backSY, backW, backH,
		ID_BACK,
		"X",
		0,
		WS_EX_NONE,
	},
};

//对话框模板
static DLGTEMPLATE DlgTemplate = {
	WS_VISIBLE,
	WS_EX_NONE,
	DIALOG_STARTX, DIALOG_STARTY, DIALOG_WIDTH, DIALOG_HEIGHT,
	"",
	0, 0,
	TABLESIZE(CtrlArr), CtrlArr,
};

//在上级UI中调用
int UI_OnIconIPCManager(int val)
{
	printf("val = %d\n", val);

	DialogBoxIndirectParam(&DlgTemplate, hIconScreen, UI_PorcIconIPCManager, 0);
	
	return 0;
}

//对话框过程函数
#ifndef MG4_0_4
static LRESULT UI_PorcIconIPCManager(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_PorcIconIPCManager(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{
	static int i = 0;
	static int ret = 0;
	static HDC hDC = HDC_INVALID;
	
	switch(message)
	{
		case MSG_PAINT:
			break;
		case MSG_CREATE:
			break;
		case MSG_INITDIALOG:
			UI_LoadResourceInIpcManager();
			 
			SetWindowElementAttr(GetDlgItem(hWnd, ID_ENSURE), WE_FGC_THREED_BODY, (DWORD)0x000000);
			SetWindowBkColor(GetDlgItem(hWnd, ID_ENSURE), PIXEL_red0);
			SetWindowFont(GetDlgItem(hWnd, ID_ENSURE), pLogFont25);

			SetWindowElementAttr(GetDlgItem(hWnd, ID_BACK), WE_FGC_THREED_BODY, (DWORD)0x000000);
			SetWindowBkColor(GetDlgItem(hWnd, ID_BACK), PIXEL_green0);
			SetWindowFont(GetDlgItem(hWnd, ID_BACK), pLogFont25);
			break;
		case MSG_COMMAND:
			switch(wParam)
			{
				case ID_BACK:
					PostMessage(hWnd, MSG_CLOSE, 0, 0);
					break;
				case ID_ENSURE:
					//PostMessage(hWnd, MSG_CLOSE, 0, 0);
					system("reboot -f");
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
		case MSG_ERASEBKGND:
			break;
		case MSG_CLOSE:
			EndDialog(hWnd, 0);
			UI_UnloadResourceInIpcManager();
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

//加载资源
static int UI_LoadResourceInIpcManager(void)
{
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapIPCManager, bitmapIPCManagerPath))
	{
		UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile()");
	}

	return 0;
}

//卸载资源
static int UI_UnloadResourceInIpcManager(void)
{
	UnloadBitmap(pBitmapIPCManager);
	return 0;
}

