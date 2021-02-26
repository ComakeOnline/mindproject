#include "ipcmanager.hpp"

using namespace std;

CIpcManager::CIpcManager(/* args */)
{
    //初始化IPC池
    for(int i=0;i<config::Config::GetInstance()->m_productCfg.MAX_IPC_NUMBER;i++)
    {
        m_mapIpc[i] = nullptr;
    }
}

CIpcManager::~CIpcManager()
{
}


//获取最近一个可用通道号 无可用则返回-1
uint32_t CIpcManager::GetEmptyChnNum()
{
    lock_guard<mutex> _lock1_{m_mtx};
    for (auto &&i : m_mapIpc)
    {
        if(i.second == nullptr)
        {
            return i.first;
        }
    }
    return -1;
}

//添加一个IPC
int CIpcManager::AddIpc(const std::string& _ipAddr, const std::string& _rtspURL, const std::string& _name, const uint32_t& _chn)
{
    lock_guard<mutex> _lock1_{m_mtx};
    if(_chn >= config::Config::GetInstance()->m_productCfg.MAX_IPC_NUMBER)
    {
        LOGGER_ERROR("IPC Chn number err. now:%u,max:%u",_chn,config::Config::GetInstance()->m_productCfg.MAX_IPC_NUMBER);
        return -1;
    }
    if(m_mapIpc[_chn] != nullptr)
    {
        LOGGER_ERROR("IPC Chn number err. has Chn:%d",_chn);
        return -2;
    }
    m_mapIpc[_chn] = make_shared<CIpc>(_ipAddr,_rtspURL,_name,_chn);
    return 0;
}

//删除一个IPC
int CIpcManager::DelIpc(const uint32_t& _chn)
{
    lock_guard<mutex> _lock1_{m_mtx};
    if(_chn >= config::Config::GetInstance()->m_productCfg.MAX_IPC_NUMBER)
    {
        LOGGER_ERROR("IPC Chn number err. now:%u,max:%u",_chn,config::Config::GetInstance()->m_productCfg.MAX_IPC_NUMBER);
        return -1;
    }
    if(m_mapIpc[_chn] == nullptr)
    {
        LOGGER_ERROR("IPC Chn number empty. Chn:%d",_chn);
        return -2;
    }
    m_mapIpc.erase(_chn);
    m_mapIpc[_chn] == nullptr;
    return 0;
}

void CIpcManager::ClearAll()
{
    lock_guard<mutex> _lock1_{m_mtx};
    for (auto &&i : m_mapIpc)
    {
        i.second = nullptr;
    }
    
}

bool CIpcManager::Bind(uint32_t _ipc,uint32_t _disp_port)
{
    lock_guard<mutex> _lock1_{m_mtx};
    auto iter = m_mapIpc.find(_ipc);
    if(iter == m_mapIpc.end() || iter->second == nullptr)
    {
        LOGGER_ERROR("Bind IPC to Disp error");
        return false;
    }
    //sleep(3);
    iter->second->m_pSession->SetCbAfterConnect([_disp_port](std::shared_ptr<CISysVideoDecoder> _vdec){
        CSysAdapter::GetSysDisplay()->SetInput(_vdec->GetChn(),_disp_port);
    });
    return true;
}

bool CIpcManager::QueryIpc(const uint32_t& _chn)
{
    lock_guard<mutex> _lock1_{m_mtx};
    auto iter = m_mapIpc.find(_chn);
    if(iter == m_mapIpc.end() || iter->second == nullptr)
    {
        return false;
    }
    return true;
}

std::vector<CIpcManager::IpcInfo> CIpcManager::GetAllIpcInfo()
{
    lock_guard<mutex> _lock1_{m_mtx};
    std::vector<CIpcManager::IpcInfo> vecIpc;
    for (auto &&i : m_mapIpc)
    {
        if(i.second)
        {
            CIpcManager::IpcInfo ipcinfo;
            ipcinfo.ipAddr = i.second->m_ipAddr;
            ipcinfo.index = i.second->m_index;
            ipcinfo.name = i.second->m_name;
            ipcinfo.rtspURL = i.second->m_rtspURL;
            ipcinfo.ip_bin = Global::IpStr2B(ipcinfo.ipAddr);
            vecIpc.emplace_back(ipcinfo);
        }
    }
    return vecIpc;
}

void CIpcManager::polling()
{
    lock_guard<mutex> _lock1_{m_mtx};
    for (auto &&i : m_mapIpc)
    {
        if(i.second)
        {
            //重连
            if(!i.second->GetRtspStatus())
            {
                i.second->RtspReconnect();
            }
        }
    }
    
}

//播放IPC chn的音频
int CIpcManager::PlayIpcAudio(const uint32_t& _chn)
{
    lock_guard<mutex> _lock1_{m_mtx};
    auto iter = m_mapIpc.find(_chn);
    if(iter == m_mapIpc.end() || iter->second == nullptr)
    {
        LOGGER_ERROR("PlayAudio error,no ipc:%u",_chn);
        return -1;
    }
    auto ret =  iter->second->m_pSession->PlayAudio();
    return ret;
}

//停止播放所有IPC音频
int CIpcManager::StopAllIpcAudio()
{
    lock_guard<mutex> _lock1_{m_mtx};
    for (auto &&i : m_mapIpc)
    {
        if(i.second)
        {
            i.second->m_pSession->StopAudio();
        }
    }
    return 0;
}

//打开对讲
int CIpcManager::PlayIpcTalk(const uint32_t& _chn)
{
    lock_guard<mutex> _lock1_{m_mtx};
    auto iter = m_mapIpc.find(_chn);
    if(iter == m_mapIpc.end() || iter->second == nullptr)
    {
        LOGGER_ERROR("PlayAudio error,no ipc:%u",_chn);
        return -1;
    }
    auto ret =  iter->second->m_pSession->PlayTalk();
    return ret;
}

//停止播放所有IPC音频
int CIpcManager::StopAllIpcTalk()
{
    lock_guard<mutex> _lock1_{m_mtx};
    for (auto &&i : m_mapIpc)
    {
        if(i.second)
        {
            i.second->m_pSession->StopTalk();
        }
    }
    return 0;
}

void CIpcManager::OpenNetInformation(const uint32_t& _chn)
{
    lock_guard<mutex> _lock1_{m_mtx};
    auto iter = m_mapIpc.find(_chn);
    if(iter == m_mapIpc.end() || iter->second == nullptr)
    {
        LOGGER_ERROR("OpenNetInformation error,no ipc:%u",_chn);
    }
    iter->second->m_pSession->OpenNetInformation();
}

void CIpcManager::CloseNetInformation(const uint32_t& _chn)
{
    lock_guard<mutex> _lock1_{m_mtx};
    auto iter = m_mapIpc.find(_chn);
    if(iter == m_mapIpc.end() || iter->second == nullptr)
    {
        LOGGER_ERROR("CloseNetInformation error,no ipc:%u",_chn);
    }
    iter->second->m_pSession->CloseNetInformation();
}

//给IPC发送指令
int CIpcManager::SendCmdToIpc(const uint32_t& _chn, const std::string& _cmd)
{
    lock_guard<mutex> _lock1_{m_mtx};
    auto iter = m_mapIpc.find(_chn);
    if(iter == m_mapIpc.end() || iter->second == nullptr)
    {
        LOGGER_ERROR("SendCmdToIpc error,no ipc:%u",_chn);
        return -1;
    }
    auto ret =  iter->second->m_pSession->SendCmd(_cmd);
    return ret;
}