#include "icon_settime.hpp"

//确认、返回按钮的坐标
const static int backSX = 620;
const static int backSY = 350;
const static int backW = 80;
const static int backH = 50;
const static int ensureSX = 0;
const static int ensureSY = backSY;
const static int ensureW = backW;
const static int ensureH = backH;

//控件ID
const static int ID_BACK = 100;
const static int ID_Ensure = 101;

//资源文件
static BITMAP bitmapSetNetBk = {0};
PBITMAP const pBitmapSetNetBk = &bitmapSetNetBk;
const char *bitmapSetNetBkPath = "./res/setnet.png";

static int UI_LoadResourceInSetnet(void);
static int UI_UnloadResourceInSetnet(void);

#ifndef MG4_0_4
static LRESULT UI_PorcIconSetNet(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_PorcIconSetNet(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

//控件
static CTRLDATA CtrlArr[] = {
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

//在上层UI中调用
int UI_OnIconSetNet(int val)
{
	printf("val = %d\n", val);

	DialogBoxIndirectParam(&DlgTemplate, hIconScreen, UI_PorcIconSetNet, 0);
	
	return 0;
}

//对话框过程函数
#ifndef MG4_0_4
static LRESULT UI_PorcIconSetNet(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_PorcIconSetNet(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{	
	switch(message)
	{
		case MSG_PAINT:
			HDC hDC;
			RECT rect;
			hDC = BeginPaint(hWnd);
			FillBoxWithBitmap(hDC, 0, 0, DIALOG_WIDTH, DIALOG_HEIGHT, pBitmapSetNetBk);
			EndPaint(hWnd, hDC);
			break;
		case MSG_CREATE:
			break;
		case MSG_INITDIALOG:
			UI_LoadResourceInSetnet();
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
			UI_UnloadResourceInSetnet();
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

			printf("x = %d, y = %d\n", UI_GetTouchX(lParam), UI_GetTouchY(lParam));
			break;
		default:
			break;
	}
	
	return DefaultDialogProc(hWnd, message, wParam, lParam);
}

//加载资源文件
static int UI_LoadResourceInSetnet(void)
{
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapSetNetBk, bitmapSetNetBkPath))
	{
		UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile()");
	}

	return 0;
}

//卸载资源文件
static int UI_UnloadResourceInSetnet(void)
{
	UnloadBitmap(pBitmapSetNetBk);
	return 0;
}

