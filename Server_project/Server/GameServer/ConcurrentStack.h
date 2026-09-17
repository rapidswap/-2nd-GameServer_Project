#pragma once
#include <mutex>


template<typename T>
class LockStack
{
public:
	LockStack() {}

	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);

		if (_stack.empty())
			return false;


		// empty -> top -> pop
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] {return _stack.empty() == false;});
		value = std::move(_stack.top());
		
		_stack.pop();
	}
	
	//bool Empty()
	//{
	//	lock_guard<mutex> lock(_mutex);
	//	return _stack.empty();
	//}



private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _condVar;
};

template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data (value)
		{ }

		T data;
		Node* next;
	};

public:
	void Push(const T& value)
	{
		// #1. 새 노드를 만들기.
		// #2. 새 노드의 next = head
		// #3. head = 새 노드.

		Node* node = new Node(value);
		node->next = head;

		/*
		head를 첫 번째 값이랑 비교를 해주고, 
		같다면 두 번째값으로 변경하고 true 반환
		if (head == node->next)  
		{
			head = node;
			return true;
		}
		else
		{
			return false;
		}
		*/

		// atomic하게 해결.
		while (head.compare_exchange_weak(node->next, node) == false)
		{
			//node->next = head;

		}
		// 이 사이에 새치기 당하기.
		 
		//head = node;
	}

	bool TryPop(T& value)
	{
		// #1. 기존의 head 읽기.
		// #2. head의 next 읽기.
		// #3. head = head->next.
		// #4. data 추출 반환.
		// #5. 추출한 노드 삭제.

		Node* oldHead = head;

		/*
		if (head == oldHead)
		{
			head = oldHead->next;
			return true;
		}
		else
		{
			oldHead = head;
			return false;
		}
		*/
		while (oldHead && head.compare_exchange_weak(oldHead, oldHead->next)==false)
		{
			// oldHead = head;
		}
		if (oldHead == nullptr)
			return false;

		// Exception
		value = oldHead->data;
		
		// 잠시 삭제 보류
		//delete oldHead;
		return true;
	}

private:
	atomic<Node*> head;
};


