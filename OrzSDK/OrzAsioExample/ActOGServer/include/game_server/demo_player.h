#ifndef DEMO_PLAYER_H_
#define DEMO_PLAYER_H_

#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <orz/OrzAsio/NetEngine/PacketFreeList.h>
#include <common/game/pos3d.h>
#include <common/game/player.h>
#include <common/data.h>
#include <common/net_msg.h>
#include <common/game/state_trigger.h>
#include <common/game/action_manager.h>
#include <game_server/game_proxy.h>
#include <packet/packet_seer.h>
#include <game_server/obj_input_queue.h>
#include <game_server/skill_manager.h>
#include <game_server/motion_manager.h>
#include <common/event_manager.h>
#include <game_server/action_frame_sequence.h>
#include <game_server/config.h>
#include <game_server/damage.h>
#include <game_server/attack.h>
#include <game_server/demo_map.h>


namespace Orz
{


class demo_player : public player
{
public:
	demo_player(ServiceManager* _svcMgr, event_manager& _event_mgr, game_proxy& _game_proxy, player_id _pid, account_id _aid, const SessionPxy& _sn, const char* _pName, const char* _pNickName, 
				demo_map& _dm, pos2d _pos, demo_player_config& _dpc)
		: m_svcMgr(_svcMgr)
		, m_game_proxy(&_game_proxy)
		, m_eoi(event_obj_id((obj_type)OT_PC, (obj_id)_pid))
		, m_pid(_pid)
		, m_aid(_aid)
		, m_sn(_sn)
		, m_name(_pName)
		, m_nick_name(_pNickName)
		, m_current_speed_x(0.0f)
		, m_current_speed_y(0.0f)
		, m_dm(&_dm)
		, m_pos(pos3d(_pos.x, _pos.y, _pos.y))
		, m_next_pos(pos3d(_pos.x, _pos.y, 0.0f))
		, m_dpc(&_dpc)
		, m_col_box(collision_box(zone(0, 0, _dpc.raw_collision_box.w, _dpc.raw_collision_box.h)))
		, m_dir(true)
		, m_state_mgr(boost::bind(&demo_player::begin_state_handler, this, _1, _2, _3), 
					  boost::bind(&demo_player::change_state_handler, this, _1, _2, _3, _4), 
					  boost::bind(&demo_player::cr_handler, this, _1, _2, _3),
					  boost::bind(&demo_player::end_state_handler, this, _1, _2, _3))
		, m_mrj_sub_trigger(boost::bind(&demo_player::mrjss_begin_handler, this, _1, _2, _3), 
							boost::bind(&demo_player::change_sub_state_handler, this, _1, _2, _3, _4), 
							boost::bind(&demo_player::mrjss_cr_handler, this, _1, _2, _3),
							boost::bind(&demo_player::mrjss_end_handler, this, _1, _2, _3))
		, m_skill_mgr(boost::bind(&demo_player::check_skill_usable, this, _1, _2))
		, m_motion_mgr(boost::bind(&demo_player::check_motion_usable, this, _1, _2))
		, m_event_mgr(&_event_mgr)
		, m_input_queue(obj_input_queue(0.5f, m_key_mgr))
		, m_has_new_input(false)
		, m_collision_type(ct_null)
		, m_print_dt(0.0f)
		, m_current_time(now())
	{
		// create event obj
		m_event_mgr->create_event_obj(m_eoi, boost::bind(&demo_player::recv_event_handler, this, _1, _2, _3, _4, _5));

		// init states
		std::vector<state> states;
		for (int i=(int)PC_NULL; i<(int)PC_TOTAL; ++i)
		{
			//boost::function<void (state_id, state_trigger&, float64)> state_hdr = boost::bind(&demo_player::state_handler, this, _1, _2, _3);
			if (i == (int)PC_MOVE || i == (int)PC_RUN || i == (int)PC_JUMP)
				states.push_back(state((state_id)i, static_any<SMAS>(m_dpc->move_speed), static_any<SMAS>(m_dpc->min_move_distance), 
										boost::bind(&demo_player::state_handler, this, _1, _2, _3)));
			else
				states.push_back(state((state_id)i, boost::bind(&demo_player::state_handler, this, _1, _2, _3)));
		}
		m_state_mgr.init(states);
		m_state_mgr.start();

		// init move state's sub states
		states.clear();
		for (int i=(int)MRJSS_NULL; i<(int)MRJSS_TOTAL; ++i)
		{
			//boost::function<void (state_id, state_trigger&, float64)> state_hdr = boost::bind(&demo_player::mrjss_state_handler, this, _1, _2, _3);
			states.push_back(state((state_id)i, boost::bind(&demo_player::mrjss_state_handler, this, _1, _2, _3)));
		}
		m_mrj_sub_trigger.init(states);

		// init keys
		std::vector<key> keys;
		boost::function<void (key_value, key_manager&, key_state, input_type)> key_hdr = boost::bind(&demo_player::key_handler, this, _1, _2, _3, _4);
		keys.push_back(key(IT_PUSH_RIGHT,	IT_RELEASE_RIGHT,	KV_RIGHT,	key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_LEFT,	IT_RELEASE_LEFT,	KV_LEFT,	key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_UP,		IT_RELEASE_UP,		KV_UP,		key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_DOWN,	IT_RELEASE_DOWN,	KV_DOWN,	key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_ATTACK,	IT_RELEASE_ATTACK,	KV_ATTACK,	key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_JUMP,	IT_RELEASE_JUMP,	KV_JUMP,	key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_SKILL,	IT_RELEASE_SKILL,	KV_SKILL,	key_hdr, key_hdr, key_hdr, key_hdr));
		m_key_mgr.init(keys);

		// init skills
		std::vector<skill> skills;
		boost::function<void (skill_type, skill_manager&)> begin_skill_hdr = boost::bind(&demo_player::begin_skill_handler, this, _1, _2);
		boost::function<void (skill_type, skill_manager&)> run_skill_hdr = boost::bind(&demo_player::run_skill_handler, this, _1, _2);
		boost::function<void (skill_type, skill_manager&)> end_skill_hdr = boost::bind(&demo_player::end_skill_handler, this, _1, _2);

		
		//skills.push_back(skill(SKT_2));
		//skills.push_back(skill(SKT_3));
		//skills.push_back(skill(SKT_4));
		//skills.push_back(skill(SKT_5));
		//skills.push_back(skill(SKT_6));
		//skills.push_back(skill(SKT_7));
		//skills.push_back(skill(SKT_8));
		//skills.push_back(skill(SKT_9));
		skills.push_back(skill(SKT_10,	begin_skill_hdr, run_skill_hdr, end_skill_hdr, SIT_PUSH_DOWN, SIT_PUSH_FORWARD, SIT_PUSH_ATTACK));
		//skills.push_back(skill(SKT_11));
		//skills.push_back(skill(SKT_12));
		skills.push_back(skill(SKT_13,	begin_skill_hdr, run_skill_hdr, end_skill_hdr, SIT_PUSH_FORWARD, SIT_PUSH_DOWN, SIT_PUSH_FORWARD, SIT_PUSH_ATTACK));
		skills.push_back(skill(SKT_1,	begin_skill_hdr, run_skill_hdr, end_skill_hdr, SIT_PUSH_ATTACK));
		skills.push_back(skill(SKT_13,	begin_skill_hdr, run_skill_hdr, end_skill_hdr, SIT_PUSH_SKILL));
		//skills.push_back(skill(SKT_14));
		//skills.push_back(skill(SKT_15));
		m_skill_mgr.init(skills);

		// init motions
		std::vector<motion> motions;
		boost::function<void (motion_type, motion_manager&)> begin_motion_hdr = boost::bind(&demo_player::begin_motion_handler, this, _1, _2);
		boost::function<void (motion_type, motion_manager&)> run_motion_hdr = boost::bind(&demo_player::run_motion_handler, this, _1, _2);
		boost::function<void (motion_type, motion_manager&)> end_motion_hdr = boost::bind(&demo_player::end_motion_handler, this, _1, _2);

		motions.push_back(motion(MT_1,	begin_motion_hdr, run_motion_hdr, end_motion_hdr, MIT_PUSH_UP));
		motions.push_back(motion(MT_2,	begin_motion_hdr, run_motion_hdr, end_motion_hdr, MIT_PUSH_RIGHT));
		motions.push_back(motion(MT_3,	begin_motion_hdr, run_motion_hdr, end_motion_hdr, MIT_PUSH_DOWN));
		motions.push_back(motion(MT_4,	begin_motion_hdr, run_motion_hdr, end_motion_hdr, MIT_PUSH_LEFT));
		motions.push_back(motion(MT_5,	begin_motion_hdr, run_motion_hdr, end_motion_hdr, MIT_PUSH_UP,		MIT_RELEASE_UP,		MIT_PUSH_UP));
		motions.push_back(motion(MT_6,	begin_motion_hdr, run_motion_hdr, end_motion_hdr, MIT_PUSH_RIGHT,	MIT_RELEASE_RIGHT,	MIT_PUSH_RIGHT));
		motions.push_back(motion(MT_7,	begin_motion_hdr, run_motion_hdr, end_motion_hdr, MIT_PUSH_DOWN,	MIT_RELEASE_DOWN,	MIT_PUSH_DOWN));
		motions.push_back(motion(MT_8,	begin_motion_hdr, run_motion_hdr, end_motion_hdr, MIT_PUSH_LEFT,	MIT_RELEASE_LEFT,	MIT_PUSH_LEFT));
		motions.push_back(motion(MT_9,	begin_motion_hdr, run_motion_hdr, end_motion_hdr, MIT_PUSH_JUMP));
		m_motion_mgr.init(motions);

		// init afs
		//std::vector<action_frame_sequence> m_afss;
		m_afss.push_back(action_frame_sequence(m_afs_mgr, (afs_id)AFSPCT_IDLE,	
											 boost::bind(&demo_player::idle_fs_start_handler, this, _1, _2), 
											 boost::bind(&demo_player::idle_afs_change_fs_handler, this, _1, _2, _3), 								 
											 boost::bind(&demo_player::idle_fs_loop_handler, this, _1, _2, _3, _4, _5, _6), 
											 boost::bind(&demo_player::idle_fs_finish_handler, this, _1, _2, _3, _4, _5, _6)));

		m_afss.push_back(action_frame_sequence(m_afs_mgr, (afs_id)AFSPCT_MOVE,	 
											 boost::bind(&demo_player::move_fs_start_handler, this, _1, _2),
											 boost::bind(&demo_player::move_afs_change_fs_handler, this, _1, _2, _3),
											 boost::bind(&demo_player::move_fs_loop_handler, this, _1, _2, _3, _4, _5, _6), 
											 boost::bind(&demo_player::move_fs_finish_handler, this, _1, _2, _3, _4, _5, _6)));

		m_afss.push_back(action_frame_sequence(m_afs_mgr, (afs_id)AFSPCT_RUN, 
											 boost::bind(&demo_player::run_fs_start_handler, this, _1, _2), 
											 boost::bind(&demo_player::run_afs_change_fs_handler, this, _1, _2, _3),
											 boost::bind(&demo_player::run_fs_loop_handler, this, _1, _2, _3, _4, _5, _6), 
											 boost::bind(&demo_player::run_fs_finish_handler, this, _1, _2, _3, _4, _5, _6)));

		m_afss.push_back(action_frame_sequence(m_afs_mgr, (afs_id)AFSPCT_ATTACK, 
											 boost::bind(&demo_player::attack_fs_start_handler, this, _1, _2), 
											 boost::bind(&demo_player::attack_afs_change_fs_handler, this, _1, _2, _3), 
											 boost::bind(&demo_player::attack_fs_loop_handler, this, _1, _2, _3, _4, _5, _6), 
											 boost::bind(&demo_player::attack_fs_finish_handler, this, _1, _2, _3, _4, _5, _6)));

		m_afss.push_back(action_frame_sequence(m_afs_mgr, (afs_id)AFSPCT_SKT10,
											 boost::bind(&demo_player::skt10_fs_start_handler, this, _1, _2), 
											 boost::bind(&demo_player::skt10_afs_change_fs_handler, this, _1, _2, _3), 
											 boost::bind(&demo_player::skt10_fs_loop_handler, this, _1, _2, _3, _4, _5, _6), 
											 boost::bind(&demo_player::skt10_fs_finish_handler, this, _1, _2, _3, _4, _5, _6)));

		m_afss.push_back(action_frame_sequence(m_afs_mgr, (afs_id)AFSPCT_SKT13, 
											 boost::bind(&demo_player::skt13_fs_start_handler, this, _1, _2), 
											 boost::bind(&demo_player::skt13_afs_change_fs_handler, this, _1, _2, _3),
											 boost::bind(&demo_player::skt13_fs_loop_handler, this, _1, _2, _3, _4, _5, _6),
											 boost::bind(&demo_player::skt13_fs_finish_handler, this, _1, _2, _3, _4, _5, _6)));

		m_afss.push_back(action_frame_sequence(m_afs_mgr, (afs_id)AFSPCT_FALL,
											 boost::bind(&demo_player::fall_fs_start_handler, this, _1, _2), 
											 boost::bind(&demo_player::fall_afs_change_fs_handler, this, _1, _2, _3), 
											 boost::bind(&demo_player::fall_fs_loop_handler, this, _1, _2, _3, _4, _5, _6), 
											 boost::bind(&demo_player::fall_fs_finish_handler, this, _1, _2, _3, _4, _5, _6)));

		m_afss.push_back(action_frame_sequence(m_afs_mgr, (afs_id)AFSPCT_JUMP,
											 boost::bind(&demo_player::jump_fs_start_handler, this, _1, _2), 
											 boost::bind(&demo_player::jump_afs_change_fs_handler, this, _1, _2, _3), 
											 boost::bind(&demo_player::jump_fs_loop_handler, this, _1, _2, _3, _4, _5, _6),
											 boost::bind(&demo_player::jump_fs_finish_handler, this, _1, _2, _3, _4, _5, _6)));


		std::vector<std::vector<cfg_key_frame> >::const_iterator cfg_itr(m_dpc->cfg_key_frames.begin());
		std::string last_label;
		for (unsigned i=0; cfg_itr!=m_dpc->cfg_key_frames.end(); ++i, ++cfg_itr)
		{
			const std::vector<cfg_key_frame>& ckfs = *cfg_itr;
			std::vector<cfg_key_frame>::const_iterator itr(ckfs.begin());
			for (; itr!=ckfs.end(); ++itr)
			{
				const cfg_key_frame& ckf = *itr;
				if (last_label == ckf.label)
					continue;
				
				m_afss[i].create_frame_segment(ckf.label);
				last_label = ckf.label;
			}
		}
		cfg_itr = m_dpc->cfg_key_frames.begin();
		for (unsigned i=0; cfg_itr!=m_dpc->cfg_key_frames.end(); ++i, ++cfg_itr)
		{
			const std::vector<cfg_key_frame>& ckfs = *cfg_itr;
			std::vector<cfg_key_frame>::const_iterator itr(ckfs.begin());
			for (; itr!=ckfs.end(); ++itr)
			{
				const cfg_key_frame& ckf = *itr;
				m_afss[i].create_action_frame(ckf.label, array2d(ckf.dx, ckf.dy), ckf.frame_times, m_dpc->frame_time, 
											ckf.has_iz==0 ? false:true, zone(ckf.injure_zone.x, ckf.injure_zone.y, ckf.injure_zone.w, ckf.injure_zone.h), 
											ckf.has_az==0 ? false:true, zone(ckf.attack_zone.x, ckf.attack_zone.y, ckf.attack_zone.w, ckf.attack_zone.h), ckf.attack_y);
			}
		}
		m_afs_mgr.init(&m_afss);

		// init action_manager
		m_fs_action_mgr.create_action((action_id)amt_beeline_move, boost::bind(&demo_player::beeline_move_handler, this, _1, _2, _3, _4, _5));
		m_fs_action_mgr.create_action((action_id)amt_falling, boost::bind(&demo_player::falling_handler, this, _1, _2, _3, _4, _5));

		m_jump_action_mgr.create_action((action_id)amt_falling, boost::bind(&demo_player::falling_handler, this, _1, _2, _3, _4, _5));
	}
	virtual ~demo_player()
	{
	}

//------------------------------------------------------------------------------------------------------------
public:		// 处理事件
	//!
	ORZ_PLUS_INLINE void								recv_event_handler		(const event_obj_id& _sender, event_manager& _event_mgr, const event_obj_id& _recver, event_id _eid, std::vector<static_any<EMAS> >& _datas)
	{
		switch ((event_type)_eid)
		{
		case et_pc_attack:			process_pc_attack(_sender, _event_mgr, _recver, _eid, _datas);			break;
		}
	}
	
