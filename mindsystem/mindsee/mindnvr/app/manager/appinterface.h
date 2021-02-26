#pragma once
#include <mutex>

//start by lison.guo
#define INDEX_INVALID -1
//end by lison.guo

class AppInterface
{
private:
    /* data */
    std::mutex m_mtx;
	
	//start by lison.guo
	//获取index 值，方便在UI 中使用
	int index;
	//end by lison.guo
	
private:
    AppInterface(/* args */);
    ~AppInterface();
public:
    static AppInterface* GetInstance(){
        static AppInterface ins;
        return &ins;
    }

	//start by lison.guo
	//想要获取index 值。
	int GetIPCIndex();
	int SetIPCIndex(int DestIndex);
	//end by lison.guo

    //API
    /*
        * 刷新画面 回到主画面 
     */
    void RefreshToMainPage();

    /*
        * 设置画面到全屏
     */
    bool SetToFullScreen(int _index);

    /*
        * 打开语音对讲
        * 参数_index : ipc频道号
    */
    int PlayTalk(int _index);

    /*
        * 关闭所有语音对讲
    */
    int StopAllTalk();

    /*
        * 打开IPC音频
        * 参数_index : ipc频道号
    */
    int PlayAudio(int _index);

    /*
        * 关闭所有IPC音频
    */
    int StopAllAudio();

    /*
        * 获取音量 参数 db
    */
    int GetVolume();

	/*
        * 设置音量 参数 db
    */
    int SetVolume(int _db);

    /*
        * 打开IPC网络统计信息消息
    */
    void OpenNetInformation(const uint32_t &_chn);

    /*
        * 关闭IPC网络统计信息消息
    */
    void CloseNetInformation(const uint32_t &_chn);
};


