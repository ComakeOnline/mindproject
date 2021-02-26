#pragma once

#include <thread>

#include "isysmouse.h"

class CSysMouse : public CISysMouse
{
private:
    int minx;
    int miny;
    int maxx;
    int maxy;

    //当前鼠标位置
    int xpos;
    int ypos;

    std::thread *m_tMouse;
    bool m_running;
private:
    CSysMouse(/* args */);
    ~CSysMouse();
public:
    DISABLE_COPY(CSysMouse);
    DISABLE_EQUAL(CSysMouse);

    static CSysMouse* GetInstance(){
        static CSysMouse ins;
        return &ins;
    }
    void Init();
    void DeInit();

    void MouseSet(int x,int y);
    void MouseUpdate(int dx, int dy, int dz);

    void Refresh() override;
};

