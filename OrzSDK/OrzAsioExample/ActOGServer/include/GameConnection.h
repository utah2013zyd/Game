#ifndef __Orz_ActOGServer_GameConnection_h__
#define __Orz_ActOGServer_GameConnection_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
#include <orz/OrzAsio/NetEngine/IConnection.h>
#include <packet/packet_seer.h>
#include <common/Idr.h>
#include <game_server/game_sr_game_controller.h>
namespace Orz
{
class GameConnection
	: public IConnection
	, public boost::enable_shared_from_this<GameConnection>
	, private boost::noncopyable
{
public:
	GameConnection(ServiceManager* _svcMgr, uint32 _maxInc=1);
	virtual ~GameConnection();

public:
	//! 读取关于游戏服务器的一些配置参数
	int32					load_config				(const char* _conf);
	void					shutdown				();

	void					OnConnected				(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc);
	void					OnReceived				(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk);
	void					OnSended				(const SessionPxy& _snPxy, SessionDeputy& _snDy);
	void					OnDisconnected			(const SessionPxy& _snPxy, SessionDeputy& _snDy);

private:
	//! load map
	int32					load_map_config			(const char* _file);

	//! load pc
	int32					load_pc_config			(const char* _file);

	//! load monster
	int32					load_monster_config		(const char* _file);

private:
	ServiceManager*									m_svcMgr;
	//!
	packet_seer										m_packet_seer;

	//! config
	game_server_config								m_gsc;

	//!
	game_sr_game_controller							m_gs_game_ctrl;
	Idr												m_actIdr;

	ClockTime										m_dt;
};
}
#endif
