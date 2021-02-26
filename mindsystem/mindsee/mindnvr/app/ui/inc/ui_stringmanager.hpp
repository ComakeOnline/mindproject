#ifndef __UI_STRINGMANAGER_HPP__
#define __UI_STRINGMANAGER_HPP__

#include <string.h>

#define ASCII_BACKSPACE 0x08
#define ASCII_DELETE 0x7F

char UI_LastChar(const char *str);
char *UI_ClearStringBuf(char *str, size_t bufSize);
char *UI_RemoveCharFromString(char *str, size_t bufSize);
char *UI_AppendChar2String(char *str, size_t bufSize, char ch);

#endif
