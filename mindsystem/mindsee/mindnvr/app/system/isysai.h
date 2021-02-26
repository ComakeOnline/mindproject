#pragma once

#include <functional>

#include "global.hpp"
#include "isysao.h"


class CISysAi
{
private:
    /* data */
public:
    
public:
    CISysAi(/* args */) {}
    ~CISysAi() {}

    //开启VI
    virtual void Refresh() = 0;

    virtual void DeInit() = 0;

    //设置音量
    //virtual void SetVolume(int _db) = 0;

    virtual int Register(int _aechn,const std::function<void(CISysAo::AudioFrame_st)> &_cb) = 0;
    virtual int CancelRegister(int _id) = 0;
};