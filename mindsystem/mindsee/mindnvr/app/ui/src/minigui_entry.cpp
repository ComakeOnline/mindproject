/*
UI 入口函数模块
*/

#include <fcntl.h>
#include <pthread.h>
#include <linux/input.h>

#include "fb_clrkey.hpp"
#include "minigui_entry.hpp"
#include "minigui_mainscreen.hpp"
#include "minigui_bootscreen.hpp"
#include "minigui_test.hpp"

extern bool CLOSE_STDERR;
static void *UI_MainPthread(void *arg);
static int UI_MatchInputDev(const char *inputDev);
static int UI_SwitchDevName(const char *dev0, const char *dev1);

/*-----------------------------------------------------------------------------
描   述：UI 打印信息的函数
返回值：返回printf 的结果
-----------------------------------------------------------------------------*/
int UI_print(const char *str)
{
#if 0 == EN_UIPRINT
	return 0;
#else
	return printf("[UI] %s", str);
#endif
}

/*-----------------------------------------------------------------------------
描   述：UI 打印出错信息的函数
返回值：返回fprintf 的结果
-----------------------------------------------------------------------------*/
int UI_perror(const char *func_name, const char *err_info)
{
#if 0 == EN_UIPERROR
	return 0;
#else
	return fprintf(stderr, \
		"[UI] File: %s, Line: %d, Func: %s():, error information: %s", \
		__FILE__, __LINE__, (func_name), (err_info));
#endif
}

/*-----------------------------------------------------------------------------
描   述：UI 模块加载
返回值：成功返回0. 失败返回-1
-----------------------------------------------------------------------------*/
int UI_ModuleLoad(void)
{
	UI_print("Enter UI_ModuleLoad().\n");
	
	int ret = 0;

	// 寻找适配的输入设备
	// 板子启动后，触摸事件对应的设备在/dev/input/ 下，可能是event0, 也可能是event1.
	// 需要确定下来。
	//UI_print("Call UI_MatchInputDev().\n");
	//if(0 != UI_MatchInputDev("/dev/input/event0"))
	{
		UI_perror("UI_ModuleLoad", "Fail to call UI_MatchInputDev().\n");
		UI_print("This may cause touch screen to be invalid.\n");
	}
	UI_print("End call UI_MatchInputDev().\n");

	// 初始化minigui, InitGUI() 是minigui 的内建函数，直接调用。
	UI_print("Call InitGUI().\n");
	ret = InitGUI(0, NULL);
	if(0 != ret)
	{
		UI_perror("UI_ModuleLoad", "Fail to call InitGUI().\n");
		return -1;
	}

	// 隐藏鼠标。
	ShowCursor(true);
	
	// 设置Framebuffer 要过滤掉的颜色，是实现背景透明的方式之一。
	UI_print("Call SetFbClrKey().\n");
	if(0 != SetFbClrKey(0xff0000))
	{
		UI_perror("UI_ModuleLoad", "Fail to call SetFbClrKey().\n");
	}

	// 创建字体。字体信息用结构体描述，并不占用过多内存。
	UI_print("Create logical font.\n");
	pLogFont15 = UI_CreatettfFont("simyou", "UTF-8", 15);
	if(NULL == pLogFont15)
	{
		UI_perror("UI_ModuleLoad", "Fail to call CreateLogFont.\n");
	}

	pLogFont20 = UI_CreatettfFont("simyou", "UTF-8", 20);
	if(NULL == pLogFont20)
	{
		UI_perror("UI_ModuleLoad", "Fail to call CreateLogFont.\n");
	}

	pLogFont25 = UI_CreatettfFont("simyou", "UTF-8", 25);
	if(NULL == pLogFont25)
	{
		UI_perror("UI_ModuleLoad", "Fail to call CreateLogFont.\n");
	}

	pLogFont30 = UI_CreatettfFont("simyou", "UTF-8", 30);
	if(NULL == pLogFont30)
	{
		UI_perror("UI_ModuleLoad", "Fail to call CreateLogFont.\n");
	}

	pLogFont35 = UI_CreatettfFont("simyou", "UTF-8", 35);
	if(NULL == pLogFont35)
	{
		UI_perror("UI_ModuleLoad", "Fail to call CreateLogFont.\n");
	}

	pLogFont40 = UI_CreatettfFont("simyou", "UTF-8", 40);
	if(NULL == pLogFont40)
	{
		UI_perror("UI_ModuleLoad", "Fail to call CreateLogFont.\n");
	}

	pLogFont45 = UI_CreatettfFont("simyou", "UTF-8", 45);
	if(NULL == pLogFont45)
	{
		UI_perror("UI_ModuleLoad", "Fail to call CreateLogFont.\n");
	}

	pLogFont50 = UI_CreatettfFont("simyou", "UTF-8", 50);
	if(NULL == pLogFont50)
	{
		UI_perror("UI_ModuleLoad", "Fail to call CreateLogFont.\n");
	}

	// 加载图片资源。图片资源在内存中以位图格式存在，占用内存较多。
	// 因此，不宜在UI 初始化的时候加载很多图片资源。
	UI_print("Load bitmap file.\n");
	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapBack, bitmapBackPath))
	{
		UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile().\n");
	}

	if(0 != LoadBitmapFromFile(HDC_SCREEN, pBitmapEnsure, bitmapEnsurePath))
	{
		UI_perror("UI_ModuleLoad", "Fail to call LoadBitmapFromFile().\n");
	}

	// 使能时间测试模块。
	UI_print("Enable time test modular.\n");
	enableTimeTest();

	// 关闭标准出错流。临时设置。
	if(CLOSE_STDERR)
	{
		UI_print("In UI_ModuleLoad(), close STDERR_FILENO.\n");
		close(STDERR_FILENO);
	}

	UI_print("Exit UI_ModuleLoad().\n");
	return 0;
}

