#include "manager.hpp"

#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <thread>

#include "global.hpp"
#include "logger.hpp"

#include "wifimanager.hpp"
#include "router.hpp"
#include "sysadapter.h"
#include "ipcmanager.hpp"
#include "appinterface.h"

//start edit by lison.guo
#include "minigui_entry.hpp"
#define EN_UI 0
#define USE_CFG 1
#define EN_WIFI 0
bool CLOSE_STDERR = false;
//end edit by lison.guo

using namespace std;

static bool s_running;

void signal_cb(int sig)
{
    switch (sig)
    {
    case SIGINT:
        LOGGER_ERROR("SIGNAL SIGINT CTRL^C");
        kill(getpid(), SIGCHLD);
        s_running = false;
        break;
    case SIGCHLD:
		LOGGER_ERROR("SIGNAL SIGCHLD");
    	s_running = false;
    	break;
    default:
		LOGGER_ERROR("SIGNAL: %d , END",sig);
        s_running = false;
        break;
    }
}

Manager::Manager(/* args */)
{
    LOGGER_INIT(stderr,3);
    signal(SIGINT, signal_cb);
    //signal(SIGCHLD, signal_cb);
}

Manager::~Manager()
{
}

void Manager::Run()
{
	// 打印版本信息
	cout << "[VERSION]Git Version:" << GIT_VERSION << endl;
	cout << "[VERSION]Git PATH:" << GIT_PATH << endl;
	cout << "[VERSION]build time:" << __DATE__ << " " << __TIME__ << endl;

#if (1 == (EN_WIFI))
	auto pWifi = WifiManager::GetInstance();
	pWifi->Enable();
#endif
    auto pDisp = CSysAdapter::GetSysDisplay();
    //pDisp->Restart();
    pDisp->FbInit();
	auto pAo = CSysAdapter::GetAo();
	pAo->Refresh();
	auto pAi = CSysAdapter::GetAi();
	pAi->Refresh();
    auto pIpcManager = CIpcManager::GetInstance();

#if (1 == (USE_CFG))
	// 如果使用本地配置文件，则可以忽略掉程序中已写入的IP 字段。
	// 配置文件一般位于/customer/nvr_cfg.json
	auto ipc_num = config::Config::GetInstance()->GetCfg("/ipc_manager/ipc_number",4);

	switch (ipc_num)
	{
	case 4:
		{
			auto ipc_ip = config::Config::GetInstance()->GetCfg("/ipc_manager/ipc/3/ip","192.168.7.16");
			auto ipc_url = config::Config::GetInstance()->GetCfg("/ipc_manager/ipc/3/url","192.168.7.16");
			auto ipc_name = config::Config::GetInstance()->GetCfg("/ipc_manager/ipc/3/name","ipc003");
			auto ipc_chn = config::Config::GetInstance()->GetCfg<int>("/ipc_manager/ipc/3/chn",3);
			pIpcManager->AddIpc(ipc_ip,ipc_url,ipc_name,ipc_chn);
			pIpcManager->Bind(ipc_chn,ipc_chn);
		}
	case 3:
		{
			auto ipc_ip = config::Config::GetInstance()->GetCfg("/ipc_manager/ipc/2/ip","192.168.7.15");
			auto ipc_url = config::Config::GetInstance()->GetCfg("/ipc_manager/ipc/2/url","192.168.7.15");
			auto ipc_name = config::Config::GetInstance()->GetCfg("/ipc_manager/ipc/2/name","ipc002");
			auto ipc_chn = config::Config::GetInstance()->GetCfg<int>("/ipc_manager/ipc/2/chn",2);
			pIpcManager->AddIpc(ipc_ip,ipc_url,ipc_name,ipc_chn);
			pIpcManager->Bind(ipc_chn,ipc_chn);
		}
	case 2:
		{
			auto ipc_ip = config::Config::GetInstance()->GetCfg("/ipc_manager/ipc/1/ip","192.168.7.14");
			auto ipc_url = config::Config::GetInstance()->GetCfg("/ipc_manager/ipc/1/url","192.168.7.14");
			auto ipc_name = config::Config::GetInstance()->GetCfg("/ipc_manager/ipc/1/name","ipc001");
			auto ipc_chn = config::Config::GetInstance()->GetCfg<int>("/ipc_manager/ipc/1/chn",1);
			pIpcManager->AddIpc(ipc_ip,ipc_url,ipc_name,ipc_chn);
			pIpcManager->Bind(ipc_chn,ipc_chn);
		}
	case 1:
		{
			auto ipc_ip = config::Config::GetInstance()->GetCfg("/ipc_manager/ipc/0/ip","192.168.7.13");
			auto ipc_url = config::Config::GetInstance()->GetCfg("/ipc_manager/ipc/0/url","192.168.7.13");
			auto ipc_name = config::Config::GetInstance()->GetCfg("/ipc_manager/ipc/0/name","ipc000");
			auto ipc_chn = config::Config::GetInstance()->GetCfg<int>("/ipc_manager/ipc/0/chn",0);
			pIpcManager->AddIpc(ipc_ip,ipc_url,ipc_name,ipc_chn);
			pIpcManager->Bind(ipc_chn,ipc_chn);
		}
	
	default:
		config::Config::GetInstance()->SaveCfg();
		break;
	}
#else	// 如果使用nvr_cfg.json 配置文件，则忽略此段。
	pIpcManager->AddIpc("192.168.7.13","192.168.0.13","audio_noopen",1);
	pIpcManager->Bind(1,1);
	pIpcManager->AddIpc("192.168.7.13","192.168.0.15","audio_noopen",3);
	pIpcManager->Bind(3,3);
#endif

#if (1 == (EN_UI))
	//start edit by lison.guo
	UI_print("ready to call UI_ModuleLoad().\n");
	if(UI_ModuleLoad() != 0)
	{
		UI_perror("In manager::run()", "Fail to load UI module!\n");
    }
	else
	{
		UI_print("Success to load UI module!\n");
	}

	UI_print("ready to call UI_Run().\n");;
	UI_ModuleRun();
	// end edit by lison.guo
#endif

	// polling
	s_running = true;
	cout<<"$$$$$$$$$$$$$$$DO"<<endl;
	while (s_running)
	{
	    pIpcManager->polling();
	    this_thread::sleep_for(chrono::seconds(1));
	}
	
#if (1 == (EN_UI))
	UI_print("Ready to call UI_ModuleUnload.\n");
	UI_ModuleUnload();
	UI_print("End of Call UI_ModuleUnload.\n");
#endif

	pIpcManager->ClearAll();
	cout<<"#####exit"<<endl;
}
