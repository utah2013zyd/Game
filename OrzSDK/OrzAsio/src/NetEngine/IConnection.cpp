#include <orz/OrzAsio/NetEngine/IConnection.h>
#include <NetEngine/IConnectionImpl.h>
using namespace Orz;

IConnection::IConnection()
	: m_impl(new IConnectionImpl)
{
}

IConnection::IConnection(uint32 _maxInc)
	: m_impl(new IConnectionImpl)
{
	m_impl->setMaxInc(_maxInc);
}

IConnection::IConnection(const IConnection& _other)
	: m_impl(new IConnectionImpl(*_other.m_impl))
{
	m_impl->setMaxInc(_other.m_impl->getMaxInc());
}

IConnection::~IConnection()
{
	if (m_impl)
	{
		delete m_impl;
	}
}

IConnection& IConnection::operator=(const IConnection& _other)
{
	if (this == &_other)
	{
		return *this;
	}

	IConnectionImpl& cnimpl = *m_impl;
	cnimpl = *_other.m_impl;
	return *this;
}

IConnectionImpl* IConnection::impl()
{
	return m_impl;
}

void IConnection::OnConnected(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc)
{
}

void IConnection::OnReceived(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk)
{
}

void IConnection::OnSended(const SessionPxy& _snPxy, SessionDeputy& _snDy)
{
}

void IConnection::OnDisconnected(const SessionPxy& _snPxy, SessionDeputy& _snDy)
{
}