/*-----------------------------------------------------------------------------
描   述：UI模块卸载
返回值：无返回值
注意：回收资源时有可能阻塞，请联系lison.guo 重新编译miniui的核心库：libminigui_ths.a
-----------------------------------------------------------------------------*/
void UI_ModuleUnload(void)
{
	UI_print("Enter UI_ModuleUnload().\n");

	disableTimeTest();

	// 销毁资源
	UI_print("clean up resource.\n");
	UnloadBitmap(pBitmapBack);
	UnloadBitmap(pBitmapEnsure);
	DestroyLogFont(pLogFont15);
	DestroyLogFont(pLogFont25);
	DestroyLogFont(pLogFont30);
	DestroyLogFont(pLogFont35);
	DestroyLogFont(pLogFont40);
	DestroyLogFont(pLogFont45);
	DestroyLogFont(pLogFont50);

	//UI_print("Call SendMessage().\n");
	//SendMessage(HWND_DESKTOP, MSG_QUIT, 0, 0);
	//SendMessage(HWND_DESKTOP, MSG_ENDSESSION, 0, 0);

	UI_print("Call pthread_cancel().\n");
	pthread_t tid = 0;
	tid = GetMainWinThread(HWND_DESKTOP);
	pthread_cancel(tid);
	UI_print("Call pthread_join().\n");
	pthread_join(tid, NULL);
	//UI_print("Call MiniGUIPanic().\n");
	// minigui 的内建函数，直接调用即可。
	//MiniGUIPanic(1);
	UI_print("Call TerminateGUI().\n");
	TerminateGUI(0);

	UI_print("Exit UI_ModuleUnload().\n");
}

/*-----------------------------------------------------------------------------
描   述：UI模块运行
返回值：成功返回0，失败返回pthread_cteate(3) 的错误码。
-----------------------------------------------------------------------------*/
int UI_ModuleRun(void)
{
	UI_print("Enter UI_ModuleRun().\n");
	
	int iRet = 0;
	pthread_attr_t attr = {0};
	unsigned stack_size = 1024 * 1024 * 2;		// 栈大小

	// 初始化线程属性。
	iRet = pthread_attr_init(&attr);
	if(0 != iRet)
	{	
		UI_perror("UI_ModuleRun()", "Fail to call pthread_attr_init().\n");
		fprintf(stderr, "%s\n", strerror(errno));
	}

	// 线程默认栈区大小为8M, 此处修改的目的是减小内存消耗，避免minigui 初始化失败。
	iRet = pthread_attr_setstacksize(&attr, stack_size);
	if(0 != iRet)
	{	
		UI_perror("UI_ModuleRun()", "Fail to call pthread_attr_setstacksize().\n");
		fprintf(stderr, "%s\n", strerror(errno));
	}

	// 创建线程
	pthread_t tid = 0;
	if(0 != (iRet = pthread_create(&tid, &attr, UI_MainPthread, NULL)))
	{
		UI_perror("UI_ModuleRun()", "Fail to call pthread_create().\n");
		fprintf(stderr, "%s\n", strerror(errno));
		return iRet;
	}

	UI_print("Exit UI_ModuleRun().\n");
    return 0;
}

