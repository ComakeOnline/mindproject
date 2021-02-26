/*
UI 要使用到的全局资源
*/

#include "minigui/common.h"
#include "minigui/minigui.h"
#include "minigui/gdi.h"
#include "minigui/window.h"
#include "minigui_resource.hpp"

// 字体变量。
PLOGFONT pLogFont15 = NULL;
PLOGFONT pLogFont20 = NULL;
PLOGFONT pLogFont25 = NULL;
PLOGFONT pLogFont30 = NULL;
PLOGFONT pLogFont35 = NULL;
PLOGFONT pLogFont40 = NULL;
PLOGFONT pLogFont45 = NULL;
PLOGFONT pLogFont50 = NULL;

// 窗口句柄
HWND hIconScreen = HWND_INVALID;
HWND hMainScreen = HWND_INVALID;
HWND hFrameWnd = HWND_INVALID;

// 定时器超时的消息每发送3.226 次代表经过了1秒。
double timeCnt1000ms = 3.226;

bool inOSD = false;			// 是否在OSD 弹窗中
bool inSingleFrame = false;	// 是否在SigleFrame 界面
bool inFourFrame = false;	// 是否在FourFrame 界面
bool inIconScreen = false;	// 是否在滑动图标界面

// 返回图标
static BITMAP bitmapBack = {0};
PBITMAP const pBitmapBack = &bitmapBack;
const char *bitmapBackPath = "./res/back.jpg";

// 确认图标
static BITMAP bitmapEnsure = {0};
PBITMAP const pBitmapEnsure = &bitmapEnsure;
const char *bitmapEnsurePath = "./res/ensure.jpg";

