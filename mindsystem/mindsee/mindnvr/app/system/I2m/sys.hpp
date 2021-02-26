#pragma once

#include <cstring>

#include "global.hpp"
#include "mi_sys.h"
#include "config.hpp"

class CSys
{
private:
    /* data */
public:
    CSys(/* args */) {
        m_initConut++;
        if(m_initConut == 1)
        {
            CHECKRESULT(MI_SYS_Init(), MI_SUCCESS);
            MI_SYS_Version_t stVersion;
            MI_U64 u64Pts = 0;
            memset(&stVersion, 0x0, sizeof(MI_SYS_Version_t));
            CHECKRESULT(MI_SYS_GetVersion(&stVersion), MI_SUCCESS);
            //ST_INFO("(%d) u8Version:0x%llx\n", __LINE__, stVersion.u8Version);
            CHECKRESULT(MI_SYS_GetCurPts(&u64Pts), MI_SUCCESS);
            //ST_INFO("(%d) u64Pts:0x%llx\n", __LINE__, u64Pts);

            u64Pts = 0xF1237890F1237890;
            CHECKRESULT(MI_SYS_InitPtsBase(u64Pts), MI_SUCCESS);

            u64Pts = 0xE1237890E1237890;
            CHECKRESULT(MI_SYS_SyncPts(u64Pts), MI_SUCCESS);
        }
    }
    ~CSys() {
        m_initConut--;
        if(m_initConut == 0)
        {
            CHECKRESULT(MI_SYS_Exit(), MI_SUCCESS);
        }
    }
private:
    static int m_initConut;
};