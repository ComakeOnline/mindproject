# wifi自组网库(libwifilink.so)使用说明

## 2019.5.20更新
* 不再可以设置0和15信道，但替换1信道为2380Mhz，13信道为2504Mhz.

* 修复wufi连接状态判断不准确的bug.

* 修复wifi AP状态判断不准确的bug.

* 修复一些地方可能发生崩溃的bug.

## 2019.4.10更新
* 增加`ApCfg`配置wifiAP-40MHz选项,当设置`ApCfg.Is40Mhz=true`时，wifiAP开启40Mhz功能。

* 新增API`int UpdateEncKey(uint8_t newkey[]);`
> 动态更新用于密钥连接的EncKey.
> 1. 参数`uint8_t newkey[]`为新的key，长度为16字节(128位)

## 2019.3.29更新
> 该版本完全托管hostapd.conf,并且要求使用默认提供的hostapd.conf,不能添加或删除参数。

> 去掉了demo.cpp的c++11依赖。

* 新增功能，通过密钥(与通密码连接的方式并存)的wifi私有连接协议，该协议允许经过配对后的自组网设备不通过密码而通过密钥进行连接，密钥在传输过程中也进行加密。
> 1. 使用该功能的方法为将`WifilinkCfg`中的`bool NoPasswdLink`设置为`true`,并设置一个128位的密钥`uint8_t EncKey[16]`。当在mac地址白名单中，且`EncKey`值相同的设备，则允许其互相连接。
> 2. 该功能开启后，通过此方法连接的wifi-sta设备，连接后将会把自己的密码和信道设置成和其连接的AP一致。

* 去掉了`WifilinkCfg`里的信道设置，并增加了结构体`ApCfg`和新API`int GetApConfig(ApCfg *apcfg)`和`int UpdateApConfig(ApCfg *apcfg)`来获取和更新当前AP(hostapd)的设置。可以设置`ssid`,`channel`和`passwd`.

* 新增API `void ReloadAp()`
> 功能:重载hostapd(kill -1方式)

* 新增API `void RebootAp()`
> 功能:重启hostapd(kill -9方式)

* 新增API `int GetBestChannel()`
> 功能: 通过wifi驱动获取当前的最佳信道<br>由于该接口暂时存在问题,后续更新中修复。为保证程序正常逻辑，该版本中其会始终返回14信道。

* 新增API `int AddMac2MacArray(uint8_t mac_array[][6], int mac_array_num, uint8_t *mac)`
> 功能:向数组长度为`mac_array_num`的mac地址数组`mac_array[][6]`里的空位置(为0的位置)添加一个`*mac`
> 成功返回值为0，失败返回值为-1;

* 新增API `int DelMac2MacArray(uint8_t mac_array[][6], int mac_array_num, uint8_t *mac)`
> 功能:向数组长度为`mac_array_num`的mac地址数组`mac_array[][6]`里的删除一个等于`*mac`的mac地址，将该位置的值设置为0;
> 成功返回值为0，失败返回值为-1;

## 2019.3.26更新

> 说明：本次更新修改0信道和15信道的替换信道，当设置信道为0时，hostaptd.conf里显示的信道为9;当设置信道为15时，hostaptd.conf里显示的信道为2.

* 新增配置`int MaxLinkLevel`用于单个设备设置允许级连的最大层级。其中NVR或与有线网相连的节点层级为1，每经过一个节点则加1。

* 新增log设置，可以设置libwifilink.so的log打印位置和log等级。

* 新增API`int GetLinkStatus()`
> 获取当前的网络连接状态，其返回值为

> * `OFFLINE` 离线 
> * `ETH_ONLINE` 有线网在线
> * `WLAN_ONLINE` WIFI在线
> * `ETH_ONLINE&WLAN_ONLINE` WIFI和有线网同时在线

* 新增API`uint32_t GetWifiRate()`
> 返回值为当前WIFI-STA到AP的吞吐量，单位为(bit/s)

* 新增API`void GetFatherMac(uint8_t *mac)`
> 获取当前wifi连接的AP的mac地址，其结果将拷贝到参数`uint8_t *mac`数组里。

* 新增API`int GetChannel()`
> 返回值为当前wifi的信道。

## 初始化使用配置(WifilinkCfg@wifilink.h)

