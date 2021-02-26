/*
UI 主画面MainScreen，显示四个IPC 画面.
*/

#include "router.hpp"
#include "message.hpp"
#include "appinterface.h"
#include "ipcmanager.hpp"

#include "minigui_entry.hpp"
#include "minigui_resource.hpp"
#include "minigui_mainscreen.hpp"
#include "minigui_singleframe.hpp"
#include "minigui_iconscreen.hpp"
#include "nvr_etc.hpp"

#define INVALID_TOUCH_CNT (-1)

// 控件ID 号
static const int ID_00 = 20;
static const int ID_01 = 21;
static const int ID_02 = 22;
static const int ID_03 = 23;
   
// 从FullFrame 切换到四画面的时候，如果触摸动作超过100ms, 很容易被识别为两次触摸，
// 从而导致刚退出FullFrame 又进入了FullFrame. 设置滤波计数器从而消除此问题。
bool inFilting = false;						// 滤波标志位。
static unsigned char filtCnt = 0;			// 滤波计数器，数值为n 代表n * 100ms.
static const unsigned char filtCntMax = 2;	// 滤波计数器最大值，超过该值则置零。

static int inTouchPeriod = false;			// 触摸标志位。处于触摸期间则为true

static void UI_SwitchSingleFrame(int index);
static bool UI_HumanDetect(CMessage &msg, void*);

/*-----------------------------------------------------------------------------
描   述：切换IPC 画面为全屏或者单画面。
参数：index 需要设为单画面的IPC 标号。
返回值：无。
-----------------------------------------------------------------------------*/
static void UI_SwitchSingleFrame(int index)
{
	UI_print("Enter UI_SwitchSingleFrame().\n");

	inSingleFrame = true;
	inFourFrame = false;
	// 切换到全画面
	AppInterface::GetInstance()->SetToFullScreen(index);
	AppInterface *pAppInterface = AppInterface::GetInstance();
	UI_CreateSingleFrameWnd(index);				// 下一级的UI 在这里面设置。

	UI_print("Exit UI_SwitchSingleFrame().\n");
}

