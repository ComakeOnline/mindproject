#include "icon_call.hpp"

// 文本框坐标
const static int textSX = 100;
const static int textSY = 40;
const static int textW = 370;
const static int textH = 45;

// 视频、音频图标坐标
const static int videoSX = 525;
const static int videoSY = 125;
const static int videoW = 100;
const static int videoH = 60;
const static int audioSX = videoSX;
const static int audioSY = videoSY + videoH + 30;
const static int audioW = videoW;
const static int audioH = videoH;

// 返回图标坐标
const static int backSX = 620;
const static int backSY = 350;
const static int backW = 80;
const static int backH = 50;

// 控件ID
const static int ID_VIDEO = 100;
const static int ID_AUDIO = 101;
const static int ID_BACK = 102;
const static int ID_TEXT = 103;
const static int ID_KEY1 = 104;
const static int ID_KEY2 = 105;
const static int ID_KEY3 = 106;
const static int ID_KEY4 = 107;
const static int ID_KEY5 = 108;
const static int ID_KEY6 = 109;
const static int ID_KEY7 = 110;
const static int ID_KEY8 = 111;
const static int ID_KEY9 = 112;
const static int ID_KEYabc = 113;
const static int ID_KEY0 = 114;
const static int ID_KEYX = 115;

// 资源文件
static BITMAP bitmapVideo = {0};
PBITMAP const pBitmapVideo = &bitmapVideo;
const char *bitmapVideoPath = "./res/video.jpg";

static BITMAP bitmapAudio = {0};
PBITMAP const pBitmapAudio = &bitmapAudio;
const char *bitmapAudioPath = "./res/audio.jpg";

static BITMAP keyBoard0 = {0};
PBITMAP const pKeyBoard0 = &keyBoard0;
const char *keyBoard0Path = "./res/keyboard/0.jpg";

static BITMAP keyBoard1 = {0};
PBITMAP const pKeyBoard1 = &keyBoard1;
const char *keyBoard1Path = "./res/keyboard/1.jpg";

static BITMAP keyBoard2 = {0};
PBITMAP const pKeyBoard2 = &keyBoard2;
const char *keyBoard2Path = "./res/keyboard/2.jpg";

static BITMAP keyBoard3 = {0};
PBITMAP const pKeyBoard3 = &keyBoard3;
const char *keyBoard3Path = "./res/keyboard/3.jpg";

static BITMAP keyBoard4 = {0};
PBITMAP const pKeyBoard4 = &keyBoard4;
const char *keyBoard4Path = "./res/keyboard/4.jpg";

static BITMAP keyBoard5 = {0};
PBITMAP const pKeyBoard5 = &keyBoard5;
const char *keyBoard5Path = "./res/keyboard/5.jpg";

static BITMAP keyBoard6 = {0};
PBITMAP const pKeyBoard6 = &keyBoard6;
const char *keyBoard6Path = "./res/keyboard/6.jpg";

static BITMAP keyBoard7 = {0};
PBITMAP const pKeyBoard7 = &keyBoard7;
const char *keyBoard7Path = "./res/keyboard/7.jpg";

static BITMAP keyBoard8 = {0};
PBITMAP const pKeyBoard8 = &keyBoard8;
const char *keyBoard8Path = "./res/keyboard/8.jpg";

static BITMAP keyBoard9 = {0};
PBITMAP const pKeyBoard9 = &keyBoard9;
const char *keyBoard9Path = "./res/keyboard/9.jpg";

static BITMAP keyBoardX = {0};
PBITMAP const pKeyBoardX = &keyBoardX;
const char *keyBoardXPath = "./res/keyboard/X.jpg";

static BITMAP keyBoardabc = {0};
PBITMAP const pKeyBoardabc = &keyBoardabc;
const char *keyBoardabcPath = "./res/keyboard/abc.jpg";

static const PBITMAP pKeyBoardArr[12] = {pKeyBoard1, pKeyBoard2, pKeyBoard3, pKeyBoard4, pKeyBoard5, pKeyBoard6, pKeyBoard7, \
	pKeyBoard8, pKeyBoard9, pKeyBoardabc, pKeyBoard0, pKeyBoardX,};

