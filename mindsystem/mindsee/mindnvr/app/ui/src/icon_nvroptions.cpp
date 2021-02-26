#include "icon_nvroptions.hpp"
#include "nvr_etc.hpp"
#include <unistd.h>
#include <stdlib.h>

// 确认、返回按钮的坐标
const static int backSX = 620;
const static int backSY = 350;
const static int backW = 80;
const static int backH = 50;
const static int ensureSX = 0;
const static int ensureSY = backSY;
const static int ensureW = backW;
const static int ensureH = backH;

// 人形检测、人性弹窗、语音识别、网络状况 的文字坐标
const static int TextSX = 60;
const static int TextSY = 50;
const static int TextW = 120;
const static int TextH = 35;
const static int TextIntervalY = TextH + 30;

const static int hcDetectTextSX = TextSX;
const static int hcDetectTextSY = TextSY;
const static int hcDetectTextW = TextW;
const static int hcDetectTextH = TextH;
const static int hcOSDTextSX = TextSX;
const static int hcOSDTextSY = TextSY + TextIntervalY;
const static int hcOSDTextW = TextW;
const static int hcOSDTextH = TextH;
const static int ASRTextSX = TextSX;
const static int ASRTextSY = TextSY + TextIntervalY * 2;
const static int ASRTextW = TextW;
const static int ASRTextH = TextH;
const static int netStatusTextSX = TextSX;
const static int netStatusTextSY = TextSY + TextIntervalY * 3;
const static int netStatusTextW = TextW;
const static int netStatusTextH = TextH;

// 人形检测、人性弹窗、语音识别、网络状况 的图片坐标
const static int BitmapSX = TextSX + TextW + 40;
const static int BitmapSY = TextSY;
const static int BitmapW = 120;
const static int BitmapH = TextH;
const static int BitmapIntervalY = TextIntervalY;

const static int hcDetectBitmapSX = BitmapSX;
const static int hcDetectBitmapSY = BitmapSY;
const static int hcDetectBitmapW = BitmapW;
const static int hcDetectBitmapH = BitmapH;
const static int hcOSDBitmapSX = BitmapSX;
const static int hcOSDBitmapSY = BitmapSY + BitmapIntervalY;
const static int hcOSDBitmapW = BitmapW;
const static int hcOSDBitmapH = BitmapH;
const static int ASRBitmapSX = BitmapSX;
const static int ASRBitmapSY = BitmapSY + BitmapIntervalY * 2;
const static int ASRBitmapW = BitmapW;
const static int ASRBitmapH = BitmapH;
const static int netStatusBitmapSX = BitmapSX;
const static int netStatusBitmapSY = BitmapSY + BitmapIntervalY * 3;
const static int netStatusBitmapW = BitmapW;
const static int netStatusBitmapH = BitmapH;

// 控件ID
const static int ID_BACK = 100;
const static int ID_Ensure = 101;
const static int ID_HCDETECT_TEXT = 102;
const static int ID_HCOSD_TEXT = 103;
const static int ID_ASR_TEXT = 104;
const static int ID_NET_TEXT = 105;
const static int ID_HCDETECT_BITMAP = 106;
const static int ID_HCOSD_BITMAP = 107;
const static int ID_ASR_BITMAP = 108;
const static int ID_NET_BITMAP = 109;

// 资源文件
static BITMAP bitmapNvrOptionsBk = {0};
static PBITMAP const pBitmapNvrOptionsBk = &bitmapNvrOptionsBk;
static const char *bitmapNvrOptionsBkPath = "./res/nvroptions.png";
static BITMAP bitmapOn = {0};
PBITMAP const pBitmapOn = &bitmapOn;
const char *bitmapOnPath = "./res/icon_on.png";
static BITMAP bitmapOff = {0};
PBITMAP const pBitmapOff = &bitmapOff;
const char *bitmapOffPath = "./res/icon_off.png";

