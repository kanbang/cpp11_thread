#pragma once
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>

class WorkerThread
{
public:
	WorkerThread();
	virtual ~WorkerThread();
	void start();
	void pause();
	void resume();
	void stop();

protected:
	std::mutex m_mutex;
	std::condition_variable m_cond_v;
	std::thread* m_thread;
	std::atomic<bool> m_b_stop;
	std::atomic<bool> m_b_pause;
	std::atomic<bool> m_b_running;

private:
	virtual void do_work() = 0;
	void __thread_func();
};
