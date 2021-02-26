#pragma once

#include <cstdio>
#include <stdexcept>
#include <cassert>

#ifdef __linux__
#include <arpa/inet.h>
#endif

#include "logger.hpp"

#define DISABLE_COPY(classname) classname(const classname&) = delete
#define DISABLE_EQUAL(classname) classname & operator = (const classname&) = delete

#define CHECKRESULT(result, value)\
    if (result != value)\
    {\
        fprintf(stderr,"[%s %s %d]exec function failed, ret = %d\n", __FILE__, __FUNCTION__, __LINE__, result);\
        assert(0);\
    }\
    else\
    {\
        fprintf(stderr,"(%s %s %d)exec function pass\n", __FILE__, __FUNCTION__,__LINE__);\
    }

#ifndef GIT_VERSION
#define GIT_VERSION '0'
#endif

#ifndef GIT_PATH
#define GIT_PATH '0'
#endif

class Global
{
private:
    /* data */
private:
    Global(/* args */) {}
    ~Global() {}
public:
    static uint32_t IpStr2B(const std::string &ip){
        uint32_t ipaddr{0};
#ifdef __linux__
        inet_pton(AF_INET, ip.c_str(), &ipaddr);
#endif
        return ipaddr;
    }

    static std::string IpB2Str(const uint32_t &ipaddr){
        unsigned char *ptr = (unsigned char*)&ipaddr;
        char str[20];
        snprintf(str, 20, "%d.%d.%d.%d", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3));
        return std::string(str);
    }
};