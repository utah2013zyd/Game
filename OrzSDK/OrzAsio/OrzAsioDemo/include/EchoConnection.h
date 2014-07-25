#ifndef __Orz_OrzAsioDemo_EchoConnection_h__
#define __Orz_OrzAsioDemo_EchoConnection_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <orz/OrzAsio/NetEngine/IConnection.h>
namespace Orz
{
class EchoConnection
	: public IConnection
	, public boost::enable_shared_from_this<EchoConnection>
	, private boost::noncopyable
{
public:
	EchoConnection(ServiceManager* _svcMgr, uint32 _maxInc=1);
	virtual ~EchoConnection();

public:
	void					OnConnected				(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc);
	void					OnReceived				(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk);
	void					OnSended				(const SessionPxy& _snPxy, SessionDeputy& _snDy);
	void					OnDisconnected			(const SessionPxy& _snPxy, SessionDeputy& _snDy);

private:
	ServiceManager*									m_svcMgr;

};
}
#endif