/*-----------------------------------------------------------------------------
描   述：UI的主窗口，是一个1024*600的、无任何控件的窗口。
		该窗口背景色最初为蓝色，通过设置FB 的color key, 实现口透明。
		该窗口存用于显示IPC 图像，接收触摸信息，然后对触摸做出响应。
参数：无意义
返回值：void *
注意：该函数在单独的线程中运行，参考UI_ModuleRun();
-----------------------------------------------------------------------------*/
void *UI_MainPthread(void *arg)
{
	UI_print("Enter UI_MainScreen().\n");

	int iRet = 0;

	// 线程分离
	iRet = pthread_detach(pthread_self());
	if(0 != iRet)
	{
		UI_perror("UI_MainScreen", "Fail to call pthread_detach().\n");
	}
	else
	{
		UI_print("Success to call pthread_detach(3).\n");
	}

	// UI 启动界面
	UI_BootScreen();

	MSG msg;	// minigui 内建的消息结构体。

	MAINWINCREATE mainWinInf = {
		WS_VISIBLE, 								//仅可见，不要边框和标题栏
		WS_EX_NONE,									//无扩展风格
		"This main window is used for four frame screen.",	//标题栏。此处不显示
		0,											//主窗口菜单的句柄。此处无意义
		GetSystemCursor(0), 0,						//使用默认光标，无图标。
		HWND_DESKTOP,								//托管窗口设为DESKTOP
		UI_ProcMainScreen,							//窗口过程函数
		0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT,	//lx, ty, rx, by
		(int)(gal_pixel)COLOR_blue,									//蓝色背景
		0, 0,										//无额外数据
	};
	
	hMainScreen = CreateMainWindow(&mainWinInf);	//创建主窗口，返回窗口句柄
	if(HWND_INVALID == hMainScreen)
	{
		UI_perror("UI_MainScreen", "fail to call CreateMainWindow.\n");
		return 0;
	}

	ShowWindow(hMainScreen, SW_SHOWNORMAL);

	while(GetMessage(&msg, hMainScreen))			//主窗口进入消息循环模式
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}	
	MainWindowCleanup(hMainScreen);
	hMainScreen = HWND_INVALID;

	UI_print("Exit UI_MainScreen().\n");
	return 0;
}

/*-----------------------------------------------------------------------------
描   述：根据触摸坐标获取触摸点所在区域。
参数：x, y 触摸坐标。rowNum, colNum 是IPC 画面的行数和列数。
返回值：返回所在屏幕的区域。
-----------------------------------------------------------------------------*/
int UI_GetTouchArea(int x, int y, int rowNum, int colNum)
{
	// 过滤无效坐标。
	if(x < 0 || x > RESOLUTION_WIDTH || y < 0 || y > RESOLUTION_HEIGHT || !(rowNum > 0) || !(colNum > 0))
	{
		UI_perror("UI_GetTouchArea", "Bad argument.\n");
		return -1;
	}

	int i = 0;
	int j = 0;
	int rowHeight = 0;
	int colWidth = 0;

	// 根据行高计算所在行。
	colWidth = RESOLUTION_WIDTH / colNum;
	for(i = 0; i < colNum; i++)
	{
		if(x < i * colWidth)
		{
			break;
		}
	}
	i--;

	// 根据列宽计算所在列。
	rowHeight = RESOLUTION_HEIGHT / rowNum;
	for(j = 0; j < rowNum; j++)
	{
		if(y < j * rowHeight)
		{
			break;
		}
	}
	j--;

	// 根据行列计算所在区域的标号。
	return j * colNum + i;

}

/*-----------------------------------------------------------------------------
描   述：获取触摸X坐标
返回值：X坐标
-----------------------------------------------------------------------------*/
int UI_GetTouchX(LPARAM lParam)
{
	return LOWORD(lParam);
}

/*-----------------------------------------------------------------------------
描   述：获取触摸Y坐标
返回值：Y坐标
-----------------------------------------------------------------------------*/
int UI_GetTouchY(LPARAM lParam)
{
	return HIWORD(lParam);
}
 
