#include "icon_sysinfo.hpp"

//确认、返回按钮的坐标
//const static int backSX = 620;
//const static int backSY = 350;
//const static int backW = 80;
//const static int backH = 50;
const static int ensureSX = 30;
const static int ensureSY = 180;
const static int ensureW = 40;
const static int ensureH = 180;

// 四个信息框的坐标
const static int msgIntervalX = 240;
const static int msgIntervalY = 145;
const static int msgW = 130;
const static int msgH = 100;
const static int msg0SX = 230;
const static int msg0SY = 100;
const static int msg0W = msgW;
const static int msg0H = msgH;
const static int msg1SX = msg0SX + msgIntervalX;
const static int msg1SY = msg0SY;
const static int msg1W = msgW;
const static int msg1H = msgH;
const static int msg2SX = msg0SX ;
const static int msg2SY = msg0SY + msgIntervalY;
const static int msg2W = msgW;
const static int msg2H = msgH;
const static int msg3SX = msg0SX + msgIntervalX;
const static int msg3SY = msg0SY + msgIntervalY;
const static int msg3W = msgW;
const static int msg3H = msgH;

//背景图片的坐标
const static int bitmapStaticW = DIALOG_WIDTH;
const static int bitmapStaticH = DIALOG_HEIGHT;
const static int bitmapStaticStartX = 0;
const static int bitmapStaticStartY = 0;

//控件ID
const static int ID_BACK = 100;
const static int ID_Ensure = 101;
const static int ID_BITMAPSTATIC = 102;
const static int ID_MSG0 = 110;
const static int ID_MSG1 = 111;
const static int ID_MSG2 = 112;
const static int ID_MSG3 = 113;

//资源文件
static BITMAP bitmapSysInfo = {0};
PBITMAP const pBitmapSysInfo = &bitmapSysInfo;
const char *bitmapSysInfoPath = "./res/sysinfo.png";

static int UI_LoadResourceInSysInfo(void);
static int UI_UnloadResourceInSysInfo(void);
#ifndef MG4_0_4
static LRESULT UI_PorcIconSysInfo(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_PorcIconSysInfo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

//控件容器
static CTRLDATA CtrlArr[] = {
	//确定和返回
	{
		CTRL_BUTTON,
		WS_VISIBLE | WS_CHILD | BS_NOTIFY,
		ensureSX, ensureSY, ensureW, ensureH,
		ID_Ensure,
		"确认",
		0,
		WS_EX_TRANSPARENT,
	},
	//文本框：图片
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_BITMAP,
		bitmapStaticStartX, bitmapStaticStartY, bitmapStaticW, bitmapStaticH,
		ID_BITMAPSTATIC,
		"背景图片",
		(DWORD)pBitmapSysInfo,
		WS_EX_TRANSPARENT,
	},
	//四个信息框之0
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_CENTER,
		msg0SX, msg0SY, msg0W, msg0H,
		ID_MSG0,
		"0",
		0,
		WS_EX_TRANSPARENT,
	},
	//四个信息框之1
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_CENTER,
		msg1SX, msg1SY, msg1W, msg1H,
		ID_MSG1,
		"1",
		0,
		WS_EX_TRANSPARENT,
	},
	//四个信息框之2
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_CENTER,
		msg2SX, msg2SY, msg2W, msg2H,
		ID_MSG2,
		"2",
		0,
		WS_EX_TRANSPARENT,
	},
	//四个信息框之3
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_CENTER,
		msg3SX, msg3SY, msg3W, msg3H,
		ID_MSG3,
		"3",
		0,
		WS_EX_TRANSPARENT,
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

/*-----------------------------------------------------------------------------
描   述：系统信息显示。
参	数：val 无意义。
返回值：返回0
注	意：该函数在上层菜单中调用。
-----------------------------------------------------------------------------*/
int UI_OnIconSysInfo(int val)
{
	printf("val = %d\n", val);

	DialogBoxIndirectParam(&DlgTemplate, hIconScreen, UI_PorcIconSysInfo, 0);
	
	return 0;
}

