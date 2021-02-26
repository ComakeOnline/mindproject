/* 
UI 滑动图标页
在IPC 四画面中长按屏幕唤出，可向左向右滑动图标，点击最后一个图标退出。
*/

#include "minigui_entry.hpp"
#include "minigui_resource.hpp"
#include "minigui_iconscreen.hpp"

#include "icon_call.hpp"
#include "icon_vdec.hpp"
#include "icon_wlan.hpp"
#include "icon_setting.hpp"
#include "icon_settime.hpp"
#include "icon_setnet.hpp"
#include "icon_nvroptions.hpp"
#include "icon_storagespace.hpp"
#include "icon_IPCManager.hpp"
#include "icon_videolist.hpp"
#include "icon_sysinfo.hpp"
#include "icon_topology.hpp"

#include <sys/time.h>

//图标尺寸及起始位置
static const int iconNum = 9;
static const int iconWidth = 100;
static const int iconHeight = iconWidth;
static const int iconIntervalX = iconWidth / 2;
static const int iconStartX = 25;
static const int iconStartY = 450;

static const int ID_TIMER0 = 31;

static bool enMove = false;
//图标位置信息的结构体
static st_iconInfo_t st_pos[iconNum] = {{0}};

//资源文件
static BITMAP iconHomePage00 = {0};
PBITMAP const pIconHomePage00 = &iconHomePage00;
const char *iconHomePage00Path = "./res/21.png";

static BITMAP iconHomePage01 = {0};
PBITMAP const pIconHomePage01 = &iconHomePage01;
const char *iconHomePage01Path = "./res/22.png";

static BITMAP iconHomePage02 = {0};
PBITMAP const pIconHomePage02 = &iconHomePage02;
const char *iconHomePage02Path = "./res/23.png";

static BITMAP iconHomePage03 = {0};
PBITMAP const pIconHomePage03 = &iconHomePage03;
const char *iconHomePage03Path = "./res/24.png";

static BITMAP iconHomePage04 = {0};
PBITMAP const pIconHomePage04 = &iconHomePage04;
const char *iconHomePage04Path = "./res/25.png";

static BITMAP iconHomePage05 = {0};
PBITMAP const pIconHomePage05 = &iconHomePage05;
const char *iconHomePage05Path = "./res/26.png";

static BITMAP iconHomePage06 = {0};
PBITMAP const pIconHomePage06 = &iconHomePage06;
const char *iconHomePage06Path = "./res/27.png";

static BITMAP iconHomePage07 = {0};
PBITMAP const pIconHomePage07 = &iconHomePage07;
const char *iconHomePage07Path = "./res/28.png";

static BITMAP iconHomePage08 = {0};
PBITMAP const pIconHomePage08 = &iconHomePage08;
const char *iconHomePage08Path = "./res/29.png";

static BITMAP iconHomePage09 = {0};
PBITMAP const pIconHomePage09 = &iconHomePage09;
const char *iconHomePage09Path = "./res/31.png";

static BITMAP iconHomePage10 = {0};
PBITMAP const pIconHomePage10 = &iconHomePage10;
const char *iconHomePage10Path = "./res/31.png";

static BITMAP iconHomePage11 = {0};
PBITMAP const pIconHomePage11 = &iconHomePage11;
const char *iconHomePage11Path = "./res/32.png";

PBITMAP const pIconHomePageArr[24] = {pIconHomePage00, pIconHomePage01, pIconHomePage02, pIconHomePage03, pIconHomePage04, \
			pIconHomePage05, pIconHomePage06, pIconHomePage07, pIconHomePage08, pIconHomePage09, pIconHomePage10, pIconHomePage11};
			
const char *iconHomePagePathArr[24] = {iconHomePage01Path, iconHomePage02Path, iconHomePage03Path, iconHomePage04Path, \
	iconHomePage05Path, iconHomePage06Path, iconHomePage07Path, iconHomePage08Path, iconHomePage09Path, iconHomePage10Path, iconHomePage11Path, };

