#include <orz/OrzAsio/NetEngine/TcpSktFreeList.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/OrzAsio/NetEngine/TcpSocket.h>
#include <orz/OrzAsio/NetEngine/ISession.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
using namespace Orz;


TcpSktFLDeleter::TcpSktFLDeleter()
{
}

TcpSktFLDeleter::~TcpSktFLDeleter()
{
}

void TcpSktFLDeleter::operator()(TcpSocket* _ptr)
{
	sTcpSktFList.free(_ptr);
}


TcpSktFreeList::TcpSktFreeList()
{
}

TcpSktFreeList::~TcpSktFreeList()
{
}

TcpSktFreeList& TcpSktFreeList::instance()
{
	static TcpSktFreeList singleton;
	return singleton;
}

void TcpSktFreeList::set(IFreeListShrPtr _freeList)
{
	if (m_freeList)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "m_freeList", 0);
		return;
	}

	m_freeList = _freeList;
}

IFreeListShrPtr TcpSktFreeList::get()
{
	return m_freeList;
}

void TcpSktFreeList::clear()
{
	if (m_freeList)
		m_freeList->clear();
}

TcpSocket* TcpSktFreeList::alloc()
{
	if (!m_freeList)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_freeList", 0);
		return 0;
	}

	return (TcpSocket*)m_freeList->alloc();
}

void TcpSktFreeList::free(TcpSocket* _ptr)
{
	if (!m_freeList)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_freeList", 0);
		return;
	}

	m_freeList->free(_ptr);
}


TcpSktAllocer::TcpSktAllocer()
{
}

TcpSktAllocer::~TcpSktAllocer()
{
}

TcpSktAllocer& TcpSktAllocer::instance()
{
	static TcpSktAllocer singleton;
	return singleton;
}

TcpSocketShrPtr TcpSktAllocer::alloc()
{
	TcpSocketShrPtr skt(sTcpSktFList.alloc(), TcpSktFLDeleter());
	if (skt && !skt->hasInited())
		return TcpSocketShrPtr();
	else
		return skt;
}
