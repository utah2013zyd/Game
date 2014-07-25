#include <orz/OrzAsio/NetEngine/SSLSktFreeList.h>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/OrzAsio/NetEngine/SSLSocket.h>
#include <orz/OrzAsio/NetEngine/ISession.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
using namespace Orz;


SSLSktFLDeleter::SSLSktFLDeleter()
{
}

SSLSktFLDeleter::~SSLSktFLDeleter()
{
}

void SSLSktFLDeleter::operator()(SSLSocket* _ptr)
{
	sSSLSktFList.free(_ptr->getContextId(), _ptr);
}


SSLSktFreeList& SSLSktFreeList::instance()
{
	static SSLSktFreeList singleton;
	return singleton;
}

void SSLSktFreeList::init(uint32 _base)
{
	if (m_fls)
	{
		return;
	}

	m_fls = HashMapShrPtr(new HashShrMap<ContextId, IFreeListShrPtr, ContextId>(_base));
	m_fls->setHash();
}

int32 SSLSktFreeList::add(ContextId _cid, IFreeListShrPtr _freeList)
{
	if (!m_fls)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_fls", 0);
		return 1;
	}

	return m_fls->add(_cid, _freeList, _cid);
}

IFreeListShrPtr SSLSktFreeList::get(ContextId _cid)
{
	if (!m_fls)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_fls", 0);
		return IFreeListShrPtr();
	}

	return m_fls->get(_cid, _cid);
}

HashMapShrPtr SSLSktFreeList::getFls()
{
	return m_fls;
}

void SSLSktFreeList::clear()
{
	if (m_fls)
		m_fls->clear();
}

SSLSocket* SSLSktFreeList::alloc(ContextId _cid)
{
	if (!m_fls)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_fls", 0);
		return 0;
	}

	DynShrLock lock(m_fls->mutex(_cid, _cid));
	IFreeListShrPtr fl = m_fls->get(_cid, _cid, false);
	if (!fl)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!fl", 0);
		return 0;
	}

	return (SSLSocket*)fl->alloc();
}

void SSLSktFreeList::free(ContextId _cid, SSLSocket* _ptr)
{
	if (!m_fls)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_fls", 0);
		return;
	}

	DynShrLock lock(m_fls->mutex(_cid, _cid));
	IFreeListShrPtr fl = m_fls->get(_cid, _cid, false);
	if (!fl)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!fl", 0);
		return;
	}

	fl->free(_ptr);
}


SSLSktAllocer::SSLSktAllocer()
{
}

SSLSktAllocer::~SSLSktAllocer()
{
}

SSLSktAllocer& SSLSktAllocer::instance()
{
	static SSLSktAllocer singleton;
	return singleton;
}

SSLSktFreeList::SSLSktFreeList()
{
}

SSLSktFreeList::~SSLSktFreeList()
{
}

SSLSocketShrPtr SSLSktAllocer::alloc(ContextId _cid)
{
	SSLSocketShrPtr skt(sSSLSktFList.alloc(_cid), SSLSktFLDeleter());
	if (skt && !skt->hasInited())
		return SSLSocketShrPtr();
	else
		return skt;
}
