#include "ui_wlankeyboard.hpp"
#include "minigui_resource.hpp"

// 键盘模块需要用到的静态变量。
// 1.键盘的句柄。
static HWND hKeyboard = HWND_INVALID;
// 2.键盘当前的输入法。
static EM_METHOD_T emCurrMethod = EM_METHOD_MAX;
// 3.键盘的行、列、键的数量。
const static int keyRow = 4;
const static int keyCol = 10;

// 键盘模块需要被外部引用的变量。
// 1.按键总数量。
const int keyNum = keyRow * keyCol;
// 2.键盘首个按键的ID 号
int ID_KEY;

// 数组长度+1, 一定程度上避免踩内存。
// 小写键盘
static const char *lowerLetters[keyNum + 1] = {
	"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", 
	"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", 
	"退", "a", "s", "d", "f", "g", "h", "j", "k", "l", 
	"a/A", "符", "空", "z", "x", "c", "v", "b", "n", "m", 
};

// 大写键盘
static const char *upperLetters[keyNum + 1] = {
	"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", 
	"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", 
	"退", "A", "S", "D", "F", "G", "H", "J", "K", "L", 
	"A/a", "符", "空", "Z", "X", "C", "V", "B", "N", "M", 
};

// 符号键盘
static const char *symbool[keyNum + 1] = {
	"!", "@", "#", "$", "%", "^", "&", "*", "(", ")",
	"~", "`", ", ", "-", "+", "=", "|", "\\", "/", "1",
	"退", "'", "\"", ",", ".", "?", ":", ";", "2", "3",
	"a/A", "符", "空", "<", ">", "{", "}", "[", "]", "4",
};

// 模块内使用的函数声明
static void KB_SetKeyID(int id);
static HWND KB_GetKeyItem(HWND hKeyboard, int keyID);
static bool KB_isLowerMethod(EM_METHOD_T emMethod);
static bool KB_isUpperMethod(EM_METHOD_T emMethod);
static bool KB_isEnglishMethod(EM_METHOD_T emMethod);
static bool KB_isSymboolMethod(EM_METHOD_T emMethod);

/*-----------------------------------------------------------------------------
描   述：创建一个10X4输入法框架。
参	数：hParent 父窗口句柄；keyID 键盘首个按键的ID; 
		keyboardSX, SY, W, H 键盘启示坐标和宽高。
返回值：成功，返回输入法框架中首个按键的句柄。
注	意：1.返回的句柄指向输入法框架中的首个按键，并非整个键盘。
		2.试图对句柄进行+ -运算从而得到其它按键句柄的做法是错误的。
		如果希望设置整个键盘，应该使用模块提供的接口函数。
		3.keyID 指定了首个按键的ID, 往后每个按键的ID依次+1, 
		所以调用者至少保证[keyID, keyID + 40) 范围内的数值，不再用于别的控件。
-----------------------------------------------------------------------------*/
HWND UI_CreateKeyboardFrame(HWND hParent, int keyID, int keyboardSX, int keyboardSY, int keyboardW, int keyboardH)
{
	// 键盘的宽度必须为分辨率宽度的（1/4, 1]；
	// 高度分辨率宽高的（1/4, 1]
	if(keyboardW < RESOLUTION_WIDTH / 4 || keyboardW > RESOLUTION_WIDTH || 
		keyboardH < RESOLUTION_HEIGHT / 4 || keyboardH > RESOLUTION_HEIGHT)
	{
		UI_print("Fail to call UI_CreateKeyboardFrame(), bad size!\n");
		return HWND_INVALID;
	}

	if(HWND_INVALID == hParent)
	{
		UI_print("Fail to call UI_CreateKeyboardFrame(), invalid parent handler!\n");
		return HWND_INVALID;
	}

	KB_SetKeyID(keyID);

	// 每个键的宽、高。
	int keyW = keyboardW / keyCol;
	int keyH = keyboardH / keyRow;
	printf("key SX, SY, W, H = %d, %d, %d, %d.\n", keyboardSX, keyboardSY, keyW, keyH);

	HWND hKey = HWND_INVALID;
	for(int i = 0; i < keyNum; i++)
	{
		// 创建按键
		hKey = CreateWindowEx(CTRL_BUTTON, "", WS_VISIBLE | WS_CHILD, WS_EX_TRANSPARENT, keyID + i, \
			keyboardSX + i % keyCol * keyW, keyboardSY + i / keyCol * keyH, keyW, keyH, hParent, 0);
		
		if(HWND_INVALID == hKey)
		{
			UI_perror("UI_CreateKeyboardFrame", "Fail to call CreateWindow().\n");
			return HWND_INVALID;
		}
	}

	// 键盘框架创建成功之后，设置默认输入法为英文小写。
	emCurrMethod = EM_METHOD_LOWER;
	hKeyboard = GetDlgItem(hParent, keyID);
	
	return hKeyboard;
}

/*-----------------------------------------------------------------------------
描   述：获取当前的输入法。
参 数：hKB 键盘句柄。
返回值：无
注 意：
-----------------------------------------------------------------------------*/
EM_METHOD_T UI_GetInputMethod(HWND hKB)
{
	// if(hKB == hKeyboard)
	
	return emCurrMethod;
}

