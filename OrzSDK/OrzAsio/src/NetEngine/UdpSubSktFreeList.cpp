#include <orz/OrzAsio/NetEngine/UdpSubSktFreeList.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/OrzAsio/NetEngine/UdpSubSocket.h>
#include <orz/OrzAsio/NetEngine/ISession.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
using namespace Orz;


UdpSubSktFLDeleter::UdpSubSktFLDeleter()
{
}

UdpSubSktFLDeleter::~UdpSubSktFLDeleter()
{
}

void UdpSubSktFLDeleter::operator()(UdpSubSocket* _ptr)
{
	sUdpSubSktFList.free(_ptr);
}


UdpSubSktFreeList::UdpSubSktFreeList()
{
}

UdpSubSktFreeList::~UdpSubSktFreeList()
{
}

UdpSubSktFreeList& UdpSubSktFreeList::instance()
{
	static UdpSubSktFreeList singleton;
	return singleton;
}

void UdpSubSktFreeList::set(IFreeListShrPtr _freeList)
{
	if (m_freeList)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "m_freeList", 0);
		return;
	}

	m_freeList = _freeList;
}

IFreeListShrPtr UdpSubSktFreeList::get()
{
	return m_freeList;
}

void UdpSubSktFreeList::clear()
{
	if (m_freeList)
		m_freeList->clear();
}

UdpSubSocket* UdpSubSktFreeList::alloc()
{
	if (!m_freeList)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_freeList", 0);
		return 0;
	}

	return (UdpSubSocket*)m_freeList->alloc();
}

void UdpSubSktFreeList::free(UdpSubSocket* _ptr)
{
	if (!m_freeList)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_freeList", 0);
		return;
	}

	m_freeList->free(_ptr);
}


UdpSubSktAllocer::UdpSubSktAllocer()
{
}

UdpSubSktAllocer::~UdpSubSktAllocer()
{
}

UdpSubSktAllocer& UdpSubSktAllocer::instance()
{
	static UdpSubSktAllocer singleton;
	return singleton;
}

UdpSubSocketShrPtr UdpSubSktAllocer::alloc()
{
	UdpSubSocketShrPtr skt(sUdpSubSktFList.alloc(), UdpSubSktFLDeleter());
	if (skt && !skt->hasInited())
		return UdpSubSocketShrPtr();
	else
		return skt;
}