static int UI_LoadResourceInIconScreen();
static int UI_UnloadResourceInIconScreen();

static int UI_HandleKeyUp(HWND hWnd, LPARAM lParam);
#ifndef MG4_0_4
static LRESULT UI_ProcIconScreen(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
#else
static LRESULT UI_ProcIconScreen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

// 绘图相关函数
static int UI_InvalidateRectArr(int offsetX);
static int UI_InvalidateRectBoundary(int offsetX);
static int UI_FillBoxWithBitmapArr();
static int UI_BitBltArr(int offsetX);
static int UI_FillBoxWithBitmapPartArr(int offsetX);

// 坐标计算相关函数
static int UI_GetOffsetX(int *newX, int *oldX, LPARAM lParam);
static int UI_GetValidOffsetX(int offsetX, int minOffset, int maxOffset, int ratio);
static void UI_IconPositionInit(void);
static int UI_IconPositionUpdate(int offset);

/*-----------------------------------------------------------------------------
描   述：主页，包含滑动图标
返回值：成功返回0. 失败返回-1
-----------------------------------------------------------------------------*/
int UI_IconScreen(void)
{
	UI_print("enter UI_IconScreen().");
	
	MSG msg;
	HDC hdc;
	RECT tmpRect;

	/*创建主窗口*/
	MAINWINCREATE mainWinInf = {
		WS_VISIBLE,									//仅可见，不要边框和标题
		WS_EX_NONE,
		"topic",		//标题栏。此处不显示
		0,
		GetSystemCursor(0), 0,						//使用默认光标，无图标。
		hMainScreen,	//托管窗口设为hMainScreen, 如果设为HWND_DESKTOP 会在结束窗口时将HWND_DESKTOP 下的所有窗口都结束
		UI_ProcIconScreen,							//窗口过程函数
		0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT,	//lx, ty, rx, by
		(int)(gal_pixel)COLOR_blue,							//蓝色背景（因为FB 将blue 设为color key, 所以实际上为透明）
		0, 0,										//无额外数据
	};	

	hIconScreen = CreateMainWindow(&mainWinInf);		//创建主窗口后获取窗口句柄
	if(HWND_INVALID == hIconScreen)
	{
		UI_perror("UI_IconScreen", "fail to call CreateMainWindow.\n");
		return -1;
	}

	ShowWindow(hIconScreen, SW_SHOWNORMAL);

	while(GetMessage(&msg, hIconScreen))				//主窗口进入消息循环模式
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	MainWindowThreadCleanup(hIconScreen);
	hIconScreen = HWND_INVALID;
	
	UI_print("exit UI_IconScreen().");
	return 0;
}

/*-----------------------------------------------------------------------------
描   述：主页的窗口过程函数
返回值：符合过程函数规范的返回值
-----------------------------------------------------------------------------*/
#ifndef MG4_0_4
static LRESULT UI_ProcIconScreen(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
static LRESULT UI_ProcIconScreen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{		
	//控件数量、句柄、宽、高、起始坐标 相关变量定义
	static int newX = 0;
	static int oldX = 0;
	static int offsetX = 0;
	static int validOffsetX = 0;
	static int firstTouch = true;
	static int firstEnter = true;
	static int ignoreMsgUp = false;

	//printf("message = %#x, wParam = %#x\n", message, wParam);

	switch(message)
	{
		case MSG_CREATE:
		{
			inFourFrame = false;
			inIconScreen = true;
			enMove = false;
			
			newX = 0;
			oldX = 0;
			offsetX = 0;
			firstEnter = true;
			firstTouch = true;
			ignoreMsgUp = false;

			// 在消息循环中设置定时器，定时器超时会发送MSG_TIMER 消息，用于滑动结束后的动画过渡效果。
			SetTimer(hWnd, ID_TIMER0, 3);
			
			UI_IconPositionInit();
			UI_LoadResourceInIconScreen();

			break;
		}
		case MSG_PAINT:
		{
			//UI_print("Receive MSG_PAINT.");
			// MSG_PAINT 消息时进行绘图的消息，收到该消息后，需要分情况处理。
			// 如果是第一次进入滑动图标页，则加载图标资源并显示图标，然后break 跳出，
			// 在后续的DefaultMainWinProc() 中由minigui 进行绘图。
			// 如果不是第一次进入滑动图标页，则不再重复加载图标资源，
			// 直接在后续的DefaultMainWinProc() 中进行绘图。
			if(firstEnter)
			{
				UI_FillBoxWithBitmapArr();
				firstEnter = false;
			}
			break;
		}
		case MSG_CLOSE:
		{
			UI_print("Recv MSG_CLOSE -- BEGIN.\n");
			// 接收到关闭消息时，设置相关标志位，清理资源，擦除窗口，销毁窗口。
			inFourFrame = true;
			inIconScreen = false;

			UI_UnloadResourceInIconScreen();
			RECT rect = {0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT};
			InvalidateRect(hIconScreen, &rect, 1);
			SendMessage(hWnd, MSG_PAINT, 0, 0);
			DestroyMainWindow(hWnd);
			// 不能投递MSG_QUIT, 处于同一线程的窗口共享同一消息队列。
			//PostQuitMessage(hWnd);
			return 0;
		}			
		case MSG_COMMAND:
			break;
		case MSG_ERASEBKGND:
			//UI_print("Receive MSG_ERASEBKGND.");
			break;
			
		// 当触摸消息来临时，先后触发的是MSG_MOUSEMOVE, MSG_LBUTTONDOWN, MSG_LBUTTONUP.
		// 这一点重要！！！
		// 因为触摸到屏幕的一瞬间，是不需要处理MSG_MOUSEMOVE 的，只有在第二次收到MSG_MOUSEMOVE 时才认为有滑动动作。
		case MSG_LBUTTONUP:
			//UI_print("3 Detect touch up!\n");
			//printf("offset = %d\n", offsetX);
			firstTouch = false;

			// 如果validOffsetX 不为0, 说明曾有滑动动作，则当触摸抬起时，启用滑动过渡效果。
			if(validOffsetX)
			{
				validOffsetX < 0 ? validOffsetX = -30 : validOffsetX = 30;
				enMove = true;	// 启用滑动过渡效果。
				oldX = newX = 0;
				break;
			}

			// 如果需要忽略抬起的动作，则直接返回。
			if(ignoreMsgUp)
			{
				ignoreMsgUp = false;
				oldX = newX = 0;
				break;
			}

			// 处理触摸抬起的动作。
			UI_HandleKeyUp(hWnd, lParam);
			firstEnter = true;
			firstTouch = true;
			oldX = newX = 0;
		
			break;
		case MSG_LBUTTONDOWN:
			//UI_print("2 Detect touch down!");

			// 触摸按下时不处理，抬起才处理。按下时只记录坐标值。
			ignoreMsgUp = false;
			oldX = LOWORD(lParam);
			firstTouch = false;
				
			break;
		case MSG_MOUSEMOVE:
			//UI_print("1 Detect touch move!\n");
			//忽略第一次的MSG_MOUSEMOVE 消息，从第二次起计算与上次的坐标偏移量。
			if(firstTouch)
			{
				break;
			}

			// 获取有效偏移量
			offsetX = UI_GetOffsetX(&newX, &oldX, lParam);
			//printf("offset = %d\n", offsetX);
			offsetX = UI_GetValidOffsetX(offsetX, 2, iconWidth, 4);
			//printf("validOffset = %d\n", offsetX);

			if(0 == offsetX)
			{
				break;
			}

			validOffsetX = offsetX;

			//保留部分无需重绘的DC
			UI_BitBltArr(validOffsetX);
			//使边界区域无效
			UI_InvalidateRectBoundary(validOffsetX);
			//重绘图标
			SendMessage(hWnd, MSG_PAINT, wParam, lParam);
			//更新部分需要重绘的DC.
			UI_FillBoxWithBitmapPartArr(validOffsetX);
			//更新坐标信息。
			UI_IconPositionUpdate(validOffsetX);
			//重绘图标
			SendMessage(hWnd, MSG_PAINT, wParam, lParam);

			// 处理了滑动消息，则不再处理按键抬起的消息。
			ignoreMsgUp = true;

			break;
		case MSG_MOVE:
		{
			// 自定义的消息类型MSG_MOVE, 用于滑动结束后的动画过渡效果。
			int offsetX = (int)wParam;
		
			offsetX = UI_GetValidOffsetX(offsetX, 0, iconWidth, 0);
			//printf("offsetX = %d\n", offsetX);
			if(0 == offsetX)
			{
				validOffsetX = 0;
			}
			
			//保留部分无需重绘的DC
			UI_BitBltArr(offsetX);
			//使边界区域无效
			UI_InvalidateRectBoundary(offsetX);
			//重绘图标
			SendMessage(hWnd, MSG_PAINT, wParam, lParam);
			//更新部分需要重绘的DC.
			UI_FillBoxWithBitmapPartArr(offsetX);
			//更新坐标信息。
			UI_IconPositionUpdate(offsetX);
			//重绘图标
			SendMessage(hWnd, MSG_PAINT, wParam, lParam);
		
		}
		break;
		case MSG_TIMER:
		{
			// 定时器超时消息，用于滑动结束后的动画过渡效果。
			if(enMove)
			{
				//UI_print("MSG_TIMER.\n");
				//for(int i = 0; i < 5; i++)
				{
					SendMessage(hWnd, MSG_MOVE, (WPARAM)validOffsetX, 0);
				}

				validOffsetX /= 1.1;		// 滑动过渡效果的移动步长自减。
				if(0 == validOffsetX)
				{
					enMove = false;
				}
			}
			break;
		}
		case MSG_IDLE:
			break;
		default:
			break;
	}

	return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

/*-----------------------------------------------------------------------------
描   述：图标位置初始化
返回值：无
-----------------------------------------------------------------------------*/
static void UI_IconPositionInit(void)
{
	int i = 0;

	for(i = 0; i < iconNum; i++)
	{
		st_pos[i].icon_startX = iconStartX + (iconIntervalX + iconWidth) * i;
		st_pos[i].icon_startY = iconStartY;
		st_pos[i].icon_picture = pIconHomePageArr[i];
	}
}

/*-----------------------------------------------------------------------------
描   述：图标位置更新
返回值：成功返回0，失败返回-1.
-----------------------------------------------------------------------------*/
static int UI_IconPositionUpdate(int offset)
{
	int i = 0;

	for(i = 0; i < iconNum; i++)
	{
		st_pos[i].icon_startX += offset;
		//printf("st_pos[%d].icon_startX = %d\n", i, st_pos[i].icon_startX);
	}

	return 0;
}

/*-----------------------------------------------------------------------------
描   述：擦除处的矩形区域。
返回值：成功返回0，失败返回-1.
-----------------------------------------------------------------------------*/
static int UI_InvalidateRectArr(int offsetX)
{
	if(ABS_Int(offsetX) > RESOLUTION_WIDTH)
	{
		UI_perror("UI_InvalidateRectArr", "Bad argument.\n");
		return -1;
	}

	int i = 0;
	RECT rect = {0, 0, 0, 0};

	rect.top = iconStartY;
	rect.bottom = iconStartY + iconHeight;

	for(i = 0; i < iconNum - 1; i++)
	{
		rect.left = st_pos[i].icon_startX + iconWidth - 1;
		rect.right = st_pos[i + 1].icon_startX + 1;
		rect.left < 0 ? rect.left = 0 : 0;
		rect.right < 0 ? rect.right = 0 : 0;

		if(rect.left > RESOLUTION_WIDTH)
		{
			continue;
		}

		InvalidateRect(hIconScreen, &rect, 1);
	}

	// 后一个图标特殊处理
	if(st_pos[iconNum - 1].icon_startX + iconWidth < RESOLUTION_WIDTH)
	{
		rect.left = st_pos[iconNum - 1].icon_startX + iconWidth;
		rect.right = RESOLUTION_WIDTH;
	
		InvalidateRect(hIconScreen, &rect, 1);
	}

	// 一个图标特殊处理
	if(st_pos[0].icon_startX > 0)
	{
		rect.left = 0;
		rect.right = st_pos[0].icon_startX;
	
		InvalidateRect(hIconScreen, &rect, 1);
	}
	
	return 0;
}

/*-----------------------------------------------------------------------------
描   述：使用位图填充图标
返回值：成功返回0，失败返回-1.
-----------------------------------------------------------------------------*/
static int UI_FillBoxWithBitmapArr()
{
	HDC hDC;
	int i = 0;
	int ret = 0;

	// 在MSG_PAINT 消息中，使用BeginPaint() 和EndPaint() 进行绘图操作。
	hDC = BeginPaint(hIconScreen);
	for(i = 0; i < iconNum; i++)
	{
		if(st_pos[i].icon_startX > RESOLUTION_WIDTH || st_pos[i].icon_startX + iconWidth < 0)
		{
			continue;
		}

		if(true != FillBoxWithBitmap(hDC, st_pos[i].icon_startX , st_pos[i].icon_startY, iconWidth, iconHeight, st_pos[i].icon_picture))
		{
			ret = -1;
			UI_perror("UI_FillBoxWithBitmapArr", "Fail to call FillBoxWithBitmap().\n");
		}
	}
	EndPaint(hIconScreen, hDC);

	return ret;
}

/*-----------------------------------------------------------------------------
描   述：对图标中无需重绘的区域使用BitBlt 进行移动
返回值：成功返回0，失败返回-1.
-----------------------------------------------------------------------------*/
static int UI_BitBltArr(int offsetX)
{
	HDC hDC = HDC_INVALID;

	// BitBlt() 时需要使用GetDC(), ReleaseDC() 获取和释放画布。
	hDC = GetDC(hIconScreen);
	//BitBlt(hDC, sx, sy, sw, sh, hDC, dx, dy, 0);
	BitBlt(hDC, 0, iconStartY, RESOLUTION_WIDTH, iconHeight, hDC, offsetX, iconStartY, 0);
	ReleaseDC(hDC);

	return 0;	
}

/*-----------------------------------------------------------------------------
描   述：对图标中需要重绘的部分区域，使用FillBoxWithBitmapPart() 进行部分重绘。
返回值：成功返回0，失败返回-1.
-----------------------------------------------------------------------------*/
static int UI_FillBoxWithBitmapPartArr(int offsetX)
{
	if(0 == offsetX)
	{
		UI_print("Return from UI_FillBoxWithBitmapPartArr(), OffsetX has 0 value.\n");
		return 0;
	}

	int i = 0;
	int left0 = 0;
	int left1 = 0;
	int right0 = 0;
	int right1 = 0;
	int offset = 0;
	HDC hDC = HDC_INVALID;

	RECT rect = {0, 0, 0, 0};
	hDC = GetDC(hIconScreen);
	if(offsetX > 0)		// 向右移动
	{
		offset = offsetX;
		for(i = 0; i < iconNum; i++)
		{
			left0 = st_pos[i].icon_startX;
			left1 = left0 + offset;			
			right1 = left1 + iconWidth;

			// 如果异动前的左边界大于0, 或者移动后的右边界小于0, 
			// 则不处理，因为已经在其它函数中通过BitBlt() 操作进行了移动。
			//printf("left0, left1 = %d, %d\n", left0, left1);
			if(left0 > 0 || right1 < 0)
			{
				continue;
			}

			//printf("Call FillBoxWithBitmap().\n");
			//printf("left0, left1 = %d, %d\n", left0, left1);
			if(TRUE != FillBoxWithBitmap(hDC, left1, iconStartY, iconWidth, iconHeight, st_pos[i].icon_picture))
			{
				UI_perror("UI_FillBoxWithBitmapPartArr", "Fail to call FillBoxWithBitmap.\n");
			}
			break;
		}
	}
	else if(offsetX < 0)
	{
		offset = -offsetX;
		for(i = iconNum - 1; i > -1; i--)
		{
			left0 = st_pos[i].icon_startX;
			right0 = left0 + iconWidth;
			left1 = left0 - offset;

			if(right0 < RESOLUTION_WIDTH || left1 > RESOLUTION_WIDTH)
			{
				continue;
			}

			//printf("left0, left1 = %d, %d\n", left0, left1);
			if(TRUE != FillBoxWithBitmap(hDC, left1, iconStartY, iconWidth, iconHeight, st_pos[i].icon_picture))
			{
				UI_perror("UI_FillBoxWithBitmapPartArr", "Fail to call FillBoxWithBitmap.\n");
			}
			break;
		}
	}
	ReleaseDC(hDC);	
	
	return 0;
}

/*-----------------------------------------------------------------------------
描述：擦除矩形边界
参数：hWnd 父窗口句柄，lParam 包含坐标信息的参数。
返回值：返回有效偏移量或者0.
-----------------------------------------------------------------------------*/
static int UI_InvalidateRectBoundary(int offsetX)
{
	// 矩形的坐标：left, top, right, bottom
	RECT rect = {0, iconStartY, 0, iconStartY + iconHeight};

	// 根据偏移量计算要擦除的矩形的坐标。
	if(offsetX > 0)
	{	
		rect.left = 0;
		rect.right = offsetX;
		//printf("rect.left, right, top, bottom = %d, %d, %d, %d\n", rect.left, rect.right, rect.top, rect.bottom);
	}
	else if(offsetX < 0)
	{
		rect.left = RESOLUTION_WIDTH + offsetX;
		rect.right = RESOLUTION_WIDTH;
		//printf("rect.left, right, top, bottom = %d, %d, %d, %d\n", rect.left, rect.right, rect.top, rect.bottom);
	}
	
	InvalidateRect(hIconScreen, &rect, 1);
	
	return 0;
}

/*-----------------------------------------------------------------------------
描述：获取有效的X偏移量
参数：newX, oldX 指向新坐标和旧坐标的指针，minOffset, maxOffset 允许的最小/最大偏移量
		ratio 偏移量比例因子，用于调节屏幕灵敏度，范围-30 ~ +30.
返回值：成功返回0，失败返回-9999.
-----------------------------------------------------------------------------*/
static int UI_GetOffsetX(int *newX, int *oldX, LPARAM lParam)
{
	if(NULL == newX || NULL == oldX)
	{
		UI_perror("UI_GetOffsetX", "Bad argument.\n");
		return 0;
	}

	int x = 0;
	int offsetX = 0;
	
	*newX = UI_GetTouchX(lParam);
	offsetX = *newX - *oldX;
	*oldX = *newX;
	
	return offsetX;
}

/*-----------------------------------------------------------------------------
描述：获取有效的X偏移量
参数：offsetX 未经处理的偏移量。
返回值：返回有效偏移量或者0.
-----------------------------------------------------------------------------*/
static int UI_GetValidOffsetX(int offsetX, int minOffset, int maxOffset, int ratio)
{
	if(0 == offsetX || minOffset < 0 || maxOffset < minOffset || ABS_Int(ratio) > 30)
	{
		UI_perror("UI_GetValidOffsetX", "Bad argument.\n");
		return 0;
	}

	offsetX += offsetX * (ratio / 10.0);

	if(ABS_Int(offsetX) < minOffset || ABS_Int(offsetX) > maxOffset)
	{
		UI_perror("UI_GetOffsetX", "X offset is out of range.\n");
		offsetX = 0;
		return 0;
	}
	

	//如果向右滑动 并且 最左边的图标的起始位置加上偏移量后 >= 图标的最大间隔
	//则重定义偏移量。
	if(offsetX > 0 &&  !((st_pos[0].icon_startX + offsetX) < iconIntervalX))
	{
		offsetX = iconIntervalX - st_pos[0].icon_startX;
	}
	//如果是向左滑动 并且 最右端的图标的右边界加上偏移量后 <= 图标的最大间隔
	//则重定义偏移量
	else if(offsetX < 0 && !((st_pos[iconNum - 1].icon_startX + iconWidth + offsetX) > (RESOLUTION_WIDTH - iconIntervalX)))
	{
		offsetX = RESOLUTION_WIDTH - st_pos[iconNum - 1].icon_startX - iconWidth - iconIntervalX;
	}

	if(ABS_Int(offsetX) > iconWidth)
	{
		offsetX = iconWidth;
	}
	
	return offsetX;
}

// 函数指针数组，每一个图标对应一个功能函数。
int (*UI_OnIcon[iconNum])(int) = {UI_OnIconWlan, UI_OnIconSetting, UI_OnIconNvrOptions, UI_OnIconSetNet, 
	UI_OnIconStorageSpace, UI_OnIconIPCManager, UI_OnIconVideoList, UI_OnIconSysInfo, NULL};

/*-----------------------------------------------------------------------------
描述：处理触摸抬起的动作
参数：hWnd 父窗口句柄，lParam 包含坐标信息的参数。
返回值：成功返回0.
-----------------------------------------------------------------------------*/
static int UI_HandleKeyUp(HWND hWnd, LPARAM lParam)
{
	int i = 0;
	int x = 0;
	int y = 0;
	int ret = 0;

	// 遇到无效Y 坐标则直接返回。
	y = UI_GetTouchY(lParam);
	if(y < iconStartY || y > iconStartY + iconHeight)
	{
		UI_print("Touch not in icon.\n");
		return 0;
	}

	// 根据X 坐标获取触摸的图标的标号。
	x = UI_GetTouchX(lParam);
	for(i = 0; i < iconNum; i++)
	{
		if(x > st_pos[i].icon_startX && x < st_pos[i].icon_startX + iconWidth)
		{
			break;
		}
	}

	// 如果标号超出范围，则直接返回。
	if(iconNum == i)
	{
		printf("Touch nothing.\n");
		return 0;
	}

	// 如果触摸的是最后一个图标，则关闭滑动图标页。
	if(iconNum - 1 == i)
	{
		PostMessage(hWnd, MSG_CLOSE, 0, lParam);
	}

	//函数指针不为NULL 则执行该函数。
	UI_OnIcon[i] ? UI_OnIcon[i](i) : NULL;
	UI_FillBoxWithBitmapArr();
	// SendMessage(hIconScreen, MSG_PAINT, 0, lParam);
	
	return 0;	
}

/*-----------------------------------------------------------------------------
描述：加载资源
参数：无
返回值：返回0.
-----------------------------------------------------------------------------*/
static int UI_LoadResourceInIconScreen()
{
	//连续加载图标资源
	for(int i = 0; i < iconNum; ++i)
	{
		if(0 != LoadBitmapFromFile(HDC_SCREEN, pIconHomePageArr[i], iconHomePagePathArr[i]))
		{
			UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile().\n");
		}
	}
	
	return 0;
}

/*-----------------------------------------------------------------------------
描述：卸载资源
参数：无
返回值：返回0.
-----------------------------------------------------------------------------*/
static int UI_UnloadResourceInIconScreen()
{
	for(int i = 0; i < iconNum; ++i)
	{
		UnloadBitmap(pIconHomePageArr[i]);
	}

	return 0;
}

