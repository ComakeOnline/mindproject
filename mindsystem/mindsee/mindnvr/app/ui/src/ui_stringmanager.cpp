#include "ui_stringmanager.hpp"
#include "minigui_entry.hpp"
#include <stdio.h>

/*-----------------------------------------------------------------------------
描   述：在字符串末尾追加字符。
参	数：str 目标字符串；bufSize 字符串缓冲区的长度；ch 要追加的字符。
返回值：返回指向字符串的指针.
注	意：如果追加的退格字符，则清除前一个字符。
-----------------------------------------------------------------------------*/
char *UI_AppendChar2String(char *str, size_t bufSize, char ch)
{
	if(NULL == str || bufSize <= 1)
	{
		UI_perror("UI_AppendChar2String", "Bad argument!\n");
		return str;
	}

	// 处理退格
	if(ASCII_BACKSPACE == ch || ASCII_DELETE == ch)
	{
		return UI_RemoveCharFromString(str, bufSize);
	}

	size_t len = 0;
	
	len = strlen(str);
	if(len >= (bufSize - 1))
	{
		UI_perror("UI_AppendChar2String", "strlen() >= bufSize - 1, overflower!\n");
		return str;
	}

	str[len + 1] = '\0';
	str[len] = ch;

	return str;
}

/*-----------------------------------------------------------------------------
描   述：从字符串末尾移除字符。
参	数：str 目标字符串；bufSize 字符串缓冲区的长度。
返回值：返回指向字符串的指针.
注	意：
-----------------------------------------------------------------------------*/
char *UI_RemoveCharFromString(char *str, size_t bufSize)
{
	if(NULL == str || bufSize <= 0)
	{
		UI_perror("UI_RemoveCharFromString", "Bad arguments!\n");
		return str;
	}

	size_t len = 0;
	
	len = strlen(str);
	if(len > bufSize)
	{
		UI_perror("UI_RemoveCharFromString", "strlen() > bufSize, overflower!\n");
		return str;
	}

	if(0 == len)
	{
		return str;
	}

	//printf("len = %d\n", len);
	str[len - 1] = '\0';

	return str;
}

/*-----------------------------------------------------------------------------
描   述：清除字符串缓冲区。
参	数：str 目标字符串；bufSize 字符串缓冲区的长度。
返回值：返回指向字符串的指针.
注	意：
-----------------------------------------------------------------------------*/
char *UI_ClearStringBuf(char *str, size_t bufSize)
{
	memset(str, bufSize, 0);

	return str;
}

/*-----------------------------------------------------------------------------
描   述：返回字符串的最后一个字符。
参	数：str 目标字符串；
返回值：如果字符串不为空，则返回最后一个字符；否则，返回'\0'
注	意：
-----------------------------------------------------------------------------*/
char UI_LastChar(const char *str)
{
	size_t len = 0;

	char ch = 0;
	len = strlen(str);
	(0 == len) ? ch = 0 : ch = str[len - 1];

	return ch;
}