/*-----------------------------------------------------------------------------
描   述：适配输入设备
参   数：设备名，一般为/dev/input/event0
返回值：成功返回0，失败返回-1
-----------------------------------------------------------------------------*/
static int UI_MatchInputDev(const char *inputDev)
{
	if(NULL == inputDev)
	{
		UI_perror("UI_MatchInputDev", "Bad argument.\n");
		return -1;
	}

	int fd = -1;
	const unsigned int bufSize = 128;
	char devMsg[bufSize] = {0};
	const char *devMsgContent = "TouchScreen";	//设备信息中应该包含TouchScreen 字样。

	// 打开设备
	if(-1 == (fd = open(inputDev, O_RDONLY)))
	{
		UI_perror("UI_MatchInputDev", "Fail to call open(2).\n");
		perror(strerror(errno));
		goto when_error_occur;
	}

	// 获取设备信息
	if(-1 == ioctl(fd, EVIOCGNAME(bufSize), devMsg))
	{
		UI_perror("UI_MatchInputDev", "Fail to call ioctl(2).\n");
		perror(strerror(errno));
		goto when_error_occur;
	}

	// 比较设备信息。
	printf("[UI] devMsg = %s\n", devMsg);
	if(NULL != strstr(devMsg, devMsgContent))		 //如果event0 的信息中包含devMsgContent 中的字样，则直接返回。
	{
		close(fd);
		fd = -1;
		return 0;
	}

	close(fd);
	fd = -1;

	// 交换设备名。
	UI_print("In UI_MatchInputDev(), switch /dev/input/event0 /dev/input/event1\n");
	if(0 !=	UI_SwitchDevName("/dev/input/event0", "/dev/input/event1"))
	{
		UI_perror("UI_MatchInputDev", "Fail to call UI_SwitchDevName().\n");
		return -1;
	}

	return 0;

	when_error_occur:
	if(-1 != fd)
	{
		close(fd);
		fd = -1;
	}

	return -1;
}

/*-----------------------------------------------------------------------------
描   述：交换dev0 和dev1 设备名
返回值：成功返回0, 失败返回-1.
-----------------------------------------------------------------------------*/
static int UI_SwitchDevName(const char *dev0, const char *dev1)
{	
	if(-1 == rename(dev0, "/dev/input/__event"))
	{
		UI_perror("UI_SwitchDevName", "Fail to call rename(2).\n");
		perror(strerror(errno));
		return -1;
	}

	if(-1 == rename(dev1, dev0))
	{
		UI_perror("UI_SwitchDevName", "Fail to call rename(2).\n");
		perror(strerror(errno));
		return -1;
	}

	if(-1 == rename("/dev/input/__event", dev1))
	{
		UI_perror("UI_SwitchDevName", "Fail to call rename(2).\n");
		perror(strerror(errno));
		return -1;
	}

	return 0;
}

/*-----------------------------------------------------------------------------
描   述：颜色索引值转化为RGB 值
返回值：返回RGB 值
-----------------------------------------------------------------------------*/
int UI_PIXEL2RGB(int pixel_val)
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	
	Pixel2RGB(HDC_SCREEN, pixel_val, &r, &g, &b);

	return ((r << 16) | (g << 8) | (b));	
}

/*-----------------------------------------------------------------------------
描   述：创建ttf 字体
参   数：family 字体名; charset 字符集, 例如"UTF-8""GB2312"; size 字体大小。
返回值：成功，返回指向字体的指针，失败返回NULL.
-----------------------------------------------------------------------------*/
PLOGFONT UI_CreatettfFont(const char* family, const char *charset, int size)
{
	if(NULL == family || NULL == charset || size <= 0)
	{
		UI_perror("UI_CreateGB2312ttfFont()", "bad argument.\n");
		return NULL;
	}

	PLOGFONT pLogFont = NULL;
	pLogFont = CreateLogFont(FONT_TYPE_NAME_SCALE_TTF, family,
						"UTF-8", FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, FONT_FLIP_NIL,
					   FONT_OTHER_NIL, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
					   size, 0);
	if(NULL == pLogFont)
	{
		UI_perror("UI_ModuleLoad", "Fail to call CreateLogFont.\n");
		return NULL;
	}
	
	return pLogFont;
}

