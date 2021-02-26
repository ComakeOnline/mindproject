/*===================================================================
//
//	八爪鱼消息分发器
//	实现消息的订阅/发布模式，用于进程间通信（可以跨机器）
//	topic类型为string
//	Jerry.Zhu 2019-08-30
//
===================================================================*/

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <map>
#include <vector>
#include <list>
#include <functional>
#include <memory.h>
#include <assert.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/un.h>
#include <sys/timeb.h>
#include <dlfcn.h>
#include <stddef.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/prctl.h>
#include "loopbuffer.h"
using namespace std;


#define DE_TopicLen 32//topic的最大长度为 DE_TopicLen - 1
typedef enum {
	PT_OrderTopic = 1000,//订阅消息
	PT_CancelOrderTopic,//取消订阅
	PT_TopicMsg,//发布消息数据
}PackageType;

struct Package_OrderTopic
{
	int packageLen = sizeof(Package_OrderTopic);
	PackageType type = PT_OrderTopic;
	char topic[DE_TopicLen] = "";
};

struct Package_CancelOrderTopic
{
	int packageLen = sizeof(Package_CancelOrderTopic);
	PackageType type = PT_CancelOrderTopic;
	char topic[DE_TopicLen] = "";
};

struct Package_TopicMsg
{
	int packageLen = sizeof(Package_TopicMsg);
	PackageType type = PT_TopicMsg;
	char topic[DE_TopicLen] = "";
	char data[0];
	void insertData(string topic, void* data, int dataLen)
	{
		packageLen = sizeof(Package_TopicMsg) + dataLen;
		type = PT_TopicMsg;
		strncpy(this->topic, topic.data(), DE_TopicLen - 1);
		this->topic[DE_TopicLen - 1] = '\0';
		memcpy(this->data, data, dataLen);
	}
};


class OctopusServer;
class OctopusClient;
class Net
{
	friend class OctopusServer;
	friend class OctopusClient;

	int sock = 0;//此sock可能作为服务器的sock，也可以作为客户端的sock，看以什么方式启动
	string ip;
	int port;
	OctopusServer* pOctopusServer;
	OctopusClient* pOctopusClient;
	bool isRunning = false;
public:
	Net(OctopusClient* pOctopusClient = NULL, OctopusServer* pOctopusServer = NULL);
	void setConfig(string ip, int port);
	int connect_nonb(int sockfd, const char* ip, int port, int nsec);
	bool sendData(int sock, const char* buf, int len);//通用的发送数据
	bool sendData(const char* buf, int len);//客户端用的发送数据
	static bool dealWithPackage(Net* pNet, int sock, LoopBuffer& loopBuffer);
	static void serverForClientWorkThread(Net* pNet, int clientSock);//服务端为每个链接过来的客户端创建的服务线程
	static void acceptThread(Net* pNet);
	bool startAsServer();//以服务器形式启动
	static void clientThread(Net* pNet);//用客户端形式启动后接受服务器信息的现场
	bool startAsClient();//以客户端形式启动
	void stopRuning();
};


class OctopusServer
{
	Net* pNet;
	map<string, vector<int>> topicUsersMap;//保存topic和对于的用户socket队列
	mutex topicUsersMapMutex;
	map<int, vector<string>> userTopicesMap;//保存socket和其订阅的topic
	mutex userTopicesMapMutex;
public:
	OctopusServer();
	~OctopusServer();
	bool startOctopusServer();//开始运行的接口
public:
	void OrderTopic(string topic, int sock);//某用户订阅某topic
	void CancelOrderTopic(string topic, int sock);//取消某用户订阅的某个topic
	void cancelSockAllTopic(int sock);//取消某个sock用户订阅的所有topic
	void turnTopicMsg(string topic, const char* data, int dataLen);//转发topicMsg数据包
};


class OctopusClient
{
	friend class Net;

	Package_TopicMsg* pTopicMsg;
	int topicMsgDataBufferLen;//记录pTopicMsg的data数据缓冲区大小
	Net* pNet;
	string serverIp;
	int port;

	//接受到TopicMsg数据后，根据topic查询其回调函数，回调函数参数分别是：topic， 数据部地址，数据部长度
	map<string, function<void(const string&, char*, int)>> callBackMap;
	mutex callBackMapMutex;
	void RunCallBack(const string topic, char* data, int dataLen);//调用消息对应的回调函数
	static void connectThread(OctopusClient* p);
public:
	OctopusClient();
	~OctopusClient();
	//开始运行的接口, ip和端口是服务器端的ip和端口，里面启动链接服务的的线程
	bool startOctopusClient(string ip, int port);
public:

	bool sendTopicMsg(string topic, const char* data, int dataLen);
	bool OrderTopic(string topic, function<void(const string, char*, int)>cb);//订阅消息要把处理消息的回调函数传入，消息到达就会被调用
	bool CancelOrderTopic(string topic);
};
