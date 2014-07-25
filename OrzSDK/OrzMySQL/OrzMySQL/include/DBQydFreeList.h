#ifndef __Orz_OrzMySQL_DBQydFreeList_h_
#define __Orz_OrzMySQL_DBQydFreeList_h_
#include <OrzMySQLConfig.h>
namespace Orz
{
namespace Mysql
{
class _OrzMySQLPrivate DBQydDeleter
{
public:
	DBQydDeleter();
	~DBQydDeleter();

	void operator()(DBQueryData* _ptr);
};

//! 用户无需关心
class _OrzMySQLPrivate DBQydFreeList
{
	DBQydFreeList();
	DBQydFreeList(const DBQydFreeList& _other);
	DBQydFreeList& operator=(const DBQydFreeList& _other);
public:
	~DBQydFreeList();

public:
	static DBQydFreeList& instance();

public:
	void				set				(IFreeListShrPtr _freeList);
	DBQueryData*		alloc			();
	DBQueryDataShrPtr	select			();
	void				free			(DBQueryData* _ptr);
	void				clear			();

private:
	IFreeListShrPtr			m_freeList;
};
}
}
#define sDBQydFList Orz::Mysql::DBQydFreeList::instance()
#define allocDBQyd Orz::Mysql::DBQydFreeList::instance().select
#endif