	//!
	ORZ_PLUS_INLINE void								process_pc_attack		(const event_obj_id& _sender, event_manager& _event_mgr, const event_obj_id& _recver, event_id _eid, std::vector<static_any<EMAS> >& _datas)
	{
		if (_datas.empty())
			return;

		attack* atk = _datas[0].get_data<attack>();
		if (!atk)
			return;

		// deal with the attack, if there is something may reduce the attack dmg or avoid it, like ac and miss, then do it now.
		// may use a model that is used to deal with attack  -to-  deal with it.
		if (m_state_mgr.get_current_state() == (state_id)PC_RISE_UP)
			return;

		// attack info:
		//printf("pid: [" << m_pid << "] - " << "attack info -  dmg: " << atk->dmg() << ", type: " << atk->type() << ", dir: " << atk->dir() << std::endl;
		// 
		m_state_mgr.change_state((state_id)PC_FALL);
		m_afs_mgr.start_afs((afs_id)AFSPCT_FALL, "fall", static_any<FSLCDS>(damage(atk->dmg(), (damage_type)atk->type(), atk->dir())));
	}

//------------------------------------------------------------------------------------------------------------
public:
	//!
	ORZ_PLUS_INLINE player_id						get_id					() const
	{
		return m_pid;
	}

	//!
	ORZ_PLUS_INLINE const event_obj_id&				get_event_obj_id		() const
	{
		return m_eoi;
	}

	//! 
	ORZ_PLUS_INLINE account_id						get_account_id			() const
	{
		return m_aid;
	}
	
	//!
	ORZ_PLUS_INLINE const SessionPxy&				get_session_proxy		() const
	{
		return m_sn;
	}
		
	//! 
	ORZ_PLUS_INLINE const std::string&				get_name				() const
	{
		return m_name;
	}

	//!
	ORZ_PLUS_INLINE const std::string&				get_nick_name			() const
	{
		return m_nick_name;
	}
	
	//!
	ORZ_PLUS_INLINE int32								get_user_type			() const
	{
		return 1;
	}

	//!
	ORZ_PLUS_INLINE int32								run						(float64 _dt)
	{
		//if (_dt > 0.05f)
		//	//printf("	demo_player::run!  beyond _dt: " << _dt << std::endl;
		
		m_state_mgr.run();
		m_print_dt += _dt;
		if (m_print_dt > 5.0f)
		{
			m_print_dt = 0.0f;
			//std::string face_dir(m_dir ? g_dirs[DIRT_RIGHT] : g_dirs[DIRT_LEFT]);
			//printf("player(id: " << m_pid << ") state - position: [ " << m_pos.x << ", " << m_pos.y 
					  //<< " ], current state: " << g_pc_states[m_state_mgr.get_current_state()] 
					  //<< ", " << g_sub_states[m_mrj_sub_trigger.get_current_state()] 
					  //<< " - face diretion: " << face_dir << std::endl;
		}
	
		return 0;
	}

	//!
	ORZ_PLUS_INLINE int32								set_input				(int _uipt)
	{
		if (m_key_mgr.push((input_type)_uipt) != 0)
			m_key_mgr.release((input_type)_uipt);

		return 0;
	}

	//!
	ORZ_PLUS_INLINE const pos3d&						get_position			() const
	{
		return m_pos;
	}

	//!
	ORZ_PLUS_INLINE const pos3d&						next_position			()
	{
		return m_next_pos;
	}

	//! 
	ORZ_PLUS_INLINE const bool						get_direction			() const
	{
		return m_dir;
	}

	//!
	ORZ_PLUS_INLINE const zone*						get_current_iz			() const
	{
		const action_frame* af = m_afs_mgr.get_current_af();
		if (!af)
			return 0;

		return &af->get_injure_zone();
	}

	//!
	ORZ_PLUS_INLINE const int						pop_event				()
	{
		return 0;
	}

	//!
	ORZ_PLUS_INLINE void								plat_collision			(int _collision)
	{
	}

	//!
	ORZ_PLUS_INLINE collision_box&					get_collision_box		()
	{
		return m_col_box;
	}

	//!
	ORZ_PLUS_INLINE int32								get_collision			() const
	{
		return m_collision;
	}

//------------------------------------------------------------------------------------------------------------
private:		// 处理actions
	//! 
	ORZ_PLUS_INLINE void								beeline_move_handler	(action_id _actid, action_manager& _action_mgr, static_any<AMAS>& _sa, float64 _pt, float64 _dt)
	{
		beeline_move* bm = _sa.get_data<beeline_move>();
		if (!bm)
		{
			//printf("pid: [" << m_pid << "] - " << "[ERROR]beeline_move_handler - !bm\n");
			return;
		}
		
		float64 dx = bm->run();
		if (m_collision_type == ct_null)
		{
			m_pos.x += dx;
		}
		if (bm->is_stopped())
		{
			//printf("pid: [" << m_pid << "] - " << "beeline_move_handler - bm->is_stopped()\n");
			_action_mgr.interrupt((action_id)amt_beeline_move);
			return;
		}
		//update_position(dx);
		////printf("pid: [" << m_pid << "] - " << "beeline_move_handler - dx: " << dx << ", m_collision_type: " << m_collision_type << ", m_pos.x: " << m_pos.x << ", dt: " << _dt << std::endl;
	}

	//! 
	ORZ_PLUS_INLINE void								falling_handler			(action_id _actid, action_manager& _action_mgr, static_any<AMAS>& _sa, float64 _pt, float64 _dt)
	{
		parabola* pba = _sa.get_data<parabola>();
		if (!pba)
			return;
		
		pos2d dpos = pba->run();
		if (m_collision_type == ct_null)
		{
			m_pos.x += dpos.x;
		}
		m_pos.y += dpos.y;
		////printf("pid: [" << m_pid << "] - " << "falling_handler - dpos.x: " << dpos.x << ", dpos.y: " << dpos.y << ", m_pos.x: " << m_pos.x << ", m_pos.y" << m_pos.y << std::endl;
		if (m_pos.y >= m_pos.z)
		{
			m_pos.y = m_pos.z;
			_action_mgr.interrupt((action_id)amt_falling);
		}
	}

//------------------------------------------------------------------------------------------------------------
private:		// 处理action frame sequence
	// idle
	//------------------------------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE void								idle_fs_start_handler		(const std::string& _label, action_frame_sequence& _afs)
	{
	}
	//!
	ORZ_PLUS_INLINE void								idle_afs_change_fs_handler	(const std::string& _old, action_frame_sequence& _afs, const std::string& _new)
	{
	}
	//!
	ORZ_PLUS_INLINE void								idle_fs_loop_handler		(const action_frame& _af, frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt)
	{
	}
	//!
	ORZ_PLUS_INLINE void								idle_fs_finish_handler		(frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt, bool _is_break)
	{
		if (!_is_break)
			_afs.change_to("idle");
	}
	//------------------------------------------------------------------------------------------------------------

	// move
	//------------------------------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE void								move_fs_start_handler		(const std::string& _label, action_frame_sequence& _afs)
	{
	}
	//!
	ORZ_PLUS_INLINE void								move_afs_change_fs_handler	(const std::string& _old, action_frame_sequence& _afs, const std::string& _new)
	{
	}
	//!
	ORZ_PLUS_INLINE void								move_fs_loop_handler		(const action_frame& _af, frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt)
	{
	}
	//!
	ORZ_PLUS_INLINE void								move_fs_finish_handler		(frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt, bool _is_break)
	{
		if (!_is_break)
			_afs.change_to("move");
	}
	//------------------------------------------------------------------------------------------------------------

	// run
	//------------------------------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE void								run_fs_start_handler		(const std::string& _label, action_frame_sequence& _afs)
	{
	}
	//!
	ORZ_PLUS_INLINE void								run_afs_change_fs_handler	(const std::string& _old, action_frame_sequence& _afs, const std::string& _new)
	{
	}
	//!
	ORZ_PLUS_INLINE void								run_fs_loop_handler			(const action_frame& _af, frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt)
	{
	}
	//!
	ORZ_PLUS_INLINE void								run_fs_finish_handler		(frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt, bool _is_break)
	{
		if (!_is_break)
			_afs.change_to("run");
	}
	//------------------------------------------------------------------------------------------------------------

