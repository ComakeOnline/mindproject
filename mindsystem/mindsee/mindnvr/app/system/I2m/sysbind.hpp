#pragma once

#include <cstdint>
#include <cstring>

#include "global.hpp"
#include "mi_sys.h"

class CSysBind
{
public:
    MI_SYS_ChnPort_t stSrcChnPort;
    MI_SYS_ChnPort_t stDstChnPort;
public:
    DISABLE_COPY(CSysBind);
    DISABLE_EQUAL(CSysBind);

    CSysBind(const MI_SYS_ChnPort_t& _stSrcChnPort, const MI_SYS_ChnPort_t& _stDstChnPort
    , uint32_t u32SrcFrmrate, uint32_t u32DstFrmrate)
    {
        memcpy(&stSrcChnPort, &_stSrcChnPort, sizeof(stSrcChnPort));
        memcpy(&stDstChnPort, &_stDstChnPort, sizeof(stDstChnPort));
        LOGGER_INFO("bind src:%d,%d,%d,%d - dst:%d,%d,%d,%d",stSrcChnPort.eModId,
            stSrcChnPort.u32ChnId,stSrcChnPort.u32DevId,stSrcChnPort.u32PortId,
            stDstChnPort.eModId,stDstChnPort.u32ChnId,stDstChnPort.u32DevId,stDstChnPort.u32PortId);
        CHECKRESULT(MI_SYS_BindChnPort(&stSrcChnPort, &stDstChnPort, \
        u32SrcFrmrate, u32DstFrmrate), MI_SUCCESS);
    }
    ~CSysBind() {
        CHECKRESULT(MI_SYS_UnBindChnPort(&stSrcChnPort, &stDstChnPort), MI_SUCCESS);
    }
};