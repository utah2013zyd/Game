#include <EchoConnection.h>
#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
using namespace Orz;

EchoConnection::EchoConnection(ServiceManager* _svcMgr, uint32 _maxInc)
	: IConnection(_maxInc)
	, m_svcMgr(_svcMgr)
{
}

EchoConnection::~EchoConnection()
{
}

void EchoConnection::OnConnected(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc)
{
	sLogger->out(OL_INFO, "Hello, world!", 0);
}

void EchoConnection::OnReceived(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk)
{
}

void EchoConnection::OnSended(const SessionPxy& _snPxy, SessionDeputy& _snDy)
{
}

void EchoConnection::OnDisconnected(const SessionPxy& _snPxy, SessionDeputy& _snDy)
{
	sLogger->out(OL_INFO, "Goodbye, world!", 0);
}
