#include "icon_setting.hpp"

//确认、返回按钮的坐标
const static int backSX = 620;
const static int backSY = 350;
const static int backW = 80;
const static int backH = 50;
const static int ensureSX = DIALOG_WIDTH - backW * 2;
const static int ensureSY = backSY;
const static int ensureW = backW;
const static int ensureH = backH;

//垂直、水平按钮的坐标
const static int buttonW = 90;
const static int buttonH = 40;
const static int vButtonSX = 20;
const static int vButtonSY = 40;
const static int vButton0SY = vButtonSY;
const static int vButton1SY = vButtonSY + buttonH * 1;
const static int vButton2SY = vButtonSY + buttonH * 2;
const static int vButton3SY = vButtonSY + buttonH * 3;
const static int vButton4SY = vButtonSY + buttonH * 4;
const static int vButton5SY = vButtonSY + buttonH * 5;
const static int vButton6SY = vButtonSY + buttonH * 6;
const static int sButtonSX = 150;
const static int sButtonSY = 40;
const static int sButton0SX = sButtonSX;
const static int sButton1SX = sButtonSX + buttonW;
const static int sButton2SX = sButtonSX + buttonW * 2;
const static int sButton3SX = sButtonSX + buttonW * 3;

//文字、开关图片-文本框的坐标
const static int textW = buttonW + 10;
const static int textH = 30;
const static int text0SX = sButtonSX;
const static int text0SY = 115;
const static int text1SX = text0SX;
const static int text1SY = text0SY + 82;
const static int text2SX = text0SX;
const static int text2SY = text1SY + 82;
const static int text3SX = text0SX + textW * 2;
const static int text3SY = text2SY;
const static int text4SX = text0SX;
const static int text4SY = text2SY + textH;
//开关图片
const static int text4W = 110;
const static int text4H = 30;
const static int text5SX = text0SX + textW * 2;
const static int text5SY = text4SY;
const static int text5W = text4W;
const static int text5H = text4H;

//音量-滑动控件的坐标
const static int trackbarW = 400;
const static int trackbarH = 40;
const static int trackbarSX = sButtonSX;
const static int trackbar0SY = 150;
const static int trackbar1SY = 230;

//控件ID
const static int ID_BACK = 100;
const static int ID_ENSURE = 101;
const static int ID_VBUTTON0 = 103;
const static int ID_VBUTTON1 = 104;
const static int ID_VBUTTON2 = 105;
const static int ID_VBUTTON3 = 106;
const static int ID_VBUTTON4 = 107;
const static int ID_VBUTTON5 = 108;
const static int ID_VBUTTON6 = 109;
const static int ID_SBUTTON0 = 110;
const static int ID_SBUTTON1 = 111;
const static int ID_SBUTTON2 = 112;
const static int ID_SBUTTON3 = 113;
const static int ID_TEXT0 = 116;
const static int ID_TEXT1 = 117;
const static int ID_TEXT2 = 118;
const static int ID_TEXT3 = 119;
const static int ID_TEXT4 = 120;
const static int ID_TEXT5 = 121;
const static int ID_TRACKBAR0 = 121;
const static int ID_TRACKBAR1 = 121;

//资源文件
static BITMAP bitmapAudioOn = {0};
PBITMAP const pBitmapAudioOn = &bitmapAudioOn;
const char *bitmapAudioOnPath = "./res/icon_on.png";

static BITMAP bitmapAudioOff = {0};
PBITMAP const pBitmapAudioOff = &bitmapAudioOff;
const char *bitmapAudioOffPath = "./res/icon_off.png";

