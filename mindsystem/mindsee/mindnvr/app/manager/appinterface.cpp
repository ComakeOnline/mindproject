
#include <mutex>

#include "appinterface.h"
#include "config.hpp"
#include "sysadapter.h"
#include "ipcmanager.hpp"

using namespace std;

AppInterface::AppInterface(/* args */)
{
}

AppInterface::~AppInterface()
{
}

void AppInterface::RefreshToMainPage()
{
    lock_guard<mutex> _Lock{m_mtx};
	//start by lison.guo
	SetIPCIndex(INDEX_INVALID);
	//end by lison.guo
    config::Config::GetInstance()->m_allcfg.eLayout = config::ELayout::E_LAYOUT_4PIC_SAMPLE;
    CSysAdapter::GetSysDisplay()->Refresh();
    auto pM = CIpcManager::GetInstance();
    for(int i=0;i<4;i++)
    {
        if(pM->QueryIpc(i))
        {
            pM->Bind(i,i);
        }
    }
    
}

bool AppInterface::SetToFullScreen(int _index)
{
    lock_guard<mutex> _Lock{m_mtx};
	//start by lison.guo
	SetIPCIndex(_index);
	//end by lison.guo
    auto pM = CIpcManager::GetInstance();
    if(!pM->QueryIpc(_index))
    {
        LOGGER_WARNING("Set empty index");
        return false;
    }
    config::Config::GetInstance()->m_allcfg.eLayout = config::ELayout::E_LAYOUT_FULLSCREEN;
    CSysAdapter::GetSysDisplay()->Refresh();
    pM->Bind(_index,0);
    return true;
}

int AppInterface::PlayTalk(int _index)
{
    lock_guard<mutex> _Lock{m_mtx};
    auto pM = CIpcManager::GetInstance();
    return pM->PlayIpcTalk(_index);
}

int AppInterface::StopAllTalk()
{
    lock_guard<mutex> _Lock{m_mtx};
    auto pM = CIpcManager::GetInstance();
    return pM->StopAllIpcTalk();
}

int AppInterface::PlayAudio(int _index)
{
    lock_guard<mutex> _Lock{m_mtx};
    auto pM = CIpcManager::GetInstance();
    return pM->PlayIpcAudio(_index);
}

int AppInterface::StopAllAudio()
{
    lock_guard<mutex> _Lock{m_mtx};
    auto pM = CIpcManager::GetInstance();
    return pM->StopAllIpcAudio();
}

int AppInterface::SetVolume(int _db)
{
    lock_guard<mutex> _Lock{m_mtx};
    auto pAo = CSysAdapter::GetAo();
    pAo->SetVolume(_db);
}

//start by lison.guo
int AppInterface::GetVolume()
{
    lock_guard<mutex> _Lock{m_mtx};
    auto pAo = CSysAdapter::GetAo();
	//需要增添获取音量的函数。
	//return pAo->GetVolume();

	return 1;
}

int AppInterface::GetIPCIndex()
{
	return index;
}

int AppInterface::SetIPCIndex(int DestIndex)
{
	index = DestIndex;
	return index;
}
//end by lison.guo


/*
    * 打开IPC网络统计信息消息
*/
void AppInterface::OpenNetInformation(const uint32_t &_chn)
{
    lock_guard<mutex> _Lock{m_mtx};
    auto pM = CIpcManager::GetInstance();
    pM->OpenNetInformation(_chn);
}

/*
    * 关闭IPC网络统计信息消息
*/
void AppInterface::CloseNetInformation(const uint32_t &_chn)
{
    lock_guard<mutex> _Lock{m_mtx};
    auto pM = CIpcManager::GetInstance();
    pM->CloseNetInformation(_chn);
}