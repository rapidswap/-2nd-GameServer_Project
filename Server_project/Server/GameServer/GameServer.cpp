#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include<Windows.h>

mutex m;
queue<int32> q;
HANDLE handle;

void Producer()
{
	

	while (true) {
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		::SetEvent(handle);
		this_thread::sleep_for(10000ms);
	}
	
}

void Consumer()
{
	while (true) {
		::WaitForSingleObject(handle, INFINITE);

		unique_lock<mutex> lock(m);
		if (q.empty() == false) {
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}

int main()
{

	handle = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	thread t1(Producer);
	thread t2(Consumer);
	
	t1.join();
	t2.join();
	

	::CloseHandle(handle);
}