/*-----------------------------------------------------------------------------
描   述：对话框过程函数
参	数：
返回值：
注	意：
-----------------------------------------------------------------------------*/
#ifndef MG4_0_4
static LRESULT UI_PorcIconSysInfo(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_PorcIconSysInfo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{
	static int i = 0;
	static int ret = 0;
	static HDC hDC = HDC_INVALID;
	
	switch(message)
	{
		case MSG_PAINT:
			break;
		case MSG_INITDIALOG:
			UI_LoadResourceInSysInfo();
			PostMessage(hWnd, MSG_SETMSGTEXT, 0, 0);
			break;
		case MSG_COMMAND:
			switch(wParam)
			{
				case ID_BACK:
					PostMessage(hWnd, MSG_CLOSE, 0, 0);
					break;
				case ID_Ensure:
					PostMessage(hWnd, MSG_CLOSE, 0, 0);
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
			UI_UnloadResourceInSysInfo();
			break;
		case MSG_KEYDOWN:
			break;
		case MSG_KEYUP:
			break;
		case MSG_LBUTTONDOWN:
			break;
		case MSG_LBUTTONUP:
			break;
		case MSG_SETMSGTEXT:
			// 准备版本信息相关的字符串。
			printf("[VERSION]Git Version: %s\n", GIT_VERSION);
			printf("[VERSION]Git PATH: %s\n", GIT_PATH);
			printf("[VERSION]build time: %s\n", __DATE__ ", " __TIME__);
			
			SetWindowText(GetDlgItem(hWnd, ID_MSG0), "");
			SetWindowText(GetDlgItem(hWnd, ID_MSG1), "系统版本:\n" GIT_VERSION);
			SetWindowText(GetDlgItem(hWnd, ID_MSG2), "GIT地址:\n" GIT_PATH);
			SetWindowText(GetDlgItem(hWnd, ID_MSG3), "发布日期:\n" __DATE__ "\n" __TIME__);

			SetWindowFont(GetDlgItem(hWnd, ID_MSG0), pLogFont15);
			SetWindowFont(GetDlgItem(hWnd, ID_MSG1), pLogFont15);
			SetWindowFont(GetDlgItem(hWnd, ID_MSG3), pLogFont15);
			SetWindowFont(GetDlgItem(hWnd, ID_MSG2), pLogFont15);

			SetWindowElementAttr(GetDlgItem(hWnd, ID_MSG0), WE_FGC_WINDOW, (DWORD)(PIXEL_yellow));
			SetWindowElementAttr(GetDlgItem(hWnd, ID_MSG1), WE_FGC_WINDOW, (DWORD)(PIXEL_yellow));
			SetWindowElementAttr(GetDlgItem(hWnd, ID_MSG2), WE_FGC_WINDOW, (DWORD)(PIXEL_yellow));
			SetWindowElementAttr(GetDlgItem(hWnd, ID_MSG3), WE_FGC_WINDOW, (DWORD)(PIXEL_yellow));
			break;
		case MSG_IDLE:
			break;
		default:
			break;
	}
	
	return DefaultDialogProc(hWnd, message, wParam, lParam);
}

/*-----------------------------------------------------------------------------
描   述：加载资源文件
参	数：无
返回值：返回0
注	意：
-----------------------------------------------------------------------------*/
static int UI_LoadResourceInSysInfo(void)
{
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapSysInfo, bitmapSysInfoPath))
	{
		UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile()");
	}

	return 0;
}

/*-----------------------------------------------------------------------------
描   述：卸载资源文件
参	数：无
返回值：返回0
注	意：
-----------------------------------------------------------------------------*/
static int UI_UnloadResourceInSysInfo(void)
{
	UnloadBitmap(pBitmapSysInfo);
	return 0;
}

