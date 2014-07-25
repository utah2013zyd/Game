#ifndef DEMOGAME_H_
#define DEMOGAME_H_

#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <orz/OrzAsio/NetEngine/PacketFreeList.h>
#include <orz/Toolkit_Plus/Toolkit/Framer.hpp>
#include <orz/Toolkit_Plus/Agent/Agent.h>
#include <orz/Toolkit_Plus/Toolkit/FastIdr.hpp>
#include <common/game/game.h>
#include <common/game/game_common.h>
#include <orz/Toolkit_Plus/Toolkit/MultiMap.hpp>
#include <game_server/demo_player.h>
#include <game_server/demo_monster.h>
#include <game_server/monster_mind.h>
#include <game_server/demo_map.h>
#include <game_server/object_generator_manager.h>


namespace Orz
{

class demo_game : public game/*, public boost::enable_shared_from_this<demo_game>*/
{
public:
	demo_game(ServiceManager* _svcMgr, game_id _gid, game_type _gt, const account_ptr _master, demo_game_config& _dgc)
		: m_svcMgr(_svcMgr)
		, m_obj_genr_mgr(boost::bind(&demo_game::object_genr_assigner, this, _1, _2))
		, m_game_proxy(game_proxy(_svcMgr, this))
		, m_event_mgr(boost::bind(&demo_game::master_recv_handler, this, _1, _2, _3, _4))
		, m_id(_gid)
		, m_gt(_gt)
		//, m_plat(Plat(_dgc.pias))
		, m_max_player(_dgc.max_player)
		, m_pc_id_genr(10000)
		, m_msr_id_genr(10000)
		, m_dgc(&_dgc)
		, m_demo_map(_dgc.dmapc)
		, m_current_time(now())
		, m_running(false)
		, m_isEnd(false)
	{
		// generator
		boost::function<int (object_type, object_generator_manager&, object_common_data&, data&)> obj_create_hdr = boost::bind(&demo_game::object_create_handler, this, _1, _2, _3, _4);
		std::vector<object_generator> obj_genrs;
		
		obj_genrs.push_back(object_generator(OT_PC, object_common_data(_dgc.bornPoint1), obj_create_hdr));
		obj_genrs.push_back(object_generator(OT_PC, object_common_data(_dgc.bornPoint2), obj_create_hdr));
		//obj_genrs.push_back(object_generator(OT_PC, object_common_data(pos3d(200.0f, 550.0f, 0.0f)), obj_create_hdr));
		//obj_genrs.push_back(object_generator(OT_MSR, object_common_data(pos3d(738.0f, 500.0f, 0.0f)), obj_create_hdr));

		m_obj_genr_mgr.init(obj_genrs);

		join(_master);

		if (m_players.size() == 1)
			m_master = m_players.begin()->second.plr;
	}
	virtual ~demo_game()
	{
		end_game();
		//m_agent.join();
	}

private:
	//!
	ORZ_PLUS_INLINE void							master_recv_handler			(const event_obj_id& _sender, event_manager& _event_mgr, event_id _eid, std::vector<static_any<EMAS> >& _datas)
	{
		switch ((event_type)_eid)
		{
		case et_pc_attack:				process_pc_attack(_sender, _event_mgr, _datas);			break;
		case et_pc_new_attack:			process_pc_new_attack(_sender, _event_mgr, _datas);		break;
		}
	}

