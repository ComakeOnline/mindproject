#pragma once
#include <string>

class CMessage
{
public:
    enum MSG_TYPE {
        E_MSG_NULL,
        E_MSG_MOUSE_DOWN,
        E_MSG_MOUSE_UP,
        E_MSG_MOUSE_MOVE,
        E_MSG_TOUCH_DOWN,
        E_MSG_TOUCH_UP,
        E_MSG_TOUCH_MOVE,
        E_MSG_VIDEO_NET_MESSAGE,
        E_MSG_AI_HC_RESULT,
    };
    enum MOUSE_KEY{
        E_MOUSE_STATUS_NULL,
        E_MOUSE_STATUS_LEFT_DOWN,
        E_MOUSE_STATUS_LEFT_UP,
        E_MOUSE_STATUS_RIGHT_DOWN,
        E_MOUSE_STATUS_RIGHT_UP,
    };

public:
    MSG_TYPE type = E_MSG_NULL;
    MOUSE_KEY mouse_key_status = E_MOUSE_STATUS_NULL;
    int mouse_x = 0;
    int mouse_y = 0;
    uint32_t param_1;
    uint32_t param_2;
    uint32_t param_3;
    uint32_t param_4;
    std::string data;
public:
    CMessage(/* args */) {}
    ~CMessage() {}
};