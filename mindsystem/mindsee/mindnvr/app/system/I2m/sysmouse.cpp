#include "sysmouse.h"

#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>

#include "mstarFb.h"
#include "sysdisplay.h"
#include "config.hpp"
#include "router.hpp"

using namespace std;

#define MOUSE_DEV_NAME "/dev/input/mice"
#define MOUSE_ICON_NAME "cursor.raw"
/* Mouse button bits*/
#define WHEEL_UP    0x10
#define WHEEL_DOWN  0x08

#define BUTTON_L    0x04
#define BUTTON_M    0x02
#define BUTTON_R    0x01
#define SCALE       1 /* default scaling factor for acceleration */
#define THRESH      1 /* default threshhold for acceleration */

static int IMPS2_Read(int fd, int *dx, int *dy, int *dz, int *bp)
{
    static unsigned char buf[5];
    static int buttons[7] = {0, 1, 3, 0, 2, 0, 0};// 1:left button, 2: mid button, 3: right button
    static int nbytes = 0;
    int n;

    while ((n = read (fd, &buf [nbytes], 4 - nbytes)))
    {
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            else
                return -1;
        }

        nbytes += n;

        if (nbytes == 4)
        {
            int wheel;
            // printf("[luther.gliethttp]: %02x %02x %02x %02x\n", buf[0], buf[1], buf[2], buf[3]);
            if ((buf[0] & 0xc0) != 0)
            {
                buf[0] = buf[1];
                buf[1] = buf[2];
                buf[2] = buf[3];
                nbytes = 3;
                return -1;
            }

            /* FORM XFree86 4.0.1 */
            *bp = buttons[(buf[0] & 0x07)];
            *dx = (buf[0] & 0x10) ? buf[1] - 256 : buf[1];
            *dy = (buf[0] & 0x20) ? -(buf[2] - 256) : -buf[2];

            /* Is a wheel event? */
            if ((wheel = buf[3]) != 0)
            {
                if(wheel > 0x7f)
                {
                    *bp |= WHEEL_UP;
                }
                else
                {
                    *bp |= WHEEL_DOWN;
                }
            }

            *dz = 0;
            nbytes = 0;
            return 1;
        }
    }

    return 0;
}


CSysMouse::CSysMouse(/* args */)
:m_tMouse{nullptr},
m_running{false}
{
}

CSysMouse::~CSysMouse()
{
    DeInit();
}

