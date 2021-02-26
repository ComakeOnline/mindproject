#pragma once
#include <string>
#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include <cstdint>

#include "global.hpp"
#include "config.hpp"
#include "stream_session.hpp"
#include "sysadapter.h"

class CIpc
{
public:
    std::string m_ipAddr;
    std::string m_rtspURL;
    uint32_t m_index; //通道编号
    std::string m_name;
    //CRtspSession m_rtspSession;
    ISession *m_pSession;

public:
    DISABLE_COPY(CIpc);
    DISABLE_EQUAL(CIpc);

    CIpc(const std::string& _ipAddr, const std::string& _rtspURL, const std::string& _name, const uint32_t& _index)
    :m_ipAddr{_ipAddr},
    m_rtspURL{_rtspURL},
    m_name{_name},
    m_index{_index}
    {
        m_pSession = new CStreamSession{};
        //((CPriProtocolSession*)m_pSession)->m_chn = m_index;
        m_pSession->Start(m_rtspURL);
        if(m_name == "audio_open")
        {
            m_pSession->PlayAudio();
            m_pSession->PlayTalk();

        }
    }
    ~CIpc() {
        delete m_pSession;
    }

    bool GetRtspStatus(){
        return m_pSession->Status();
    }

    void RtspReconnect(){
        if(!m_pSession->Status())
        {
            m_pSession->Stop();
            m_pSession->Start(m_rtspURL);
            if(m_name == "audio_open")
            {
                m_pSession->PlayAudio();
                m_pSession->PlayTalk();

            }
        }
    }

    int SendCmd(const std::string& _cmd)
    {
        return m_pSession->SendCmd(_cmd);
    }
};


class CIpcManager
{
public:
    struct IpcInfo
    {
        std::string ipAddr;
        uint32_t ip_bin;
        std::string rtspURL;
        uint32_t index; //通道编号
        std::string name;
    };
    
private:
    std::map<uint32_t,std::shared_ptr<CIpc>> m_mapIpc;
    std::mutex m_mtx;
public:
    static CIpcManager* GetInstance(){
        static CIpcManager ins;
        return &ins;
    }

    //获取当前IPC列表
    //const std::map<uint32_t,std::shared_ptr<CIpc>> & GetAllIpc(){return m_mapIpc;}

    //获取最近一个可用通道号 无可用则返回-1
    uint32_t GetEmptyChnNum();

    //添加一个IPC
    int AddIpc(const std::string& _ipAddr, const std::string& _rtspURL, const std::string& _name, const uint32_t& _chn);

    //删除一个IPC
    int DelIpc(const uint32_t& _chn);

    //删除所有IPC
    void ClearAll();

    //绑定IPC到显示端口
    bool Bind(uint32_t _ipc,uint32_t _disp_port);

    //查询IPC是否存在
    bool QueryIpc(const uint32_t& _chn);

    //获取所有IPC信息
    std::vector<IpcInfo> GetAllIpcInfo();

    //播放IPC chn的音频
    int PlayIpcAudio(const uint32_t& _chn);

    //停止播放所有IPC音频
    int StopAllIpcAudio();

    //打开对讲
    int PlayIpcTalk(const uint32_t& _chn);

    //停止播放所有IPC音频
    int StopAllIpcTalk();

    //轮询动作
    void polling();

    void OpenNetInformation(const uint32_t& _chn);

    void CloseNetInformation(const uint32_t& _chn);

    //给IPC发送指令
    int SendCmdToIpc(const uint32_t& _chn, const std::string& _cmd);

private:
    CIpcManager(/* args */);
    ~CIpcManager();
};


