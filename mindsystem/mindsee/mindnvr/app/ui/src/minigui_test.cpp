#include "minigui/common.h"
#include "minigui/minigui.h"
#include "minigui/gdi.h"
#include "minigui/window.h"
#include "minigui/control.h"
#include <string.h>
#include "mgncs/mgncs.h"

#include <stdio.h>
#include <errno.h>

#define ID_CONTAINER0 31
#define ID_CONTAINER2 32

#define IDC_CONTAINER 100
#define IDC_RDOGROUP 151
#define IDC_CKGROUP 152
#define IDC_LAMIAN 101
#define IDC_CHOUDOUFU 102
#define IDC_JIANBING 103
#define IDC_MAHUA 104
#define IDC_SHUIJIAO 105
#define IDC_XIAN 110
#define IDC_LA 111
#define IDC_PROMPT 200

static NCS_PROP_ENTRY radioGroup_props[] = {
	{NCSP_BUTTON_GROUPID, IDC_RDOGROUP},
	{0, 0}
};

static NCS_WND_TEMPLATE ncsCtrlYourTaste[] = {
	{ NCSCTRL_BUTTONGROUP, IDC_RDOGROUP, 16, 10, 230, 160, WS_VISIBLE, WS_EX_TRANSPARENT, "optional snack", NULL, NULL, NULL, NULL, 0, 0 },\
	{ NCSCTRL_RADIOBUTTON, IDC_LAMIAN, 36, 38, 200, 20, WS_VISIBLE | WS_TABSTOP | NCSS_BUTTON_AUTOCHECK, WS_EX_NONE, "northwest pulled noodle", radioGroup_props, NULL, NULL, NULL, 0, 0 },
	{ NCSCTRL_RADIOBUTTON, IDC_CHOUDOUFU, 36, 64, 200, 20, WS_VISIBLE | WS_TABSTOP | NCSS_BUTTON_AUTOCHECK, WS_EX_NONE, "chang sha bad smelling bean curd", radioGroup_props, NULL, NULL, 0, 0 },
	{ NCSCTRL_RADIOBUTTON, IDC_JIANBING, 36, 90, 200, 20, WS_VISIBLE | WS_TABSTOP | WS_DISABLED, WS_EX_NONE, "shan dong thini pancake", radioGroup_props, NULL, NULL, NULL, 0, 0 },
	{ NCSCTRL_RADIOBUTTON, IDC_MAHUA, 36, 116, 200, 20, WS_VISIBLE | WS_TABSTOP | NCSS_BUTTON_AUTOCHECK, WS_EX_NONE, "tianjin fired dough twist", radioGroup_props, NULL, NULL, NULL, 0, 0 },
	{ NCSCTRL_RADIOBUTTON, IDC_SHUIJIAO, 36, 142, 200, 20, WS_VISIBLE | WS_TABSTOP | NCSS_BUTTON_AUTOCHECK, WS_EX_NONE, "chengdu red oil boiled dumpling", radioGroup_props, NULL, NULL, NULL, 0, 0 },
	{ NCSCTRL_BUTTONGROUP, IDC_CKGROUP, 250, 10, 100, 160, WS_VISIBLE, WS_EX_TRANSPARENT, "flavor", NULL, NULL, NULL, NULL, 0, 0 },
	{ NCSCTRL_CHECKBUTTON, IDC_XIAN, 260, 38, 80, 20, WS_VISIBLE | NCSS_BUTTON_AUTOCHECK, WS_EX_NONE, "partial salty", NULL, NULL, NULL, NULL, 0, 0},
	{ NCSCTRL_CHECKBUTTON, IDC_LA, 260, 64, 80, 20, WS_VISIBLE | NCSS_BUTTON_AUTOCHECK, WS_EX_NONE, "partial spicy", NULL, NULL, NULL, NULL, 0, 0 },
	{ NCSCTRL_STATIC, IDC_PROMPT, 16, 180, 360, 40, WS_VISIBLE, WS_EX_NONE, "northwests", NULL, NULL, NULL, NULL, 0, 0 },
	{ NCSCTRL_BUTTON, IDOK, 70, 230, 70, 30, WS_VISIBLE | WS_TABSTOP | NCSS_NOTIFY, WS_EX_NONE, "Ok", NULL, NULL, NULL, NULL, 0, 0 },
	{ NCSCTRL_BUTTON, IDCANCEL, 200, 230, 70, 30, WS_VISIBLE | WS_TABSTOP | NCSS_NOTIFY, WS_EX_NONE, "Cancel", NULL, NULL, NULL, NULL, 0, 0 },
};

static void dialog_onCSizeChanged(mWidget* self, int clientWidth, int clientHeight)
{ 
	HWND hContainer = GetDlgItem(self->hwnd, IDC_CONTAINER);
	if(hContainer != HWND_NULL || hContainer != HWND_INVALID)
		MoveWindow(hContainer, 0, 0, clientWidth, clientHeight, TRUE);
}

static BOOL container_onCommand(mWidget* self, int id, int nc, HWND hCtrl)
{
	if (id == IDOK || id == IDCANCEL)
	{ 
		if(nc == NCSN_BUTTON_PUSHED)
		{ //close dialog 
			HWND hParent = GetParent(self->hwnd);
			SendNotifyMessage(hParent, MSG_COMMAND, (WPARAM)MAKELONG(id, nc), (LPARAM)hCtrl);
		}
		return FALSE;
	}
	return FALSE;
}

static NCS_EVENT_HANDLER container_handlers[] = { 
	{MSG_COMMAND, (void *)container_onCommand},
	{0, NULL} 
};

static NCS_EVENT_HANDLER mainWin_handlers[] = { 
	{MSG_CSIZECHANGED, (void *)ncsCreateMainWindow},
	{0, NULL} 
};

int MiniGUIMain(int argc, const char *argv[])
{
	ncsInitialize();

	mDialogBox *dialog = (mDialogBox*)ncsCreateMainWindow(NCSCTRL_DIALOGBOX, "Container Demo", \
		WS_CAPTION | WS_BORDER | WS_VISIBLE | NCSS_MNWND_MODE, 0, 1, 0, 0, 974, 550, \
		HWND_DESKTOP, 0, 0, NULL, NULL, NULL, 0);

	ncsSetComponentHandler((mComponent*)dialog, MSG_CSIZECHANGED, (void *)ncsCreateMainWindow);

	mContainer *container = (mContainer*)ncsCreateWindow(NCSCTRL_CONTAINER, "", \
		WS_BORDER | WS_VISIBLE, WS_EX_NONE, IDC_CONTAINER, 100, 50, 724, 400, \
		dialog->hwnd, NULL, NULL, container_handlers, 0);

	_c(container)->addChildren(container, ncsCtrlYourTaste, sizeof(ncsCtrlYourTaste)/sizeof(NCS_WND_TEMPLATE));

	_c(dialog)->doModal(dialog, TRUE); 

	MainWindowThreadCleanup(dialog->hwnd);
	ncsUninitialize();

	return 0;
}