	// attack
	//------------------------------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE void								attack_fs_start_handler		(const std::string& _label, action_frame_sequence& _afs)
	{
		if (_label == "attack1" || _label == "attack2" || _label == "attack3")
		{
			//printf("pid: [" << m_pid << "] - " << " attack_fs_start_handler: _label: " << _label << std::endl << std::endl;
			push_new_attack();
		}
	}
	//!
	ORZ_PLUS_INLINE void								attack_afs_change_fs_handler(const std::string& _old, action_frame_sequence& _afs, const std::string& _new)
	{
	}
	//!
	ORZ_PLUS_INLINE void								attack_fs_loop_handler		(const action_frame& _af, frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt)
	{
		if (m_collision_type == ct_null)
			m_collision_type = check_collision(m_dir?DIRT_RIGHT:DIRT_LEFT);
		
		if (m_collision_type == ct_null)
		{
			////printf("pid: [" << m_pid << "] - " << "\n attack_fs_loop_handler - m_collision_type == ct_null\n");
			float64 ft = (float64)_af.get_unit_ft();
			float64 ft_num = (float64)_af.get_ft_num();
			ft = ft / 1000000.0f;
			float64 dx = (float64)_af.get_dpos().x / ft_num;
			float64 dy = (float64)_af.get_dpos().y / ft_num;
			float64 d = _dt / ft;
			dx = dx * d;
			dy = dy * d;

			if (m_dir)
			{
				m_pos.x += dx;
				m_pos.y += dy;
			}
			else
			{
				m_pos.x -= dx;
				m_pos.y -= dy;
			}
		}

		// check attack zone
		if (_af.has_attack_zone())
		{
			int dmg = 0;
			attack_type at= at_null;
			if (_af.get_label() == "attack1")
			{
				dmg = m_dpc->attack1_dmg;
				at = at_normal;
			}
			else if (_af.get_label() == "attack2")
			{
				dmg = m_dpc->attack2_dmg;
				at = at_normal;
			}
			else
			{
				dmg = m_dpc->attack3_dmg;
				at = at_serious;
			}

			push_attack(_af.get_attack_zone(), m_dir, m_pos, _af.get_attack_y(), attack(dmg, at, m_dir ? DIRT_RIGHT:DIRT_LEFT));
		}
	}
	//!
	ORZ_PLUS_INLINE void								attack_fs_finish_handler	(frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt, bool _is_break)
	{
		if (_is_break)
			return;

		if (_fs.get_label() == "attack1")
		{
			// 有下一个动作
			if (m_next_act.is_valid())
			{
				_afs.change_to(std::string("attack2"));
				m_state_mgr.change_state((state_id)PC_ATTACK_2);
			}
			else	// 无
			{
				m_state_mgr.change_state((state_id)PC_IDLE);
				//printf("pid: [" << m_pid << "] - " << "attack 1 over, change to idle state!\n");
			}
		}
		else if (_fs.get_label() == "attack2")
		{
			// 有下一个动作
			if (m_next_act.is_valid())
			{
				_afs.change_to(std::string("attack3"));
				m_state_mgr.change_state((state_id)PC_ATTACK_3);
			}
			else	// 无
			{
				m_state_mgr.change_state((state_id)PC_IDLE);
				//printf("pid: [" << m_pid << "] - " << "attack 2 over, change to idle state!\n");
			}
		}
		else if (_fs.get_label() == "attack3")
		{
			//printf("pid: [" << m_pid << "] - " << "attack 3 over, change to idle state!\n");
			m_state_mgr.change_state((state_id)PC_IDLE);
			m_next_act.clear();
		}
	}
	//------------------------------------------------------------------------------------------------------------

	// skt10
	//------------------------------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE void								skt10_fs_start_handler		(const std::string& _label, action_frame_sequence& _afs)
	{
		//printf("pid: [" << m_pid << "] - " << "\n skt10_fs_start_handler \n");
		push_new_attack();
	}
	//!
	ORZ_PLUS_INLINE void								skt10_afs_change_fs_handler	(const std::string& _old, action_frame_sequence& _afs, const std::string& _new)
	{
	}
	//!
	ORZ_PLUS_INLINE void								skt10_fs_loop_handler		(const action_frame& _af, frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt)
	{
		if (m_collision_type == ct_null)
			m_collision_type = check_collision(m_dir?DIRT_RIGHT:DIRT_LEFT);

		if (m_collision_type == ct_null)
		{
			float64 ft = (float64)_af.get_unit_ft();
			float64 ft_num = (float64)_af.get_ft_num();
			ft = ft / 1000000.0f;
			float64 dx = (float64)_af.get_dpos().x / ft_num;
			float64 dy = (float64)_af.get_dpos().y / ft_num;
			float64 d = _dt / ft;
			dx = dx * d;
			dy = dy * d;

			if (m_dir)
			{
				m_pos.x += dx;
				m_pos.y += dy;
			}
			else
			{
				m_pos.x -= dx;
				m_pos.y -= dy;
			}
		}

		// check attack zone
		if (_af.has_attack_zone())
		{
			int dmg = m_dpc->skt10_dmg;
			attack_type at= at_serious;

			push_attack(_af.get_attack_zone(), m_dir, m_pos, _af.get_attack_y(), attack(dmg, at, m_dir ? DIRT_RIGHT:DIRT_LEFT));
		}
	}
	//!
	ORZ_PLUS_INLINE void								skt10_fs_finish_handler		(frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt, bool _is_break)
	{
		m_skill_mgr.end_skill();
		if (_is_break)
		{
			//printf("pid: [" << m_pid << "] - " << "SKT_10 break!\n");
			return;
		}
		//printf("pid: [" << m_pid << "] - " << "SKT_10 over!\n");
		m_state_mgr.change_state((state_id)PC_IDLE);
	}
	//------------------------------------------------------------------------------------------------------------

	// skt13
	//------------------------------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE void								skt13_fs_start_handler		(const std::string& _label, action_frame_sequence& _afs)
	{
		//printf("pid: [" << m_pid << "] - " << "\n skt13_fs_start_handler \n");
		push_new_attack();
	}
	//!
	ORZ_PLUS_INLINE void								skt13_afs_change_fs_handler	(const std::string& _old, action_frame_sequence& _afs, const std::string& _new)
	{
	}
	//!
	ORZ_PLUS_INLINE void								skt13_fs_loop_handler		(const action_frame& _af, frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt)
	{
		if (m_collision_type == ct_null)
			m_collision_type = check_collision(m_dir?DIRT_RIGHT:DIRT_LEFT);

		if (m_collision_type == ct_null)
		{
			float64 ft = (float64)_af.get_unit_ft();
			float64 ft_num = (float64)_af.get_ft_num();
			ft = ft / 1000000.0f;
			float64 dx = (float64)_af.get_dpos().x / ft_num;
			float64 dy = (float64)_af.get_dpos().y / ft_num;
			float64 d = _dt / ft;
			dx = dx * d;
			dy = dy * d;

			if (m_dir)
			{
				m_pos.x += dx;
				m_pos.y += dy;
			}
			else
			{
				m_pos.x -= dx;
				m_pos.y -= dy;
			}
		}

		// check attack zone
		if (_af.has_attack_zone())
		{
			int dmg = m_dpc->skt13_dmg;
			attack_type at= at_serious;

			push_attack(_af.get_attack_zone(), m_dir, m_pos, _af.get_attack_y(), attack(dmg, at, m_dir ? DIRT_RIGHT:DIRT_LEFT));
		}
	}
	//!
	ORZ_PLUS_INLINE void								skt13_fs_finish_handler		(frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt, bool _is_break)
	{
		m_skill_mgr.end_skill();
		if (_is_break)
		{
			//printf("pid: [" << m_pid << "] - " << "SKT_13 break!\n");
			return;
		}
		//printf("pid: [" << m_pid << "] - " << "SKT_13 over!\n");
		m_state_mgr.change_state((state_id)PC_IDLE);
	}
	//------------------------------------------------------------------------------------------------------------

	// fall
	//------------------------------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE void								fall_fs_start_handler		(const std::string& _label, action_frame_sequence& _afs)
	{
		const static_any<FSLCDS>* sa = _afs.get_fs_lcd(_label);
		if (!sa) 
		{
			//printf("fall_fs_start_handler - !sa - label: " << _label << std::endl;
			return;
		}

		damage* dmg = sa->get_data<damage>();
		if (!dmg) 
		{
			//printf("fall_fs_start_handler - !dmg - label: " << _label << std::endl;
			return;
		}

		if (_label == "fall" && m_fs_action_mgr.is_interrupted((action_id)amt_beeline_move))
		{
			state_id sid = m_state_mgr.get_last_state();
			if ((pc_state)sid == PC_FALL || (pc_state)sid == PC_JUMP)
				_afs.change_to("fall1", *sa);
			else
			{
				// 检测攻击类型，如果是击飞则切换到fall1；如果不是，则根据击退距离播放fall1动画
				switch (dmg->type())
				{
				case dmgt_normal:		
					{
						// 根据attack的dmg值和自己的修正值设定被击退的最终距离
						int bt = get_beat_back_by_dmg(*dmg);
						// 根据fall持续的时间，计算速度
						const cfg_key_frame* ckf = get_ckf(6, 0);
						if (ckf)
						{
							float64 unit_ft = m_dpc->frame_time / 1000000.0f;
							float64 v0 = dmg->dir() == DIRT_RIGHT ? (bt / (ckf->frame_times * unit_ft)) : -(bt / (ckf->frame_times * unit_ft));
							//v0 /= (float64)m_dpc->client_fps;
							//printf("pid: [" << m_pid << "] - " << "fall_fs_start_handler - fall - v0: " << v0 << ", bt: " << bt << std::endl;
							m_fs_action_mgr.awake((action_id)amt_beeline_move, static_any<AMAS>(beeline_move(v0, (float64)bt)));
							// send dmg msg
							broadcast_dmg_and_bm_msg(*dmg, v0/(float64)m_dpc->client_fps, 0.0f, bt);
						}
					}break;
				case dmgt_serious:	
				case dmgt_rise:	
					_afs.change_to("fall1", *sa);	break;
				}
			}
		}
		else if (_label == "fall1")
		{
			//state_id sid = m_state_mgr.get_last_state();
			//if ((pc_state)sid != PC_FALL && (pc_state)sid != PC_JUMP)
			{
				// 根据attack的dmg值和自己的修正值设定抛物线
				parabola pba = get_parabola_by_dmg(*dmg, (float64)m_dpc->g, m_dpc->move_speed, 0.0f, m_dpc->client_fps);
				m_fs_action_mgr.awake((action_id)amt_falling, static_any<AMAS>(pba));
				//printf("pid: [" << m_pid << "] - " << "fall_fs_start_handler - fall1 - v0: " << -pba.get_v0() << std::endl;
				// send dmg msg
				broadcast_dmg_and_pda_msg(*dmg, -pba.get_v0(), dmg->dir()?(m_dpc->move_speed*2.0f):-(m_dpc->move_speed*2.0f), 0.0f);
			}
			//else
			//{
			//	// 使用固定v0的空中挨揍的抛物线
			//	//parabola pba = get_parabola_by_dmg(*dmg, (float64)m_dpc->g, m_dpc->move_speed/2.0f, 0.0f, m_dpc->client_fps);
			//	parabola pba = parabola(m_dpc->f_max_atk_y, m_dpc->move_speed, 0.0f, m_dpc->g, m_dpc->client_fps);
			//	m_fs_action_mgr.awake((action_id)amt_falling, static_any<AMAS>(pba));
			//	//printf("pid: [" << m_pid << "] - " << "fall_fs_start_handler - fall1 - combo! - v0: " << -pba.get_v0() << std::endl;
			//	// send dmg msg
			//	broadcast_dmg_and_pda_msg(*dmg, -pba.get_v0(), dmg->dir()?(m_dpc->move_speed*2.0f):-(m_dpc->move_speed*2.0f), 0.0f);
			//}
		}
	}
	//!
	ORZ_PLUS_INLINE void								fall_afs_change_fs_handler	(const std::string& _old, action_frame_sequence& _afs, const std::string& _new)
	{
	}
	//!
	ORZ_PLUS_INLINE void								fall_fs_loop_handler		(const action_frame& _af, frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt)
	{
		if (m_collision_type == ct_null)
			m_collision_type = check_collision(!m_dir?DIRT_RIGHT:DIRT_LEFT);

		// run action manager in fall state to manager pc's move
		m_fs_action_mgr.simulate();
		const std::string& label = _fs.get_label();

		const static_any<FSLCDS>* sa = _afs.get_fs_lcd(label);
		if (!sa) 
		{
			//printf("fall_fs_loop_handler - !sa - label: " << label << std::endl;
			return;
		}

		damage* dmg = sa->get_data<damage>();
		if (!dmg) 
		{
			//printf("fall_fs_loop_handler - !dmg - label: " << label << std::endl;
			return;
		}


		if (label == "fall")
		{
			if (m_fs_action_mgr.is_interrupted((action_id)amt_beeline_move))
			{
				broadcast_pos_msg();
				m_state_mgr.change_state((state_id)PC_IDLE);
			}
		}
		else if (label == "fall1")
		{
		}
		else if (label == "fall2")
		{
			// 检测人物坐标是否落地
			if (m_fs_action_mgr.is_interrupted((action_id)amt_falling))
			{
				broadcast_pos_msg();
				_afs.change_to(std::string("fall3"), *sa);
			}
		}
	}
	//!
	ORZ_PLUS_INLINE void								fall_fs_finish_handler		(frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt, bool _is_break)
	{
		const std::string& label = _fs.get_label();

		const static_any<FSLCDS>* sa = _afs.get_fs_lcd(label);
		if (!sa) 
		{
			//printf("fall_fs_finish_handler - !sa - label: " << label << std::endl;
			return;
		}

		////printf("pid: [" << m_pid << "] - " << "fall_fs_finish_handler - label: " << label << std::endl;

		if (label == "fall")
		{
			if (_is_break)
				m_fs_action_mgr.interrupt_all();
			else
				_afs.change_to(std::string("fall"), *sa);
		}
		else if (label == "fall1")
		{
			if (_is_break)
				m_fs_action_mgr.interrupt_all();
			else
				_afs.change_to(std::string("fall2"), *sa);
		}
		else if (label == "fall2")
		{
			if (!_is_break)
				_afs.change_to(std::string("fall2"), *sa);
			else
				m_fs_action_mgr.interrupt_all();
		}
		else if (label == "fall3")
		{
			if (_is_break)
				m_fs_action_mgr.interrupt_all();
			else
			{
				m_state_mgr.change_state((state_id)PC_RISE_UP);
				_afs.change_to(std::string("riseup"), *sa);
			}
		}
		else if (label == "riseup")
		{
			if (_is_break)
				m_fs_action_mgr.interrupt_all();
			m_state_mgr.change_state((state_id)PC_IDLE);
		}
	}
	//------------------------------------------------------------------------------------------------------------

