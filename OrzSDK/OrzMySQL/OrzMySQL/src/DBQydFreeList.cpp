#include <DBQydFreeList.h>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <DBQueryData.hpp>
using namespace Orz;
using namespace Orz::Mysql;


DBQydDeleter::DBQydDeleter()
{
}

DBQydDeleter::~DBQydDeleter()
{
}

void DBQydDeleter::operator()(DBQueryData* _ptr)
{
	if (_ptr)
	{
		_ptr->clear();
	}
	sDBQydFList.free(_ptr);
}

DBQydFreeList::DBQydFreeList()
{
}

DBQydFreeList::~DBQydFreeList()
{
}

DBQydFreeList& DBQydFreeList::instance()
{
	static DBQydFreeList singleton;
	return singleton;
}

void DBQydFreeList::set(IFreeListShrPtr _freeList)
{
	if (m_freeList)
		return;

	m_freeList = _freeList;
}

DBQueryData* DBQydFreeList::alloc()
{
	if (!m_freeList)
		return 0;

	return (DBQueryData*)m_freeList->alloc();
}

DBQueryDataShrPtr DBQydFreeList::select()
{
	return DBQueryDataShrPtr(sDBQydFList.alloc(), DBQydDeleter());
}

void DBQydFreeList::free(DBQueryData* _ptr)
{
	if (!m_freeList)
		return;

	m_freeList->free(_ptr);
}

void DBQydFreeList::clear()
{
	m_freeList = IFreeListShrPtr();
}