/*-----------------------------------------------------------------------------
描   述：设置输入法。
参	数：hKB 键盘句柄；emMethod 目标输入法。
返回值：成功，返回新设置的输入法；失败，返回无效输入法的枚举值。
注	意：
-----------------------------------------------------------------------------*/
EM_METHOD_T UI_SetInputMethod(HWND hKB, EM_METHOD_T emMethod)
{
	// if(hKB == hKeyboard)
	const char **p = NULL;

	switch(emMethod)
	{
		case EM_METHOD_LOWER:
			emCurrMethod = EM_METHOD_LOWER;
			p = lowerLetters;
			break;
		case EM_METHOD_UPPER:
			emCurrMethod = EM_METHOD_UPPER;
			p = upperLetters;
			break;
		case EM_METHOD_SYMBOOL:
			emCurrMethod = EM_METHOD_SYMBOOL;
			p = symbool;
			break;
		case EM_METHOD_CAPSLOCK:
		{			
			bool isLowerMethod = false;

			isLowerMethod = KB_isLowerMethod(emCurrMethod);
			isLowerMethod ? p = upperLetters : p = lowerLetters;
			isLowerMethod ? emCurrMethod = EM_METHOD_UPPER : emCurrMethod = EM_METHOD_LOWER;
			break;
		}
			break;
		default:
			return EM_METHOD_MAX;
	}

	int i = 0;	
	for(i = 0; i < keyNum; i++)
	{
		SetWindowText(KB_GetKeyItem(hKeyboard, ID_KEY + i), p[i]);
		//printf("%s\n", p[i]);
	}

	return emCurrMethod;
}

/*-----------------------------------------------------------------------------
描   述：根据传入的id 值设置键盘中部分按键的ID。
参	数：id 键盘上第一个按键的起始ID.
返回值：无
注	意：
-----------------------------------------------------------------------------*/
static void KB_SetKeyID(int id)
{
	ID_KEY = id;
}

/*-----------------------------------------------------------------------------
描   述：获取按键的句柄。
参	数：hKeyboard 键盘句柄，通过UI_CreateKeyboardFrame 得到。keyID 按键ID.
返回值：无
注	意：
-----------------------------------------------------------------------------*/
static HWND KB_GetKeyItem(HWND hKeyboard, int keyID)
{
	return GetDlgItem(GetParent(hKeyboard), keyID);
}

/*-----------------------------------------------------------------------------
描   述：判断是不是小写输入法。
参 数：emMethod 输入法的枚举类型。
返回值：布尔值
注 意：
-----------------------------------------------------------------------------*/
static bool KB_isLowerMethod(EM_METHOD_T emMethod)
{
	return (EM_METHOD_LOWER ==  emMethod);
}

/*-----------------------------------------------------------------------------
描   述：判断是不是大写输入法。
参 数：emMethod 输入法的枚举类型。
返回值：布尔值
注 意：
-----------------------------------------------------------------------------*/
static bool KB_isUpperMethod(EM_METHOD_T emMethod)
{
	return (EM_METHOD_UPPER ==  emMethod);
}

/*-----------------------------------------------------------------------------
描   述：判断是不是英文输入法。
参	数：emMethod 输入法的枚举类型。
返回值：布尔值
注	意：
-----------------------------------------------------------------------------*/
static bool KB_isEnglishMethod(EM_METHOD_T emMethod)
{
	return (KB_isLowerMethod(emMethod) || KB_isUpperMethod(emMethod));
}

/*-----------------------------------------------------------------------------
描   述：判断是不是符号输入法。
参	数：emMethod 输入法的枚举类型。
返回值：布尔值
注	意：
-----------------------------------------------------------------------------*/
static bool KB_isSymboolMethod(EM_METHOD_T emMethod)
{
	return (EM_METHOD_SYMBOOL ==  emMethod);
}

/*-----------------------------------------------------------------------------
描   述：判断是不是按键ID.
参	数：id 要判断的控件ID.
返回值：布尔值
注	意：
-----------------------------------------------------------------------------*/
bool isKeyID(int id)
{
	return ((id >= ID_KEY) && (id < ID_KEY + keyNum));
}

/*-----------------------------------------------------------------------------
描   述：从键盘获取char 字符。
参	数：hKB 键盘句柄; keyID 按键ID号.
返回值：成功，返回获取的字符；失败，返回-1.
注	意：
-----------------------------------------------------------------------------*/
char UI_GetCharFromKeyID(HWND hKB, int keyID)
{
	if(hKB != hKeyboard || !isKeyID(keyID))
	{
		UI_perror("UI_GetCharFromKeyboardID", "Bad argument.\n");
		return 0;
	}

	const char **p = NULL;				// p 将指向字符串数组。
	const int index = keyID - ID_KEY;	// id 相减得到按键对应的字符在数组中的下标。

	switch(index)
	{
		// 特殊按键处理处理：退格、大小写切换、符号切换、空格
		case EM_INDEX_BACKSPACE:
			return EM_ASCII_BACKSPACE;
			break;
		case EM_INDEX_CAPSLOCK:
			return EM_ASCII_CAPSLOCK;
			break;
		case EM_INDEX_SYMBOOL:
			return EM_ASCII_SYMBOOL;
			break;
		case EM_INDEX_SPACE:
			return EM_ASCII_SPACE;
			break;
		case EM_INDEX_MAX:
			return EM_ASCII_MAX;
			break;
		// 一般按键处理：
		default:
		{
			EM_METHOD_T emMethod = EM_METHOD_MAX;

			emMethod = UI_GetInputMethod(hKB);
			KB_isSymboolMethod(emMethod) ? p = symbool :
				KB_isLowerMethod(emMethod) ? p = lowerLetters : 
					KB_isUpperMethod(emMethod) ? p = upperLetters : NULL;
			break;
		}
		break;
	}

	return ((NULL == p) ? (-1) : (p[index][0]));
}