	// jump
	//------------------------------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE void								jump_fs_start_handler		(const std::string& _label, action_frame_sequence& _afs)
	{
	}
	//!
	ORZ_PLUS_INLINE void								jump_afs_change_fs_handler	(const std::string& _old, action_frame_sequence& _afs, const std::string& _new)
	{
	}
	//!
	ORZ_PLUS_INLINE void								jump_fs_loop_handler		(const action_frame& _af, frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt)
	{
		const std::string& label = _fs.get_label();
		m_jump_action_mgr.simulate();

		if (label == "jump2")
		{	
			// 检测人物坐标是否落地
			if (m_pos.y >= m_pos.z)
			{
				m_pos.y = m_pos.z;
				m_jump_action_mgr.interrupt((action_id)amt_falling);

				_afs.change_to(std::string("jump3"));
			}
		}
	}
	//!
	ORZ_PLUS_INLINE void								jump_fs_finish_handler		(frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt, bool _is_break)
	{
		const std::string& label = _fs.get_label();

		if (label == "jump1")
		{
			//if (_is_break)
			//	m_jump_action_mgr.interrupt((action_id)amt_falling);
			m_jump_action_mgr.awake((action_id)amt_falling, static_any<AMAS>(parabola(m_dpc->jump_high, 0.0f, 0.0f, m_dpc->g, m_dpc->client_fps)));
			_afs.change_to(std::string("jump2"));
		}
		else if (label == "jump2")
		{
			if (!_is_break)
			{
				// 检测人物坐标是否落地
				if (m_pos.y >= m_pos.z)
				{
					m_pos.y = m_pos.z;
					m_jump_action_mgr.interrupt((action_id)amt_falling);

					_afs.change_to(std::string("jump3"));
				}
				else
					_afs.change_to(std::string("jump2"));
			}
		}
		else if (label == "jump3")
		{
			m_jump_action_mgr.interrupt((action_id)amt_falling);
			m_state_mgr.change_state((state_id)PC_IDLE);
		}
	}
	//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
private:		// state 's handler
	//! begin state handler
	ORZ_PLUS_INLINE void								begin_state_handler		(state_id _stid, state_trigger& _state_mgr, float64 _dt)
	{
		//printf("pid: [" << m_pid << "] - " << "begin state!\n");
		_state_mgr.change_state((state_id)PC_IDLE);
		m_afs_mgr.start_afs((afs_id)AFSPCT_IDLE, "idle");
	}

	//! state handler
	ORZ_PLUS_INLINE void								state_handler			(state_id _stid, state_trigger& _state_mgr, float64 _dt)
	{
		switch ((int)_stid)
		{
		case PC_IDLE:		idle_state_handler(_state_mgr, _dt);	break;
		case PC_MOVE:		move_state_handler(_state_mgr, _dt);	break;
		case PC_ATTACK_1:	attack1_state_handler(_state_mgr, _dt);	break;
		case PC_RUN:		run_state_handler(_state_mgr, _dt);		break;
		case PC_JUMP:		jump_state_handler(_state_mgr, _dt);	break;
		case PC_ATTACK_2:	attack2_state_handler(_state_mgr, _dt);	break;
		case PC_ATTACK_3:	attack3_state_handler(_state_mgr, _dt);	break;
		case PC_SKILL:		skill_state_handler(_state_mgr, _dt);	break;
		case PC_FALL:		fall_state_handler(_state_mgr, _dt);	break;
		case PC_RISE_UP:	rise_up_state_handler(_state_mgr, _dt);	break;
		default:													break;
		}
	}

	//!
	ORZ_PLUS_INLINE void								change_state_handler	(state_id _old, state_trigger& _state_mgr, state_id _new, std::vector<static_any<SMAS> >& _datas)
	{
		//printf("pid: [" << m_pid << "] - " << g_pc_states[_old] << "  -to-  " << g_pc_states[_new] << std::endl;
		m_collision_type = ct_null;
		if (_old == (state_id)PC_IDLE && _new == (state_id)PC_MOVE)
		{
			if (_datas.empty())
				return;
			direction_type* pdt = _datas[0].get_data<direction_type>();
			if (!pdt)
				return;

			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_MOVE, "move");

			_state_mgr.set_cs_lcd<float64>(0, m_dpc->move_speed);
			_state_mgr.set_cs_lcd<float64>(1, m_dpc->min_move_distance);

			direction_type dt = *pdt;

			if (dt != DIRT_NULL)
			{
				broadcast_move_msg(dt, true);
				if (dt == DIRT_RIGHT || dt == DIRT_RIGHT_UP || dt == DIRT_RIGHT_DOWN)
					m_dir = true;
				else if (dt == DIRT_LEFT || dt == DIRT_LEFT_UP || dt == DIRT_LEFT_DOWN)
					m_dir = false;
			}
		}
		else if (_old == (state_id)PC_IDLE && _new == (state_id)PC_RUN)
		{
			if (_datas.empty())
				return;

			direction_type* pdt = _datas[0].get_data<direction_type>();
			if (!pdt)
				return;

			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_RUN, "run");

			_state_mgr.set_cs_lcd<float64>(0, m_dpc->run_speed);
			_state_mgr.set_cs_lcd<float64>(1, m_dpc->min_run_distance);

			direction_type dt = *pdt;

			if (dt != DIRT_NULL)
			{
				broadcast_run_msg(dt, true);
				if (dt == DIRT_RIGHT || dt == DIRT_RIGHT_UP || dt == DIRT_RIGHT_DOWN)
					m_dir = true;
				else if (dt == DIRT_LEFT || dt == DIRT_LEFT_UP || dt == DIRT_LEFT_DOWN)
					m_dir = false;
			}
			return;
		}
		else if (_old == (state_id)PC_IDLE && _new == (state_id)PC_JUMP)
		{
			if (_datas.empty())
				return;

			direction_type* pdt = _datas[0].get_data<direction_type>();
			if (!pdt)
				return;

			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_JUMP, "jump1");

			_state_mgr.set_cs_lcd<float64>(0, m_dpc->move_speed);
			_state_mgr.set_cs_lcd<float64>(1, m_dpc->min_move_distance);

			direction_type dt = *pdt;

			broadcast_jump_msg(dt, true);
			m_current_speed_x = m_dpc->move_speed;
			m_current_speed_y = (float64)m_dpc->min_move_distance;
			return;
		}
		else if (_old == (state_id)PC_IDLE && _new == (state_id)PC_ATTACK_1)
		{
			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_ATTACK, "attack1");

			broadcast_action_msg(AT_NORMAL_ATTACK, m_dir ? DIRT_RIGHT : DIRT_LEFT, AS_1, AS_NULL, true);
		}
		else if (_old == (state_id)PC_MOVE && _new == (state_id)PC_IDLE)
		{
			if (_datas.empty())
				return;

			pc_state* pps = _datas[0].get_data<pc_state>();
			if (!pps)
				return;

			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_IDLE, "idle");

			pc_state cs = *pps;

			if (cs == PC_MOVE || cs == PC_RUN)
			{
				if (cs == PC_MOVE)
					broadcast_move_msg(m_dir ? DIRT_RIGHT : DIRT_LEFT, false);
				else
					broadcast_run_msg(m_dir ? DIRT_RIGHT : DIRT_LEFT, false);
			
				m_mrj_sub_trigger.stop();
			}
		}
		else if (_old == (state_id)PC_MOVE && _new == (state_id)PC_JUMP)
		{
			if (_datas.empty())
				return;

			direction_type* pdt = _datas[0].get_data<direction_type>();
			if (!pdt)
				return;

			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_JUMP, "jump1");
			_state_mgr.set_cs_lcd<float64>(0, m_dpc->move_speed);
			_state_mgr.set_cs_lcd<float64>(1, m_dpc->min_move_distance);

			direction_type dt = *pdt;
			
			broadcast_jump_msg(dt, true);
			m_current_speed_x = m_dpc->move_speed;
			m_current_speed_y = (float64)m_dpc->min_move_distance;
			return;
		}
		else if (_old == (state_id)PC_MOVE && _new == (state_id)PC_ATTACK_1)
		{
			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_ATTACK, "attack1");

			broadcast_action_msg(AT_NORMAL_ATTACK, m_dir ? DIRT_RIGHT : DIRT_LEFT, AS_1, AS_NULL, true);
			m_mrj_sub_trigger.stop();
		}
		else if (_old == (state_id)PC_MOVE && _new == (state_id)PC_SKILL)
		{
			if (_datas.empty())
				return;

			skill_type* pst = _datas[0].get_data<skill_type>();
			if (!pst)
				return;

			skill_type st = *pst;
			
			broadcast_skill_msg(st, m_dir ? DIRT_RIGHT : DIRT_LEFT);
			m_mrj_sub_trigger.stop();
			m_skill_mgr.start_skill(st);
		}
		else if (_old == (state_id)PC_IDLE && _new == (state_id)PC_SKILL)
		{
			if (_datas.empty())
				return;

			skill_type* pst = _datas[0].get_data<skill_type>();
			if (!pst)
				return;

			skill_type st = *pst;
			
			broadcast_skill_msg(st, m_dir ? DIRT_RIGHT : DIRT_LEFT);
			//m_mrj_sub_trigger.stop();
			m_skill_mgr.start_skill(st);
		}
		else if (_old == (state_id)PC_RUN && _new == (state_id)PC_IDLE)
		{
			if (_datas.empty())
				return;

			pc_state* pps = _datas[0].get_data<pc_state>();
			if (!pps)
				return;

			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_IDLE, "idle");

			pc_state cs = *pps;

			if (cs == PC_MOVE || cs == PC_RUN)
			{
				if (cs == PC_MOVE)
					broadcast_move_msg(m_dir ? DIRT_RIGHT : DIRT_LEFT, false);
				else
					broadcast_run_msg(m_dir ? DIRT_RIGHT : DIRT_LEFT, false);
				
				m_mrj_sub_trigger.stop();
			}
		}
		else if (_old == (state_id)PC_RUN && _new == (state_id)PC_JUMP)
		{
			if (_datas.empty())
				return;

			direction_type* pdt = _datas[0].get_data<direction_type>();
			if (!pdt)
				return;

			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_JUMP, "jump1");
			_state_mgr.set_cs_lcd<float64>(0, m_dpc->run_speed);
			_state_mgr.set_cs_lcd<float64>(1, m_dpc->min_run_distance);

			direction_type dt = *pdt;
			
			broadcast_jump_msg(dt, true);
			m_current_speed_x = m_dpc->move_speed;
			m_current_speed_y = (float64)m_dpc->min_move_distance;
			return;
		}
		else if (_old == (state_id)PC_RUN && _new == (state_id)PC_SKILL)
		{
			if (_datas.empty())
				return;

			skill_type* pst = _datas[0].get_data<skill_type>();
			if (!pst)
				return;

			skill_type st = *pst;

			broadcast_skill_msg(st, m_dir ? DIRT_RIGHT : DIRT_LEFT);
			m_mrj_sub_trigger.stop();
			m_skill_mgr.start_skill(st);
		}
		else if (_old == (state_id)PC_ATTACK_1 && _new == (state_id)PC_ATTACK_2)
		{
			m_next_act.clear();
		}
		else if (_old == (state_id)PC_ATTACK_2 && _new == (state_id)PC_ATTACK_3)
		{
			m_next_act.clear();
		}
		else if (_old == (state_id)PC_ATTACK_1 && _new == (state_id)PC_IDLE)
		{
			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_IDLE, "idle");
		}
		else if (_old == (state_id)PC_ATTACK_2 && _new == (state_id)PC_IDLE)
		{
			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_IDLE, "idle");
		}
		else if (_old == (state_id)PC_ATTACK_3 && _new == (state_id)PC_IDLE)
		{
			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_IDLE, "idle");
		}
		else if (_old == (state_id)PC_SKILL && _new == (state_id)PC_IDLE)
		{
			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_IDLE, "idle");
		}
		else if (_old == (state_id)PC_JUMP && _new == (state_id)PC_IDLE)
		{
			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_IDLE, "idle");
		}
		else if (_old == (state_id)PC_FALL && _new == (state_id)PC_RISE_UP)
		{
			// change afs manager
		}
		else if (_old == (state_id)PC_FALL && _new == (state_id)PC_IDLE)
		{
			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_IDLE, "idle");
		}
		else if (_old == (state_id)PC_RISE_UP && _new == (state_id)PC_IDLE)
		{
			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSPCT_IDLE, "idle");
		}
	}

	//! common rum handler
	ORZ_PLUS_INLINE void								cr_handler				(state_id _stid, state_trigger& _state_mgr, float64 _dt)
	{
		m_afs_mgr.run();
	}

	//! end state handler
	ORZ_PLUS_INLINE void								end_state_handler		(state_id _stid, state_trigger& _state_mgr, float64 _dt)
	{
		//printf("pid: [" << m_pid << "] - " << "end state! now stop!" << std::endl << std::endl;
		_state_mgr.stop();
	}