static int UI_LoadResourceInNvrOptions(void);
static int UI_UnloadResourceInNvrOptions(void);
static void UI_NotifProcBitmap(HWND hWnd, LINT id, int nc, DWORD add_data);
#ifndef MG4_0_4
static LRESULT UI_PorcIconNvrOptions(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_PorcIconNvrOptions(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

// 控件
static CTRLDATA CtrlArr[] = {
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER,
		hcDetectTextSX, hcDetectTextSY, hcDetectTextW, hcDetectTextH,
		ID_HCDETECT_TEXT,
		"打开人形检测",
		0,
		WS_EX_TRANSPARENT,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER,
		hcOSDTextSX, hcOSDTextSY, hcOSDTextW, hcOSDTextH,
		ID_HCOSD_TEXT,
		"人形检测弹窗",
		0,
		WS_EX_TRANSPARENT,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER,
		ASRTextSX, ASRTextSY, ASRTextW, ASRTextH,
		ID_ASR_TEXT,
		"打开语音识别",
		0,
		WS_EX_TRANSPARENT,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER,
		netStatusTextSX, netStatusTextSY, netStatusTextW, netStatusTextH,
		ID_NET_TEXT,
		"显示丢包率",
		0,
		WS_EX_TRANSPARENT,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY | SS_BITMAP,
		hcDetectBitmapSX, hcDetectBitmapSY, hcDetectBitmapW, hcDetectBitmapH,
		ID_HCDETECT_BITMAP,
		"人形检测",
		(DWORD)pBitmapOn,
		WS_EX_TRANSPARENT,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY | SS_BITMAP,
		hcOSDBitmapSX, hcOSDBitmapSY, hcOSDBitmapW, hcOSDBitmapH,
		ID_HCOSD_BITMAP,
		"人形弹窗",
		(DWORD)pBitmapOn,
		WS_EX_TRANSPARENT,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY | SS_BITMAP,
		ASRBitmapSX, ASRBitmapSY, ASRBitmapW, ASRBitmapH,
		ID_ASR_BITMAP,
		"语音识别",
		(DWORD)pBitmapOn,
		WS_EX_TRANSPARENT,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | SS_CENTER | SS_NOTIFY | SS_BITMAP,
		netStatusBitmapSX, netStatusBitmapSY, netStatusBitmapW, netStatusBitmapH,
		ID_NET_BITMAP,
		"网络状况",
		(DWORD)pBitmapOn,
		WS_EX_TRANSPARENT,
	},
};

// 对话框模板
static DLGTEMPLATE DlgTemplate = {
	WS_VISIBLE,
	WS_EX_NONE,
	DIALOG_STARTX, DIALOG_STARTY, DIALOG_WIDTH, DIALOG_HEIGHT,
	"",
	0, 0,
	TABLESIZE(CtrlArr), CtrlArr,
};

/*-----------------------------------------------------------------------------
描   述：NVR 选项菜单
参	数：val 无意义
返回值：返回0
注	意：
-----------------------------------------------------------------------------*/
int UI_OnIconNvrOptions(int val)
{
	printf("val = %d\n", val);

	DialogBoxIndirectParam(&DlgTemplate, hIconScreen, UI_PorcIconNvrOptions, 0);
	
	return 0;
}

/*-----------------------------------------------------------------------------
描   述：对话框过程函数
参	数：
返回值：
注	意：
-----------------------------------------------------------------------------*/
#ifndef MG4_0_4
static LRESULT UI_PorcIconNvrOptions(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_PorcIconNvrOptions(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{	
	switch(message)
	{
		case MSG_PAINT:
			HDC hDC;
			hDC = BeginPaint(hWnd);
			FillBoxWithBitmap(hDC, 0, 0, DIALOG_WIDTH, DIALOG_HEIGHT, pBitmapNvrOptionsBk);
			EndPaint(hWnd, hDC);
			break;
		case MSG_INITDIALOG:
			UI_LoadResourceInNvrOptions();

			// 设置图片控件的回调函数。
			SetNotificationCallback(GetDlgItem(hWnd, ID_HCDETECT_BITMAP), UI_NotifProcBitmap);
			SetNotificationCallback(GetDlgItem(hWnd, ID_HCOSD_BITMAP), UI_NotifProcBitmap);
			SetNotificationCallback(GetDlgItem(hWnd, ID_ASR_BITMAP), UI_NotifProcBitmap);
			SetNotificationCallback(GetDlgItem(hWnd, ID_NET_BITMAP), UI_NotifProcBitmap);

			// 设置提示框字体。
			SetWindowFont(GetDlgItem(hWnd, ID_HCDETECT_TEXT), pLogFont20);
			SetWindowFont(GetDlgItem(hWnd, ID_HCOSD_TEXT), pLogFont20);
			SetWindowFont(GetDlgItem(hWnd, ID_ASR_TEXT), pLogFont20);
			SetWindowFont(GetDlgItem(hWnd, ID_NET_TEXT), pLogFont20);

			// 设置图片为on 或off.
			SendMessage(GetDlgItem(hWnd, ID_HCDETECT_BITMAP), STM_SETIMAGE, (WPARAM)(UI_HcDetectEnabled() ? pBitmapOn : pBitmapOff), 0);
			SendMessage(GetDlgItem(hWnd, ID_HCOSD_BITMAP), STM_SETIMAGE, (WPARAM)(UI_HcOSDEnabled() ? pBitmapOn : pBitmapOff), 0);
			SendMessage(GetDlgItem(hWnd, ID_ASR_BITMAP), STM_SETIMAGE, (WPARAM)(UI_ASREnabled() ? pBitmapOn : pBitmapOff), 0);
			SendMessage(GetDlgItem(hWnd, ID_NET_BITMAP), STM_SETIMAGE, (WPARAM)(UI_NetStatusDispEnabled() ? pBitmapOn : pBitmapOff), 0);
			
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
			UI_UnloadResourceInNvrOptions();
			break;
		case MSG_KEYDOWN:
			break;
		case MSG_KEYUP:
			break;
		case MSG_LBUTTONDOWN:
			break;
		case MSG_LBUTTONUP:
			//坐标计算与触摸动作处理
			UI_print("MSG_LBUTTONUP.\n");
			if(UI_GetTouchX(lParam) > ensureSX && UI_GetTouchX(lParam) < (ensureSX + ensureW) 
				&& UI_GetTouchY(lParam) > ensureSY && UI_GetTouchY(lParam) < (ensureSY + ensureH))
			{
				PostMessage(hWnd, MSG_CLOSE, wParam, lParam);
			}
			else if(UI_GetTouchX(lParam) > backSX && UI_GetTouchX(lParam) < (backSX + backW) 
				&& UI_GetTouchY(lParam) > backSY && UI_GetTouchY(lParam) < (backSY + backH))
			{
				PostMessage(hWnd, MSG_CLOSE, wParam, lParam);
			}

			//printf("x = %d, y = %d\n", UI_GetTouchX(lParam), UI_GetTouchY(lParam));
			break;
		default:
			break;
	}
	
	return DefaultDialogProc(hWnd, message, wParam, lParam);
}

/*-----------------------------------------------------------------------------
描   述：加载资源文件
参	数：
返回值：
注	意：
-----------------------------------------------------------------------------*/
static int UI_LoadResourceInNvrOptions(void)
{
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapOn, bitmapOnPath))
	{
		UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile()");
	}

	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapOff, bitmapOffPath))
	{
		UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile()");
	}

	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapNvrOptionsBk, bitmapNvrOptionsBkPath))
	{
		UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile()");
	}
	
	return 0;
}

