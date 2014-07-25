#ifndef __Orz_OrzAsio_UdpSktFreeList_h_
#define __Orz_OrzAsio_UdpSktFreeList_h_
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
namespace Orz
{
class _OrzOrzAsioExport UdpSktFLDeleter
{
public:
	UdpSktFLDeleter();
	~UdpSktFLDeleter();

	void operator()(UdpSocket* _ptr);
};

class _OrzOrzAsioExport UdpSktFreeList
{
	UdpSktFreeList();
	UdpSktFreeList(const UdpSktFreeList& _other);
	UdpSktFreeList& operator=(const UdpSktFreeList& _other);
public:
	~UdpSktFreeList();

public:
	static UdpSktFreeList& instance();
	
public:
	void				set				(IFreeListShrPtr _freeList);
	IFreeListShrPtr		get				();
	void				clear			();
	UdpSocket*			alloc			();
	void				free			(UdpSocket* _ptr);

private:
	IFreeListShrPtr			m_freeList;
};

class _OrzOrzAsioExport UdpSktAllocer
{
	UdpSktAllocer();
	UdpSktAllocer(const UdpSktAllocer& _other);
	UdpSktAllocer& operator=(const UdpSktAllocer& _other);
public:
	~UdpSktAllocer();

public:
	static UdpSktAllocer& instance();

public:
	UdpSocketShrPtr alloc();
};
}
#define sUdpSktFList Orz::UdpSktFreeList::instance()
#define allocUdpSkt Orz::UdpSktAllocer::instance().alloc
#endif