static int UI_LoadResourceInSetting(void);
static int UI_UnloadResourceInSetting(void);
#ifndef MG4_0_4
static LRESULT UI_PorcIconSetting(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_PorcIconSetting(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

static CTRLDATA CtrlArr[] = {
	//垂直的7个按钮
	{
		CTRL_BUTTON,
		WS_VISIBLE | BS_NOBORDER,
		vButtonSX, vButton0SY, buttonW, buttonH,
		ID_VBUTTON0,
		"声音设置",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE | BS_NOBORDER,
		vButtonSX, vButton1SY, buttonW, buttonH,
		ID_VBUTTON1,
		"免扰设置",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE,
		vButtonSX, vButton2SY, buttonW, buttonH,
		ID_VBUTTON2,
		"报警设置",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE,
		vButtonSX, vButton3SY, buttonW, buttonH,
		ID_VBUTTON3,
		"模式设置",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE,
		vButtonSX, vButton4SY, buttonW, buttonH,
		ID_VBUTTON4,
		"转移设置",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE,
		vButtonSX, vButton5SY, buttonW, buttonH,
		ID_VBUTTON5,
		"通用设置",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE,
		vButtonSX, vButton6SY, buttonW, buttonH,
		ID_VBUTTON6,
		"本机设置",
		0,
		WS_EX_NONE,
	},
	//水平的四个button
	{
		CTRL_BUTTON,
		WS_VISIBLE,
		sButton0SX, sButtonSY, buttonW, buttonH,
		ID_SBUTTON0,
		"对讲设置",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE,
		sButton1SX, sButtonSY, buttonW, buttonH,
		ID_SBUTTON1,
		"铃声设置",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE,
		sButton2SX, sButtonSY, buttonW, buttonH,
		ID_SBUTTON2,
		"报警设置",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE,
		sButton3SX, sButtonSY, buttonW, buttonH,
		ID_SBUTTON3,
		"其它",
		0,
		WS_EX_NONE,
	},
	//静态框
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_SIMPLE | SS_CENTER,
		text0SX, text0SY, textW, textH,
		ID_TEXT0,
		"麦克风音量",
		0,
		WS_EX_TRANSPARENT,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_SIMPLE | SS_CENTER,
		text1SX, text1SY, textW, textH,
		ID_TEXT1,
		"扬声器音量",
		0,
		WS_EX_TRANSPARENT,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_SIMPLE | SS_CENTER,
		text2SX, text2SY, textW, textH,
		ID_TEXT2,
		"麦克风静音",
		0,
		WS_EX_TRANSPARENT,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_SIMPLE | SS_CENTER,
		text3SX, text3SY, textW, textH,
		ID_TEXT3,
		"扬声器静音",
		0,
		WS_EX_TRANSPARENT,
	},
	//滑块
	{
		CTRL_TRACKBAR,
		WS_CHILD | WS_VISIBLE | TBS_NOTIFY | TBS_TIP,
		trackbarSX, trackbar0SY, trackbarW, trackbarH,
		ID_TRACKBAR0,
		"",
		0,
		WS_EX_TRANSPARENT,
		0,
	},
	{
		CTRL_TRACKBAR,
		WS_CHILD | WS_VISIBLE | TBS_NOTIFY | TBS_TIP | TBS_FOCUS,
		trackbarSX, trackbar1SY, trackbarW, trackbarH,
		ID_TRACKBAR1,
		"",
		0,
		WS_EX_TRANSPARENT,
		0,
	},
	//静态框带图片
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_SIMPLE | SS_CENTER | SS_BITMAP,
		text4SX, text4SY, text4W, text4H,
		ID_TEXT4,
		"",
		(DWORD)pBitmapAudioOn,
		WS_EX_TRANSPARENT,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_SIMPLE | SS_CENTER | SS_BITMAP,
		text5SX, text5SY, text5W, text5H,
		ID_TEXT5,
		"",
		(DWORD)pBitmapAudioOn,
		WS_EX_TRANSPARENT,
	},
	//确认和返回按钮
	{
		CTRL_BUTTON,
		WS_VISIBLE | BS_BITMAP,
		backSX, backSY, backW, backH,
		ID_BACK,
		"",
		(DWORD)pBitmapBack,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE | BS_BITMAP,
		ensureSX, ensureSY, ensureW, ensureH,
		ID_ENSURE,
		"",
		(DWORD)pBitmapEnsure,
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

//在上级菜单中调用
int UI_OnIconSetting(int val)
{
	printf("val = %d\n", val);

	DialogBoxIndirectParam(&DlgTemplate, hIconScreen, UI_PorcIconSetting, 0);
	
	return 0;
}

//对话框过程函数
#ifndef MG4_0_4
static LRESULT UI_PorcIconSetting(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_PorcIconSetting(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{
	static int i = 0;
	
	switch(message)
	{
		case MSG_INITDIALOG:
			SetWindowBkColor(hWnd, 0X000FFF);
			for(i = ID_VBUTTON0; i < ID_SBUTTON3 + 1; i++)
			{
				SetWindowBkColor(GetDlgItem(hWnd, i), 0x003fff);
			}
			UI_LoadResourceInSetting();
			break;
		case MSG_COMMAND:
			switch(wParam)
			{
				case ID_BACK:
					PostMessage(hWnd, MSG_CLOSE, 0, 0);
					break;
				case ID_ENSURE:
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
		case MSG_PAINT:
			break;
		case MSG_ERASEBKGND:
			break;
		case MSG_CLOSE:
			EndDialog(hWnd, 0);
			UI_UnloadResourceInSetting();
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

//加载资源文件
static int UI_LoadResourceInSetting(void)
{
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapAudioOn, bitmapAudioOnPath))
	{
		UI_perror("UI_LoadResourceInSetting", "Fail to call LoadBitmapFromFile()");
	}

	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapAudioOff, bitmapAudioOffPath))
	{
		UI_perror("UI_LoadResourceInSetting", "Fail to call LoadBitmapFromFile()");
	}

	return 0;
}


//卸载资源文件
static int UI_UnloadResourceInSetting(void)
{
	UnloadBitmap(pBitmapAudioOn);
	UnloadBitmap(pBitmapAudioOff);

	return 0;
}