/*-----------------------------------------------------------------------------
描   述：卸载资源文件
参	数：
返回值：
注	意：
-----------------------------------------------------------------------------*/
static int UI_UnloadResourceInNvrOptions(void)
{
	UnloadBitmap(pBitmapOn);
	UnloadBitmap(pBitmapOff);
	UnloadBitmap(pBitmapNvrOptionsBk);
	
	return 0;
}

/*-----------------------------------------------------------------------------
描   述：控件的消息回调函数
参	数：
返回值：
注	意：
-----------------------------------------------------------------------------*/
static void UI_NotifProcBitmap(HWND hWnd, LINT id, int nc, DWORD add_data)
{
	if(STN_CLICKED != nc)
	{
		UI_print("Message is not STN_CLICKED, return.\n");
		return;
	}

	switch(id)
	{
		case ID_HCDETECT_BITMAP:
		{
			UI_print("Click ID_HCDETECT_BITMAP.\n");

			UI_InvertHcDetect();
			SendMessage(hWnd, STM_SETIMAGE, (WPARAM)(UI_HcDetectEnabled() ? pBitmapOn : pBitmapOff), 0);

			break;
		}
		case ID_HCOSD_BITMAP:
			UI_print("Click ID_HCOSD_BITMAP.\n");
			
			UI_InvertHcOSD();
			SendMessage(hWnd, STM_SETIMAGE, (WPARAM)(UI_HcOSDEnabled() ? pBitmapOn : pBitmapOff), 0);
			
			break;
		case ID_ASR_BITMAP:
			UI_print("Click ID_ASR_BITMAP.\n");
			
			UI_InvertASR();
			SendMessage(hWnd, STM_SETIMAGE, (WPARAM)(UI_ASREnabled() ? pBitmapOn : pBitmapOff), 0);
			
			break;
		case ID_NET_BITMAP:
			UI_print("Click ID_NET_BITMAP.\n");
			
			UI_InvertNetStatusDisp();
			SendMessage(hWnd, STM_SETIMAGE, (WPARAM)(UI_NetStatusDispEnabled() ? pBitmapOn : pBitmapOff), 0);
			if(UI_ASREnabled())
			{
				// openASR();
			}
			else
			{
				// closeASR();
			}
			
			break;
		default:
			UI_print("Click nothing.\n");
			break;
	}
}

