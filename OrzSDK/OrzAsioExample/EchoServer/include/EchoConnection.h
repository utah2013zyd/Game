#ifndef __Orz_EchoServer_EchoConnection_h__
#define __Orz_EchoServer_EchoConnection_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
#include <orz/Toolkit_Plus/Toolkit/Counter.hpp>
#include <EchoServerPreDeclare.h>
#include <orz/OrzAsio/NetEngine/IConnection.h>
namespace Orz
{
class EchoConnection
	: public IConnection
	, public boost::enable_shared_from_this<EchoConnection>
	, private boost::noncopyable
{
public:
	EchoConnection(uint32 _maxInc=1);
	virtual ~EchoConnection();

public:
	void					OnConnected				(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc);
	void					OnReceived				(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk);
	void					OnSended				(const SessionPxy& _snPxy, SessionDeputy& _snDy);
	void					OnDisconnected			(const SessionPxy& _snPxy, SessionDeputy& _snDy);

private:
	Counter											m_connectionNum;
};
}
#endif