	//!
	ORZ_PLUS_INLINE void							process_pc_attack			(const event_obj_id& _sender, event_manager& _event_mgr, std::vector<static_any<EMAS> >& _datas)
	{
		if (_datas.size() < 5)
			return;

		// get the datas
		zone* zn = _datas[0].get_data<zone>();
		bool* dir = _datas[1].get_data<bool>();
		pos3d* pos = _datas[2].get_data<pos3d>();
		unsigned int* atk_y = _datas[3].get_data<unsigned int>();
		attack* atk = _datas[4].get_data<attack>();
		if (!zn || !dir || !pos || !atk_y || !atk)
		{
			printf("process_pc_attack - !zn || !dir || !pos !\n");
			return;
		}

		//printf("\n demo_game::process_pc_attack - *atk_y: " << *atk_y << std::endl;

		// get obj
		player_imp* plr_imp = m_players.getById3((player_id)_sender.get_obj_id());
		if (!plr_imp)
		{
			printf("process_pc_attack - !plr_imp !\n");
			return;
		}

		// convert attack zone to currect dir
		zone az = zn->convert_by_dir(*dir, (int)pos->x, (int)pos->y);
		//printf("	demo_game::process_pc_attack - zn->x: " << zn->x << ", zn->y: " << zn->y << ", zn->w: " << zn->w << ", zn->h: " << zn->h << std::endl;
		//printf("	demo_game::process_pc_attack - az.x: " << az.x << ", az.y: " << az.y << ", az.w: " << az.w << ", az.h: " << az.h << std::endl;

		std::vector<player_imp*> targets;
		// find the right targets (player)
		std::map<SessionPxy, player_imp, SessionPxyLess>::iterator itr(m_players.begin());
		for (; itr!=m_players.end(); ++itr)
		{
			player_imp* plr_i = &itr->second;
			if (plr_imp->plr->get_id() == plr_i->plr->get_id())	// 自我攻击去除
				continue;

			// 从列表中寻找是否已经被此次攻击攻击过了
			if (plr_imp->get_act_if().is_attacked(plr_i->plr->get_event_obj_id()))
				continue;

			// #1 check y distance
			const pos3d& plr_pos = plr_i->plr->get_position();
			float64 dis = fabs(plr_pos.y - pos->y);
			//printf("	demo_game::process_pc_attack - dis: " << dis << std::endl;
			if (dis > (float64)(*atk_y))
				continue;

			// #2 check attack zone and target's injure zone
			const zone* plr_iz = plr_i->plr->get_current_iz();
			if (!plr_iz)
				continue;
			zone iz = plr_iz->convert_by_dir(plr_i->plr->get_direction(), (int)plr_pos.x, (int)plr_pos.y);

			//printf("	demo_game::process_pc_attack - plr_iz->x: " << plr_iz->x << ", plr_iz->y: " << plr_iz->y << ", plr_iz->w: " << plr_iz->w << ", plr_iz->h: " << plr_iz->h << std::endl;
			//printf("	demo_game::process_pc_attack - iz.x: " << iz.x << ", iz.y: " << iz.y << ", iz.w: " << iz.w << ", iz.h: " << iz.h << std::endl;

			// 将被攻击者的伤害区域移动到和攻击者的攻击区域的y坐标上
			if (plr_pos.y - pos->y <= 0)	// 被攻击的目标在攻击者上方
				iz.y += (int)dis;
			else
				iz.y -= (int)dis;

			// 判断攻击区域和伤害区域是否有重叠部分
			if (az.is_wrap(iz))
				targets.push_back(plr_i);
		}

		// find the right targets (monster)	..need impl


		// 保存这个攻击者所攻击到的目标 并发送所有被攻击到的目标对应的消息
		std::vector<player_imp*>::iterator titr(targets.begin());
		for (; titr!=targets.end(); ++titr)
		{
			if (!(*titr))
				continue;

			// set attacked
			plr_imp->get_act_if().add_attacked_objs((*titr)->plr->get_event_obj_id());

			//printf("	demo_game::process_pc_attack - (*itr)->plr->get_id(): " << (*itr)->plr->get_id() << std::endl << std::endl;
			std::vector<static_any<64> > sas;
			sas.push_back(_datas[4]);
			_event_mgr.send(_sender, (*titr)->plr->get_event_obj_id(), (event_id)et_pc_attack, sas);
		}
	}

	//!
	ORZ_PLUS_INLINE void							process_pc_new_attack		(const event_obj_id& _sender, event_manager& _event_mgr, std::vector<static_any<EMAS> >& _datas)
	{
		if (_datas.size() < 1)
			return;

		// get obj
		player_imp* plr_imp = m_players.getById3((player_id)_sender.get_obj_id());
		if (!plr_imp)
		{
			printf("process_pc_new_attack - !plr_imp !\n");
			return;
		}

		// reset obj's attacked list
		plr_imp->get_act_if().clear_attacked_objs();
	}

//------------------------------------------------------------------------------------------------------------
private:	// 对象产生器 handlers
	//! game's object generator assigner
	ORZ_PLUS_INLINE object_generator*			object_genr_assigner		(object_type _ot, std::vector<object_generator>& _obj_genrs)
	{
		switch (_ot)
		{
		case OT_PC:
			{
				std::sort(_obj_genrs.begin(), _obj_genrs.end(), object_generator_less);
				return &(*_obj_genrs.begin());
			}break;
		case OT_MSR:
			{
				std::sort(_obj_genrs.begin(), _obj_genrs.end(), object_generator_less);
				return &(*_obj_genrs.begin());
			}break;
		}

		return 0;
	}

