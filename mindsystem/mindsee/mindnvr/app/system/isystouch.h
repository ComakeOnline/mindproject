#pragma once

#include "global.hpp"

class CISysTouch
{
private:
    /* data */
public:
    CISysTouch(/* args */) {}
    ~CISysTouch() {}

    //刷新触屏功能
    virtual void Refresh() = 0;
};