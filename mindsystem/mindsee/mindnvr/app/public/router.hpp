#pragma once
#include "message.hpp"
#include "threadpool.hpp"
//#include "log.h"

#include <atomic>
#include <map>
#include <list>
#include <functional>
#include <memory>

#define WAIT_PACKET_MUL 8

class Router
{
public:
	static Router* GetInstance() {
		static Router instance;
		return &instance;
	}
	bool Use(CMessage::MSG_TYPE index, std::function<bool(CMessage&, void*)> func) {
		if (m_routelist.find(index) == m_routelist.end())
		{
			std::list<std::function<bool(CMessage&, void*)>> funlist;
			funlist.push_back(func);
			m_routelist[index] = funlist;
		}
		else
		{
            m_routelist[index].push_back(func);
		}
        return true;
	}
	void Do(const CMessage &msg, void* _userdata) {
		if (m_threadPool.get_current_task_num() > m_threadNUM * WAIT_PACKET_MUL)
		{
			//Log::GetInstance()->warn("[router]loss message.Because waiting queue is full");
			return;
		}
		m_threadPool.commit([&](CMessage msg, void* _userdata) {
			if(m_routelist.find(msg.type) == m_routelist.end())
			{
				return;
			}
			for (auto& each : m_routelist[msg.type])
			{
				if(each(msg,_userdata) == false)
				{
					break;
				}
			}
		}, msg, _userdata);
		
	}
	void Clear() {
		m_routelist.clear();
	}
private:
	Router(size_t threadNUM = 1) :m_threadPool{ threadNUM } { m_threadNUM = threadNUM; }
	~Router(){
		m_threadPool.terminate_all_thread();
	}
private:
	size_t m_threadNUM;
	std::map<CMessage::MSG_TYPE, std::list<std::function<bool(CMessage&, void*)>>> m_routelist;
	thread_pool m_threadPool;
};