	//!
	ORZ_PLUS_INLINE int32							object_create_handler		(object_type _ot, object_generator_manager& _obj_genr_mgr, object_common_data& _obj_cm_d, data& _data)
	{
		switch (_ot)
		{
		case OT_PC:
			{
				pc_create_data* pcd = _data.get_data<pc_create_data>();
				if (!pcd)
				{
					printf("demo_game::object_create_handler - pcd == 0\n");
					return 1;
				}

				const account_ptr _pAct = pcd->act;
				player_id pid = m_pc_id_genr.generate();

				pos2d pos(_obj_cm_d.pos.x, _obj_cm_d.pos.y);

				//pos2d gridxy = m_plat.GetOnMapContainerPoint((unsigned int)pos.x, (unsigned int)pos.y);
				//player_ptr plr(new demo_player(m_svcMgr, m_game_proxy, pid, _pAct->get_id(), _pAct->get_session_proxy(), _pAct->get_name().c_str(), 
				//							 _pAct->get_nick_name().c_str(), m_plat, gridxy, m_dgc->dpc, (user_type)_pAct->get_user_type()));

				player_ptr plr(new demo_player(m_svcMgr, m_event_mgr, m_game_proxy, pid, _pAct->get_id(), _pAct->get_session_proxy(), _pAct->get_name().c_str(), 
											 _pAct->get_nick_name().c_str(), /*m_plat, */m_demo_map, pos, m_dgc->dpc));

				player_imp pi(plr);
				pi.m_posx = (int32)_obj_cm_d.pos.x;
				pi.m_posy = (int32)_obj_cm_d.pos.y;

				if (m_players.add(plr->get_session_proxy(), plr->get_account_id(), plr->get_id(), pi) != 0)
				{
					printf("demo_game::object_create_handler - m_players.add failed!\n");
					return 1;
				}

				//add2map(m_broadcast_list, pid, plr.get());

				// send LG
				{
					IPacketShrPtr pk = allocPacket();
					pk->setMsg(M_LG);
					//pk->put("playerid=", 9);
					NumStr plrid(d2s(_pAct->get_id()));
					pk->put(plrid.c_str(), plrid.size());
					pk->put("#", 1);
					//pk->put("posx=", 5);
					NumStr posx(d2s((int)pos.x));
					pk->put(posx.c_str(), posx.size());
					pk->put("|", 1);
					//pk->put("posy=", 5);
					NumStr posy(d2s((int)pos.y));
					pk->put(posy.c_str(), posy.size());

					m_svcMgr->send(_pAct->get_session_proxy(), pk);
				}

				// send JG
				if (get_player_num_st() > 1)
				{
					IPacketShrPtr jpk = allocPacket();
					jpk->setMsg(M_JG);
					//jpk->put("playerNum=", 10);
					NumStr pnum(d2s((unsigned int)(get_player_num_st() - 1)));
					jpk->put(pnum.c_str(), pnum.size());
					unsigned int anum = 0;
					std::map<SessionPxy, player_imp, SessionPxyLess>::iterator itr(m_players.begin());
					for (; itr!=m_players.end(); ++itr)
					{
						player_ptr p = itr->second.plr;
						if (p->get_account_id() != _pAct->get_id())
						{
							if (anum == 0)
								jpk->put("#", 1);
							else
								jpk->put("|", 1);
							//const pos3d& p_pos = p->get_position();
							pos3d p_pos((float64)itr->second.m_posx, (float64)itr->second.m_posy, 0.0f);
							//jpk->put("pid=", 4);
							NumStr pcid(d2s(p->get_account_id()));
							jpk->put(pcid.c_str(), pcid.size());
							jpk->put("&", 1);
							//jpk->put("posx=", 5);
							NumStr posx(d2s((int)p_pos.x));
							jpk->put(posx.c_str(), posx.size());
							jpk->put(",", 1);
							//jpk->put("posy=", 5);
							//NumStr posy(d2s((int)p_pos.z));
							NumStr posy(d2s((int)p_pos.y));
							jpk->put(posy.c_str(), posy.size());
							++anum;
						}
					}
					m_svcMgr->send(_pAct->get_session_proxy(), jpk);
				}

				// 广播其他游戏内的玩家，这个新加入的id
				if (get_player_num_st() > 1)
				{
					IPacketShrPtr pk = allocPacket();
					pk->setMsg(M_PCJ);
					//pk->put("playerid=", 9);
					NumStr plrid(d2s(_pAct->get_id()));
					pk->put(plrid.c_str(), plrid.size());
					pk->put("#", 1);
					//pk->put("posx=", 5);
					NumStr posx(d2s((int)pos.x));
					pk->put(posx.c_str(), posx.size());
					pk->put("|", 1);
					//pk->put("posy=", 5);
					NumStr posy(d2s((int)pos.y));
					pk->put(posy.c_str(), posy.size());
					std::map<SessionPxy, player_imp, SessionPxyLess>::iterator itr(m_players.begin());
					for (; itr!=m_players.end(); ++itr)
					{
						player_ptr p = itr->second.plr;
						if (p->get_account_id() != _pAct->get_id())
							m_svcMgr->send(p->get_session_proxy(), pk);
					}
				}

				// 发送这个游戏内当前的所有怪物的信息给这个玩家
				std::map<monster_id, monster_imp>::iterator itr(m_monsters.begin());
				for (; itr!=m_monsters.end(); ++itr)
				{
					monster_id msrid = itr->second.msr->get_id();
					const pos3d& pos3d = itr->second.msr->get_position();
					IPacketShrPtr pk = allocPacket();
					create_new_msr_packet(pk, msrid, (int)pos3d.x, (int)pos3d.y, itr->second.msr->get_direction() ? DIRT_RIGHT : DIRT_LEFT);
					m_svcMgr->send(plr->get_session_proxy(), pk);
				}

			}break;
		case OT_MSR:
			{
				msr_create_data* mcd = _data.get_data<msr_create_data>();
				if (!mcd)
				{
					printf("demo_game::object_create_handler - mcd == 0\n");
					return 1;
				}

				//monster_id msrid = m_msr_id_genr.generate();
				monster_id msrid = (monster_id)m_pc_id_genr.generate();
				pos2d pos(_obj_cm_d.pos.x, _obj_cm_d.pos.y);

				monster_ptr msr(new demo_monster(m_svcMgr, m_event_mgr, m_game_proxy, msrid, mcd->pName, false, /*m_plat, */m_demo_map, pos, m_dgc->dmc));
				monster_imp msri(m_svcMgr, m_event_mgr, msr, m_game_proxy, m_dgc->mmc, m_demo_map);

				if (add2map(m_monsters, msrid, msri) == 0)
				{
					printf("demo_game::object_create_handler - m_monsters add failed!\n");
					return 1;
				}

				// broadcast create monster msg
				const pos3d& pos3d = msr->get_position();
				broadcast_create_msr_msg(msrid, (int)pos3d.x, (int)pos3d.y, msr->get_direction() ? DIRT_LEFT : DIRT_RIGHT);
			}break;
		}
		return 0;
	}

public:
	//!
	ORZ_PLUS_INLINE game_id						get_id						()
	{
		BoostShrLock lock(m_mutex);
		return m_id;
	}

