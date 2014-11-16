#pragma once

#include <Windows.h>

template <class T>
class ThreadDelegate : public ThreadEntryPoint
{
public:
	typedef void (T::*ThreadProc)();

	ThreadDelegate(T &obj, ThreadProc proc)
		: m_Object(obj), m_Proc(proc)
	{
	}

	void Run()
	{
		(m_Object.*m_Proc)();
	};

private:
	T &m_Object;
	ThreadProc m_Proc;
};

struct ThreadEntryPoint
{
	virtual void Run() = 0;
};


class Thread
{
public:

	Thread(ThreadEntryPoint *entry);
	~Thread();	

private:
	static unsigned int __stdcall EntryPoint(void* userdata);
	HANDLE m_ThreadHandle;
};