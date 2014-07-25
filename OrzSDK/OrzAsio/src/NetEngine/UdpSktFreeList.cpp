#include <orz/OrzAsio/NetEngine/UdpSktFreeList.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/OrzAsio/NetEngine/UdpSocket.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
using namespace Orz;


UdpSktFLDeleter::UdpSktFLDeleter()
{
}

UdpSktFLDeleter::~UdpSktFLDeleter()
{
}

void UdpSktFLDeleter::operator()(UdpSocket* _ptr)
{
	sUdpSktFList.free(_ptr);
}

UdpSktFreeList::UdpSktFreeList()
{
}

UdpSktFreeList::~UdpSktFreeList()
{
}

UdpSktFreeList& UdpSktFreeList::instance()
{
	static UdpSktFreeList singleton;
	return singleton;
}

void UdpSktFreeList::set(IFreeListShrPtr _freeList)
{
	if (m_freeList)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "m_freeList", 0);
		return;
	}

	m_freeList = _freeList;
}

IFreeListShrPtr UdpSktFreeList::get()
{
	return m_freeList;
}

void UdpSktFreeList::clear()
{
	if (m_freeList)
		m_freeList->clear();
}

UdpSocket* UdpSktFreeList::alloc()
{
	if (!m_freeList)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_freeList", 0);
		return 0;
	}

	return (UdpSocket*)m_freeList->alloc();
}

void UdpSktFreeList::free(UdpSocket* _ptr)
{
	if (!m_freeList)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_freeList", 0);
		return;
	}

	m_freeList->free(_ptr);
}


UdpSktAllocer::UdpSktAllocer()
{
}

UdpSktAllocer::~UdpSktAllocer()
{
}

UdpSktAllocer& UdpSktAllocer::instance()
{
	static UdpSktAllocer singleton;
	return singleton;
}

UdpSocketShrPtr UdpSktAllocer::alloc()
{
	UdpSocketShrPtr skt(sUdpSktFList.alloc(), UdpSktFLDeleter());
	if (skt && !skt->hasInited())
		return UdpSocketShrPtr();
	else
		return skt;
}
