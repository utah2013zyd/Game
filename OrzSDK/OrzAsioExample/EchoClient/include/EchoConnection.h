#ifndef __Orz_EchoServer_EchoConnection_h__
#define __Orz_EchoServer_EchoConnection_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
#include <orz/Toolkit_Plus/Agent/Agent.h>
#include <orz/OrzAsio/NetEngine/IConnection.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
namespace Orz
{
class EchoConnection
	: public IConnection
	, public boost::enable_shared_from_this<EchoConnection>
	, private boost::noncopyable
{
public:
	EchoConnection(std::string& _outIp, uint16 _port, uint32 _maxInc=1);
	virtual ~EchoConnection();

public:
	void					OnConnected				(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc);
	void					OnReceived				(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk);
	void					OnSended				(const SessionPxy& _snPxy, SessionDeputy& _snDy);
	void					OnDisconnected			(const SessionPxy& _snPxy, SessionDeputy& _snDy);
	void					shutdown				();

	void					write					(IPacketShrPtr _pk);
	void					writeto					(IPacketShrPtr _pk);
	void					inputEchoStr			();
	void					printPtr				(const char* _title);

private:
	std::string										m_outIp;
	uint16											m_port;
	SessionPxy										m_snPxy;
	SessionPxy										m_udpSnPxy;
	Agent											m_agent;
	bool											m_usingUdp;
	bool											m_udpConnection;
	ClockTime										m_old;
};
}
#endif