	//!
	ORZ_PLUS_INLINE const player_ptr			get_master					()
	{
		BoostShrLock lock(m_mutex);
		return m_master;
	}

	//!
	ORZ_PLUS_INLINE bool						is_allow_join				()
	{
		BoostShrLock lock(m_mutex);
		if (m_players.size() >= m_max_player || m_isEnd)
			return false;

		return true;
	}

private:
	ORZ_PLUS_INLINE bool						is_allow_join_st			()
	{
		if (m_players.size() >= m_max_player || m_isEnd)
			return false;

		return true;
	}
public:

	//! 0 成功，1 失败
	ORZ_PLUS_INLINE int32						join						(const account_ptr _pAct)
	{
		BoostLock lock(m_mutex);
		if (!is_allow_join_st())
		{
			printf("can't join!\n");
			return 1;
		}

		pc_create_data pcd(_pAct);
		data d(pcd);
		return m_obj_genr_mgr.generate_object(OT_PC, d);
	}

	//! 0 成功，1 失败
	ORZ_PLUS_INLINE int32						leave						(const SessionPxy& _sn)
	{
		BoostLock lock(m_mutex);
		player_id pid = get_player_id_by_sn_st(_sn);
		player_ptr plr = get_player_st(pid);
		if (!plr)
		{
			printf("demo_game::leave - plr.get() == 0\n");
			return 1;
		}

		// check if is master leave
		bool master_leave = false;
		if (m_master && m_master->get_id() == pid)
			master_leave = true;

		//else
		{
			account_id aid = plr->get_account_id();
			remove_player(plr->get_id());
			
			//IPacketShrPtr pk = allocPacket();
			IPacketShrPtr pk = allocPacket();
			pk->setMsg(get_msg("LO"));
			//pk->put("playerid=", 9);
			NumStr plrid(d2s(aid));
			pk->put(plrid.c_str(), plrid.size());
			
			std::map<SessionPxy, player_imp, SessionPxyLess>::iterator itr(m_players.begin());
			for (; itr!=m_players.end(); ++itr)
			{
				player_ptr p = itr->second.plr;
				if (master_leave)
				{
					master_leave = false;
					m_master = p;
				}
				m_svcMgr->send(p->get_session_proxy(), pk);
			}
		}
		
		return 0;
	}