/*-----------------------------------------------------------------------------
描   述：主窗口的过程函数。处理触摸消息，短按切换IPC 画面，长按打开UI 滑动窗口。
返回值：
-----------------------------------------------------------------------------*/
#ifndef MG4_0_4
LRESULT UI_ProcMainScreen(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
#else
LRESULT UI_ProcMainScreen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#endif
{	
	// 控件数量、句柄、宽、高、起始坐标 相关变量定义
	static const int ctrlNum = 4;
	static HWND hCtrl[ctrlNum] = {0};
	static const int width = 150;
	static const int height = 100;
	static const int start_x = 62;
	static const int start_y = 100;
	static const int start_x0 = RESOLUTION_WIDTH / 2 - width;
	static const int start_y0 = 0;
	static const int start_x1 = RESOLUTION_WIDTH - width;
	static const int start_y1 = 0;
	static const int start_x2 = 512 - width;
	static const int start_y2 = 300;
	static const int start_x3 = RESOLUTION_WIDTH - width;
	static const int start_y3 = 300;

	// 触摸相关的变量定义
	static unsigned char longTouchCnt = 0;		//长触摸计数器。
	static const int longTouchThreshold = 10;	//长触摸阈值。大于该值视为长触摸
	static unsigned int touch_x = 0;			//触摸点的坐标
	static unsigned int touch_y = 0;
	static int isFullScreen = 0;				//全画面和四画面切换标志位
	
	// OSD 弹窗相关变量
	static int testCnt = 0;
	static int Cnt100ms = 0;
	// minigui 定时器每隔310ms 产生一次中断，一秒产生3.226 次中断。
	const static int Cnt5s = (double)3.5 * timeCnt1000ms;

	switch(message)
	{
		case MSG_CREATE:
		{
			inFourFrame = true;

			// 注册人形检测的处理函数
			{
				auto R = Router::GetInstance();
				R->Use(CMessage::E_MSG_AI_HC_RESULT, UI_HumanDetect);
			}
			
			// 创建四个静态文本框，放在IPC 画面的右上角，用于显示时间。
			hCtrl[0] = CreateWindowEx(CTRL_STATIC, "2018.9.1 18:00", WS_VISIBLE | SS_SIMPLE | SS_CENTER, \
				WS_EX_USEPARENTFONT | WS_EX_TRANSPARENT, ID_00, start_x0, start_y0, width, height, hWnd, 0);
			hCtrl[1] = CreateWindowEx(CTRL_STATIC, "2018.9.1 18:00", WS_VISIBLE | SS_SIMPLE | SS_CENTER, \
				WS_EX_USEPARENTFONT | WS_EX_TRANSPARENT, ID_01, start_x1, start_y1, width, height, hWnd, 0);
			hCtrl[2] = CreateWindowEx(CTRL_STATIC, "2018.9.1 18:00", WS_VISIBLE | SS_SIMPLE | SS_CENTER, \
				WS_EX_USEPARENTFONT | WS_EX_TRANSPARENT, ID_02, start_x2, start_y2, width, height, hWnd, 0);
			hCtrl[3] = CreateWindowEx(CTRL_STATIC, "2018.9.1 18:00", WS_VISIBLE | SS_SIMPLE | SS_CENTER, \
				WS_EX_USEPARENTFONT | WS_EX_TRANSPARENT, ID_03, start_x3, start_y3, width, height, hWnd, 0);

			return 0;
		}
		case MSG_CLOSE:
		{
			UI_print("MSG_CLOSE.\n");
			DestroyMainWindow(hWnd);
			PostQuitMessage(hWnd);
			break;
		}
		case MSG_COMMAND:
			switch(wParam)
			{
				break;
			}
			break;
			
		/*
			关于长按键。
			触摸按下的消息为：MSG_LBUTTONDOWN
			触摸抬起的消息为：MSG_LBUTTONUP
			MINIGUI 无消息时的休眠消息为：MSG_IDLE，该消息连续发送，每隔0.5秒发一次。
			在长时间触摸的情况下，MSG_IDLE 消息发送频率更快了。
			熟悉以上内容的基础上，长触摸的检测方法为：
			在触摸按下没有抬起期间，令长触摸计数器自增。当触摸抬起时清零。
			计数器的值如果大于阈值，则认为是长触摸，否则为短触摸。
		*/
		
		case MSG_LBUTTONDOWN:
			// 当触摸按下时，获取按下时的坐标，设置相关标志位用于记录按下的时长。
			UI_print("UI receive MSG_LBUTTONDOWN and touch is detected.\n");
			touch_x = UI_GetTouchX(lParam);
			touch_y = UI_GetTouchY(lParam);
			inTouchPeriod = 1;		//在按下期间
			longTouchCnt = 0;		//长按计数器清零
			printf("x = %d, y = %d\n", touch_x, touch_y);
			break;
		// case 触摸抬起时
		case MSG_LBUTTONUP:
			//如果长按计数器小于长按阈值 并且 长按计数器不为无效计数器, 则识别为短按
			if(longTouchCnt < longTouchThreshold && INVALID_TOUCH_CNT != longTouchCnt)
			{
				printf("[UI] Receive MSG_LBUTTONUP and short touch is detected.\n");
				printf("x = %d, y = %d\n", touch_x, touch_y);
				inTouchPeriod = 0;		//长按标志位置0
				
				int index = 0;
				index = UI_GetTouchArea(touch_x, touch_y, 2, 2);	//获取触摸区域0, 1, 2, 3
				printf("[UI], index = %d\n", index);

				bool ipcExist = true;
				auto pIpcManager = CIpcManager::GetInstance();
				ipcExist = pIpcManager->QueryIpc(index);
				if(!ipcExist)	//如果ipc 不存在，则返回；如果存在，则切换大小画面
				{
					UI_perror("UI_SwitchSingleFrame", "ipc is not exist.\n");
					break;
				}

				// 如果本来就在单画面中，那么在此接收到MSG_LBUTTONUP 消息时不再处理，
				// 防止重复进入单画面引起的BUG 类错误。
				if(inSingleFrame)
				{
					break;
				}

				// 如果在滤波期间再次接收到MSG_LBUTTONUP 消息，直接返回。
				if(inFilting)
				{
					break;
				}

				// 发送用户自定消息，切换画面。重置长按计数器及相关的标志位。
				PostMessage(hWnd, MSG_SWITCH_FRAME, index, 0);
				longTouchCnt = INVALID_TOUCH_CNT;
				inTouchPeriod = 0;
			}
			break;
		case MSG_SWITCH_FRAME:				
				UI_print("Call UI_SwitchSingleFrame().\n");
				UI_SwitchSingleFrame((int)wParam);	//切换为大小画面，进入下一级UI
				#if 1
				if(isFullScreen)
				{
					hCtrl[0] = CreateWindowEx(CTRL_STATIC, "2018.9.1 18:00", WS_VISIBLE | SS_SIMPLE | SS_CENTER, \
						WS_EX_TRANSPARENT, ID_00, start_x0, start_y0, width, height, hWnd, 0);
					hCtrl[1] = CreateWindowEx(CTRL_STATIC, "2018.9.1 18:00", WS_VISIBLE | SS_SIMPLE | SS_CENTER, \
						WS_EX_TRANSPARENT, ID_01, start_x1, start_y1, width, height, hWnd, 0);
					hCtrl[2] = CreateWindowEx(CTRL_STATIC, "2018.9.1 18:00", WS_VISIBLE | SS_SIMPLE | SS_CENTER, \
						WS_EX_TRANSPARENT, ID_02, start_x2, start_y2, width, height, hWnd, 0);
					hCtrl[3] = CreateWindowEx(CTRL_STATIC, "2018.9.1 18:00", WS_VISIBLE | SS_SIMPLE | SS_CENTER, \
						WS_EX_TRANSPARENT, ID_03, start_x3, start_y3, width, height, hWnd, 0);
				}
				else
				{
					printf("[UI] Ready to call DestroyWindow.\n");
					DestroyWindow(hCtrl[0]); 
					DestroyWindow(hCtrl[1]);
					DestroyWindow(hCtrl[2]);
					DestroyWindow(hCtrl[3]);
					hCtrl[4] = CreateWindowEx(CTRL_STATIC, "2018.9.1 18:00", WS_VISIBLE | SS_SIMPLE | SS_CENTER, \
						WS_EX_TRANSPARENT, ID_03, start_x1, start_y1, width, height, hWnd, 0);
				}
				#endif
				isFullScreen = !isFullScreen;
			break;
		case MSG_ENTER_ICON_SCREEN:
			UI_IconScreen();			// 进入图标页面
			break;
		case MSG_RCV_HC_DETCT:
			// 用户自定义消息，当检测到人形时，显示OSD 弹窗信息。
			//UI_print("receive MSG_RCV_HC_DETCT.\n");

			// 如果当前不是四画面，则不处理人形检测结果。
			if(!inFourFrame)
			{
				break;
			}
			// 否则OSD 标志位置true, 发送MSG_SWITCH_FRAME 消息进行OSD 弹窗。
			Cnt100ms = 0;
			if(HWND_INVALID == hFrameWnd)
			{
				UI_print("OSD---------ON.\n");
				inOSD = true;
				SendMessage(hWnd, MSG_SWITCH_FRAME, (int)wParam, 0);
			}
			break;
		case MSG_CLOSE_HC_OSD:
		{
			// 用户自定义消息类型，关闭OSD 弹窗。
			if(HWND_INVALID != hFrameWnd)
			{
				UI_print("OSD---------OFF.\n");
				inOSD = false;
				UI_DestroySingleFrameWnd();
				PostMessage(hFrameWnd, MSG_CLOSE, 0, 0);
			}
			break;
		}
		case MSG_IDLE:
		{
			//printf("%lu\n", getIntervalMs());
			//UI_print("main screen MSG_IDLE.\n");
			// 休眠期间更新长按计时器、滤波计数器、OSD 显示时长计数器等变量。

			// 更新长按计数器。
			if(inTouchPeriod)
			{
				//长触摸计数器自增，超过长按阈值时长判断为长按，运行主页窗口（包含滑动图标）。
				if(++longTouchCnt > longTouchThreshold)
				{
					printf("[lison.guo] UI receive MSG_LBUTTONDOWN and long touch is detected.\n");
					inTouchPeriod = 0;
					longTouchCnt = INVALID_TOUCH_CNT;
					PostMessage(hWnd, MSG_ENTER_ICON_SCREEN, 0, 0);		//进入滑动图标窗口
				}					
			}

			// 更新滤波计数器。
			if(inFilting)
			{
				if(++filtCnt >= filtCntMax)
				{
					filtCnt = 0;
					inFilting = false;
				}
			}

			// 计算OSD 弹窗时长。
			if(inOSD)
			{
				Cnt100ms++;
				if(Cnt100ms > Cnt5s)
				{
					Cnt100ms = 0;
					UI_print("Call PostMessage().\n");
					PostMessage(hMainScreen, MSG_CLOSE_HC_OSD, 0, 0);
				}
			}
			else
			{
				Cnt100ms = 0;
			}
			break;
		}
		default:
			break;
	}

	return DefaultMainWinProc(hWnd, message, wParam, lParam);
	//return DefaultDialogProc(hWnd, message, wParam, lParam);
}

/*-----------------------------------------------------------------------------
描   述：检测到人形时的回调函数
参	数：参考router.hpp 中的如下函数：
		Router::Use(CMessage::MSG_TYPE index, std::function<bool(CMessage&, void*)> func)
返回值：成功，返回true; 失败，返回false.
注	意：
-----------------------------------------------------------------------------*/
static bool UI_HumanDetect(CMessage &msg, void *userdata)
{
	UI_print("Call UI_HumanDetect.\n");

// 防止重入。如果在极短的时间内（例如1秒）连续收到多次HC_DETECT 消息，则只处理一次。
#if 1
	unsigned intervalSec = 0;
	unsigned minIntervalSec = 2;
	
	intervalSec = getIntervalSec();
	if(intervalSec < minIntervalSec)
	{
		UI_perror("UI_HumanDetect()", "Interval time is too short!\n");
		return false;
	}
#endif

	if(inTouchPeriod)
	{
		UI_print("Return from UI_HumanDetect because of touching event.\n");
		return false;
	}
	
	// 通过参数一获取检测到人形的IPC 标号。
	int index = 0;
	index = msg.param_1;
	
	printf("[UI] HC Detected. index = %#x\n", index);
	if(UI_HcOSDEnabled())
	{
		//UI_print("Start of Calling SendMessage() with message of MSG_RCV_HC_DETECT.\n");
		//SendMessage(hMainScreen, MSG_RCV_HC_DETCT, (WPARAM)index, 0);
		PostMessage(hMainScreen, MSG_RCV_HC_DETCT, (WPARAM)index, 0);
		//UI_print("End of Calling SendMessage() with message of MSG_RCV_HC_DETECT.\n");
		return true;
	}
	else
	{
		UI_print("HC detected! Please configure flag of HC_OSD if you want to show OSD!\n");
	}
	
	return false;
}

