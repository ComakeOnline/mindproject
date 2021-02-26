#include "icon_topology.hpp"
#include <stdlib.h>
#include <string.h>

const static int backSX = 620;
const static int backSY = 350;
const static int backW = 80;
const static int backH = 50;
const static int ensureSX = 100;
const static int ensureSY = backSY;
const static int ensureW = backW;
const static int ensureH = backH;

const static int ID_BACK = 100;
const static int ID_ENSURE = 101;
const static int ID_STATICTITLE = 102;
const static int ID_STATICNVR = 103;
const static int ID_STATICIPC0 = 104;
const static int ID_STATICIPC1 = 105;
const static int ID_STATICIPC2 = 106;
const static int ID_STATICIPC3 = 107;
const static int ID_STATICIPC4 = 108;
const static int ID_STATICIPC5 = 109;
const static int ID_STATICIPC6 = 110;
const static int ID_STATICIPC7 = 111;
const static int ID_STATICIPC8 = 112;

//标题
const static int TitleStaticW = 100;
const static int TitleStaticH = DIALOG_HEIGHT;
const static int TitleStaticSX = 0;
const static int TitleStaticSY = 0;

//
const static int DelStaticW = 500;
const static int DelStaticH = 35;
const static int DelStaticSX = 324;
const static int DelStaticSY = 325;

//NVR 位置
const static int NVRW = 100;
const static int NVRH = 40;
const static int NVRSX = 500;
const static int NVRSY = 100;

static int si_test_cnt = 0;			//用于测试的计数器
const static int IPCNUM = 8;		//IPC 最大数量
const static int CircleRadius = 150;	//UI 中圆的半径
const static int CircleDiameter = CircleRadius * 2;	//直径
//IPC 的图标位于同心圆上，把圆平均分成9等分，NVR 位于定点，剩余8 个点用于放置IPC 和交换机。
//st_FixedPosArr[IPCNUM] 结构体数组用来记录8 个点的UI 坐标信息, 
const static st_IPCPosition_t st_FixedPosArr[IPCNUM] = {\
	{NVRSX + 96, NVRSY + 35}, {NVRSX + 148, NVRSY + 124}, {NVRSX + 130, NVRSY + 225}, {NVRSX + 51, NVRSY + 291}, \
	{NVRSX - 51, NVRSY + 291}, {NVRSX - 130, NVRSY + 225}, {NVRSX - 148, NVRSY + 124}, {NVRSX - 96, NVRSY + 35}, };
//而st_RealPosArr[IPCNUM] 结构体数组则用来记录一定数量的IPC 的真实坐标信息，数组长度为IPCNUM，
//但实际上不一定有这么多个IPC, 所以引用该数组时，必须给定要引用的最大数组长度。
static st_IPCPosition_t st_RealPosArr[IPCNUM] = {{0}};

static int UI_GetIPCNumber();
static int UI_GetIPCRealPosition(const int IPCNum);
static int UI_DrawIPCTopology(const int IPCNum, HWND hParent);
#ifndef MG4_0_4
static LRESULT UI_PorcIconTopology(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_PorcIconTopology(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif


static CTRLDATA CtrlArr[] = {
	//确定和返回
	{
		CTRL_BUTTON,
		WS_VISIBLE | WS_CHILD | BS_NOTIFY,
		ensureSX, ensureSY, ensureW, ensureH,
		ID_ENSURE,
		"确定",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE | WS_CHILD | BS_NOTIFY,
		backSX, backSY, backW, backH,
		ID_BACK,
		"返回",
		0,
		WS_EX_NONE,
	},
	//文本框：标题
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_CENTER,
		TitleStaticSX, TitleStaticSY, TitleStaticW, TitleStaticH,
		ID_STATICTITLE,
		"\n\n\n\n\n级\n联\n信\n息\n",
		0,
		WS_EX_NONE,
	},
	//文本框：NVR
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_CENTER,
		NVRSX, NVRSY, NVRW, NVRH,
		ID_STATICNVR,
		"NVR",
		0,
		WS_EX_NONE,
	},
};

static DLGTEMPLATE DlgTemplate = {
	WS_VISIBLE,
	WS_EX_NONE,
	DIALOG_STARTX, DIALOG_STARTY, DIALOG_WIDTH, DIALOG_HEIGHT,
	"",
	0, 0,
	TABLESIZE(CtrlArr), CtrlArr,
};

int UI_OnIconTopology(int val)
{
	printf("val = %d\n", val);

	DialogBoxIndirectParam(&DlgTemplate, hIconScreen, UI_PorcIconTopology, 0);
	
	return 0;
}