void CSysMouse::Init()
{
    minx = 0;
    miny = 0;
    maxx = config::Config::GetInstance()->m_allcfg.resolution_width;
    maxy = config::Config::GetInstance()->m_allcfg.resolution_height;
    //open
    MI_FB_CursorAttr_t stCursorAttr;
    FILE *fp = NULL;
    char *pbuff = (char*)malloc(40*40*4);
    fp = fopen(MOUSE_ICON_NAME, "rb");
    if (fp)
    {
        int ret = fread(pbuff, 1, 40*40*4, fp);
        
        LOGGER_ERROR("!!!%d",ret);
        fclose(fp);
    }
    //set curosr Icon && set positon
    stCursorAttr.stCursorImageInfo.u32Width = 40;
    stCursorAttr.stCursorImageInfo.u32Height = 40;
    stCursorAttr.stCursorImageInfo.u32Pitch = 40; //?????
    stCursorAttr.stCursorImageInfo.eColorFmt = E_MI_FB_COLOR_FMT_ARGB8888;
    stCursorAttr.stCursorImageInfo.data = pbuff;
    stCursorAttr.u32HotSpotX = 5;
    stCursorAttr.u32HotSpotY = 5;
    stCursorAttr.u32XPos = 100;
    stCursorAttr.u32YPos = 600;
    stCursorAttr.u16CursorAttrMask = E_MI_FB_CURSOR_ATTR_MASK_ICON
        | E_MI_FB_CURSOR_ATTR_MASK_SHOW | E_MI_FB_CURSOR_ATTR_MASK_POS;
    auto fbfd = CSysDisplay::GetInstance()->GetFbFd();
    cout<<"fbfd = "<<fbfd<<endl;
    int ret;
    if (ret = ioctl(fbfd, FBIOSET_CURSOR_ATTRIBUTE, &stCursorAttr)) {
        LOGGER_ERROR("Error FBIOSET_CURSOR_ATTRIBUTE");
    }
    if (pbuff)
    {
        free(pbuff);
    }
    m_tMouse = new std::thread{[this](){
        auto R = Router::GetInstance();
        int fd,retval;
        fd_set readfds;
        struct input_event inputEv[64];
        int readlen = 0;
        int i = 0;
        unsigned char buf[32];
        int x_pos, y_pos;
        unsigned char imps2_param [] = {243, 200, 243, 100, 243, 80};
        int dx, dy, dz, button;
        int old_xpos, old_ypos;
        MI_SYS_WindowRect_t stRect;
        int d_xpos, d_ypos = 0;

        do
        {
            fd = open(MOUSE_DEV_NAME, O_RDWR);
            if (fd < 0)
            {
                printf("can not open %s\n", MOUSE_DEV_NAME);
            }
            sleep (5);
        } while (fd < 0);

        printf("open %s success, fd:%d\n", MOUSE_DEV_NAME, fd);

        write(fd, imps2_param, sizeof (imps2_param));
        struct timeval tv;
        memset(&tv,0,sizeof(tv));
        tv.tv_sec = 3;

        bool lbtnstatus{false};
        bool rbtnstatus{false};

        while(m_running)
        {
            FD_ZERO(&readfds);
            FD_SET(fd, &readfds);
            retval = select(fd + 1, &readfds, NULL, NULL, &tv);
            if(retval == 0)
            {
                //printf("Time out!\n");
                continue;
            }

            if(FD_ISSET(fd, &readfds))
            {
                IMPS2_Read(fd, &dx, &dy, &dz, &button);

                this->MouseUpdate(dx, dy, dz);

                if (button == 0x1 && !lbtnstatus)
                {
                    lbtnstatus = true;
                    // left button down
                    CMessage mmsg;
                    mmsg.type = CMessage::E_MSG_MOUSE_DOWN;
                    mmsg.mouse_key_status = CMessage::E_MOUSE_STATUS_LEFT_DOWN;
                    mmsg.mouse_x = xpos;
                    mmsg.mouse_y = ypos;
                    R->Do(mmsg,nullptr);
                }
                else if (button == 0x3 && !rbtnstatus)
                {
                    rbtnstatus = true;
                    // right button down
                    CMessage mmsg;
                    mmsg.type = CMessage::E_MSG_MOUSE_DOWN;
                    mmsg.mouse_key_status = CMessage::E_MOUSE_STATUS_RIGHT_DOWN;
                    mmsg.mouse_x = xpos;
                    mmsg.mouse_y = ypos;
                    R->Do(mmsg,nullptr);
                }
                else if(button == 0x0)
                {
                    if(lbtnstatus)
                    {
                        lbtnstatus = false;
                        // left button up
                        CMessage mmsg;
                        mmsg.type = CMessage::E_MSG_MOUSE_UP;
                        mmsg.mouse_key_status = CMessage::E_MOUSE_STATUS_LEFT_UP;
                        mmsg.mouse_x = xpos;
                        mmsg.mouse_y = ypos;
                        R->Do(mmsg,nullptr);
                    }
                    if(rbtnstatus)
                    {
                        rbtnstatus = false;
                        // right button up
                        CMessage mmsg;
                        mmsg.type = CMessage::E_MSG_MOUSE_UP;
                        mmsg.mouse_key_status = CMessage::E_MOUSE_STATUS_RIGHT_UP;
                        mmsg.mouse_x = xpos;
                        mmsg.mouse_y = ypos;
                        R->Do(mmsg,nullptr);
                    }
                }

                

                // printf("xpos:%d,ypos:%d, button:0x%X\n", xpos, ypos, button);
                this->MouseSet(xpos, ypos);
                old_xpos = xpos;
                old_ypos = ypos;
            }
        }
        close(fd);
    }};
    this->MouseSet(1, 1);
    m_running = true;
}

void CSysMouse::DeInit()
{
    m_running = false;
    if(m_tMouse)
    {
        m_tMouse->join();
        delete m_tMouse;
        m_tMouse = nullptr;
    }
}

void CSysMouse::MouseSet(int x,int y)
{
    xpos = x;
    ypos = y;
    MI_FB_CursorAttr_t stCursorAttr;

    stCursorAttr.u32XPos = x;
    stCursorAttr.u32YPos = y;
    stCursorAttr.u16CursorAttrMask = E_MI_FB_CURSOR_ATTR_MASK_POS;
    auto fbfd = CSysDisplay::GetInstance()->GetFbFd();
    if (ioctl(fbfd, FBIOSET_CURSOR_ATTRIBUTE, &stCursorAttr)) {
        LOGGER_ERROR("Error FBIOSET_CURSOR_ATTRIBUTE");
    }
}


void CSysMouse::MouseUpdate(int dx, int dy, int dz)
{
    int r;
    int sign;

    sign = 1;
    if (dx < 0)
    {
        sign = -1;
        dx = -dx;
    }

    if (dx > THRESH)
        dx = THRESH + (dx - THRESH) * SCALE;

    dx *= sign;
    xpos += dx;

    if( xpos < minx )
        xpos = minx;
    if( xpos > maxx )
        xpos = maxx;

    sign = 1;
    if (dy < 0)
    {
        sign = -1;
        dy = -dy;
    }

    if (dy > THRESH)
         dy = THRESH + (dy - THRESH) * SCALE;

    dy *= sign;
    ypos += dy;

    if (ypos < miny)
        ypos = miny;

    if (ypos > maxy)
        ypos = maxy;
}

void CSysMouse::Refresh()
{
    this->DeInit();
    this->Init();
}