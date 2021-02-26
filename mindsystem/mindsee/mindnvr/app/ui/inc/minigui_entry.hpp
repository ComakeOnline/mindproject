#ifndef __MINIGUI_ENTRY_H__
#define __MINIGUI_ENTRY_H__

//minigui 基础头文件，不能调换包含顺序。
#include "minigui/common.h"
#include "minigui/minigui.h"
#include "minigui/gdi.h"
#include "minigui/window.h"
#include "minigui/control.h"
#include <string.h>
#include "mgncs/mgncs.h"
#include "minigui_debug.hpp"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef MG4_0_4
#define LRESULT int
#define LINT int
#else
#endif

// 控制UI 打印信息的宏，0 表示禁用打印 非0表示启用打印。
#define EN_UIPRINT 1
#define EN_UIPERROR 1

#define MSG_ENTER_ICON_SCREEN ((MSG_USER) + 0)
#define MSG_RCV_HC_DETCT ((MSG_USER) + 1)
#define MSG_CLOSE_HC_OSD ((MSG_USER) + 2)
#define MSG_SWITCH_FRAME ((MSG_USER) + 3)
#define MSG_MOVE ((MSG_USER) + 4)
#define MSG_SETMSGTEXT ((MSG_USER) + 5)

//#define UI_print(info) printf("[UI] %s", (info))
//#define UI_perror(func_name, err_info) fprintf(stderr, \
	//"[UI] File: %s, Line: %d, Func: %s():, error information: %s", \
	//__FILE__, __LINE__, (func_name), (err_info))

int UI_print(const char *str);
int UI_perror(const char *func_name, const char *err_info);

#define ABS_Int(val) (((val) > (0)) ? (val) : (-(val)))
#define UI_RGB2PIXEL(val) (RGB2Pixel(HDC_SCREEN, (((val) & 0xff0000) >> 16), (((val) & 0x00ff00) >> 8), ((val) & 0x00ff)))

// 本UI 中主要采用的颜色为红蓝绿黄，颜色的十六进制值由如下值给出。
#define PIXEL_red0 (UI_RGB2PIXEL(0xFFDAF4))
#define PIXEL_blue0 (UI_RGB2PIXEL(0x34B5E2))
#define PIXEL_green0 (UI_RGB2PIXEL(0x85E9BE))
#define PIXEL_yellow0 (UI_RGB2PIXEL(0xFAAA20))

int UI_ModuleLoad(void);
void UI_ModuleUnload(void);
int UI_ModuleRun(void);
int UI_GetTouchX(LPARAM lParam);
int UI_GetTouchY(LPARAM lParam);
int UI_GetTouchArea(int x, int y, int rowNum, int colNum);
int UI_PIXEL2RGB(int pixel_val);
PLOGFONT UI_CreatettfFont(const char* family, const char *charset, int size);

#endif
