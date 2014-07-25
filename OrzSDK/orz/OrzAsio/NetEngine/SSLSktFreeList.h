#ifndef __Orz_OrzAsio_SSLSktFreeList_h_
#define __Orz_OrzAsio_SSLSktFreeList_h_
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
#include <orz/Toolkit_Plus/Toolkit/HashMap.hpp>
namespace Orz
{
class _OrzOrzAsioExport SSLSktFLDeleter
{
public:
	SSLSktFLDeleter();
	~SSLSktFLDeleter();

	void operator()(SSLSocket* _ptr);
};

typedef boost::shared_ptr<HashShrMap<ContextId, IFreeListShrPtr, ContextId> > HashMapShrPtr;
class _OrzOrzAsioExport SSLSktFreeList
{
	SSLSktFreeList();
	SSLSktFreeList(const SSLSktFreeList& _other);
	SSLSktFreeList& operator=(const SSLSktFreeList& _other);
public:
	~SSLSktFreeList();

public:
	static SSLSktFreeList& instance();
	
public:
	void				init			(uint32 _base);
	int32				add				(ContextId _cid, IFreeListShrPtr _freeList);
	IFreeListShrPtr		get				(ContextId _cid);
	HashMapShrPtr		getFls			();
	void				clear			();
	SSLSocket*			alloc			(ContextId _cid);
	void				free			(ContextId _cid, SSLSocket* _ptr);

private:
	HashMapShrPtr		m_fls;
};

class _OrzOrzAsioExport SSLSktAllocer
{
	SSLSktAllocer();
	SSLSktAllocer(const SSLSktAllocer& _other);
	SSLSktAllocer& operator=(const SSLSktAllocer& _other);

public:
	~SSLSktAllocer();

public:
	static SSLSktAllocer& instance();

public:
	SSLSocketShrPtr alloc(ContextId _cid);
};
}
#define sSSLSktFList Orz::SSLSktFreeList::instance()
#define allocSSLSkt(cid) Orz::SSLSktAllocer::instance().alloc((cid))
#endif
