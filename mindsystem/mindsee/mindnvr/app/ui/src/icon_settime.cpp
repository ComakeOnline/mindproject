#include "icon_settime.hpp"

const static int backSX = 620;
const static int backSY = 350;
const static int backW = 80;
const static int backH = 50;
const static int ensureSX = 0;
const static int ensureSY = backSY;
const static int ensureW = backW;
const static int ensureH = backH;

const static int ID_BACK = 100;
const static int ID_ENSURE = 101;
const static int ID_CLOCK = 102;
const static int ID_STATIC0 = 103;
const static int ID_STATIC1 = 104;
const static int ID_CALENDAR = 105;
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

const static int calendarStartX = 350;
const static int calendarStartY = 50;
const static int calendarW = 300;
const static int calendarH = 200;

const static int lineStartX = 70;
const static int lineW = 260;
const static int lineOneStartY = 60;
const static int lineTwoStartY = lineOneStartY + 80;

const static int clockStartX = lineStartX;
const static int clockStartY = lineOneStartY + 10;
const static int clockW = lineW;
const static int clockH = 60;

const static int textDestTimeStartX = lineStartX;
const static int textDestTimeStartY = 200;
const static int textDestTimeW = lineW;
const static int textDestTimeH = 100;

const static int textTimeValStartX = lineStartX;
const static int textTimeValStartY = 300;
const static int textTimeValW = DIALOG_WIDTH - textTimeValStartX * 2;
const static int textTimeValH = 100;

const static int textIncDecW = 30;
const static int textIncDecH = textIncDecW;
const static int textIncDecIntervalY = 75;
const static int textIncStartY = 265;
const static int textDecStartY = textIncStartY + textIncDecIntervalY;
const static int textYearStartX = 200;
const static int textMonStartX = 280;
const static int textDayStartX = 340;
const static int textHourStartX = 420;
const static int textMinStartX = 500;

#ifndef MG4_0_4	
static LRESULT UI_PorcIconSetTime(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_PorcIconSetTime(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

/*
typedef struct _CTRLDATA
{
	const char* class_name; 			
	DWORD		dwStyle;				
	int 		x, y, w, h; 			
	int 		id; 					
	const char* caption;				
	DWORD		dwAddData;				
	DWORD		dwExStyle;				
	const char* werdr_name;
	const WINDOW_ELEMENT_ATTR* we_attrs;
}CTRLDATA;
*/

static CTRLDATA CtrlArr[] = {
	//月历
	{
		CTRL_MONTHCALENDAR,
		WS_VISIBLE | WS_CHILD | MCS_NOTIFY,
		calendarStartX, calendarStartY, calendarW, calendarH,
		ID_CALENDAR,
		"",
		0,
		WS_EX_TRANSPARENT,
	},
	//时钟
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_SIMPLE | SS_CENTER,
		clockStartX, clockStartY, clockW, clockH,
		ID_CLOCK,
		"08 : 00",
		0,
		WS_EX_TRANSPARENT,
	},
	//文本框0
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_SIMPLE | SS_CENTER,
		textDestTimeStartX, textDestTimeStartY, textDestTimeW, textDestTimeH,
		ID_STATIC0,
		"时间设置：",
		0,
		WS_EX_TRANSPARENT,
	},
	//文本框1：时间值
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_SIMPLE | SS_CENTER,
		textTimeValStartX, textTimeValStartY, textTimeValW, textTimeValH,
		ID_STATIC1,
		"2020年 10月 11日 18时 31分",
		0,
		WS_EX_TRANSPARENT,
	},
	//返回按钮
	{
		CTRL_BUTTON,
		WS_VISIBLE,
		backSX, backSY, backW, backH,
		ID_BACK,
		"X",
		(DWORD)pBitmapBack,
		WS_EX_NONE,
	},
	//确认按钮
	{
		CTRL_BUTTON,
		WS_VISIBLE,
		ensureSX, ensureSY, ensureW, ensureH,
		ID_ENSURE,
		"…",
		(DWORD)pBitmapEnsure,
		WS_EX_NONE,
	},
	//+ - 图标
	//year
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_SIMPLE | SS_CENTER,
		textYearStartX, textIncStartY, textIncDecW, textIncDecH,
		ID_YEARINC,
		"+",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_SIMPLE | SS_CENTER,
		textYearStartX, textDecStartY, textIncDecW, textIncDecH,
		ID_YEARDEC,
		"-",
		0,
		WS_EX_NONE,
	},
	//mon
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_SIMPLE | SS_CENTER,
		textMonStartX, textIncStartY, textIncDecW, textIncDecH,
		ID_MONINC,
		"+",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_SIMPLE | SS_CENTER,
		textMonStartX, textDecStartY, textIncDecW, textIncDecH,
		ID_MONDEC,
		"-",
		0,
		WS_EX_NONE,
	},
	//day
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_SIMPLE | SS_CENTER,
		textDayStartX, textIncStartY, textIncDecW, textIncDecH,
		ID_DAYINC,
		"+",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_SIMPLE | SS_CENTER,
		textDayStartX, textDecStartY, textIncDecW, textIncDecH,
		ID_DAYDEC,
		"-",
		0,
		WS_EX_NONE,
	},
	//hour
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_SIMPLE | SS_CENTER,
		textHourStartX, textIncStartY, textIncDecW, textIncDecH,
		ID_HOURINC,
		"+",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_SIMPLE | SS_CENTER,
		textHourStartX, textDecStartY, textIncDecW, textIncDecH,
		ID_HOURDEC,
		"-",
		0,
		WS_EX_NONE,
	},
	//min
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_SIMPLE | SS_CENTER,
		textMinStartX, textIncStartY, textIncDecW, textIncDecH,
		ID_MININC,
		"+",
		0,
		WS_EX_NONE,
	},
	{
		CTRL_STATIC,
		WS_VISIBLE | WS_CHILD | SS_SIMPLE | SS_CENTER,
		textMinStartX, textDecStartY, textIncDecW, textIncDecH,
		ID_MINDEC,
		"-",
		0,
		WS_EX_NONE,
	},
};