//------------------------------------------------------------------------------------------------------------
private:		// key 's handler
	//!
	ORZ_PLUS_INLINE void								key_handler				(key_value _kv, key_manager& _key_mgr, key_state _ks, input_type _it)
	{
		if (_ks == KS_PUSHED || _ks == KS_RELEASED)
		{
			//if (_it == IT_PUSH_ATTACK)
				//printf("pid: [" << m_pid << "] - " << "IT_PUSH_ATTACK!\n");
			m_has_new_input = true;
			obj_input pi(_it, _ks);
			pi.now();
			m_undo_inputs.push(pi);
			m_state_mgr.run();
		}
	}

//------------------------------------------------------------------------------------------------------------
private:		// skill's handler
	//! 技能检查回调函数
	ORZ_PLUS_INLINE bool								check_skill_usable		(skill_type _st, skill_manager& _skill_mgr)
	{
		return true;
	}

	//! 
	ORZ_PLUS_INLINE void								begin_skill_handler		(skill_type _st, skill_manager& _skill_mgr)
	{
		switch (_st)
		{
		case SKT_1:		// J
			{
			}break;
		case SKT_10:	// ↓→		+ J
			{
				// change afs manager
				m_afs_mgr.start_afs((afs_id)AFSPCT_SKT10, "skt10");
			}break;
		case SKT_13:	// →↓→	+ J
			{
				// change afs manager
				m_afs_mgr.start_afs((afs_id)AFSPCT_SKT13, "skt13");
			}break;
		}
	}

	//! 
	ORZ_PLUS_INLINE void								run_skill_handler		(skill_type _st, skill_manager& _skill_mgr)
	{
		switch (_st)
		{
		case SKT_1:		// J
			{
			}break;
		case SKT_10:	// ↓→		+ J
			{
			}break;
		case SKT_13:	// ↓→		+ J
			{
			}break;
		}
	}

	//! 
	ORZ_PLUS_INLINE void								end_skill_handler		(skill_type _st, skill_manager& _skill_mgr)
	{
		switch (_st)
		{
		case SKT_1:		// J
			{
			}break;
		case SKT_10:	// ↓→		+ J
			{
			}break;
		case SKT_13:	// ↓→		+ J
			{
			}break;
		}
	}

//------------------------------------------------------------------------------------------------------------
private:		// motion's handler
	//! motion检查回调函数
	ORZ_PLUS_INLINE bool								check_motion_usable		(motion_type _mt, motion_manager& _motion_mgr)
	{
		return true;
	}

	//! 
	ORZ_PLUS_INLINE void								begin_motion_handler	(motion_type _mt, motion_manager& _motion_mgr)
	{
		switch (_mt)
		{
		case MT_1:		// walk
			{
			}break;
		}
	}

	//! 
	ORZ_PLUS_INLINE void								run_motion_handler		(motion_type _mt, motion_manager& _motion_mgr)
	{
		switch (_mt)
		{
		case MT_1:		// walk
			{
			}break;
		}
	}

	//! 
	ORZ_PLUS_INLINE void								end_motion_handler		(motion_type _mt, motion_manager& _motion_mgr)
	{
	}


