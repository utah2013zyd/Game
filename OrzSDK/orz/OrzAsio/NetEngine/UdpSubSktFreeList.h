#ifndef __Orz_OrzAsio_UdpSubSktFreeList_h_
#define __Orz_OrzAsio_UdpSubSktFreeList_h_
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
namespace Orz
{
class _OrzOrzAsioExport UdpSubSktFLDeleter
{
public:
	UdpSubSktFLDeleter();
	~UdpSubSktFLDeleter();

	void operator()(UdpSubSocket* _ptr);
};

class _OrzOrzAsioExport UdpSubSktFreeList
{
	UdpSubSktFreeList();
	UdpSubSktFreeList(const UdpSubSktFreeList& _other);
	UdpSubSktFreeList& operator=(const UdpSubSktFreeList& _other);
public:
	~UdpSubSktFreeList();

public:
	static UdpSubSktFreeList& instance();

public:
	void				set				(IFreeListShrPtr _freeList);
	IFreeListShrPtr		get				();
	void				clear			();
	UdpSubSocket*		alloc			();
	void				free			(UdpSubSocket* _ptr);

private:
	IFreeListShrPtr			m_freeList;
};

class _OrzOrzAsioExport UdpSubSktAllocer
{
	UdpSubSktAllocer();
	UdpSubSktAllocer(const UdpSubSktAllocer& _other);
	UdpSubSktAllocer& operator=(const UdpSubSktAllocer& _other);
public:
	~UdpSubSktAllocer();

public:
	static UdpSubSktAllocer& instance();

public:
	UdpSubSocketShrPtr alloc();
};
}
#define sUdpSubSktFList Orz::UdpSubSktFreeList::instance()
#define allocUdpSubSkt Orz::UdpSubSktAllocer::instance().alloc
#endif
