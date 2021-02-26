#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <dirent.h>
#include <sys/stat.h>

#include <iostream>

#include "wifilink.h"

using namespace std;



int cb_Save(char *data,size_t len)
{
    FILE *fp = fopen("/ext/demo/wifilink.cfg"/*"/customer/wifilink.cfg"*/, "wb");
	//FILE *fp = fopen("/customer/wifilink.cfg", "wb");
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
    FILE *fp = fopen("/ext/demo/wifilink.cfg"/*"/customer/wifilink.cfg"*/, "rb");
	//FILE *fp = fopen("/customer/wifilink.cfg", "rb");
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
        system("sh /ext/demo/setip.sh");
        //system("ifconfig br0 mtu 1400");
        //system("sh /customer/setip.sh");
        //ifconfig br0 192.168.1.10
        //route add default dev br0
    }
    if(strcmp(ifname,"eth0") == 0)
    {
        system("busybox killall udhcpc");
        //system("udhcpc -i eth0 &");
        system("sh /ext/demo/setethip.sh");
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
   printf("start udhpcd\n");
   system("ifconfig p2p0 192.168.0.1");
   system("route add default gw 192.168.0.1 ");
}

void cb_pair()
{
    printf("pair success\n");
}

void cb_wifistatus(int status)
{
	
}

void *creat1(void *arg)
{
    WifiLinkIPCLoop();
    //WifiLinkNVRLoop();
    while(1)
    {
        sleep(1);
    }

    return (void *)0;
}

void *creat2(void *arg)
{
	WifiRssi rssi;
    while (1)
    {
        printf("GetLinkStatus ===== %d\n",GetLinkStatus());
		GetWifiRssi(&rssi);
        usleep(3*1000);
    }

    return (void *)0;
}

