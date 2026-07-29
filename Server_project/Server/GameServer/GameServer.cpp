#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include<Windows.h>

atomic<bool> ready;
int32 value;

void Producer()
{
	value = 10;
	ready.store(true, memory_order_seq_cst);
}

void Consumer()
{
	while (ready.load(memory_order_seq_cst) == false);

	cout << value << endl;
}

int main()
{
	// 1. seq_cst (가장 엄경 = 컴파일러 최적화  여지 적음 = 직관적임)
	// 가시성 문제 바로 해결. 코드 재배치 해결
	// 
	// 
	// 2. acquire-release
	// release 명령 이전의 메모리 명령들이 해당 명령 이후로 재배치 되는 것을 금지.
	// acquire로 같은 변수를 읽는 쓰레드가 있다면
	// release 이전의 명령들이 acquire 하는 순간 관찰 가능 (가시성 보장)
	// 
	// 
	// 3. relaxed (자유롭다 = 컴파일러 최적화 여지 많음 = 직관적이지 않음)
	// 코드 재배치도 멋대로 가능 가시성 해결 불가능.
	// 가장 기본 조건 (동일 객체에 대한 동일 관전 순서만 보장)
	
	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);
	t1.join();
	t2.join();

}
