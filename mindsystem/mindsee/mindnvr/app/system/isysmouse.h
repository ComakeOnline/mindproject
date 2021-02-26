#pragma once

#include "global.hpp"

class CISysMouse
{
private:
    /* data */
public:
    CISysMouse(/* args */) {}
    ~CISysMouse() {}

    //刷新鼠标功能
    virtual void Refresh() = 0;
};