//------------------------------------------------------------------------------------------------------------
private:
	//------------------------------------------------------------------------------------------------------------
	//! PC_IDLE
	ORZ_PLUS_INLINE void								idle_state_handler			(state_trigger& _state_mgr, float64 _dt)
	{
		// #1 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(PC_IDLE);
		if (pi.get_input() != IT_NULL)
		{
			// check input. 按键序列多的优先级大于少的
			motion_type mt = m_motion_mgr.check_motion(m_motion_mgr.match_input(m_dir, false, m_input_queue.get_inputs(m_dpc->skill_act_time, KS_NULL)));
			if (mt != MT_NULL)
			{
				switch (mt)
				{
				case MT_1:	case MT_2:	case MT_3:	case MT_4:
					{
						_state_mgr.change_state((state_id)PC_MOVE, static_any<SMAS>(get_direction_by_input(pi.get_input())));
						return;
					}break;
				case MT_5:	case MT_6:	case MT_7:	case MT_8:
					{
						_state_mgr.change_state((state_id)PC_RUN, static_any<SMAS>(get_direction_by_input(pi.get_input())));
						return;
					}break;
				//case MT_9:
				//	{
				//		_state_mgr.change_state((state_id)PC_JUMP, static_any<SMAS>(check_motion_direction()));
				//		return;
				//	}break;
				}
			}
			
			// skill, attack, and so on
			// we use skill manager  -to-  deal with attack and skills
			skill_type skt = m_skill_mgr.check_skill(m_skill_mgr.match_input(m_dir, false, m_input_queue.get_inputs(m_dpc->skill_act_time, KS_PUSHED)));
			if (skt != SKT_NULL)
			{
				switch (skt)
				{
				case SKT_1:		// J
					{
						_state_mgr.change_state((state_id)PC_ATTACK_1);
						return;
					}break;
				case SKT_10:		// ↓→	+ J
				{
					m_input_queue.clear();
					_state_mgr.change_state((state_id)PC_SKILL, static_any<SMAS>(SKT_10));
					return;
				}break;
				case SKT_13:		// →↓→ + J
				{
					m_input_queue.clear();
					_state_mgr.change_state((state_id)PC_SKILL, static_any<SMAS>(SKT_13));
					return;
				}break;
				}
			}
			//if (pi.get_input() == IT_PUSH_ATTACK)
			//	//printf("pid: [" << m_pid << "] - " << "idle_state_handler - IT_PUSH_ATTACK!\n");
		}

		// check key state
		if (m_key_mgr.get_key_state_by_kv(KV_RIGHT) == KS_PUSHED ||
			m_key_mgr.get_key_state_by_kv(KV_LEFT) == KS_PUSHED ||
			m_key_mgr.get_key_state_by_kv(KV_UP) == KS_PUSHED ||
			m_key_mgr.get_key_state_by_kv(KV_DOWN) == KS_PUSHED )
		{
			_state_mgr.change_state((state_id)PC_MOVE, static_any<SMAS>(check_motion_direction()));
			return;
		}
	}
	//------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------
	//! PC_MOVE
	ORZ_PLUS_INLINE void								move_state_handler			(state_trigger& _state_mgr, float64 _dt)
	{
		if (m_collision_type == ct_null)
			m_collision_type = check_collision(check_motion_direction());
		else if (m_has_new_input)
			m_collision_type = check_collision(check_motion_direction());
		

		// #1 deal with move
		if (!m_mrj_sub_trigger.is_start())
		{
			static_any<SMAS>* cs_x = _state_mgr.get_cs_lcd(0);
			static_any<SMAS>* cs_y = _state_mgr.get_cs_lcd(1);
			if (!cs_x || !cs_y)
			{
				//printf("pid: [" << m_pid << "] - " << "	move_state_handler - ERROR! !cs_x || !cs_y\n");
				m_mrj_sub_trigger.start();
			}
			else
				m_mrj_sub_trigger.start(*cs_x, *cs_y);
		}
		
		m_mrj_sub_trigger.run(_dt);

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(PC_MOVE);
		if (pi.get_input() != IT_NULL)
		{
			// check input. 按键序列多的优先级大于少的
			motion_type mt = m_motion_mgr.check_motion(m_motion_mgr.match_input(m_dir, false, m_input_queue.get_inputs(g_min, KS_NULL)));
			if (mt != MT_NULL)
			{
				//switch (mt)
				//{
				//case MT_9:
				//	{
				//		_state_mgr.change_state((state_id)PC_JUMP, static_any<SMAS>(m_dir?DIRT_RIGHT:DIRT_LEFT));
				//		return;
				//	}break;
				//}
			}

			// attack
			// we use skill manager  -to-  deal with attack and skills
			skill_type skt = m_skill_mgr.check_skill(m_skill_mgr.match_input(m_dir, false, m_input_queue.get_inputs(m_dpc->skill_act_time, KS_PUSHED)));
			switch (skt)
			{
			case SKT_1:		// J
				{
					_state_mgr.change_state((state_id)PC_ATTACK_1);
					return;
				}break;
			case SKT_10:		// ↓→	+ J
				{
					m_input_queue.clear();
					_state_mgr.change_state((state_id)PC_SKILL, static_any<SMAS>(SKT_10));
					return;
				}break;
			case SKT_13:		// →↓→ + J
				{
					m_input_queue.clear();
					_state_mgr.change_state((state_id)PC_SKILL, static_any<SMAS>(SKT_13));
					return;
				}break;
			}
		}
		// second check keys state 
		if (m_key_mgr.get_key_state_by_kv(KV_RIGHT) != KS_PUSHED &&
			m_key_mgr.get_key_state_by_kv(KV_LEFT) != KS_PUSHED &&
			m_key_mgr.get_key_state_by_kv(KV_UP) != KS_PUSHED &&
			m_key_mgr.get_key_state_by_kv(KV_DOWN) != KS_PUSHED )
		{
			pc_state cs = (pc_state)_state_mgr.get_current_state();
			_state_mgr.change_state(PC_IDLE, static_any<SMAS>(cs));
		}
	}
		//! mss begin
		ORZ_PLUS_INLINE void								mrjss_begin_handler			(state_id _stid, state_trigger& _state_mgr, float64 _dt)
		{
			float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
			float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
			if (!curr_speed_x || !curr_speed_y)
			{
				//printf("pid: [" << m_pid << "] - " << "mrjss_begin_handler - !curr_speed_x || !curr_speed_y\n");
				return;
			}
			
			// 检查输入， 判断移动方向
			opinion_motion_direction(false);

			if (_state_mgr.get_current_state() == 0)
				_state_mgr.change_to_end();
			
			_state_mgr.run(_dt);
		}

		//!
		ORZ_PLUS_INLINE void								change_sub_state_handler	(state_id _old, state_trigger& _state_mgr, state_id _new, std::vector<static_any<SMAS> >& _datas)
		{	
		}

		//! mrj's common rum handler
		ORZ_PLUS_INLINE void								mrjss_cr_handler			(state_id _stid, state_trigger& _state_mgr, float64 _dt)
		{
		}

		//! mss handler
		ORZ_PLUS_INLINE void								mrjss_state_handler			(state_id _stid, state_trigger& _state_mgr, float64 _dt)
		{
			switch ((int)_stid)
			{
			case MRJSS_RIGHT:			mrjss_right_handler(_state_mgr, _dt);			break;
			case MRJSS_LEFT:			mrjss_left_handler(_state_mgr, _dt);			break;
			case MRJSS_UP:				mrjss_up_handler(_state_mgr, _dt);				break;
			case MRJSS_DOWN:			mrjss_down_handler(_state_mgr, _dt);			break;
			case MRJSS_RIGHT_UP:		mrjss_right_up_handler(_state_mgr, _dt);		break;
			case MRJSS_LEFT_UP:			mrjss_left_up_handler(_state_mgr, _dt);			break;
			case MRJSS_RIGHT_DOWN:		mrjss_right_down_handler(_state_mgr, _dt);		break;
			case MRJSS_LEFT_DOWN:		mrjss_left_down_handler(_state_mgr, _dt);		break;
			case MRJSS_JUMP_IDLE:		mrjss_jump_idle_handler(_state_mgr, _dt);		break;
			default:																	break;
			}
		}
		//! PC_JUMP's sub state
		ORZ_PLUS_INLINE void								mrjss_jump_idle_handler		(state_trigger& _state_mgr, float64 _dt)
		{
			opinion_motion_direction();
		}
		//! PC_MOVE,RUN,JUMP's sub state
		ORZ_PLUS_INLINE void								mrjss_right_handler			(state_trigger& _state_mgr, float64 _dt)
		{
			if (m_collision_type == ct_null)
			{
				float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
				float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
				if (!curr_speed_x || !curr_speed_y)
				{
					//printf("pid: [" << m_pid << "] - " << "mrjss_right_handler - !curr_speed_x || !curr_speed_y\n");
					return;
				}

				float64 cs_x = *curr_speed_x;
				float64 cs_y = *curr_speed_y;
				float64 fix = _dt * m_dpc->client_fps;
				m_pos.x += fix * cs_x;
			}
			opinion_motion_direction();
		}
			//! PC_MOVE,RUN,JUMP's sub state
			ORZ_PLUS_INLINE void								mrjss_right_up_handler		(state_trigger& _state_mgr, float64 _dt)
			{
				if (m_collision_type == ct_null)
				{
					float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
					float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
					if (!curr_speed_x || !curr_speed_y)
					{
						//printf("pid: [" << m_pid << "] - " << "mrjss_right_up_handler - !curr_speed_x || !curr_speed_y\n");
						return;
					}
		
					float64 cs_x = *curr_speed_x;
					float64 cs_y = *curr_speed_y;
					float64 fix = _dt * m_dpc->client_fps;
					m_pos.x += fix * cs_x;
					m_pos.y -= fix * cs_y;
					m_pos.z -= fix * cs_y;
				}
				opinion_motion_direction();
			}
		//! PC_MOVE,RUN,JUMP's sub state
		ORZ_PLUS_INLINE void								mrjss_left_handler			(state_trigger& _state_mgr, float64 _dt)
		{
			if (m_collision_type == ct_null)
			{
				float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
				float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
				if (!curr_speed_x || !curr_speed_y)
				{
					//printf("pid: [" << m_pid << "] - " << "mrjss_left_handler - !curr_speed_x || !curr_speed_y\n");
					return;
				}
				float64 cs_x = *curr_speed_x;
				float64 cs_y = *curr_speed_y;
				float64 fix = _dt * m_dpc->client_fps;
				m_pos.x -= fix * cs_x;
			}
			opinion_motion_direction();
		}
			//! PC_MOVE,RUN,JUMP's sub state
			ORZ_PLUS_INLINE void								mrjss_left_up_handler			(state_trigger& _state_mgr, float64 _dt)
			{
				if (m_collision_type == ct_null)
				{
					float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
					float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
					if (!curr_speed_x || !curr_speed_y)
					{
						//printf("pid: [" << m_pid << "] - " << "mrjss_left_up_handler - !curr_speed_x || !curr_speed_y\n");
						return;
					}
					float64 cs_x = *curr_speed_x;
					float64 cs_y = *curr_speed_y;
					float64 fix = _dt * m_dpc->client_fps;
					m_pos.x -= fix * cs_x;
					m_pos.y -= fix * cs_y;
					m_pos.z -= fix * cs_y;
				}
				opinion_motion_direction();
			}
		//! PC_MOVE,RUN,JUMP's sub state
		ORZ_PLUS_INLINE void								mrjss_up_handler				(state_trigger& _state_mgr, float64 _dt)
		{
			if (m_collision_type == ct_null)
			{
				float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
				float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
				if (!curr_speed_x || !curr_speed_y)
				{
					//printf("pid: [" << m_pid << "] - " << "mrjss_up_handler - !curr_speed_x || !curr_speed_y\n");
					return;
				}
				float64 cs_x = *curr_speed_x;
				float64 cs_y = *curr_speed_y;
				float64 fix = _dt * m_dpc->client_fps;
				m_pos.y -= fix * cs_y;
				m_pos.z -= fix * cs_y;
			}
			opinion_motion_direction();
		}
			//! PC_MOVE,RUN,JUMP's sub state
			ORZ_PLUS_INLINE void								mrjss_right_down_handler		(state_trigger& _state_mgr, float64 _dt)
			{
				if (m_collision_type == ct_null)
				{
					float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
					float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
					if (!curr_speed_x || !curr_speed_y)
					{
						//printf("pid: [" << m_pid << "] - " << "mrjss_right_down_handler - !curr_speed_x || !curr_speed_y\n");
						return;
					}
					float64 cs_x = *curr_speed_x;
					float64 cs_y = *curr_speed_y;
					float64 fix = _dt * m_dpc->client_fps;
					m_pos.x += fix * cs_x;
					m_pos.y += fix * cs_y;
					m_pos.z += fix * cs_y;
				}
				opinion_motion_direction();
			}
		//! PC_MOVE,RUN,JUMP's sub state
		ORZ_PLUS_INLINE void								mrjss_down_handler			(state_trigger& _state_mgr, float64 _dt)
		{
			if (m_collision_type == ct_null)
			{
				float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
				float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
				if (!curr_speed_x || !curr_speed_y)
				{
					//printf("pid: [" << m_pid << "] - " << "mrjss_down_handler - !curr_speed_x || !curr_speed_y\n");
					return;
				}
				float64 cs_x = *curr_speed_x;
				float64 cs_y = *curr_speed_y;
				float64 fix = _dt * m_dpc->client_fps;
				m_pos.y += fix * cs_y;
				m_pos.z += fix * cs_y;
			}
			opinion_motion_direction();
		}
			//! PC_MOVE,RUN,JUMP's sub state
			ORZ_PLUS_INLINE void								mrjss_left_down_handler		(state_trigger& _state_mgr, float64 _dt)
			{
				if (m_collision_type == ct_null)
				{
					float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
					float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
					if (!curr_speed_x || !curr_speed_y)
					{
						//printf("pid: [" << m_pid << "] - " << "mrjss_left_down_handler - !curr_speed_x || !curr_speed_y\n");
						return;
					}
					float64 cs_x = *curr_speed_x;
					float64 cs_y = *curr_speed_y;
					float64 fix = _dt * m_dpc->client_fps;
					m_pos.x -= fix * cs_x;
					m_pos.y += fix * cs_y;
					m_pos.z += fix * cs_y;
				}
				opinion_motion_direction();
			}
		//! mss end
		ORZ_PLUS_INLINE void								mrjss_end_handler				(state_id _stid, state_trigger& _state_mgr, float64 _dt)
		{
			////printf("pid: [" << m_pid << "] - " << "mss end!\n");
			_state_mgr.stop();
		}
	//------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------
	//! PC_ATTACK_1
	ORZ_PLUS_INLINE void								attack1_state_handler		(state_trigger& _state_mgr, float64 _dt)
	{
		// #1 deal with attack1

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(PC_ATTACK_1);
		if (pi.get_input() != IT_NULL)
		{
			if (!m_next_act.is_valid())
			{
				skill_type skt = m_skill_mgr.check_skill(m_skill_mgr.match_input(m_dir, true, m_input_queue.get_inputs(g_min, KS_NULL)));
				switch (skt)
				{
				case SKT_1:		// J
					{
						m_next_act.set(AT_NORMAL_ATTACK, AS_2);
						broadcast_action_msg(AT_NORMAL_ATTACK, m_dir ? DIRT_RIGHT : DIRT_LEFT, AS_1, AS_2, true);
						return;
					}break;
				}
			}
		}
	}
	//------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------
	//! PC_RUN
	ORZ_PLUS_INLINE void								run_state_handler			(state_trigger& _state_mgr, float64 _dt)
	{
		if (m_collision_type == ct_null)
			m_collision_type = check_collision(check_motion_direction());
		else if (m_has_new_input)
			m_collision_type = check_collision(check_motion_direction());

		// #1 deal with move
		if (!m_mrj_sub_trigger.is_start())
		{
			static_any<SMAS>* cs_x = _state_mgr.get_cs_lcd(0);
			static_any<SMAS>* cs_y = _state_mgr.get_cs_lcd(1);
			if (!cs_x || !cs_y)
			{
				//printf("pid: [" << m_pid << "] - " << "	run_state_handler - ERROR! !cs_x || !cs_y\n");
				m_mrj_sub_trigger.start();
			}
			else
				m_mrj_sub_trigger.start(*cs_x, *cs_y);
		}
		
		m_mrj_sub_trigger.run(_dt);

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(PC_RUN);
		if (pi.get_input() != IT_NULL)
		{
			// check input. 按键序列多的优先级大于少的
			motion_type mt = m_motion_mgr.check_motion(m_motion_mgr.match_input(m_dir, false, m_input_queue.get_inputs(g_min, KS_NULL)));
			if (mt != MT_NULL)
			{
				//switch (mt)
				//{
				//case MT_9:
				//	{
				//		_state_mgr.change_state((state_id)PC_JUMP, static_any<SMAS>(m_dir?DIRT_RIGHT:DIRT_LEFT));
				//		return;
				//	}break;
				//}
			}
			// attack
			// we use skill manager  -to-  deal with attack and skills
			skill_type skt = m_skill_mgr.check_skill(m_skill_mgr.match_input(m_dir, true, m_input_queue.get_inputs(m_dpc->skill_act_time, KS_PUSHED)));
			switch (skt)
			{
			case SKT_1:		// J
				{
					_state_mgr.change_state((state_id)PC_SKILL, static_any<SMAS>(SKT_13));
					return;
				}break;
			case SKT_13:		// →↓→ + J
				{
					m_input_queue.clear();
					_state_mgr.change_state((state_id)PC_SKILL, static_any<SMAS>(SKT_13));
					return;
				}break;
			}
		}
		// second check keys state 
		if (m_key_mgr.get_key_state_by_kv(KV_RIGHT) != KS_PUSHED &&
			m_key_mgr.get_key_state_by_kv(KV_LEFT) != KS_PUSHED &&
			m_key_mgr.get_key_state_by_kv(KV_UP) != KS_PUSHED &&
			m_key_mgr.get_key_state_by_kv(KV_DOWN) != KS_PUSHED )
		{
			pc_state cs = (pc_state)_state_mgr.get_current_state();
			static_any<SMAS> sa(cs);
			_state_mgr.change_state(PC_IDLE, sa);
		}
	}
	//------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------
	//! PC_JUMP
	ORZ_PLUS_INLINE void								jump_state_handler			(state_trigger& _state_mgr, float64 _dt)
	{
		if (m_collision_type == ct_null)
			m_collision_type = check_collision(check_motion_direction());
		else if (m_has_new_input)
			m_collision_type = check_collision(check_motion_direction());

		// #1 deal with jump
		if (!m_mrj_sub_trigger.is_start())
		{
			static_any<SMAS>* cs_x = _state_mgr.get_cs_lcd(0);
			static_any<SMAS>* cs_y = _state_mgr.get_cs_lcd(1);
			if (!cs_x || !cs_y)
			{
				//printf("pid: [" << m_pid << "] - " << "	jump_state_handler - ERROR! !cs_x || !cs_y\n");
				m_mrj_sub_trigger.start();
			}
			else
				m_mrj_sub_trigger.start(*cs_x, *cs_y);
		}
		
		m_mrj_sub_trigger.run(_dt);

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(PC_RUN);
		if (pi.get_input() != IT_NULL)
		{
		}

		// second check keys state 

	}
	//------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------
	//! PC_ATTACK_2
	ORZ_PLUS_INLINE void								attack2_state_handler		(state_trigger& _state_mgr, float64 _dt)
	{
		// #1 deal with attack2

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(PC_ATTACK_2);
		if (pi.get_input() != IT_NULL)
		{
			if (!m_next_act.is_valid())
			{
				skill_type skt = m_skill_mgr.check_skill(m_skill_mgr.match_input(m_dir, true, m_input_queue.get_inputs(g_min, KS_NULL)));
				switch (skt)
				{
				case SKT_1:		// J
					{
						////printf("pid: [" << m_pid << "] - " << "recv go on attack msg! will go  -to-  attack 3!\n");
						m_next_act.set(AT_NORMAL_ATTACK, AS_3);
						broadcast_action_msg(AT_NORMAL_ATTACK, m_dir ? DIRT_RIGHT : DIRT_LEFT, AS_2, AS_3, true);
						return;
					}break;
				}
			}
		}

		// second check keys state 

	}
	//------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------
	//! PC_ATTACK_3
	ORZ_PLUS_INLINE void								attack3_state_handler		(state_trigger& _state_mgr, float64 _dt)
	{	
		// #1 deal with attack3

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(PC_ATTACK_2);
		if (pi.get_input() != IT_NULL)
		{
		}

		// second check keys state 

	}
	//------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------
	//! PC_SKILL
	ORZ_PLUS_INLINE void								skill_state_handler			(state_trigger& _state_mgr, float64 _dt)
	{
		// #1 deal with skills
		m_skill_mgr.run_skill();

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(PC_SKILL);
		if (pi.get_input() != IT_NULL)
		{
		}

		// second check keys state 
	}
	//------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------
	//! PC_FALL
	ORZ_PLUS_INLINE void								fall_state_handler			(state_trigger& _state_mgr, float64 _dt)
	{
		// #1 deal with others

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(PC_FALL);
		if (pi.get_input() != IT_NULL)
		{
		}

		// second check keys state 

	}
	//------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------
	//! PC_RISE_UP
	ORZ_PLUS_INLINE void								rise_up_state_handler		(state_trigger& _state_mgr, float64 _dt)
	{
		// #1 deal with others

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(PC_RISE_UP);
		if (pi.get_input() != IT_NULL)
		{
		}

		// second check keys state 

	}
	//------------------------------------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------------------------------
	// 以下是工具函数
	//!
	ORZ_PLUS_INLINE collision_type					check_collision				(direction_type _dt)
	{
		m_col_box.update_raw_box(pos2d(m_pos.x, m_pos.z));
		if (!m_dm->check_raw_box(m_col_box.get_curr_raw_box()))
		{
			////printf("\n	check_collision ! pc state: " << m_state_mgr.get_current_state() << std::endl;
			return ct_null;
		}

		// 取得injure_zone
		const zone* iz = get_current_iz();
		if (!iz)
		{
			//printf("\n	check_collision ! !iz pc state: " << m_state_mgr.get_current_state() << std::endl;
			return ct_null;
		}

		zone zn = iz->convert_by_dir(m_dir, (int)m_pos.x, (int)m_pos.z);

		// 构建四边形碰撞盒，以人物位置为中心 
		zn.h = (unsigned int)((float64)zn.w * m_dpc->min_move_distance / m_dpc->move_speed);
		zn.y = (int)(m_pos.z - zn.h/2.0f);
		zn.x = (int)(m_pos.x - zn.w/2.0f);
		// 构建八边形

		// #1 上边，p8, p1
		pos2d p8((float64)zn.x + (float64)zn.w/3.0f, (float64)zn.y);
		pos2d p1((float64)zn.x + (float64)zn.w/3.0f*2.0f, (float64)zn.y);
		// #2 右，p2, p3
		pos2d p2((float64)zn.x + (float64)zn.w, (float64)zn.y + (float64)zn.h/3.0f);
		pos2d p3((float64)zn.x + (float64)zn.w, (float64)zn.y + (float64)zn.h/3.0f*2.0f);
		// #3 下，p4, p5
		pos2d p4((float64)zn.x + (float64)zn.w/3.0f, (float64)zn.y + (float64)zn.h);
		pos2d p5((float64)zn.x + (float64)zn.w/3.0f*2.0f, (float64)zn.y + (float64)zn.h);
		// #4 左，p6, p7
		pos2d p6((float64)zn.x, (float64)zn.y + (float64)zn.h/3.0f*2.0f);
		pos2d p7((float64)zn.x, (float64)zn.y + (float64)zn.h/3.0f);
		//
		m_col_box.set_bound(p1, p2, p3, p4, p5, p6, p7, p8);

		collision_type ct = m_dm->check_collision(_dt, m_col_box);
		state_id sid = m_state_mgr.get_current_state();
		
		return ct;
	}

	//! send event  -to-  game
	ORZ_PLUS_INLINE void								push_attack					(const zone& _zone, bool _dir, const pos3d& _pos, unsigned int _atk_y, const attack& _atk)
	{
		std::vector<static_any<EMAS> > datas;
		datas.push_back(static_any<EMAS>(_zone, sizeof(zone)));
		datas.push_back(static_any<EMAS>(_dir, sizeof(bool)));
		datas.push_back(static_any<EMAS>(_pos, sizeof(pos3d)));
		datas.push_back(static_any<EMAS>(_atk_y, sizeof(unsigned int)));
		datas.push_back(static_any<EMAS>(_atk, sizeof(attack)));
		m_event_mgr->push_event(m_eoi, (event_id)et_pc_attack, datas);
	}

	//!
	ORZ_PLUS_INLINE void								push_new_attack				()
	{
		std::vector<static_any<EMAS> > datas;
		datas.push_back(static_any<EMAS>(m_eoi, sizeof(event_obj_id)));
		m_event_mgr->push_event(m_eoi, (event_id)et_pc_new_attack, datas);
	}

	//! broadcast 伤害+直线运动
	ORZ_PLUS_INLINE void								broadcast_dmg_and_bm_msg	(damage& _dmg, float64 _v0, float64 _a, int _max_dis)
	{
		IPacketShrPtr pk = allocPacket();
		pk->setMsg(M_DAMAGE);
		//pk->put("type=", 5);
		NumStr tp("1");
		pk->put(tp.c_str(), tp.size());
		pk->put("#", 1);
		//pk->put("playerid=", 9);
		NumStr pid(d2s(m_aid));
		pk->put(pid.c_str(), pid.size());
		pk->put("|", 1);
		//pk->put("direct=", 7);
		NumStr dt(d2s(_dmg.dir()));
		pk->put(dt.c_str(), dt.size());
		pk->put("|", 1);
		//pk->put("dmgtype=", 8);
		NumStr dmt(d2s(_dmg.type()));
		pk->put(dmt.c_str(), dmt.size());

		// mt
		pk->put("|", 1);
		pk->put("1", 1);

		// v0
		pk->put("&", 1);
		//pk->put("v0=", 3);
		NumStr v0(f2s(_v0));
		pk->put(v0.c_str(), v0.size());

		// a
		pk->put(",", 1);
		//pk->put("a=", 2);
		NumStr a(f2s(_a));
		pk->put(a.c_str(), a.size());

		// max dis
		pk->put(",", 1);
		//pk->put("maxdis=", 7);
		NumStr maxdis(d2s(_max_dis));
		pk->put(maxdis.c_str(), maxdis.size());

		m_game_proxy->broadcast(pk);
	}

	//! broadcast 伤害+抛物线运动
	ORZ_PLUS_INLINE void								broadcast_dmg_and_pda_msg	(damage& _dmg, float64 _v0, float64 _spx, float64 _spy)
	{
		IPacketShrPtr pk = allocPacket();
		pk->setMsg(M_DAMAGE);
		//pk->put("type=", 5);
		NumStr tp("1");
		pk->put(tp.c_str(), tp.size());
		pk->put("#", 1);
		//pk->put("playerid=", 9);
		NumStr pid(d2s(m_aid));
		pk->put(pid.c_str(), pid.size());
		pk->put("|", 1);
		//pk->put("direct=", 7);
		NumStr dt(d2s(_dmg.dir()));
		pk->put(dt.c_str(), dt.size());
		pk->put("|", 1);
		//pk->put("dmgtype=", 8);
		NumStr dmt(d2s(_dmg.type()));
		pk->put(dmt.c_str(), dmt.size());

		// mt
		pk->put("|", 1);
		pk->put("2", 1);

		// v0
		pk->put("&", 1);
		//pk->put("v0=", 3);
		NumStr v0(f2s(_v0));
		pk->put(v0.c_str(), v0.size());

		// spx
		pk->put(",", 1);
		//pk->put("spx=", 4);
		NumStr spx(f2s(_spx));
		pk->put(spx.c_str(), spx.size());

		// spy
		pk->put(",", 1);
		//pk->put("spy=", 4);
		NumStr spy(f2s(_spy));
		pk->put(spy.c_str(), spy.size());

		m_game_proxy->broadcast(pk);
	}

	//! broadcast move event
	ORZ_PLUS_INLINE void								broadcast_pos_msg			()
	{
		IPacketShrPtr pk = allocPacket();
		pk->setMsg(M_POS);
		//pk->put("type=", 5);
		NumStr tp("1");
		pk->put(tp.c_str(), tp.size());
		pk->put("#", 1);
		//pk->put("playerid=", 9);
		NumStr pid(d2s(m_aid));
		pk->put(pid.c_str(), pid.size());
		pk->put("|", 1);
		//pk->put("direct=", 7);
		NumStr dt(d2s(m_dir?1:2));
		pk->put(dt.c_str(), dt.size());
		pk->put("&", 1);
		//pk->put("posx=", 5);
		NumStr posx(d2s((int)m_pos.x));
		pk->put(posx.c_str(), posx.size());
		pk->put("&", 1);
		//pk->put("posy=", 5);
		NumStr posy(d2s((int)m_pos.z));
		pk->put(posy.c_str(), posy.size());

		//printf("pid: [" << m_pid << "] - " << "broadcast_pos_msg: m_pos.x: " << m_pos.x << ", m_pos.y: " << m_pos.y << ", m_pos.z: " << m_pos.z << std::endl;

		m_game_proxy->broadcast(pk, &m_sn);
	}

	//! broadcast move event
	ORZ_PLUS_INLINE void								broadcast_move_msg			(direction_type _dt, bool _state)
	{
		IPacketShrPtr pk = allocPacket();
		pk->setMsg(M_MOVE);
		//pk->put("type=", 5);
		NumStr tp("1");
		pk->put(tp.c_str(), tp.size());
		pk->put("#", 1);
		//pk->put("playerid=", 9);
		NumStr pid(d2s(m_aid));
		pk->put(pid.c_str(), pid.size());
		pk->put("|", 1);
		//pk->put("direct=", 7);
		NumStr dt(d2s(_dt));
		pk->put(dt.c_str(), dt.size());
		pk->put("&", 1);
		//pk->put("posx=", 5);
		NumStr posx(d2s((int)m_pos.x));
		pk->put(posx.c_str(), posx.size());
		pk->put("&", 1);
		//pk->put("posy=", 5);
		NumStr posy(d2s((int)m_pos.y));
		pk->put(posy.c_str(), posy.size());
		pk->put("&", 1);
		//pk->put("state=", 6);
		NumStr state(d2s(_state ? 0 : 1));
		pk->put(state.c_str(), state.size());

		m_game_proxy->broadcast(pk, &m_sn);
	}

	//! broadcast run event
	ORZ_PLUS_INLINE void								broadcast_run_msg			(direction_type _dt, bool _state)
	{
		IPacketShrPtr pk = allocPacket();
		pk->setMsg(M_RUN);
		//pk->put("type=", 5);
		std::string tp("1");
		pk->put(tp.c_str(), tp.size());
		pk->put("#", 1);
		//pk->put("playerid=", 9);
		NumStr pid(d2s(m_aid));
		pk->put(pid.c_str(), pid.size());
		pk->put("|", 1);
		//pk->put("direct=", 7);
		NumStr dt(d2s(_dt));
		pk->put(dt.c_str(), dt.size());
		pk->put("&", 1);
		//pk->put("posx=", 5);
		NumStr posx(d2s((int)m_pos.x));
		pk->put(posx.c_str(), posx.size());
		pk->put("&", 1);
		//pk->put("posy=", 5);
		NumStr posy(d2s((int)m_pos.y));
		pk->put(posy.c_str(), posy.size());
		pk->put("&", 1);
		//pk->put("state=", 6);
		NumStr state(d2s(_state ? 0 : 1));
		pk->put(state.c_str(), state.size());

		m_game_proxy->broadcast(pk, &m_sn);
	}

	//! broadcast jump event
	ORZ_PLUS_INLINE void								broadcast_jump_msg			(direction_type _dt, bool _state)
	{
		IPacketShrPtr pk = allocPacket();
		pk->setMsg(M_JUMP);
		//pk->put("type=", 5);
		NumStr tp("1");
		pk->put(tp.c_str(), tp.size());
		pk->put("#", 1);
		//pk->put("playerid=", 9);
		NumStr pid(d2s(m_aid));
		pk->put(pid.c_str(), pid.size());
		pk->put("|", 1);
		//pk->put("direct=", 7);
		NumStr dt(d2s(_dt));
		pk->put(dt.c_str(), dt.size());
		pk->put("&", 1);
		//pk->put("posx=", 5);
		NumStr posx(d2s((int)m_pos.x));
		pk->put(posx.c_str(), posx.size());
		pk->put("&", 1);
		//pk->put("posy=", 5);
		NumStr posy(d2s((int)m_pos.y));
		pk->put(posy.c_str(), posy.size());
		pk->put("&", 1);
		//pk->put("state=", 6);
		NumStr state(d2s(_state ? 0 : 1));
		pk->put(state.c_str(), state.size());

		m_game_proxy->broadcast(pk, &m_sn);
	}

	//! broadcast skill event
	ORZ_PLUS_INLINE void								broadcast_skill_msg			(skill_type _st, direction_type _dt)
	{
		IPacketShrPtr pk = allocPacket();
		pk->setMsg(M_SKILL);
		//pk->put("type=", 5);
		NumStr tp("1");
		pk->put(tp.c_str(), tp.size());
		pk->put("#", 1);
		//pk->put("playerid=", 9);
		NumStr pid(d2s(m_aid));
		pk->put(pid.c_str(), pid.size());
		pk->put("|", 1);
		//pk->put("skilltype=", 10);
		NumStr st(d2s(_st));
		pk->put(st.c_str(), st.size());
		pk->put("&", 1);
		//pk->put("direct=", 7);
		NumStr dt(d2s(_dt));
		pk->put(dt.c_str(), dt.size());
		pk->put("&", 1);
		//pk->put("posx=", 5);
		NumStr posx(d2s((int)m_pos.x));
		pk->put(posx.c_str(), posx.size());
		pk->put("&", 1);
		//pk->put("posy=", 5);
		NumStr posy(d2s((int)m_pos.y));
		pk->put(posy.c_str(), posy.size());

		m_game_proxy->broadcast(pk, &m_sn);
	}

	//! broadcast action event
	ORZ_PLUS_INLINE void								broadcast_action_msg		(action_type _at, direction_type _dt, action_step _cur_step, action_step _next_step, bool _value)
	{
		IPacketShrPtr pk = allocPacket();
		pk->setMsg(M_ACTION);
		//pk->put("playerid=", 9);
		NumStr pid(d2s(m_aid));
		pk->put(pid.c_str(), pid.size());
		pk->put("#", 1);
		//pk->put("actiontype=", 11);
		NumStr action_type(d2s(_at));
		pk->put(action_type.c_str(), action_type.size());
		pk->put("|", 1);
		//pk->put("direct=", 7);
		NumStr dt(d2s(_dt));
		pk->put(dt.c_str(), dt.size());
		pk->put("&", 1);
		//pk->put("curstep=", 8);
		NumStr cur_step(d2s(_cur_step));
		pk->put(cur_step.c_str(), cur_step.size());
		pk->put("&", 1);
		//pk->put("nextstep=", 9);
		NumStr next_step(d2s(_next_step));
		pk->put(next_step.c_str(), next_step.size());
		pk->put(",", 1);
		//pk->put("value=", 6);
		NumStr value(d2s(_value ? 0 : 1));
		pk->put(value.c_str(), value.size());

		m_game_proxy->broadcast(pk, &m_sn);
	}

	//! 检查undo inputs
	ORZ_PLUS_INLINE obj_input						check_undo_inputs			(pc_state _cs)
	{
		obj_input pi;
		if (!m_undo_inputs.empty() && m_has_new_input)
		{
			m_has_new_input = false;
			pi = m_undo_inputs.front();
			m_undo_inputs.pop();
			input_type it = (input_type)pi.get_input();
			m_input_queue.push(pi);
		}
		return pi;
	}

	//! 检查undo inputs
	ORZ_PLUS_INLINE obj_input						check_undo_inputs_no_pop	(pc_state _cs)
	{
		obj_input pi;
		if (!m_undo_inputs.empty() && m_has_new_input)
		{
			pi = m_undo_inputs.front();
			input_type it = (input_type)pi.get_input();
		}
		return pi;
	}

	//! 
	ORZ_PLUS_INLINE direction_type					check_motion_direction		()
	{
		std::vector<key_value> range;
		range.push_back(KV_RIGHT);
		range.push_back(KV_LEFT);
		range.push_back(KV_UP);
		range.push_back(KV_DOWN);

		std::vector<key_value> key_time_order = m_key_mgr.get_order(KS_PUSHED, range);
		direction_type dt = DIRT_NULL;
		mrj_sub_state mss = MRJSS_NULL;
		if (key_time_order.empty())
			return dt;
		else
		{
			if (key_time_order.size() == 1)
				mss = get_mrj_sub_state_by_kv(key_time_order[0]);
			else
				mss = get_mrj_sub_state_by_kv(key_time_order[0], key_time_order[1]);
			
			switch (mss)
			{
			case MRJSS_RIGHT:		dt = DIRT_RIGHT;		break;
			case MRJSS_LEFT:		dt = DIRT_LEFT;		break;
			case MRJSS_UP:			dt = DIRT_UP;			break;
			case MRJSS_DOWN:		dt = DIRT_DOWN;		break;
			case MRJSS_RIGHT_UP:	dt = DIRT_RIGHT_UP;	break;
			case MRJSS_LEFT_UP:		dt = DIRT_LEFT_UP;	break;
			case MRJSS_RIGHT_DOWN:	dt = DIRT_RIGHT_DOWN;	break;
			case MRJSS_LEFT_DOWN:	dt = DIRT_LEFT_DOWN;	break;
			}

			return dt;
		}
	}

	//! 检查输入，判断移动方向 _is_send 是否发送消息通知客户端
	ORZ_PLUS_INLINE void								opinion_motion_direction	(bool _is_send=true)
	{
		std::vector<key_value> range;
		range.push_back(KV_RIGHT);
		range.push_back(KV_LEFT);
		range.push_back(KV_UP);
		range.push_back(KV_DOWN);
		std::vector<key_value> key_time_order = m_key_mgr.get_order(KS_PUSHED, range);
		
		mrj_sub_state mss = MRJSS_NULL;
		if (key_time_order.empty())
		{	
			if (m_state_mgr.get_current_state() == (state_id)PC_JUMP)
				m_mrj_sub_trigger.change_state(MRJSS_JUMP_IDLE);
			else
				m_mrj_sub_trigger.change_to_end();
		}
		else
		{
			if (key_time_order.size() == 1)
				mss = get_mrj_sub_state_by_kv(key_time_order[0]);
			else
				mss = get_mrj_sub_state_by_kv(key_time_order[0], key_time_order[1]);
			
			if (mss != MRJSS_NULL && m_mrj_sub_trigger.get_current_state() != (state_id)mss)
			{
				if (mss == MRJSS_RIGHT || mss == MRJSS_RIGHT_UP || mss == MRJSS_RIGHT_DOWN)
					m_dir = true;
				else if (mss == MRJSS_LEFT || mss == MRJSS_LEFT_UP || mss == MRJSS_LEFT_DOWN)
					m_dir = false;
				if (_is_send)
				{
					if (m_state_mgr.get_current_state() == PC_MOVE)
						broadcast_move_msg(get_direction_by_mrj_ss(mss), true);
					else if (m_state_mgr.get_current_state() == PC_RUN)
						broadcast_run_msg(get_direction_by_mrj_ss(mss), true);
					else if (m_state_mgr.get_current_state() == PC_JUMP)
						broadcast_jump_msg(get_direction_by_mrj_ss(mss), true);
				}
				m_mrj_sub_trigger.change_state(mss);
			}
		}
	}


	//! 根据给定的二位数组的indexs 返回对应的ckf
	ORZ_PLUS_INLINE const cfg_key_frame*				get_ckf						(unsigned int _i1, unsigned _i2)
	{
		if (_i1 >= m_dpc->cfg_key_frames.size() || _i2 >= m_dpc->cfg_key_frames[_i1].size())
			return 0;

		return &m_dpc->cfg_key_frames[_i1][_i2];
	}

	//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
