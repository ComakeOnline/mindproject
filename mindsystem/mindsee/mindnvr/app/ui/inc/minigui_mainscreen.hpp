#ifndef __MINIGUI_MAINSCREEN_H__
#define __MINIGUI_MAINSCREEN_H__

#include "minigui_entry.hpp"
#include "minigui_resource.hpp"

#ifndef MG4_0_4
LRESULT UI_ProcMainScreen(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
LRESULT UI_ProcMainScreen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

#endif