#ifndef MG4_0_4
static LRESULT UI_PorcIconTopology(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_PorcIconTopology(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{
	static int i = 0;
	static int ret = 0;
	static int IPCNum = 0;
	static HDC hDC = HDC_INVALID;
	
	switch(message)
	{
		case MSG_PAINT:
			break;
		case MSG_CREATE:
			break;
		case MSG_INITDIALOG:
			SetWindowBkColor(hWnd, PIXEL_green0);
			SetWindowElementAttr(GetDlgItem(hWnd, ID_STATICTITLE), WE_FGC_WINDOW, (DWORD)0xffffff);
			SetWindowBkColor(GetDlgItem(hWnd, ID_STATICTITLE), PIXEL_blue0);
			SetWindowFont(GetDlgItem(hWnd, ID_STATICTITLE), pLogFont25);
			
			SetWindowElementAttr(GetDlgItem(hWnd, ID_ENSURE), WE_FGC_THREED_BODY, (DWORD)0xffffff);
			SetWindowBkColor(GetDlgItem(hWnd, ID_ENSURE), PIXEL_blue0);
			SetWindowFont(GetDlgItem(hWnd, ID_ENSURE), pLogFont25);

			SetWindowElementAttr(GetDlgItem(hWnd, ID_BACK), WE_FGC_THREED_BODY, (DWORD)0xff);
			SetWindowBkColor(GetDlgItem(hWnd, ID_BACK), PIXEL_blue0);
			SetWindowFont(GetDlgItem(hWnd, ID_BACK), pLogFont25);

			SetWindowFont(GetDlgItem(hWnd, ID_STATICNVR), pLogFont25);

			IPCNum = UI_GetIPCNumber() + si_test_cnt;
			UI_GetIPCRealPosition(IPCNum);
			UI_DrawIPCTopology(IPCNum, hWnd);
			si_test_cnt++;
			if(IPCNUM + 1 == si_test_cnt)
			{
				si_test_cnt = 0;
			}
			
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

static int UI_GetIPCNumber()
{
	
	return 0;
}

static int UI_GetIPCRealPosition(const int IPCNum)
{
	if(IPCNum < 0 || IPCNum > IPCNUM)
	{
		UI_perror("UI_DrawIPCTopology()", "Bad argument");
		return -1;
	}

	int i = 0;

	memset(st_RealPosArr, 0, sizeof(st_IPCPosition_t) * IPCNUM);
	switch(IPCNum)
	{
		case 1:
			st_RealPosArr[0].pos_sx = NVRSX;
			st_RealPosArr[0].pos_sy = NVRSY + CircleDiameter;
			
			break;
		case 2:
			st_RealPosArr[0].pos_sx = st_FixedPosArr[2].pos_sx;
			st_RealPosArr[0].pos_sy = st_FixedPosArr[2].pos_sy;

			st_RealPosArr[1].pos_sx = st_FixedPosArr[5].pos_sx;
			st_RealPosArr[1].pos_sy = st_FixedPosArr[5].pos_sy;
			break;
		case 3:
			st_RealPosArr[0].pos_sx = st_FixedPosArr[1].pos_sx;
			st_RealPosArr[0].pos_sy = st_FixedPosArr[1].pos_sy;

			st_RealPosArr[1].pos_sx = NVRSX;
			st_RealPosArr[1].pos_sy = NVRSY + CircleDiameter;

			st_RealPosArr[2].pos_sx = st_FixedPosArr[6].pos_sx;
			st_RealPosArr[2].pos_sy = st_FixedPosArr[6].pos_sy;
			break;
		case 4:
			st_RealPosArr[0].pos_sx = st_FixedPosArr[1].pos_sx;
			st_RealPosArr[0].pos_sy = st_FixedPosArr[1].pos_sy;

			st_RealPosArr[1].pos_sx = st_FixedPosArr[2].pos_sx;
			st_RealPosArr[1].pos_sy = st_FixedPosArr[2].pos_sy;

			st_RealPosArr[2].pos_sx = st_FixedPosArr[5].pos_sx;
			st_RealPosArr[2].pos_sy = st_FixedPosArr[5].pos_sy;

			st_RealPosArr[3].pos_sx = st_FixedPosArr[6].pos_sx;
			st_RealPosArr[3].pos_sy = st_FixedPosArr[6].pos_sy;
			break;
		case 5:
			st_RealPosArr[0].pos_sx = st_FixedPosArr[1].pos_sx;
			st_RealPosArr[0].pos_sy = st_FixedPosArr[1].pos_sy;

			st_RealPosArr[1].pos_sx = st_FixedPosArr[2].pos_sx;
			st_RealPosArr[1].pos_sy = st_FixedPosArr[2].pos_sy;

			st_RealPosArr[2].pos_sx = NVRSX;
			st_RealPosArr[2].pos_sy = NVRSY + CircleDiameter;

			st_RealPosArr[3].pos_sx = st_FixedPosArr[5].pos_sx;
			st_RealPosArr[3].pos_sy = st_FixedPosArr[5].pos_sy;

			st_RealPosArr[4].pos_sx = st_FixedPosArr[6].pos_sx;
			st_RealPosArr[4].pos_sy = st_FixedPosArr[6].pos_sy;
			break;
		case 6:
			for(i = 0; i < IPCNum; i++)
			{
				st_RealPosArr[i].pos_sx = st_FixedPosArr[i + 1].pos_sx;
				st_RealPosArr[i].pos_sy = st_FixedPosArr[i + 1].pos_sy;
			}			
			break;
		case 7:
		case 8:
			for(i = 0; i < IPCNum; i++)
			{
				st_RealPosArr[i].pos_sx = st_FixedPosArr[i].pos_sx;
				st_RealPosArr[i].pos_sy = st_FixedPosArr[i].pos_sy;
			}
			break;
	}
	
	return 0;
}


int UI_DrawIPCTopology(const int IPCNum, HWND hParent)
{
	if(IPCNum < 0 || IPCNum > IPCNUM)
	{
		UI_perror("UI_DrawIPCTopology()", "Bad argument");
		return -1;
	}

	int i = 0;
	HWND hCtrl = 0;

	for(i = 0; i < IPCNum; i++)
	{
		hCtrl = CreateWindow(CTRL_STATIC, "IPC", WS_VISIBLE | WS_CHILD | SS_CENTER, ID_STATICIPC0 + i, \
				st_RealPosArr[i].pos_sx, st_RealPosArr[i].pos_sy, NVRW, NVRH, hParent, 0);
		SetWindowFont(hCtrl, pLogFont25);
	}

	return 0;
}

