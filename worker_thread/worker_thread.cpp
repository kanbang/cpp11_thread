

#include "worker_thread.h"
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
		std::cout << "start thread with id: " << m_thread->get_id() << std::endl;
		m_b_running = true;
	}
}

void WorkerThread::pause()
{
	std::cout << "pause thread with id: " << m_thread->get_id() << std::endl;
	std::unique_lock<std::mutex> lock(m_mutex);
	m_b_pause = true;
	m_cond_v.notify_one();
}

void WorkerThread::resume()
{
	std::cout << "resume thread with id: " << m_thread->get_id() << std::endl;
	std::unique_lock<std::mutex> lock(m_mutex);
	m_b_pause = false;
	m_cond_v.notify_one();
}

void WorkerThread::stop()
{
	std::cout << "stop thread with id: " << m_thread->get_id() << std::endl;

	std::unique_lock<std::mutex> lock(m_mutex);
	m_b_stop = true;
	m_cond_v.notify_one();

	std::cout << "joining\n";
	m_thread->join();
	std::cout << "joined\n";

	m_b_running = false;

	std::cout << "stop thread\n";
}

void WorkerThread::__thread_func()
{
	while (!m_b_stop)
	{
		std::cout << "==> begin " << "function thread id : " << std::this_thread::get_id() << std::endl;
		if (m_b_pause)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			while (m_b_pause)
			{
				m_cond_v.wait(lock);
			}
		}
		do_work();
		std::cout << "-------------------------------------- end" << std::endl << std::endl;
	}

	std::cout << "done running\n";
}
