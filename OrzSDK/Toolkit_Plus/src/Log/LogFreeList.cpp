#include <orz/Toolkit_Plus/Log/LogFreeList.h>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/Toolkit_Plus/Log/Log.hpp>
using namespace Orz;


LogDeleter::LogDeleter()
{
}

LogDeleter::~LogDeleter()
{
}

void LogDeleter::operator()(Log* _ptr)
{
	if (_ptr)
	{
		_ptr->clean();
	}
	sLogFL.free(_ptr);
}

LogFreeList::LogFreeList()
{
}

LogFreeList::~LogFreeList()
{
}

LogFreeList& LogFreeList::instance()
{
	static LogFreeList singleton;
	return singleton;
}

void LogFreeList::set(IFreeListShrPtr _freeList)
{
	if (m_freeList)
		return;

	m_freeList = _freeList;
}

Log* LogFreeList::alloc()
{
	if (!m_freeList)
		return 0;

	return (Log*)m_freeList->alloc();
}

LogShrPtr LogFreeList::select(OutLevel _ol, bool _usingDoubleBuff)
{
	LogShrPtr log(alloc(), LogDeleter());
	if (log)
	{
		log->initArg(_ol, _usingDoubleBuff);
	}
	return log;
}

void LogFreeList::free(Log* _ptr)
{
	if (!m_freeList)
		return;

	m_freeList->free(_ptr);
}

void LogFreeList::clear()
{
	m_freeList = IFreeListShrPtr();
}
