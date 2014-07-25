
#ifndef GAMEPROXY_H_
#define GAMEPROXY_H_

#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <common/game/game.h>
#include <common/game/player.h>

namespace Orz
{

class player;
typedef boost::shared_ptr<player> player_ptr;

class game_proxy
{
public:
	game_proxy(ServiceManager* _svcMgr, game* _game)
		: m_svcMgr(_svcMgr)
		, m_game(_game)
	{
	}
	~game_proxy()
	{
	}

public:
	//!
	ORZ_PLUS_INLINE int32					broadcast				(IPacketShrPtr _pk, const SessionPxy* _eraser=0)
	{
		if (!m_game)
			return 1;

		return m_game->broadcast(_pk, _eraser);
	}

private:
	ServiceManager*				m_svcMgr;
	game*						m_game;
};

}

#endif
