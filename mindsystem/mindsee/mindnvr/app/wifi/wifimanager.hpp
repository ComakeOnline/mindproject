#pragma once
#include <thread>

class WifiManager
{
private:
    /* data */
    std::thread *m_pth = nullptr;
private:
    WifiManager(/* args */) {}
    ~WifiManager() {this->Disable();}
public:
    static WifiManager* GetInstance(){
        static WifiManager ins;
        return &ins;
    }
    int Enable();
    int Disable();
};