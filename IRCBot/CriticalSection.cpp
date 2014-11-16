#include "CriticalSection.h"

CriticalSection::CriticalSection()
{
	InitializeCriticalSection(&m_CriticalSection);
}

CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&m_CriticalSection);
}

void CriticalSection::Lock()
{
	EnterCriticalSection(&m_CriticalSection);
}

void CriticalSection::Unlock()
{
	LeaveCriticalSection(&m_CriticalSection);
}

ScopedCriticalSection::ScopedCriticalSection(CriticalSection &section)
	: m_CS(section)
{
	m_CS.Lock();
}

ScopedCriticalSection::~ScopedCriticalSection()
{
	m_CS.Unlock();
}