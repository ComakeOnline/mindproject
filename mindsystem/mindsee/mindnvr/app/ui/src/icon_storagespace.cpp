#include "icon_storagespace.hpp"

//确认、返回按钮的坐标
const static int backSX = 200;
const static int backSY = 280;
const static int backW = 400;
const static int backH = 40;
const static int ensureSX = backSX;
const static int ensureSY = backSY + backH + 15;
const static int ensureW = backW;
const static int ensureH = backH;

//控件ID
const static int ID_BACK = 100;
const static int ID_ENSURE = 101;
const static int ID_TITLESTATIC = 102;
const static int ID_DELSTATIC = 103;
const static int ID_SPACESTATIC = 104;
const static int ID_BITMAPSTATIC = 105;
const static int ID_YEARINC = 106;
const static int ID_MONINC = 107;
const static int ID_DAYINC = 108;
const static int ID_HOURINC = 109;
const static int ID_MININC = 110;
const static int ID_YEARDEC = 111;
const static int ID_MONDEC = 112;
const static int ID_DAYDEC = 113;
const static int ID_HOURDEC = 114;
const static int ID_MINDEC = 115;

//标题
const static int TitleStaticW = 100;
const static int TitleStaticH = DIALOG_WIDTH;
const static int TitleStaticStartX = 0;
const static int TitleStaticStartY = 0;

//提示"删除过久视频"
const static int DelStaticW = 500;
const static int DelStaticH = 35;
const static int DelStaticStartX = 150;
const static int DelStaticStartY = 220;

//剩余空间统计
const static int SpaceStaticW = 230;
const static int SpaceStaticH = 100;
const static int SpaceStaticStartX = 380;
const static int SpaceStaticStartY = 70;

//图片坐标
const static int bitmapStaticW = 150;
const static int bitmapStaticH = bitmapStaticW;
const static int bitmapStaticStartX = 200;
const static int bitmapStaticStartY = 45;

//资源文件
static BITMAP bitmapSpace = {0};
PBITMAP const pBitmapSpace = &bitmapSpace;
const char *bitmapSpacePath = "./res/space.png";

static int UI_LoadResourceInStorage(void);
static int UI_UnloadResourceInStorage(void);
#ifndef MG4_0_4
static LRESULT UI_PorcIconStorageSpace(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_PorcIconStorageSpace(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

//控件容器
static CTRLDATA CtrlArr[] = {
	//确定和返回
	{
		CTRL_BUTTON,
		WS_VISIBLE | WS_CHILD | BS_NOTIFY,
		ensureSX, ensureSY, ensureW, ensureH,
		ID_ENSURE,
		"立即清理",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE | WS_CHILD | BS_NOTIFY,
		backSX, backSY, backW, backH,
		ID_BACK,
		"放 弃 清 理",
		0,
		WS_EX_NONE,
	},
	//文本框：存储空间
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_CENTER,
		TitleStaticStartX, TitleStaticStartY, TitleStaticW, TitleStaticH,
		ID_TITLESTATIC,
		"\n\n\n\n\n存\n储\n空\n间\n",
		0,
		WS_EX_NONE,
	},
	//文本框：提示删除视频
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_CENTER,
		DelStaticStartX, DelStaticStartY, DelStaticW, DelStaticH,
		ID_DELSTATIC,
		"删除时间过久的视屏以释放存储空间",
		0,
		WS_EX_TRANSPARENT,
	},
	//文本框：剩余空间
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_CENTER,
		SpaceStaticStartX, SpaceStaticStartY, SpaceStaticW, SpaceStaticH,
		ID_SPACESTATIC,
		"视频数据：42GB\n缩略图：512MB\n可用空间：21GB\n",
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
		(DWORD)pBitmapSpace,
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
int UI_OnIconStorageSpace(int val)
{
	printf("val = %d\n", val);

	DialogBoxIndirectParam(&DlgTemplate, hIconScreen, UI_PorcIconStorageSpace, 0);
	
	return 0;
}

//窗口过程函数
#ifndef MG4_0_4
static LRESULT UI_PorcIconStorageSpace(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_PorcIconStorageSpace(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			UI_LoadResourceInStorage();
			
			SetWindowBkColor(hWnd, PIXEL_green0);
			SetWindowElementAttr(GetDlgItem(hWnd, ID_TITLESTATIC), WE_FGC_WINDOW, (DWORD)0xffffff);
			SetWindowBkColor(GetDlgItem(hWnd, ID_TITLESTATIC), PIXEL_blue0);
			SetWindowFont(GetDlgItem(hWnd, ID_TITLESTATIC), pLogFont25);
			
			SetWindowElementAttr(GetDlgItem(hWnd, ID_ENSURE), WE_FGC_THREED_BODY, (DWORD)0xffffff);
			SetWindowBkColor(GetDlgItem(hWnd, ID_ENSURE), PIXEL_blue0);
			SetWindowFont(GetDlgItem(hWnd, ID_ENSURE), pLogFont25);

			SetWindowElementAttr(GetDlgItem(hWnd, ID_BACK), WE_FGC_THREED_BODY, (DWORD)0x0000ff);
			SetWindowBkColor(GetDlgItem(hWnd, ID_BACK), PIXEL_blue0);
			SetWindowFont(GetDlgItem(hWnd, ID_BACK), pLogFont25);

			SetWindowElementAttr(GetDlgItem(hWnd, ID_DELSTATIC), WE_FGC_WINDOW, (DWORD)UI_PIXEL2RGB(PIXEL_yellow0));
			SetWindowFont(GetDlgItem(hWnd, ID_DELSTATIC), pLogFont25);

			SetWindowElementAttr(GetDlgItem(hWnd, ID_SPACESTATIC), WE_FGC_WINDOW, (DWORD)UI_PIXEL2RGB(PIXEL_yellow0));
			SetWindowFont(GetDlgItem(hWnd, ID_SPACESTATIC), pLogFont25);
			
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
		case MSG_ERASEBKGND:
			break;
		case MSG_CLOSE:
			EndDialog(hWnd, 0);
			UI_UnloadResourceInStorage();
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
static int UI_LoadResourceInStorage(void)
{
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapSpace, bitmapSpacePath))
	{
		UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile()");
	}

	return 0;
}

//卸载资源文件
static int UI_UnloadResourceInStorage(void)
{
	UnloadBitmap(pBitmapSpace);
	return 0;
}

