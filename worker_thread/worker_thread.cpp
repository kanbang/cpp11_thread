

#include "worker_thread.h"
#include "easylogging++.h"
#include <iostream>


WorkerThread::WorkerThread() :
m_b_pause(false),
m_b_stop(false),
m_b_running(false),
m_thread(NULL)
{
}

WorkerThread::~WorkerThread()
{
	if (m_b_running)
		stop();
	delete m_thread;
}

void WorkerThread::start()
{
	if (NULL == m_thread)
	{
		m_thread = new std::thread([this]{ __thread_func(); });
		LOG(TRACE) << "start thread with id: " << m_thread->get_id().hash();
		m_b_running = true;
	}
}

void WorkerThread::pause()
{
	LOG(TRACE) << "pause thread with id: " << m_thread->get_id();
	std::unique_lock<std::mutex> lock(m_mutex);
	m_b_pause = true;
	m_cond_v.notify_one();
}

void WorkerThread::resume()
{
	LOG(TRACE) << "resume thread with id: " << m_thread->get_id();
	std::unique_lock<std::mutex> lock(m_mutex);
	m_b_pause = false;
	m_cond_v.notify_one();
}

void WorkerThread::stop()
{
	LOG(TRACE) << "stop thread with id: " << m_thread->get_id();

	std::unique_lock<std::mutex> lock(m_mutex);
	m_b_stop = true;
	m_cond_v.notify_one();

	LOG(TRACE) << "joining\n";
	m_thread->join();
	LOG(TRACE) << "joined\n";

	m_b_running = false;

	LOG(TRACE) << "stop thread\n";
}

void WorkerThread::__thread_func()
{
	while (!m_b_stop)
	{
		LOG(TRACE) << "==> begin " << "function thread id : " << std::this_thread::get_id().hash();
		if (m_b_pause)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			while (m_b_pause)
			{
				m_cond_v.wait(lock);
			}
		}
		do_work();
		LOG(TRACE) << "-------------------------------------- end";
	}

	LOG(TRACE) << "done running\n";
}
