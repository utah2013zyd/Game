#include <orz/Toolkit_Plus/Database/QueryFreeList.h>
#include <orz/Toolkit_Plus/Database/Query.hpp>
using namespace Orz;


QueryDeleter::QueryDeleter()
{
}

QueryDeleter::~QueryDeleter()
{
}

void QueryDeleter::operator()(Query* _ptr)
{
	if (_ptr)
	{
		_ptr->clean();
	}
	sQueryFL.free(_ptr);
}

QueryFreeList::QueryFreeList()
{
}

QueryFreeList::~QueryFreeList()
{
}

QueryFreeList& QueryFreeList::instance()
{
	static QueryFreeList singleton;
	return singleton;
}

void QueryFreeList::set(IFreeListShrPtr _freeList)
{
	if (m_freeList)
		return;

	m_freeList = _freeList;
}

Query* QueryFreeList::alloc()
{
	if (!m_freeList)
		return 0;

	return (Query*)m_freeList->alloc();
}

QueryShrPtr QueryFreeList::select()
{
	return QueryShrPtr(alloc(), QueryDeleter());
}

void QueryFreeList::free(Query* _ptr)
{
	if (!m_freeList)
		return;

	m_freeList->free(_ptr);
}

void QueryFreeList::clear()
{
	m_freeList = IFreeListShrPtr();
}
