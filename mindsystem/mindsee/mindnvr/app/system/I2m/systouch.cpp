#include "systouch.h"

#include <linux/version.h>
#include <linux/input.h>

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <ctype.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


#include "config.hpp"
#include "router.hpp"

#define FILENEME "/dev/input/event1"

using namespace std;

/**
 * Filter for the AutoDevProbe scandir on /dev/input.
 *
 * @param dir The current directory entry provided by scandir.
 *
 * @return Non-zero if the given directory entry starts with "event", or zero
 * otherwise.
 */
static int is_event_device(const struct dirent *dir) {
	return strncmp("event", dir->d_name, 5) == 0;
}

CSysTouch::CSysTouch()
:m_running{false}
,m_tTouch{nullptr}
{
}

CSysTouch::~CSysTouch()
{

}


void CSysTouch::Init()
{
    if(m_running || m_tTouch != nullptr)
    {
        LOGGER_ERROR("Repeat SysTouch Init");
        return;
    }
    m_running = true;
    m_tTouch = new thread{[this](){
        int fd;
        dirent **namelist;
        char filename[256];
        memset(filename,0, sizeof(filename));
        int ndev = scandir("/dev/input", &namelist, is_event_device, versionsort);
        for (int i = 0; i < ndev; i++)
        {
            char fname[64];
            int fd = -1;
            char name[256];
            memset(name,0,sizeof(name));

            snprintf(fname, sizeof(fname),
			    "%s/%s", "/dev/input", namelist[i]->d_name);
            fd = open(fname, O_RDONLY);
            if (fd < 0)
                continue;
            ioctl(fd, EVIOCGNAME(sizeof(name)), name);

            if(strstr(name,"TouchScreen") != nullptr)
            {
                memcpy(filename,fname,strlen(fname));
            }
            close(fd);

            free(namelist[i]);
        }
        if ((fd = open(filename, O_RDONLY)) < 0) {
            if (errno == EACCES && getuid() != 0)
                LOGGER_ERROR("You do not have access to %s. Try "
                        "running as root instead.\n",
                        filename);
            return;
	    }
        struct input_event ev[64];
        int i, rd;
        fd_set rdfs;

        //自定义数据初始化
        abs_x = 0;
        abs_y = 0;
        int btn_touch = 0;
        auto R = Router::GetInstance();

        FD_ZERO(&rdfs);
        FD_SET(fd, &rdfs);

        while (m_running) {
            select(fd + 1, &rdfs, NULL, NULL, NULL);
            if (!m_running)
                break;
            rd = read(fd, ev, sizeof(ev));

            if (rd < (int) sizeof(struct input_event)) {
                printf("expected %d bytes, got %d\n", (int) sizeof(struct input_event), rd);
                perror("\nevtest: error reading");
                return;
            }

            for (i = 0; i < rd / sizeof(struct input_event); i++) {
                unsigned int type, code;

                type = ev[i].type;
                code = ev[i].code;

                //printf("Event: time %ld.%06ld, ", ev[i].time.tv_sec, ev[i].time.tv_usec);

                if (type == EV_SYN) {
                    if (code == SYN_MT_REPORT)
                    {
                        //printf("++++++++++++++ %s ++++++++++++\n", codename(type, code));
                    }
                    else if (code == SYN_DROPPED)
                    {
                        //printf(">>>>>>>>>>>>>> %s <<<<<<<<<<<<\n", codename(type, code));
                    }
                    else if (code == SYN_REPORT)
                    {
                        CMessage mmsg;
                        if(btn_touch == 2)
                        {
                            mmsg.type = CMessage::E_MSG_TOUCH_MOVE;
                        }
                        else if(btn_touch == 1)
                        {
                            btn_touch = 2;
                            mmsg.type = CMessage::E_MSG_TOUCH_DOWN;
                        }
                        else if(btn_touch == 0)
                        {
                            mmsg.type = CMessage::E_MSG_TOUCH_UP;
                        }
                        mmsg.mouse_key_status = CMessage::E_MOUSE_STATUS_NULL;
                        mmsg.mouse_x = abs_x;
                        mmsg.mouse_y = abs_y;
                        R->Do(mmsg,nullptr);
                    }
                    else
                    {
                        //printf("-------------- %s ------------\n", codename(type, code));
                    }
                } else {
                    // printf("type %d (%s), code %d (%s), ",
                    //     type, typename(type),
                    //     code, codename(type, code));
                    // if (type == EV_MSC && (code == MSC_RAW || code == MSC_SCAN))
                    //     printf("value %02x\n", ev[i].value);
                    // else
                    //     printf("value %d\n", ev[i].value);
                    if(type == EV_ABS && code == ABS_MT_POSITION_X)
                    {
                        abs_x = ev[i].value;
                    }
                    else if(type == EV_ABS && code == ABS_MT_POSITION_Y)
                    {
                        abs_y = ev[i].value;
                    }
                    else if(type == EV_KEY && code == BTN_TOUCH)
                    {
                        btn_touch = ev[i].value;
                    }

                }
            }

        }

        ioctl(fd, EVIOCGRAB, (void*)0);
        return;
    }};
}
void CSysTouch::DeInit()
{
    m_running = false;
    if(m_tTouch)
    {
        m_tTouch->join();
        delete m_tTouch;
        m_tTouch = nullptr;
    }
}

void CSysTouch::Refresh()
{
    this->DeInit();
    this->Init();
}