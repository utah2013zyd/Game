#ifndef __Orz_ToolkitPlus_QueryFreeList_h_
#define __Orz_ToolkitPlus_QueryFreeList_h_
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
namespace Orz
{
class _OrzToolkitPlusExport QueryDeleter
{
public:
	QueryDeleter();
	~QueryDeleter();

	void operator()(Query* _ptr);
};

class _OrzToolkitPlusExport QueryFreeList
{
	QueryFreeList();
	QueryFreeList(const QueryFreeList& _other);
	QueryFreeList& operator=(const QueryFreeList& _other);
public:
	~QueryFreeList();

public:
	static QueryFreeList& instance();

public:
	void				set				(IFreeListShrPtr _freeList);
	Query*				alloc			();
	QueryShrPtr			select			();
	void				free			(Query* _ptr);
	void				clear			();

private:
	IFreeListShrPtr			m_freeList;
};
}
#define sQueryFL Orz::QueryFreeList::instance()
#define allocQuery Orz::QueryFreeList::instance().select
#endif