* `int(*save_cb)(char*,size_t)`
> 写入文件回调函数，该库使用的所有写入文件操作都将使用该回调函数,使用方法见main.cpp

> * `char*` 为buff指针
> * `size_t` 为数据长度
> * 返回值为成功写入的长度;

* `int(*load_cb)(char*,size_t)`
> 读取文件回调函数，该库使用的所有读取文件操作都将使用该回调函数,使用方法见main.cpp

> * `char*` 为buff指针
> * `size_t` 为数据长度
> * 返回值为成功写入的长度，失败时返回值小于0;

* `void(*setip_cb)(const char *ifname)`
> 设置ip回调函数，当任意网络(wifi和有线网络)连接上时，都会调用此函数。
> * `const char *ifname` 为网卡名称("eth0"或"br0")

* `double a,b,c,d`
> 该4个变量为自组网算法参数，无必要修改时，使用默认参数

> * 默认参数 a = -45, b = -85, c = 0.7, d = 0.12

* `char ApIfName[10]`
> WifiAP的网卡名称，需要设置成与hostapd.conf中的`interface`参数一致。

* `bool WifiDbg`
> wifi驱动debug模式开关

* `int Channel`
> 设置信道，有效值为[0,15]。其中0和15为特殊频率

* `bool TxPower`
> Wifi驱动加大功率开关

* `bool CustomGroup`
> 使用自定义自组网分组规则开关
> * 当该值设置为`true`时，将使用自定义的mac地址白名单，并关闭默认自组网分组配对算法
> * 设置为`false`则使用默认自组网分组配对算法

* `uint8_t mac_db[8][6]`
> 配合`bool CustomGroup`使用，当`bool CustomGroup`设置为`true`时该参数有效。wifi自组网将使用该mac地址进行分组过滤。<br>
初始化后使用`int UpdateCustomMacDb(uint8_t **mac_db)`更新该数组。

* `bool EthFirst`
> 有线网络优先模式: 
> * 当该参数设置成`true`时，有线网(eth0)连接后将关闭wifi连接，并提供AP服务，将AP与有线网络桥接。<
> * 当该参数设置成`false`时，有线网(eth0)将与wifi共存，互不影响，当(需要自己设置路由规则)

* `int DelayTimes`
> wifi网络延迟切换时间参数。当wifi扫描到比当前连接的wifi-AP综合质量更优的AP时，将会根据此参数设定决定是否切换。
> * wifi扫描频率为15s，延迟时间 = DelayTimes*15s
> * 例如将DelayTimes设置成4，则当前连接的wifi-AP在60秒的每次扫描结果中都不是综合质量(算法决定)最优的AP，则会切换至最优AP
> * 将该参数设置为0则每次发现更优AP都会切换。

*uint8_t WifiWorkMode;    //wifi工作在ap,sta,monitor,mesh 模式下配置

*char ko_path[64];  //ko存放的位置

*char conf_path[64]; //wpa_supplicant.conf hostapd.conf 存放路径

## 常规函数

* `int WifiLinkInit(WifilinkCfg *cfg)`
> 初始化
> * `*cfg` 初始化配置

* `void WifiLinkIPCLoop()`
> IPC模式运行，该函数为阻塞函数。

* `void WifiLinkNVRLoop()`
> NVR模式运行，该函数为阻塞函数。

* `void WifiLinkCancel()`
> 使`WifiLinkIPCLoop()`和`WifiLinkNVRLoop()`停止运行。

* `int UpdateCustomMacDb(uint8_t mac_db[][6])`
> 更新自定义自组网分组配对算法中的mac白名单，其中`mac_db`的大小为`uint8_t[14][6]`。最多记录8个mac地址，不包含自己的mac地址。
> <br>当`CustomGroup`设置为`true`时该函数才生效。

* `void GetNodeInfo(NodeInfo **nInfo)`
> 获取当前wifi网络中，每个节点的mac地址和其连接的父节点的mac地址。该数据是保存在各台设备中的缓存，当某台设备离线候，则保留它最后一次更新的状态。(不能用此接口判断设备是否在线)
> * 当使用自定义自组网分组规则时(`CustomGroup = true`),该函数无效
> * 不需要为`NodeInfo *nInfo`分配内存
