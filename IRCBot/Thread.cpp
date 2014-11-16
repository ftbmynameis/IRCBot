#include "Thread.h"

#include <process.h>
#include <memory>

Thread::Thread(ThreadEntryPoint *entry)
{
	m_ThreadHandle = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &Thread::EntryPoint, entry, 0, NULL));
}

Thread::~Thread()
{
	if(m_ThreadHandle)
		CloseHandle(m_ThreadHandle);
}

unsigned int __stdcall Thread::EntryPoint(void* userdata)
{
	ThreadEntryPoint* entry = reinterpret_cast<ThreadEntryPoint*>(userdata);
	entry->Run();
	return 0;
}