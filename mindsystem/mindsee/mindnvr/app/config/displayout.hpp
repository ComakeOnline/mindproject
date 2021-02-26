#pragma once
#include <cstdint>
#include <vector>

namespace config
{
/*
    Disp --> Layer1 -->Port 0 ~ 15
         --> Layer1 -->Port 0 ~ 15
*/

struct PortWnd_St
{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t hetght;
};

struct LayerWnd
{
    //layer size
    uint16_t vsize_width;
    uint16_t vsize_height;
    //windows pos
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    std::vector<PortWnd_St> vecPort;
};


// fullscreen mode
enum ELayout : int {
    E_LAYOUT_FULLSCREEN, //单画面全屏幕
    E_LAYOUT_4PIC_SAMPLE,  //四画面标准布局
};


} // namespace config
