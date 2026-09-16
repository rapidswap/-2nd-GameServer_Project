#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include<Windows.h>
#include <future>
#include <chrono>

#include "ConcurrentQueue.h"
#include "ConcurrentStack.h"
using namespace std::chrono_literals;

// __declspec(thread) int32 value;

LockQueue<int32> q;
LockStack<int32> s;

void Push()
{
	while (true)
	{
		int32 value = rand() % 100;
		q.Push(value);

		this_thread::sleep_for(10ms);
	}
}

void Pop()
{
	while (true)
	{
		int32 outValue;
		if (q.TryPop(outValue))
			continue;

		cout << outValue << endl;
	}
}

int main()
{
	thread t1(Push);
	thread t2(Pop);
	thread t3(Pop);

	t1.join();
	t2.join();
	t3.join();

}