/*
typedef struct _DLGTEMPLATE
{
	DWORD		dwStyle;				
	DWORD		dwExStyle;				
	int 		x, y, w, h; 			
	const char* caption;				
	HICON		hIcon;					
	HMENU		hMenu;					
	int 		controlnr;				
	PCTRLDATA	controls;				
	DWORD		dwAddData;				
}DLGTEMPLATE;
*/

static DLGTEMPLATE DlgTemplate = {
	WS_VISIBLE,
	WS_EX_NONE,
	DIALOG_STARTX, DIALOG_STARTY, DIALOG_WIDTH, DIALOG_HEIGHT,
	"",
	0, 0,
	TABLESIZE(CtrlArr), CtrlArr,
};


int UI_OnIconSetTime(int val)
{
	printf("val = %d\n", val);

	DialogBoxIndirectParam(&DlgTemplate, hIconScreen, UI_PorcIconSetTime, 0);
	
	return 0;
}

#ifndef MG4_0_4
static LRESULT UI_PorcIconSetTime(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_PorcIconSetTime(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{
	static int i = 0;
	static int ret = 0;
	static HDC hDC = HDC_INVALID;
	
	switch(message)
	{
		case MSG_PAINT:
			hDC = BeginPaint(hWnd);
			MoveTo(hDC, lineStartX, lineOneStartY);
			LineTo(hDC, lineStartX + lineW, lineOneStartY);
			MoveTo(hDC, lineStartX, lineTwoStartY);
			LineTo(hDC, lineStartX + lineW, lineTwoStartY);
			EndPaint(hWnd, hDC);
			hDC = HDC_INVALID;
			break;
		case MSG_CREATE:
			break;
		case MSG_INITDIALOG:
			SetWindowBkColor(hWnd, PIXEL_green0);

			for(i = 0; i < 2; i++)
			{
				SetWindowFont(GetDlgItem(hWnd, ID_BACK + i), pLogFont25);
				SetWindowBkColor(GetDlgItem(hWnd, ID_BACK + i), PIXEL_blue0);	//淡蓝色
			}

			SetWindowFont(GetDlgItem(hWnd, ID_CLOCK), pLogFont50);				//时钟
			SetWindowBkColor(GetDlgItem(hWnd, ID_CLOCK), PIXEL_green0);	//淡绿色

			for(i = 0; i < 2; i++)
			{
				SetWindowFont(GetDlgItem(hWnd, ID_STATIC0 + i), pLogFont25);
				SetWindowBkColor(GetDlgItem(hWnd, ID_STATIC0 + i), PIXEL_green0);	//淡绿色
			}
			
			SetWindowFont(GetDlgItem(hWnd, ID_CALENDAR), pLogFont15);

			for(i = 0; i < 10; i++)
			{
				SetWindowFont(GetDlgItem(hWnd, ID_YEARINC + i), pLogFont25);
				SetWindowBkColor(GetDlgItem(hWnd, ID_YEARINC + i), PIXEL_green0);	//淡绿色
			}

			HWND hCalendar;
			hCalendar = GetDlgItem(hWnd, ID_CALENDAR);
			if(HWND_INVALID == hCalendar)
			{
				UI_perror("UI_PorcIconSetTime", "Fail to call GetDlgItem");
			}

			MCCOLORINFO st_colorInfo;
			st_colorInfo.clr_titlebk = PIXEL_green0;	//标题背景淡绿
			st_colorInfo.clr_titletext = PIXEL_black;	//标题文字黑色
			st_colorInfo.clr_arrow = PIXEL_black;		//箭头黑色
			st_colorInfo.clr_arrowHibk = PIXEL_darkgray;	//箭头高亮背景色
			st_colorInfo.clr_weekcaptbk = PIXEL_green0;	//星期标题背景淡绿
			st_colorInfo.clr_weekcapttext = PIXEL_black;	//星期标题文字白色
			st_colorInfo.clr_daybk = PIXEL_green0;		//天数部分背景淡绿
			st_colorInfo.clr_dayHibk = PIXEL_blue0;		//天数部分高亮背景淡蓝
			st_colorInfo.clr_daytext = PIXEL_black;		//天数部分文字颜色黑色
			st_colorInfo.clr_trailingtext =PIXEL_green0;	//非当前月天数部分文字颜色黑色
			st_colorInfo.clr_dayHitext = PIXEL_black;	//高亮文字颜色黑色

			ret = SendMessage(hCalendar, MCM_SETCOLOR, 0, (LPARAM)&st_colorInfo);
			if(ret < 0)
			{
				UI_perror("UI_PorcIconSetTime", "Fail to call UI_PorcIconSetTime().\n");
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
