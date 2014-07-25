#ifndef __Orz_ActOGClient_GameConnection_h__
#define __Orz_ActOGClient_GameConnection_h__
#include <orz/View_asio/ViewAsioConfig.h>
#include <orz/View_asio/NetEngine/Socket/SocketPreDeclare.h>
#include <orz/Tookit_Plus/Tookit/Counter.hpp>
#include <orz/Tookit_Plus/Agent/Agent.h>
#include <orz/View_asio/NetEngine/IConnection.h>
#include <orz/View_asio/NetEngine/SessionPxy.h>
#include <packet/packet_seer.h>
namespace Orz
{
enum { TEST_NUM = 20 };
enum { THREAD_NUM = 10 };
class GameConnection
	: public IConnection
	, public boost::enable_shared_from_this<GameConnection>
	, private boost::noncopyable
{
public:
	GameConnection(ServiceManager* _svcMgr, uint32 _maxInc=1);
	virtual ~GameConnection();

public:
	void					OnConnected				(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc);
	void					OnReceived				(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk);
	void					OnSended				(const SessionPxy& _snPxy, SessionDeputy& _snDy);
	void					OnDisconnected			(const SessionPxy& _snPxy, SessionDeputy& _snDy);
	void					shutdown				();

	void					write					(IPacketShrPtr _pk);
	void					inputEchoStr			();

private:
	void					timerHandler			(DlTimerShrPtr _tmr, const SessionPxy& _snPxy);

private:
	ServiceManager*									m_svcMgr;
	boost::asio::io_service							m_iosvc;
	boost::asio::io_service::work					m_work;
	boost::thread									m_t;
	SessionPxy										m_snPxy;
	Agent											m_agent;
	Counter											m_connectionNum;

};
}
#endif
