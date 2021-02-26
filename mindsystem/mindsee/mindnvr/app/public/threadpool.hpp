/*
*	threadpool-libyu
*	copyright - libyu
*	date - 2017-1-10
*	github - https://github.com/66neko/libyu
*/
#pragma once
#include <memory>
#include <queue>
#include <mutex>
#include <string>
#include <functional>
#include <iostream>
#include <thread>
#include <atomic>
#include <future>
#include <condition_variable>

#ifdef __linux__
#include <sys/prctl.h>
#define _LIBYU_TP_SET_THREAD_NAME(x) prctl(PR_SET_NAME, x);
#else
#define _LIBYU_TP_SET_THREAD_NAME(x)
#endif // __linux__

class thread_pool {
	using Task = std::function<void()>;
private:
	std::vector<std::thread> pool;
	std::queue<Task> tasks;
	std::mutex m_task;
	std::condition_variable cv_task;
	std::atomic<bool> commit_flag;
	std::atomic<bool> terminate_flag;

public:
	thread_pool(size_t size = 0) : commit_flag{ false }, terminate_flag(false) {
		size = size < 1 ? std::thread::hardware_concurrency() : size;
		for (size_t i = 0; i< size; ++i) {
			pool.emplace_back(&thread_pool::schedual, this);
		}
	}

	~thread_pool() {
	}

	void close_commit() {
		this->commit_flag.store(true);
	}

	bool get_commit_flag() {
		return commit_flag.load();
	}

	void open_commit() {
		this->commit_flag.store(false);
	}

	void terminate_all_thread() {
		this->terminate_flag.store(true);
		cv_task.notify_all();
		for (std::thread& thread : pool) {
			thread.join();
		}
	}

	size_t get_current_task_num() {
		std::lock_guard<std::mutex> lock{ m_task };
		return tasks.size();
	}

	void wait_and_terminate_all() {
		while (get_current_task_num() > 0) {};
		terminate_all_thread();
	}

	template<class F, class... Args>
	auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))> {
		if (commit_flag.load()) {
			throw std::runtime_error("task pool have closed commit.");
		}
		using ResType = decltype(f(args...));
		auto task = std::make_shared<std::packaged_task<ResType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		{
			std::lock_guard<std::mutex> lock{ m_task };
			tasks.emplace([task]() {(*task)(); });
		}
		cv_task.notify_all();
		std::future<ResType> future = task->get_future();
		return future;
	}

private:
	bool get_one_task(Task& task) {
		std::unique_lock<std::mutex> lock{ m_task };
		cv_task.wait(lock, [this]() { return !tasks.empty() || terminate_flag.load(); });
		if (!terminate_flag.load()) {
			task = std::move(tasks.front());
			tasks.pop();
			return true;
		}
		return false;
	}

	void schedual() {
		_LIBYU_TP_SET_THREAD_NAME("LIBYU_THREADPOOL");
		while (!terminate_flag.load()) {
			Task task;
			if (get_one_task(task)) {
				task();
			}
			else {
				std::this_thread::sleep_for(std::chrono::microseconds(1));
			}
		}
	}
};

