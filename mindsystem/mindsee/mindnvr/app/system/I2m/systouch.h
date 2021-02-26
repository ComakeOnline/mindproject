#pragma once

#include <thread>

#include "isystouch.h"

class CSysTouch : public CISysTouch
{
private:
    int abs_x;
    int abs_y;
    std::thread *m_tTouch;
    bool m_running;
private:
    CSysTouch(/* args */);
    ~CSysTouch();
public:
    DISABLE_COPY(CSysTouch);
    DISABLE_EQUAL(CSysTouch);

    static CSysTouch* GetInstance(){
        static CSysTouch ins;
        return &ins;
    }
    void Init();
    void DeInit();

    void Refresh() override;
};