private:	// private's struct
	//! 连续动作
	struct sequence_action
	{
		sequence_action()
			: at(AT_NULL)
			, as(AS_NULL)
		{
		}
		sequence_action(action_type _at, action_step _as)
			: at(_at)
			, as(_as)
		{
		}

		action_type get_type()
		{
			return at;
		}

		action_step get_step()
		{
			return as;
		}

		void set(action_type _at, action_step _as)
		{
			at = _at;
			as = _as;
		}

		void clear()
		{
			at = AT_NULL;
			as = AS_NULL;
		}

		bool is_valid()
		{
			if (at == AT_NULL)
				return false;
			else 
				return true;
		}

		action_type			at;
		action_step			as;
	};

//------------------------------------------------------------------------------------------------------------
private:	// member attrs
	ServiceManager*													m_svcMgr;
	event_obj_id													m_eoi;
	player_id														m_pid;
	account_id														m_aid;
	SessionPxy														m_sn;
	std::string														m_name;
	std::string														m_nick_name;

	//! 人物朝向 true 朝右，反之朝左
	bool															m_dir;

	//! 移动的时候当前速度x轴方向（走、跑、跳跃是不同的）
	float64															m_current_speed_x;

	//! 移动的时候当前速度y轴方向（走、跑、跳跃是不同的）
	float64															m_current_speed_y;

	//! 位置
	pos3d															m_pos;
	// 下一frame位置，注意：并非每frame都计算它，而是只有检测碰撞的时候计算
	pos3d															m_next_pos;

	//!
	//Plat*															m_plat;
	demo_map*														m_dm;

	// 碰撞盒
	collision_box													m_col_box;
	//! 碰撞类型 0 无碰撞 1 地图阻挡
	int																m_collision;

	//!
	game_proxy*														m_game_proxy;

	// state manager
	state_trigger													m_state_mgr;

	// move状态中的子状态机	切换状态的原则：每个状态机之负责切换属于自身的状态，例如子状态机不会处理父状态机的状态切换事务，遇到这样的输入，则忽略不处理
	state_trigger													m_mrj_sub_trigger;

	//! 是否有新输入进来
	bool															m_has_new_input;

	//!
	collision_type													m_collision_type;

	//! key manager
	key_manager														m_key_mgr;

	//!
	skill_manager													m_skill_mgr;

	//!
	motion_manager													m_motion_mgr;

	//!
	action_manager													m_fs_action_mgr;

	//!
	action_manager													m_jump_action_mgr;

	//!
	event_manager*													m_event_mgr;

	//! action_frame_sequence manager 
	action_frame_sequence_manager									m_afs_mgr;
	std::vector<action_frame_sequence>								m_afss;

	//!
	obj_input_queue													m_input_queue;
	//! 当前未处理的输入事件
	std::queue<obj_input>											m_undo_inputs;

	//! 保存的下一个阶段的连续动作
	sequence_action													m_next_act;

	// config
	demo_player_config*												m_dpc;

	// test 
	float64															m_print_dt;
	ClockTime														m_current_time;
};

typedef boost::shared_ptr<demo_player> demo_player_ptr;

}

#endif