	//!
	ORZ_PLUS_INLINE int32						set_ready					(player_id _pid, bool _r)
	{
		return 0;
	}

	//! 开始游戏 0 成功 1 失败
	ORZ_PLUS_INLINE int32						start_game					()
	{
		BoostLock lock(m_mutex);
		//if (m_agent.start() != 0)
		//	return 1;
		//m_framer.start((ClockTime)1000000/m_dgc->run_fps);
		m_running = true;
		//m_agent.post(boost::bind(&demo_game::run, this));
		return 0;
	}

	ORZ_PLUS_INLINE void						updatePosition				(const SessionPxy& _sid, int32 _x, int32 _y)
	{
		BoostLock lock(m_mutex);
		player_imp* pi = m_players.getById1(_sid);
		if (!pi)
			return;

		pi->m_posx = _x;
		pi->m_posy = _y;
		
	}

	//!
	ORZ_PLUS_INLINE bool						is_gaming					()
	{
		BoostShrLock lock(m_mutex);
		return m_running;
	}

	//!
	ORZ_PLUS_INLINE bool						is_end						()
	{
		BoostShrLock lock(m_mutex);
		return m_isEnd;
	}

	//!
	ORZ_PLUS_INLINE int32						end_game					()
	{
		BoostLock lock(m_mutex);
		m_running = false;
		m_isEnd = true;
		//m_agent.stop();
		return 0;
	}

	//!
	ORZ_PLUS_INLINE const player_ptr			get_player					(player_id _pid)
	{
		BoostLock lock(m_mutex);
		player_ptr null;
		player_imp* p = m_players.getById3(_pid);
		if (!p)
			return null;

		return p->plr;
	}
private:
	ORZ_PLUS_INLINE const player_ptr			get_player_st				(player_id _pid)
	{
		player_ptr null;
		player_imp* p = m_players.getById3(_pid);
		if (!p)
			return null;

		return p->plr;
	}
public:

	//!
	ORZ_PLUS_INLINE const player_id				get_player_id_by_aid		(account_id _aid)
	{
		BoostLock lock(m_mutex);
		player_id* pid = m_players.getId3ById2(_aid);
		if (!pid)
			return 0;

		return *pid;
	}

	//!
	ORZ_PLUS_INLINE const player_id				get_player_id_by_sn			(const SessionPxy& _sid)
	{
		BoostLock lock(m_mutex);
		player_id* pid = m_players.getId3ById1(_sid);
		if (!pid)
			return 0;

		return *pid;
	}
private:
	ORZ_PLUS_INLINE const player_id				get_player_id_by_sn_st		(const SessionPxy& _sid)
	{
		player_id* pid = m_players.getId3ById1(_sid);
		if (!pid)
			return 0;

		return *pid;
	}
public:

	//!
	ORZ_PLUS_INLINE int32							get_grid_xy					(player_id _pid, pos2d& _pos)
	{
		//player_ptr p = get_player(_pid);
		//if (!p.get())
		//	return 1;

		//pos3d pos = p->get_position();
		//pos2d p2(pos.x, pos.y);
		////_pos = m_plat.GetOnMapAxisPoint(p2);
		return 0;
	}

