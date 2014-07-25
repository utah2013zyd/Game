#ifndef GAME_SR_GAME_CONTROLLER_H_
#define GAME_SR_GAME_CONTROLLER_H_
#include <orz/Toolkit_Plus/Toolkit/ViolenceMap.hpp>
#include <common/Idr.h>
#include <game_server/game_account.h>
#include <game_server/demo_game.h>

namespace Orz
{

class game_sr_game_controller
{
public:
	game_sr_game_controller(ServiceManager* _svcMgr, gs_gc_config& _ggc)
		: m_svcMgr(_svcMgr)
		, m_ggc(_ggc)
		, m_gameIdr(20, 500)
		, m_games(3)
	{
	}
	~game_sr_game_controller()
	{
	}

public:
	//! 按照登录的顺序依次加入游戏
	ORZ_PLUS_INLINE game_ptr					join_game				(const account_ptr _account, const SessionPxy& _sn, game_type _gt)
	{
		//BoostLock lock(m_shr_mutex);
		//printf("game_sr_game_controller::join_game!");
		game_ptr g;
		if (!_account)
			return g;

		if (m_games.size() >= m_ggc.max_game_num ||
			m_games.size() >= m_gameIdr.getMax())
		{
			printf("[ERROR]Max games!\n");
			m_svcMgr->kick(_sn, true);
			return g;
		}
		
		bool isGet = false;
		for (uint32 i=0; i<m_games.arraySize(); ++i)
		{
			if (isGet) break;
			m_games.lockWrite(i);
			std::map<Number, game_ptr>& elem = m_games.map(i);
			for (std::map<Number, game_ptr>::iterator itr=elem.begin(); itr!=elem.end(); ++itr)
			{
				game_ptr g_ptr = itr->second;
				if (g_ptr->is_allow_join())
				{
					g = g_ptr;
					isGet = true;
					break;
				}
			}
			m_games.unlockWrite(i);
		}

		// 如果游戏满则新建立一个游戏
		if (!g || g->join(_account) != 0)
		{
			//if (!g)
			//	printf("game_sr_game_controller::join_game - !g!\n");
			
			game_id gid = m_gameIdr.generate();
			team_id tid = gid;

			game_ptr gn(new demo_game(m_svcMgr, gid, GT_DEMO1, _account, m_ggc.dgc));
			g = gn;
			m_games.add(gid, g);
		}
		if (g)
		{
			if (!g->is_gaming())
				g->start_game();
		}
		printf("game_sr_game_controller::join_game! game num: %d\n", m_games.size());

		return g;
	}


	ORZ_PLUS_INLINE void						shutdown				()
	{
		for (uint32 i=0; i<m_games.arraySize(); ++i)
		{
			m_games.lockWrite(i);
			std::map<Number, game_ptr>& elem = m_games.map(i);
			for (std::map<Number, game_ptr>::iterator itr=elem.begin(); itr!=elem.end(); ++itr)
			{
				game_ptr g_ptr = itr->second;
				if (g_ptr)
				{
					g_ptr->end_game();
					g_ptr->joinThread();
				}
			}
			m_games.unlockWrite(i);
		}
	}

	//! 玩家离开游戏
	ORZ_PLUS_INLINE int32						leave_game				(game_id _gid, const SessionPxy& _sn)
	{
		//BoostLock lock(m_shr_mutex);
		game_ptr g = m_games[_gid];
		if (!g)
			return 1;

		if (g->get_player_num() == 0)
		{
			g->end_game();
			m_games.erase(_gid);
			m_gameIdr.release(_gid);
		}

		printf("game_sr_game_controller::leave_game! game num: %d\n", m_games.size());

		return 0;
	}

private:
	ServiceManager*							m_svcMgr;
	gs_gc_config&							m_ggc;
	Idr										m_gameIdr;
	ViolenceShrMap<game_ptr>				m_games;
	mutable boost::shared_mutex				m_shr_mutex;
};

}

#endif
