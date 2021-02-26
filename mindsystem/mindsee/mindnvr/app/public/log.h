#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/async.h"


class Log
{
public:
    static std::shared_ptr<spdlog::logger> GetInstance(){
        static Log ins;
        return ins.m_lg;
    }
private:
    Log(/* args */) {
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S %e] [%n] [%l] [process %P] [thread %t] %v");
        m_lg = spdlog::daily_logger_mt<spdlog::async_factory>("daily_logger", "logs/daily.txt", 0, 1);
        spdlog::flush_every(std::chrono::seconds(3));
    }
    ~Log() { }

private:
    std::shared_ptr<spdlog::logger> m_lg;
};