#include "wifimanager.hpp"
#include <iostream>

#include "global.hpp"
#include "wifilink.h"

using namespace std;

int cb_Save(char *data,size_t len)
{
    FILE *fp = fopen("/customer/wifilink.cfg"/*"/customer/wifilink.cfg"*/, "wb");
    if(!fp)
    {
        cerr<<"cb_save: -1"<<std::endl;
        return -1;
    }
    int ret = fwrite(data,1,len,fp);
    fclose(fp);
    cerr<<"cb_save:"<<ret<<std::endl;
    return ret;
}

int cb_Load(char *data,size_t len)
{
    FILE *fp = fopen("/customer/wifilink.cfg" /*"/customer/wifilink.cfg"*/, "rb");
    if(!fp)
    {
        cerr<<"cb_Load: -1"<<std::endl;
        return -1;
    }
    int ret = fread(data,1,len,fp);
    fclose(fp);
    cerr<<"cb_load:"<<ret<<std::endl;
    return ret;
}

//该函数不可阻塞,如需长时间占用请设置超时机制(不超过15秒为宜)
void cb_setip(const char *ifname)
{
    cout<<"####################################################################setip#########################################\n";
    cout<<"123:"<<ifname<<endl;
    if(strcmp(ifname,"br0") == 0)
    {
        system("busybox killall udhcpc");
        //system("udhcpc -i br0 &");
        system("sh /customer/setip.sh");
        //system("sh /customer/setip.sh");
        //ifconfig br0 192.168.1.10
        //route add default dev br0
    }
    if(strcmp(ifname,"eth0") == 0)
    {
        system("busybox killall udhcpc");
        //system("udhcpc -i eth0 &");
        system("sh /customer/setethip.sh");
        //system("sh /customer/setip.sh");
    }
}

int recv_monitor_package(char *buf, int length)
{
    printf("Callback %s here\n", __func__);
    printf("Receive buf(%d):<%s>\n", length, buf);
    return 0;
}

void cb_setapip(const char *ifname)
{
//    printf("start udhpcd\n");
//    system("ifconfig p2p0 192.168.0.1");
//    system("route add default gw 192.168.0.1 ");
}

void cb_pair()
{
    printf("pair success\n");
}

void *creat1(void *arg)
{
    WifiLinkIPCLoop();
    //WifiLinkNVRLoop();
    return (void *)0;
}

int WifiManager::Enable()
{
    if(m_pth != nullptr)
    {
        LOGGER_ERROR("WIFI ENABLE ERROR");
        return -1;
    }
    m_pth = new thread{[this](){
        WifilinkCfg cfg;
        memset(&cfg,0,sizeof(WifilinkCfg));
        cfg.load_cb = cb_Load;
        cfg.save_cb = cb_Save;
        cfg.setip_cb = cb_setip;
        cfg.monitor_cb = recv_monitor_package;
        cfg.setapip_cb = cb_setapip;
        cfg.pair_cb = cb_pair;
        cfg.ApDelayTimes = 30;
        cfg.a = 1.7;
        cfg.b = 0.03;
        cfg.c = 0.7;
        cfg.d = 0.08;
        cfg.e = 0.1;
        strcpy(cfg.ApIfName,"wlan0");
        cfg.TxPower = true;
        cfg.isSpecialFreq = true;
        cfg.WifiDbg = false;
        cfg.CustomGroup = false;
        cfg.EthFirst = true;
        cfg.DelayTimes = 4;
        cfg.MaxLinkLevel = 8;
        cfg.MaxChildNode = 3;
        cfg.LogLevel = 3;
        cfg.IsRepeater = false;

        memset(cfg.LogFile, 0, sizeof(cfg.LogFile));

        memset(cfg.ko_path,0,sizeof(cfg.ko_path));
        memset(cfg.conf_path, 0, sizeof(cfg.conf_path));

        //strcpy(cfg.ko_path,"/ext/demo/wifi");
        //strcpy(cfg.conf_path,"/ext/demo/wifi");
        strcpy(cfg.ko_path, "/customer");
        strcpy(cfg.conf_path, "/customer");

        //strcpy(cfg.apcfg.Ssid,"hellowillop");
        //cfg.apcfg.Channel = 1;
        strcpy(cfg.apcfg.Passwd,"12345678");
        cfg.apcfg.Is40Mhz = false;
        cfg.apcfg.IsOpen = true;

        //new
        cfg.NoPasswdLink = true;
        uint8_t Key[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
        memcpy(cfg.EncKey, Key, sizeof(Key));
        uint8_t mac_db[8][6] = {0};
        int i;
        int ret;
        StaCfg stacfg;
        memset(&stacfg,0,sizeof(StaCfg));
        strcpy(stacfg.Ssid,"TP_GW");
        strcpy(stacfg.Passwd,"12345678");
        WifiLinkInit(&cfg);
    
        WifiLinkNVRLoop();
    }};
    return 0;
}

int WifiManager::Disable()
{
    if(m_pth != nullptr)
    {
        WifiLinkCancel();
        m_pth->join();
        delete m_pth;
        m_pth = nullptr;
    }
    return 0;
}