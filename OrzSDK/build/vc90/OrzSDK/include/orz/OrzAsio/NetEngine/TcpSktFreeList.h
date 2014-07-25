#ifndef __Orz_OrzAsio_TcpSktFreeList_h_
#define __Orz_OrzAsio_TcpSktFreeList_h_
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
namespace Orz
{
class _OrzOrzAsioExport TcpSktFLDeleter
{
public:
	TcpSktFLDeleter();
	~TcpSktFLDeleter();

	void operator()(TcpSocket* _ptr);
};

class _OrzOrzAsioExport TcpSktFreeList
{
	TcpSktFreeList();
	TcpSktFreeList(const TcpSktFreeList& _other);
	TcpSktFreeList& operator=(const TcpSktFreeList& _other);
public:
	~TcpSktFreeList();

public:
	static TcpSktFreeList& instance();

public:
	void				set				(IFreeListShrPtr _freeList);
	IFreeListShrPtr		get				();
	void				clear			();
	TcpSocket*			alloc			();
	void				free			(TcpSocket* _ptr);

private:
	IFreeListShrPtr			m_freeList;
};

class _OrzOrzAsioExport TcpSktAllocer
{
	TcpSktAllocer();
	TcpSktAllocer(const TcpSktAllocer& _other);
	TcpSktAllocer& operator=(const TcpSktAllocer& _other);

public:
	~TcpSktAllocer();

public:
	static TcpSktAllocer& instance();

public:
	TcpSocketShrPtr alloc();
};
}
#define sTcpSktFList Orz::TcpSktFreeList::instance()
#define allocTcpSkt Orz::TcpSktAllocer::instance().alloc
#endif
