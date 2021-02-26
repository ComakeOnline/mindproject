#include "icon_videolist.hpp"

//确认、返回按钮的坐标
const static int backSX = 560;
const static int backSY = 350;
const static int backW = 80;
const static int backH = 50;
const static int ensureSX = 170;
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
const static int ID_Ensure = 101;
const static int ID_TITLESTATIC = 102;
const static int ID_DELSTATIC = 103;
const static int ID_SPACESTATIC = 104;
const static int ID_BITMAPSTATIC = 105;

//资源文件
static BITMAP bitmapVideoList = {0};
PBITMAP const pBitmapVideoList = &bitmapVideoList;
const char *bitmapVideoListPath = "./res/videolist.png";

static int UI_LoadResourceInVideoList(void);
static int UI_UnloadResourceInVideoList(void);
#ifndef MG4_0_4
static LRESULT UI_PorcIconVideoList(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_PorcIconVideoList(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

static CTRLDATA CtrlArr[] = {
	//确定和返回
	{
		CTRL_BUTTON,
		WS_VISIBLE | WS_CHILD | BS_NOTIFY,
		ensureSX, ensureSY, ensureW, ensureH,
		ID_Ensure,
		"",
		0,
		WS_EX_TRANSPARENT,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE | WS_CHILD | BS_NOTIFY,
		backSX, backSY, backW, backH,
		ID_BACK,
		"",
		0,
		WS_EX_TRANSPARENT,
	},
	//文本框：图片
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_BITMAP,
		bitmapStaticStartX, bitmapStaticStartY, bitmapStaticW, bitmapStaticH,
		ID_BITMAPSTATIC,
		"视频数据：42GB\n缩略图：512MB\n可用空间：21GB\n",
		(DWORD)pBitmapVideoList,
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

//在上级UI中调用
int UI_OnIconVideoList(int val)
{
	printf("val = %d\n", val);

	DialogBoxIndirectParam(&DlgTemplate, hIconScreen, UI_PorcIconVideoList, 0);
	
	return 0;
}

//对话框过程函数
#ifndef MG4_0_4
static LRESULT UI_PorcIconVideoList(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_PorcIconVideoList(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			UI_LoadResourceInVideoList();
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
			UI_UnloadResourceInVideoList();
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
static int UI_LoadResourceInVideoList(void)
{
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapVideoList, bitmapVideoListPath))
	{
		UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile()");
	}

	return 0;
}

//卸载资源文件
static int UI_UnloadResourceInVideoList(void)
{
	UnloadBitmap(pBitmapVideoList);
	
	return 0;
}