	//!
	ORZ_PLUS_INLINE int32							get_player_frame			(player_id _pid, unsigned int& _frame)
	{
		return 0;
	}

	//!
	ORZ_PLUS_INLINE unsigned						get_player_num				()
	{
		BoostShrLock lock(m_mutex);
		return m_players.size();
	}

private:
	ORZ_PLUS_INLINE unsigned						get_player_num_st			()
	{
		return m_players.size();
	}
public:

	//!
	ORZ_PLUS_INLINE int32							get_type					()
	{
		BoostShrLock lock(m_mutex);
		return (int32)m_gt;
	}

	//!
	ORZ_PLUS_INLINE int32							run							(float64 _dt)
	{
		// deal dt
		ClockTime current_time = now();
		float64 dt = (float64)(current_time - m_current_time) / 1000000.0f;
		m_current_time = current_time;
		// create monsters
		//if (m_monsters.empty())
		//	create_monster();

		// loop monsters
		//std::map<monster_id, monster_imp>::iterator itr(m_monsters.begin());
		//for (; itr!=m_monsters.end(); ++itr)
		//{
		//	//itr->second.mmd->run(_dt);
		//	itr->second.msr->run(dt);
		//}

		//! loop players
		BoostLock lock(m_mutex);
		std::map<SessionPxy, player_imp, SessionPxyLess>::iterator pitr(m_players.begin());
		for (; pitr!=m_players.end(); ++pitr)
		{
			if (pitr->second.plr)
				pitr->second.plr->run(dt);
		}
		return 0;
	}

	//!
	ORZ_PLUS_INLINE int32							set_input					(player_id _pid, int _uipt)
	{
		BoostLock lock(m_mutex);
		player_ptr p = get_player_st(_pid);
		if (!p)
			return 1;

		p->set_input(_uipt);

		return 0;
	}

	//!
	ORZ_PLUS_INLINE int32							broadcast					(IPacketShrPtr _pk, const SessionPxy* _eraser)
	{
		BoostLock lock(m_mutex);
		std::map<SessionPxy, player_imp, SessionPxyLess>::iterator itr(m_players.begin());
		for (; itr!=m_players.end(); ++itr)
		{
			if (_eraser && itr->first == *_eraser)
				continue;
			else
				m_svcMgr->send(itr->first, _pk);
		}
		return 0;
	}

	ORZ_PLUS_INLINE void							joinThread				()
	{
		m_agent.join();
	}
	
private:
	ORZ_PLUS_INLINE void						run							()
	{
		ClockTime dt = 0;
		while (m_running)
		{
			dt = m_framer.control();
			run((float64)dt/1000000.0f);
		}
	}

	//!	
	ORZ_PLUS_INLINE void							broadcast_create_msr_msg	(monster_id _msrid, int _posx, int _posy, direction_type _dt)
	{
		IPacketShrPtr pk = allocPacket();
		create_new_msr_packet(pk, _msrid, _posx, _posy, _dt);

		m_game_proxy.broadcast(pk);
	}

	//!
	ORZ_PLUS_INLINE int32							create_new_msr_packet		(IPacketShrPtr _pk, monster_id _msrid, int _posx, int _posy, direction_type _dt)
	{
		_pk->setMsg(M_NEWOBJ);
		//_pk->put("type=", 5);
		std::string tp("2");
		_pk->put(tp.c_str(), tp.size());
		_pk->put("#", 1);
		//_pk->put("msrid=", 6);
		NumStr msrid(d2s(_msrid));
		_pk->put(msrid.c_str(), msrid.size());
		_pk->put("|", 1);
		//_pk->put("direct=", 7);
		NumStr direct(d2s(_dt));
		_pk->put(direct.c_str(), direct.size());
		_pk->put("&", 1);
		//_pk->put("posx=", 5);
		NumStr posx(d2s(_posx));
		_pk->put(posx.c_str(), posx.size());
		_pk->put("&", 1);
		//_pk->put("posy=", 5);
		NumStr posy(d2s(_posy));
		_pk->put(posy.c_str(), posy.size());

		return 0;
	}

private:

	//! for create pc
	struct pc_create_data
	{
		pc_create_data(const account_ptr _act)
			: act(_act)
		{
		}

