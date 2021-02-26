#ifndef __MINIGUI_RESOURCE_H__
#define __MINIGUI_RESOURCE_H__

#define RESOLUTION_WIDTH 1024
#define RESOLUTION_HEIGHT 600

#define DIALOG_WIDTH 700
#define DIALOG_HEIGHT 400
#define DIALOG_STARTX (((RESOLUTION_WIDTH) - (DIALOG_WIDTH)) / 2)
#define DIALOG_STARTY (((RESOLUTION_HEIGHT) - (DIALOG_HEIGHT)) / 2)

extern bool inOSD;
extern bool inSingleFrame;
extern bool inFourFrame;
extern bool inIconScreen;

extern PLOGFONT pLogFont15;
extern PLOGFONT pLogFont20;
extern PLOGFONT pLogFont25;
extern PLOGFONT pLogFont30;
extern PLOGFONT pLogFont35;
extern PLOGFONT pLogFont40;
extern PLOGFONT pLogFont45;
extern PLOGFONT pLogFont50;

extern HWND hMainScreen;
extern HWND hIconScreen;
extern HWND hFrameWnd;

extern double timeCnt1000ms;

extern int ID_START;

extern PBITMAP const pBitmapBack;
extern const char *bitmapBackPath;

extern PBITMAP const pBitmapEnsure;
extern const char *bitmapEnsurePath;

#endif
