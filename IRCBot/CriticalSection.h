#pragma once

#include <Windows.h>

class CriticalSection
{
public:
	CriticalSection();
	~CriticalSection();

	void Lock();
	void Unlock();
private:
	CRITICAL_SECTION m_CriticalSection;
};

class ScopedCriticalSection
{
public:
	ScopedCriticalSection(CriticalSection &section);
	~ScopedCriticalSection();
private:
	CriticalSection &m_CS;
};