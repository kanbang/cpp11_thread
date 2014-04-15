// worker_thread.cpp : 定义控制台应用程序的入口点。
//


#include <stdio.h>
#include <tchar.h>
#include "worker_thread.h"
#include <iostream>
#include <iomanip>

#include "easylogging++.h"

class MyThread : public WorkerThread
{
private:
	virtual void do_work()
	{
		static int n = 0;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		LOG(TRACE) << "my_do_work:" << std::setw(4) << ++n;
	}

};


_INITIALIZE_EASYLOGGINGPP

int _tmain(int argc, _TCHAR* argv[])
{
	MyThread t;
	t.start();
	std::this_thread::sleep_for(std::chrono::seconds(10));
	t.pause();
	std::this_thread::sleep_for(std::chrono::seconds(10));
	t.resume();
	std::this_thread::sleep_for(std::chrono::seconds(10));
	t.stop();

	system("pause");
	return 0;
}