// 局部函数声明
static int UI_LoadResourceInCall(void);
static int UI_UnloadResourceInCall(void);
static int UI_CreateKeyBoard(HWND hWnd);
#ifndef MG4_0_4
static LRESULT UI_PorcIconCall(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_PorcIconCall(HWND hWnd, UINIT message, WPARAM wParam, LPARAM lParam);
#endif

static CTRLDATA CtrlArr[] = {
	//视频、音频图标
	{
		CTRL_BUTTON,
		WS_VISIBLE | BS_BITMAP,
		videoSX, videoSY, videoW, videoH,
		ID_VIDEO,
		"",
		(DWORD)pBitmapVideo,
		WS_EX_NONE,
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE | BS_BITMAP,
		audioSX, audioSY, audioW, audioH,
		ID_AUDIO,
		"",
		(DWORD)pBitmapAudio,
		WS_EX_NONE,
	},
	//返回图标
	{
		CTRL_BUTTON,
		WS_VISIBLE | BS_BITMAP,
		backSX, backSY, backW, backH,
		ID_BACK,
		"",
		(DWORD)pBitmapBack,
		WS_EX_NONE,
	},
	//文本框
	{
		CTRL_TEXTEDIT,
		WS_VISIBLE,
		textSX, textSY, textW, textH,
		ID_TEXT,
		"",
		0,
		WS_EX_NONE,
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

int UI_OnIconCall(int val)
{
	printf("val = %d\n", val);

	DialogBoxIndirectParam(&DlgTemplate, hIconScreen, UI_PorcIconCall, 0);
	
	return 0;
}

// 对话框过程函数
#ifndef MG4_0_4
static LRESULT UI_PorcIconCall(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_PorcIconCall(HWND hWnd, UINIT message, WPARAM wParam, LPARAM lParam)
#endif

{
	switch(message)
	{
		case MSG_INITDIALOG:
			SetWindowBkColor(hWnd, 0X000FFF);
			UI_LoadResourceInCall();		//注意，加载资源比创建键盘先调用。
			UI_CreateKeyBoard(hWnd);
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
		case MSG_PAINT:
			break;
		case MSG_ERASEBKGND:
			break;
		case MSG_CLOSE:
			EndDialog(hWnd, 0);
			UI_UnloadResourceInCall();
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

static int UI_CreateKeyBoard(HWND hWnd)
{
	int i = 0;
	int j = 0;
	const int row = 4;
	const int col = 3;

	const static int keySX = textSX;
	const static int keySY = textSY + textH + 20;
	const static int keyW = 110;
	const static int keyH = 50;
	const static int keyIX = 20;
	const static int keyIY = 15;


	for(i = 0; i < row; i++)
	{
		for(j = 0; j < col; j++)
		{
			CreateWindow(CTRL_BUTTON, "", WS_VISIBLE | BS_BITMAP, ID_KEY1 + (i * col + j), \
				keySX + (keyW + keyIX) * j, keySY + (keyH + keyIY) * i, keyW, keyH, hWnd, (DWORD)pKeyBoardArr[i * col + j]);
		}
	}
	return 0;
}

// 加载资源文件
static int UI_LoadResourceInCall(void)
{
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pKeyBoard0, keyBoard0Path))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}
	
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pKeyBoard1, keyBoard1Path))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}
	
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pKeyBoard2, keyBoard2Path))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}
	
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pKeyBoard3, keyBoard3Path))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}
	
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pKeyBoard4, keyBoard4Path))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}
	
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pKeyBoard5, keyBoard5Path))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}
	
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pKeyBoard6, keyBoard6Path))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}
	
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pKeyBoard7, keyBoard7Path))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}
	
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pKeyBoard8, keyBoard8Path))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}
	
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pKeyBoard9, keyBoard9Path))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}
	
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pKeyBoardX, keyBoardXPath))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}
	
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pKeyBoardabc, keyBoardabcPath))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}

	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapVideo, bitmapVideoPath))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}
	
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapAudio, bitmapAudioPath))
	{
		UI_perror("UI_LoadResourceInCall", "Fail to call LoadBitmapFromFile()");
	}

	return 0;
}

//卸载资源文件
static int UI_UnloadResourceInCall(void)
{
	UnloadBitmap(pKeyBoard0);
	UnloadBitmap(pKeyBoard1);
	UnloadBitmap(pKeyBoard2);
	UnloadBitmap(pKeyBoard3);
	UnloadBitmap(pKeyBoard4);
	UnloadBitmap(pKeyBoard5);
	UnloadBitmap(pKeyBoard6);
	UnloadBitmap(pKeyBoard7);
	UnloadBitmap(pKeyBoard8);
	UnloadBitmap(pKeyBoard9);
	UnloadBitmap(pKeyBoardX);
	UnloadBitmap(pKeyBoardabc);
	UnloadBitmap(pBitmapVideo);
	UnloadBitmap(pBitmapAudio);

	return 0;
}

#include "icon_call.hpp"