		const account_ptr			act;
	};

	//! for create monster
	struct msr_create_data
	{
		msr_create_data(const char* _pName)
			: pName(_pName)
		{
		}

		const char*					pName;
	};

	struct attack_info
	{
		attack_info()
		{
		}

		//!
		ORZ_PLUS_INLINE void				add_attacked_objs			(const event_obj_id& _eoi)
		{
			attacked_objs.push_back(_eoi);
		}

		//!
		ORZ_PLUS_INLINE void				clear_attacked_objs			()
		{
			attacked_objs.clear();
		}

		//!
		ORZ_PLUS_INLINE bool				is_attacked					(const event_obj_id& _eoi)
		{
			std::vector<event_obj_id>::iterator itr(std::find(attacked_objs.begin(), attacked_objs.end(), _eoi));
			if (itr == attacked_objs.end())
				return false;
			else
				return true;
		}

	private:
		// 被攻击过的对象
		std::vector<event_obj_id>			attacked_objs;
	};

	//! game 包装的 player
	struct player_imp
	{
		player_imp()
		{
		}

		player_imp(player_ptr _plr)
			: plr(_plr)
		{
		}

		//!
		attack_info&			get_act_if				()
		{
			return act_if;
		}

		attack_info				act_if;
		player_ptr				plr;
		int32					m_posx;
		int32					m_posy;
	};

	//! game 包装的 monster
	struct monster_imp
	{
		monster_imp(ServiceManager* _svcMgr, event_manager& _event_mgr, const monster_ptr _monster, game_proxy& _game_proxy, monster_mind_config& _mmc, demo_map& _demo_map)
			: msr(_monster)
			, mmd(new monster_mind(_svcMgr, _event_mgr, _monster.get(), _game_proxy, _mmc, _demo_map))
		{
		}

		//!
		attack_info&			get_act_if				()
		{
			return act_if;
		}

		attack_info				act_if;
		monster_ptr				msr;
		monster_mind_ptr		mmd;
	};


private:

	//! create monster
	ORZ_PLUS_INLINE void							create_monster				()
	{
		data d(msr_create_data("monster"));
		m_obj_genr_mgr.generate_object(OT_MSR, d);
	}

	//! delete monster
	ORZ_PLUS_INLINE void							erase_monster				(monster_id _msrid)
	{
		std::map<monster_id, monster_imp>::iterator itr(m_monsters.find(_msrid));
		if (itr == m_monsters.end())
			return;

		m_event_mgr.erase_event_obj(itr->second.msr->get_event_obj_id());

		m_msr_id_genr.release(_msrid);
		rmv4mMap(m_monsters, _msrid);
	}

	//!
	ORZ_PLUS_INLINE int32							remove_player				(player_id _pid)
	{
		player_ptr p = get_player_st(_pid);
		m_event_mgr.erase_event_obj(p->get_event_obj_id());
		m_pc_id_genr.release(_pid);
		//rmv4mMap(m_broadcast_list, _pid);

		return m_players.rmvById3(_pid);
	}


private:
	//! sr mgr
	ServiceManager*													m_svcMgr;

	//!
	object_generator_manager										m_obj_genr_mgr;

	//!
	game_proxy														m_game_proxy;

	//!
	event_manager													m_event_mgr;

	//! gid
	game_id															m_id;
	game_type														m_gt;
	player_ptr														m_master;

	//
	FastIdr															m_pc_id_genr;

	//
	FastIdr															m_msr_id_genr;

	//! map
	//Plat															m_plat;

	//!
	demo_map														m_demo_map;

	//! max players
	const unsigned int												m_max_player;

	//! manage players
	TrebleMap<SessionPxy, account_id, player_id, player_imp, SessionPxyLess>			
																	m_players;	
	//! monsters
	std::map<monster_id, monster_imp>								m_monsters;

	//! 用于广播的列表
	//std::map<player_id, player*>										m_broadcast_list;

	//! config
	demo_game_config*												m_dgc;

	//! clock
	ClockTime														m_current_time;

	Agent															m_agent;
	bool															m_running;
	bool															m_isEnd;
	boost::shared_mutex												m_mutex;
	Framer															m_framer;
};

typedef boost::shared_ptr<demo_game> demo_game_ptr;

}

#endif
