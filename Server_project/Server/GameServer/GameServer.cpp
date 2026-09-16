#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include<Windows.h>
#include <future>

// __declspec(thread) int32 value;

thread_local int32 LThreadId = 0;

void ThreadMain(int32 threadId)
{
	LThreadId = threadId;

	while (true)
	{
		cout << "Hi! I am Thread " << LThreadId << endl;
		this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main()
{
	
	vector<thread> threads;

	for (int32 i = 0;i < 10;++i)
	{
		int32 threadId = i + 1;
		threads.emplace_back(ThreadMain,threadId);
	}

	for (thread& t : threads)
	{
		t.join();
	}
}
