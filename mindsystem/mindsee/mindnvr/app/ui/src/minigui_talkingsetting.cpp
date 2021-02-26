/*
通话设置界面
*/

#include "minigui_talkingsetting.hpp"
#include "minigui_resource.hpp"
#include "minigui_entry.hpp"
#include "appinterface.h"

// 对话框坐标
const static int dlgSX = 162;
const static int dlgSY = 80;
const static int dlgW = 700;
const static int dlgH = 320;

// 图标坐标
const static int volumeBarSX = 200;
const static int volumeBarSY = 80;
const static int volumeBarW = 300;
const static int volumeBarH = 50;
const static int backSX = 300;
const static int backSY = 250;
const static int backW = 100;
const static int backH = 50;

// 控件ID
const static int ID_BACK = 110;
const static int ID_VOLUME_BAR = 111;

// 控件句柄
static HWND hVolumeBar = HWND_INVALID;

// 函数声明
static void UI_ProcVolumeNotif(HWND hWnd, LINT id, int nc, DWORD add_data);
#ifndef MG4_0_4
static LRESULT UI_DlgTalkingSettingProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_DlgTalkingSettingProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

static CTRLDATA ctrlData[] = {
	// 返回
	{
    	CTRL_BUTTON,
    	WS_VISIBLE | WS_CHILD | BS_NOTIFY,
    	backSX, backSY, backW, backH,
    	ID_BACK,
    	"返回",
    	0,
    	WS_EX_NONE,
    	//NULL, NULL,
	},
	// 滑动控件
	{
		CTRL_TRACKBAR,
		WS_VISIBLE | WS_CHILD | TBS_NOTIFY | TBS_TIP,
		volumeBarSX, volumeBarSY, volumeBarW, volumeBarH,
		ID_VOLUME_BAR,
		"功能预留",
		0,
		WS_EX_NONE,
		NULL, NULL,
	},
};

static DLGTEMPLATE dlgTemplate = {
    WS_VISIBLE,
    WS_EX_NONE,
    dlgSX, dlgSY, dlgW, dlgH,
    "",
    0, 0,
    TABLESIZE(ctrlData), ctrlData,
    0,
};

/*-----------------------------------------------------------------------------
描   述：进入通话设置菜单
参 数：无
返回值：返回新建窗口的句柄。
注 意：
-----------------------------------------------------------------------------*/
int UI_SettingInTalking(HWND hParent)
{
	return DialogBoxIndirectParam(&dlgTemplate, hParent, UI_DlgTalkingSettingProc, 0);
}

/*-----------------------------------------------------------------------------
描   述：对话框过程函数
参	数：
返回值：
注	意：
-----------------------------------------------------------------------------*/
#ifndef MG4_0_4
static LRESULT UI_DlgTalkingSettingProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_DlgTalkingSettingProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{	
	switch(message)
	{
		case MSG_PAINT:
			break;
		case MSG_CREATE:
			break;
		case MSG_INITDIALOG:
		{
			hVolumeBar = GetDlgItem(hWnd, ID_VOLUME_BAR);

			// "返回"按钮的字体颜色、背景色、字体大小。
			SetWindowElementAttr(GetDlgItem(hWnd, ID_BACK), WE_FGC_THREED_BODY, (DWORD)0x000000);
			SetWindowBkColor(GetDlgItem(hWnd, ID_BACK), PIXEL_red0);
			SetWindowFont(GetDlgItem(hWnd, ID_BACK), pLogFont25);

			// 设置滑块的回调函数、背景颜色。
			SetNotificationCallback(hVolumeBar, UI_ProcVolumeNotif);
			SetWindowBkColor(hVolumeBar, PIXEL_yellow0);
			
			//SendMessage(hVolumeBar, TBM_SETRANGE, 0, 10);		最大值和最小值默认就是0~10.
			//SendMessage(hVolumeBar, TBM_SETPAGESIZE, 1, 1);	步进值默认就是1
			//SendMessage(hVolumeBar, TBM_SETPOS, 1, 1); 		滑块位置设置
			SendMessage(hVolumeBar, TBM_SETTIP, (LPARAM)"-", (WPARAM)"+"); 	// 两端文字说明

			AppInterface *pAppInterface = AppInterface::GetInstance();
			int ret = pAppInterface->GetVolume() + 5;
			SendMessage(hVolumeBar, TBM_SETPOS, (WPARAM)ret, 0); 			// 滑块位置设置

			SetWindowFont(hVolumeBar, pLogFont20);
		}
			break;
		case MSG_COMMAND:
			switch(wParam)
			{
				case ID_BACK:
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

/*-----------------------------------------------------------------------------
描   述：音量控件的消息回调函数
参	数：
返回值：
注	意：
-----------------------------------------------------------------------------*/
static void UI_ProcVolumeNotif(HWND hWnd, LINT id, int nc, DWORD add_data)
{
	int ret = 0;

	if(nc == TBN_CHANGE && ID_VOLUME_BAR == id)
	{
		ret = SendMessage(hWnd, TBM_GETPOS, 0, 0);				// 滑块位置设置
		printf("ret = %d\n", ret);
		if(-1 == ret)
		{
			UI_perror("UI_ProcVolumeNotif()", "Fail to call SendMessage() with argument TBM_SETPOS.\n");
		}
		else
		{
			UI_print("Call pAppInterface->SetVolume().\n");
			AppInterface *pAppInterface = AppInterface::GetInstance();
			pAppInterface->SetVolume(ret);
		}
	}
}

