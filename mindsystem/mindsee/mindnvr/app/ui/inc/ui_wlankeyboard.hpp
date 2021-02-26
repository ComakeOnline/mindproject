#ifndef __UI_WLANKEYBOARD_HPP__
#define __UI_WLANKEYBOARD_HPP__

#include "minigui_entry.hpp"

extern int ID_KEY;
extern const int keyNum;

// 输入法类型的枚举，用于切入输入法时的传参
typedef enum{
	EM_METHOD_LOWER,		// 小写字母
	EM_METHOD_UPPER ,		// 大写字母
	EM_METHOD_CAPSLOCK, 	// 大小写切换
	EM_METHOD_SYMBOOL,		// 符号
	EM_METHOD_MAX,
}EM_METHOD_T;

// 特殊按键在字符数组中的下标索引
typedef enum{
	EM_INDEX_BACKSPACE = 20,// 退格
	EM_INDEX_CAPSLOCK = 30,	// 大小写切换
	EM_INDEX_SYMBOOL = 31, 	// 符号
	EM_INDEX_SPACE = 32,	// 空格
	EM_INDEX_MAX = 40,
}EM_INDEX_T;

// 特殊按键对应的ASCII 值
typedef enum{
	EM_ASCII_BACKSPACE = 0x7F,	// 退格
	EM_ASCII_CAPSLOCK = 0x0F,	// 大小写切换，0x0F 在ASCII 中为“切换”
	EM_ASCII_SYMBOOL = 0x07,	// 符号，0x07 在ASCII 中为“响铃”
	EM_ASCII_SPACE = ' ',		// 空格
	EM_ASCII_MAX = 255,			// -1 == 254
}EM_ASCII_T;

// 键盘相关函数声明
bool isKeyID(int id);
EM_METHOD_T UI_GetInputMethod(HWND hKB);
EM_METHOD_T UI_SetInputMethod(HWND hKB, EM_METHOD_T emMethod);
HWND UI_CreateKeyboardFrame(HWND hParent, int keyID, int keyboardSX, int keyboardSY, int keyboardW, int keyboardH);
char UI_GetCharFromKeyID(HWND hKB, int keyID);

#endif
