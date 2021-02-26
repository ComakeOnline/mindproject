/*
*	logger-libyu
*	copyright - libyu
*	date - 2017-1-10
*	github - https://github.com/yuyugenius/libyu
*/
#pragma once

#include <fstream>
#include <cstdio>
#include <cstring>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <cstdlib>
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#define _getpid() getpid()
#endif // _WIN32

#define _LIBYU namespace libyu {
#define _END }

#define ENABLE_LOGGER

#ifdef ENABLE_LOGGER
#define LOGGER_INIT(fileName, level) libyu::Logger::GetInstance()->Load(fileName,level);
#define LOGGER_SET(level, fileName) libyu::Logger::GetInstance()->SetStream(level,fileName);
#define LOGGER_SAMPLE(lvl, format, ...) {std::lock_guard<std::mutex> lock{libyu::Logger::GetInstance()->mtx}; \
	if(libyu::Logger::GetInstance()->WriteHead(__FILE__,__LINE__,lvl) >= 0){ \
	libyu::Logger::GetInstance()->WriteBody(lvl, format,## __VA_ARGS__); \
	libyu::Logger::GetInstance()->WriteBody(lvl, "\n"); \
	libyu::Logger::GetInstance()->Flush(); \
	}}

#define LOGGER_ERROR(format, ...) LOGGER_SAMPLE(libyu::Logger::LEVEL::LV_ERROR,format,##__VA_ARGS__)
#define LOGGER_WARNING(format, ...) LOGGER_SAMPLE(libyu::Logger::LEVEL::LV_WARNING,format,##__VA_ARGS__)
#define LOGGER_INFO(format, ...) LOGGER_SAMPLE(libyu::Logger::LEVEL::LV_INFO,format,##__VA_ARGS__)
#define LOGGER_DEBUG(format, ...) LOGGER_SAMPLE(libyu::Logger::LEVEL::LV_DEBUG,format,##__VA_ARGS__)
#else
#define LOGGER_INIT(...)
#define LOGGER_SAMPLE(lvl, format, ...)

#define LOGGER_ERROR(format, ...)
#define LOGGER_WARNING(format, ...)
#define LOGGER_INFO(format, ...) 
#define LOGGER_DEBUG(format, ...)
#endif // ENABLE_LOGGER



#pragma warning(disable:4996)

_LIBYU

class Logger
{
public:
	enum LEVEL : int
	{
		LV_ERROR,
		LV_WARNING,
		LV_INFO,
		LV_DEBUG
	};
public:
	inline static Logger* GetInstance() {
		static Logger instance{};
		return &(instance);
	}

	template <typename  T>
	bool Load(const char* fileName, T lvl) {
		if (m_out != nullptr)
		{
			return false;
		}
		m_out = fopen(fileName, "a+");
		if (m_out != nullptr)
		{
			m_lv = (LEVEL)lvl;
			return true;
		}
		return false;
	}
	template <typename  T>
	bool Load(FILE* file, T lvl) {
		if (m_out != nullptr || (file != stdout && file != stderr))
		{
			return false;
		}
		m_out = file;
		if (m_out != nullptr)
		{
			m_lv = (LEVEL)lvl;
			return true;
		}
		return false;
	}

	template <typename  T>
	bool SetStream(T lvl, const char* fileName) {
		if (lvl > 3)
		{
			return false;
		}
		if (m_out == nullptr)
		{
			return false;
		}
		if (m_lvl_out[(int)lvl] != nullptr && m_lvl_out[(int)lvl] != stdout && m_lvl_out[(int)lvl] != stderr)
		{
			fclose(m_lvl_out[(int)lvl]);
		}
		m_lvl_out[(int)lvl] = fopen(fileName, "a+");
		if (m_lvl_out[(int)lvl] != nullptr)
		{
			//m_lv = lvl;
			return true;
		}
		return false;
	}

	template <typename  T>
	bool SetStream(T lvl, FILE* file) {
		if (lvl > 3)
		{
			return false;
		}
		if (m_out == nullptr || (file != stdout && file != stderr))
		{
			return false;
		}
		if (m_lvl_out[(int)lvl] != nullptr && m_lvl_out[(int)lvl] != stdout && m_lvl_out[(int)lvl] != stderr)
		{
			fclose(m_lvl_out[(int)lvl]);
		}
		m_lvl_out[(int)lvl] = file;
		if (m_lvl_out[(int)lvl] != nullptr)
		{
			//m_lv = lvl;
			return true;
		}
		return false;
	}

	int WriteHead(const char* file, int line, LEVEL log_lv) {
		if (m_out == nullptr || log_lv > m_lv)
		{
			return -1;
		}
		std::chrono::time_point<std::chrono::system_clock> raw_time = std::chrono::system_clock::now();
		auto now_time = std::chrono::system_clock::to_time_t(raw_time);
		auto ptm = std::localtime(&now_time);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>
			(raw_time - std::chrono::system_clock::from_time_t(std::mktime(ptm))).count();
		char str[60]{ 0 };
		sprintf(str, "[%d-%02d-%02d %02d:%02d:%02d.%03d]",
			(int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
			(int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec, (int)(ms));
		//std::cerr << str << log << std::endl;
		//m_out << log << std::endl;
		WriteBody(log_lv, str);
		WriteBody(log_lv, "[%s:%d]", file, line);
		WriteBody(log_lv, "[pid=%d]", _getpid());
		WriteBody(log_lv, "[thread=%u]", std::this_thread::get_id());
		WriteBody(log_lv, "[%s]", lv_str[log_lv].c_str());
		return log_lv;
	}

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
	template <typename  ... Types>
	void WriteBody(const int lvl, const char *head, const Types ... _value) {
		if (m_lvl_out[lvl] == nullptr)
		{
			if (m_out != stderr)
			{
				fprintf(m_out, head, _value...);
			}
		}
		else
		{
			if (m_lvl_out[lvl] != stderr)
			{
				fprintf(m_lvl_out[lvl], head, _value...);
			}
		}

		fprintf(stderr, head, _value...);
		return;
	}
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

	void Flush() {
		fflush(m_out);
	}

	std::mutex mtx;
private:
	Logger() :
		m_out{ nullptr }
	{
		//m_out.open(fileName, std::ios::out | std::ios::app);
		lv_str[LV_DEBUG] = "\033[44;30mDebug\033[0m";
		lv_str[LV_INFO] = "\033[42;30mInfo\033[0m";
		lv_str[LV_WARNING] = "\033[43;30mWarning\033[0m";
		lv_str[LV_ERROR] = "\033[41;30mError\033[0m";

		m_lvl_out[0] = nullptr;
		m_lvl_out[1] = nullptr;
		m_lvl_out[2] = nullptr;
		m_lvl_out[3] = nullptr;
	}

	~Logger()
	{
		if (m_out != nullptr && m_out != stdout && m_out != stderr)
		{
			fclose(m_out);
			m_out = nullptr;
		}
		for (int i = 0; i < 4; i++)
		{
			if (m_lvl_out[i] != nullptr && m_lvl_out[i] != stdout && m_lvl_out[i] != stderr)
			{
				fclose(m_lvl_out[i]);
				m_lvl_out[i] = nullptr;
			}
		}
	}
private:
	LEVEL m_lv;
	FILE *m_out;
	FILE *m_lvl_out[4];
	std::string lv_str[4];
};

_END