int main(int argc, char const *argv[])
{
    WifilinkCfg cfg;
    memset(&cfg,0,sizeof(WifilinkCfg));
    cfg.load_cb = cb_Load;
    cfg.save_cb = cb_Save;
    cfg.setip_cb = cb_setip;
    cfg.monitor_cb = recv_monitor_package;
    cfg.setapip_cb = cb_setapip;
    cfg.pair_cb = cb_pair;
	cfg.wifistatus_cb = cb_wifistatus;
    cfg.ApDelayTimes = 30;
    cfg.a = 1.7;
    cfg.b = 0.03;
    cfg.c = 0.7;
    cfg.d = 0.08;
    cfg.e = 0.1;
    strcpy(cfg.ApIfName,"p2p0");
    cfg.TxPower = true;
    cfg.isSpecialFreq = true;
    cfg.WifiDbg = false;
    cfg.CustomGroup = false;
    cfg.EthFirst = true;
    cfg.DelayTimes = 4;
    cfg.MaxLinkLevel = 8;
    cfg.MaxChildNode = 3;
    cfg.LogLevel = 2;
    cfg.IsRepeater = true;
    cfg.IsConnectRepeater = false;
	cfg.broadcast_interval = 1;
	cfg.isSpecialCh = true;     //base使用特殊信道  false就会跟普通的AP信道一样
	cfg.Detection_network = true; 
	cfg.isEthNotstartAp = false;
	
    memset(cfg.LogFile, 0, sizeof(cfg.LogFile));

    memset(cfg.ko_path,0,sizeof(cfg.ko_path));
    memset(cfg.conf_path, 0, sizeof(cfg.conf_path));

    strcpy(cfg.ko_path,"/ext/demo/wifi");
    strcpy(cfg.conf_path,"/ext/demo/wifi");
    //strcpy(cfg.ko_path, "/customer/wifi");
    //strcpy(cfg.conf_path, "/customer/wifi");

    //strcpy(cfg.apcfg.Ssid,"hellowillop");
    cfg.apcfg.Channel = 1;
    strcpy(cfg.apcfg.Passwd,"12345678");
    cfg.apcfg.Is40Mhz = true;
    cfg.apcfg.IsOpen = false;
	
	int i;
    int ret;
	uint8_t mac_db[14][6] = {0};
    //new
    cfg.NoPasswdLink = true;
    uint8_t Key[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    memcpy(cfg.EncKey, Key, sizeof(Key));

	//测试接口，可以不用设置
	#if 0
    uint8_t mac_db[8][6] = {0};
    int i;
    int ret;
    StaCfg stacfg;
	mac_db[0][0] = 0x01;
	mac_db[0][1] = 0x02;
	mac_db[0][2] = 0x03;
	mac_db[0][3] = 0x04;
	mac_db[0][4] = 0x05;
	mac_db[0][5] = 0x06;
	#endif
	StaCfg stacfg;
	memset(&stacfg,0,sizeof(StaCfg));
    strcpy(stacfg.Ssid,"TP-LINK_7798");
    strcpy(stacfg.Passwd,"12345678");
	
    pthread_t trd_s;
    int error;
    WifiLinkInit(&cfg);
	//测试获取IP地址
	//UpdateCustomMacDb(mac_db);
	printf("best channel %d\n",WifiGetBestChannel(REGULATORY_FCC));
	
    WifiLinkStart();
    UpdateStaConfig(&stacfg);
    ret = WifiGetMacArray((uint8_t **)mac_db);
    for(i = 0; i< 14;i++)
    {
        printf("num:%d,%02X:%02X:%02X:%02X:%02X:%02X\n", ret,mac_db[i][0],mac_db[i][1], mac_db[i][2], mac_db[i][3], mac_db[i][4], mac_db[i][5]);
    }

    //WifiDelMacArray(1);
    //WifiRssi Rssi;
    //GetWifiRssi(&Rssi);
    //sleep(20);
    //WifiDelMacArray(0);
    //unsigned char mac[6] = {0};
    //WifiLinkGetMACaddr(mac,"wlan0");
    //printf("mac-----------------:%2X-%2X-%2X-%2X-%2X-%2X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	//测试删除单个MAC地址的code，正常使用不需要
    //sleep(5);
	//WifiDelMac2MacArray(mac_db[0]);
	//ret = WifiGetMacArray((uint8_t **)mac_db);
    //for(i = 0; i< 8;i++)
    //{
   //     printf("num:%d,%02X:%02X:%02X:%02X:%02X:%02X\n", ret,mac_db[i][0],mac_db[i][1], mac_db[i][2], mac_db[i][3], mac_db[i][4], mac_db[i][5]);
    //}
    #if 1
    pthread_t trd;
    printf("creat thread \n");
    error = pthread_create(&trd, NULL, creat1, (void *)NULL);
    if(error)
    {
        printf("pthread_creat fail \n");
        exit(-1);
    }
    #endif
    WifiNetPairing();
	#if 0
    printf("creat thread \n");
    error = pthread_create(&trd_s, NULL, creat2, (void *)NULL);
    if (error)
    {
        printf("pthread_creat fail \n");
        exit(-1);
    }
    #endif
    //WifiDelMacArray(0);
    //WifiLinkCancel();
    //sleep(1);
    //UpdateApConfig(&cfg.apcfg);
    //sleep(30);
    //UpdateStaConfig(&stacfg);
    //sleep(10);
    //WifiLinkIPCLoop();
    //sleep(20);
    //WifiLinkCancel();
    //printf("UpdateAp Info\n");
    //UpdateApConfig(&cfg.apcfg);
    //WifiRssi wifirssi;
    //memset(&wifirssi, 0, sizeof(WifiRssi));
    //GetWifiRssi(&wifirssi);
    //ret = WifiOneKeyPairing(30);
    //printf("WifiOneKeyPairing num:%d\n",ret);
    //sleep(20);
    //WifiLinkIPCLoop();
    //sleep(1);
    //ret = WifiGetMacArray((uint8_t **)mac_db);
    //for (i = 0; i < 8; i++)
    //{
    //    printf("num:%d,%02X:%02X:%02X:%02X:%02X:%02X\n", ret, mac_db[i][0], mac_db[i][1], mac_db[i][2], mac_db[i][3], mac_db[i][4], mac_db[i][5]);
    //}
    //WifiSmartLinkStart();
    //sleep(40);
    //WifiDelMacArray(1);
    //WifiSmartLnkStop();
    /* code */
    while (1)
    {
        sleep(1);
    }

    return 0;
}
