#ifndef DEMO_MONSTER_H_
#define DEMO_MONSTER_H_

#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <common/game/pos3d.h>
#include <common/game/monster.h>
#include <common/net_msg.h>
#include <common/game/state_trigger.h>
#include <game_server/game_proxy.h>
#include <packet/packet_seer.h>
#include <game_server/obj_input_queue.h>
#include <game_server/skill_manager.h>
#include <game_server/motion_manager.h>
#include <common/event_manager.h>
#include <game_server/action_frame_sequence.h>
#include <game_server/config.h>
#include <game_server/demo_map.h>


namespace Orz
{

class demo_monster : public monster
{
public:
	demo_monster(ServiceManager* _svcMgr, event_manager& _event_mgr, game_proxy& _game_proxy, monster_id _msrid, const char* _pName, bool _dir, demo_map& _dm, /*Plat& _plat,*/ 
				 pos2d _pos, demo_monster_config& _dmc)
		: m_svcMgr(_svcMgr)
		, m_game_proxy(&_game_proxy)
		, m_id(_msrid)
		, m_eoi(event_obj_id((obj_type)OT_MSR, (obj_id)_msrid))
		, m_name(_pName)
		//, m_plat(&_plat)
		, m_dm(&_dm)
		, m_pos(pos3d(_pos.x, _pos.y, _pos.y))
		, m_next_pos(pos3d(_pos.x, _pos.y, _pos.y))
		, m_dmc(&_dmc)
		, m_col_box(collision_box(zone(0, 0, _dmc.raw_collision_box.w, _dmc.raw_collision_box.h)))
		, m_current_speed_x(0.0f)
		, m_current_speed_y(0.0f)
		, m_dir(_dir)
		, m_state_mgr(boost::bind(&demo_monster::begin_state_handler, this, _1, _2, _3), 
					  boost::bind(&demo_monster::change_state_handler, this, _1, _2, _3, _4), 
					  boost::bind(&demo_monster::cr_handler, this, _1, _2, _3),
					  boost::bind(&demo_monster::end_state_handler, this, _1, _2, _3))
		, m_mrj_sub_trigger(boost::bind(&demo_monster::mrjss_begin_handler, this, _1, _2, _3), 
							boost::bind(&demo_monster::change_sub_state_handler, this, _1, _2, _3, _4), 
							boost::bind(&demo_monster::mrjss_cr_handler, this, _1, _2, _3),
							boost::bind(&demo_monster::mrjss_end_handler, this, _1, _2, _3))
		, m_skill_mgr(boost::bind(&demo_monster::check_skill_usable, this, _1, _2))
		, m_motion_mgr(boost::bind(&demo_monster::check_motion_usable, this, _1, _2))
		, m_event_mgr(&_event_mgr)
		, m_input_queue(obj_input_queue(1.5f, m_key_mgr))
		, m_has_new_input(false)
		, m_collision_type(ct_null)
		, m_print_dt(0.0f)
	{
		// create event obj
		m_event_mgr->create_event_obj(m_eoi, boost::bind(&demo_monster::recv_event_handler, this, _1, _2, _3, _4, _5));

		// init states
		std::vector<state> states;
		for (int i=(int)MSRS_NULL; i<(int)MSRS_TOTAL; ++i)
		{
			if (i == (int)MSRS_MOVE)
				states.push_back(state((state_id)i, static_any<SMAS>(m_dmc->move_speed), static_any<SMAS>(m_dmc->min_move_distance), 
										boost::bind(&demo_monster::state_handler, this, _1, _2, _3)));
			else
				states.push_back(state((state_id)i, boost::bind(&demo_monster::state_handler, this, _1, _2, _3)));
		}
		m_state_mgr.init(states);
		m_state_mgr.start();

		// init move state's sub states
		states.clear();
		for (int i=(int)MRJSS_NULL; i<(int)MRJSS_TOTAL; ++i)
		{
			states.push_back(state((state_id)i, boost::bind(&demo_monster::mrjss_state_handler, this, _1, _2, _3)));
		}
		m_mrj_sub_trigger.init(states);

		// init keys
		std::vector<key> keys;
		boost::function<void (key_value, key_manager&, key_state, input_type)> key_hdr = boost::bind(&demo_monster::key_handler, this, _1, _2, _3, _4);
		keys.push_back(key(IT_PUSH_RIGHT,	IT_RELEASE_RIGHT,	KV_RIGHT,	key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_LEFT,	IT_RELEASE_LEFT,	KV_LEFT,	key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_UP,		IT_RELEASE_UP,		KV_UP,		key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_DOWN,	IT_RELEASE_DOWN,	KV_DOWN,	key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_ATTACK,	IT_RELEASE_ATTACK,	KV_ATTACK,	key_hdr, key_hdr, key_hdr, key_hdr));
		m_key_mgr.init(keys);

		// init skills
		std::vector<skill> skills;
		boost::function<void (skill_type, skill_manager&)> begin_skill_hdr = boost::bind(&demo_monster::begin_skill_handler, this, _1, _2);
		boost::function<void (skill_type, skill_manager&)> run_skill_hdr = boost::bind(&demo_monster::run_skill_handler, this, _1, _2);
		boost::function<void (skill_type, skill_manager&)> end_skill_hdr = boost::bind(&demo_monster::end_skill_handler, this, _1, _2);

		skills.push_back(skill(SKT_10));
		skills.push_back(skill(SKT_13));
		skills.push_back(skill(SKT_1,	begin_skill_hdr, run_skill_hdr, end_skill_hdr, SIT_PUSH_ATTACK));
		m_skill_mgr.init(skills);

		// init motions
		std::vector<motion> motions;
		boost::function<void (motion_type, motion_manager&)> begin_motion_hdr = boost::bind(&demo_monster::begin_motion_handler, this, _1, _2);
		boost::function<void (motion_type, motion_manager&)> run_motion_hdr = boost::bind(&demo_monster::run_motion_handler, this, _1, _2);
		boost::function<void (motion_type, motion_manager&)> end_motion_hdr = boost::bind(&demo_monster::end_motion_handler, this, _1, _2);

		motions.push_back(motion(MT_1,	begin_motion_hdr, run_motion_hdr, end_motion_hdr, MIT_PUSH_UP));
		motions.push_back(motion(MT_2,	begin_motion_hdr, run_motion_hdr, end_motion_hdr, MIT_PUSH_RIGHT));
		motions.push_back(motion(MT_3,	begin_motion_hdr, run_motion_hdr, end_motion_hdr, MIT_PUSH_DOWN));
		motions.push_back(motion(MT_4,	begin_motion_hdr, run_motion_hdr, end_motion_hdr, MIT_PUSH_LEFT));
		m_motion_mgr.init(motions);

		// init afs
		m_afss.push_back(action_frame_sequence(m_afs_mgr, (afs_id)AFSMSRT_IDLE,	
											 boost::bind(&demo_monster::idle_fs_start_handler, this, _1, _2), 
											 boost::bind(&demo_monster::idle_afs_change_fs_handler, this, _1, _2, _3), 								 
											 boost::bind(&demo_monster::idle_fs_loop_handler, this, _1, _2, _3, _4, _5, _6), 
											 boost::bind(&demo_monster::idle_fs_finish_handler, this, _1, _2, _3, _4, _5, _6)));

		m_afss.push_back(action_frame_sequence(m_afs_mgr, (afs_id)AFSMSRT_MOVE,	 
											 boost::bind(&demo_monster::move_fs_start_handler, this, _1, _2),
											 boost::bind(&demo_monster::move_afs_change_fs_handler, this, _1, _2, _3),
											 boost::bind(&demo_monster::move_fs_loop_handler, this, _1, _2, _3, _4, _5, _6), 
											 boost::bind(&demo_monster::move_fs_finish_handler, this, _1, _2, _3, _4, _5, _6)));

		m_afss.push_back(action_frame_sequence(m_afs_mgr, (afs_id)AFSMSRT_ATTACK, 
											 boost::bind(&demo_monster::attack_fs_start_handler, this, _1, _2), 
											 boost::bind(&demo_monster::attack_afs_change_fs_handler, this, _1, _2, _3), 
											 boost::bind(&demo_monster::attack_fs_loop_handler, this, _1, _2, _3, _4, _5, _6), 
											 boost::bind(&demo_monster::attack_fs_finish_handler, this, _1, _2, _3, _4, _5, _6)));

		m_afss.push_back(action_frame_sequence(m_afs_mgr, (afs_id)AFSMSRT_FALL, 
											 boost::bind(&demo_monster::fall_fs_start_handler, this, _1, _2), 
											 boost::bind(&demo_monster::fall_afs_change_fs_handler, this, _1, _2, _3), 
											 boost::bind(&demo_monster::fall_fs_loop_handler, this, _1, _2, _3, _4, _5, _6), 
											 boost::bind(&demo_monster::fall_fs_finish_handler, this, _1, _2, _3, _4, _5, _6)));

		m_afss.push_back(action_frame_sequence(m_afs_mgr, (afs_id)AFSMSRT_BIRTH, 
											 boost::bind(&demo_monster::birth_fs_start_handler, this, _1, _2), 
											 boost::bind(&demo_monster::birth_afs_change_fs_handler, this, _1, _2, _3), 
											 boost::bind(&demo_monster::birth_fs_loop_handler, this, _1, _2, _3, _4, _5, _6), 
											 boost::bind(&demo_monster::birth_fs_finish_handler, this, _1, _2, _3, _4, _5, _6)));

		std::vector<std::vector<cfg_key_frame> >::const_iterator cfg_itr(m_dmc->cfg_key_frames.begin());
		std::string last_label;
		for (unsigned i=0; cfg_itr!=m_dmc->cfg_key_frames.end(); ++i, ++cfg_itr)
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
		cfg_itr = m_dmc->cfg_key_frames.begin();
		for (unsigned i=0; cfg_itr!=m_dmc->cfg_key_frames.end(); ++i, ++cfg_itr)
		{
			const std::vector<cfg_key_frame>& ckfs = *cfg_itr;
			std::vector<cfg_key_frame>::const_iterator itr(ckfs.begin());
			for (; itr!=ckfs.end(); ++itr)
			{
				const cfg_key_frame& ckf = *itr;
				m_afss[i].create_action_frame(ckf.label, array2d(ckf.dx, ckf.dy), ckf.frame_times, m_dmc->frame_time, 
											ckf.has_iz==0 ? false:true, zone(ckf.injure_zone.x, ckf.injure_zone.y, ckf.injure_zone.w, ckf.injure_zone.h), 
											ckf.has_az==0 ? false:true, zone(ckf.attack_zone.x, ckf.attack_zone.y, ckf.attack_zone.w, ckf.attack_zone.h), ckf.attack_y);
			}
		}
		m_afs_mgr.init(&m_afss);

		// init action_manager
		m_fs_action_mgr.create_action((action_id)amt_beeline_move, boost::bind(&demo_monster::beeline_move_handler, this, _1, _2, _3, _4, _5));
		m_fs_action_mgr.create_action((action_id)amt_falling, boost::bind(&demo_monster::falling_handler, this, _1, _2, _3, _4, _5));
	}
	virtual ~demo_monster()
	{
	}

public:
	//!
	ORZ_PLUS_INLINE void								recv_event_handler		(const event_obj_id& _sender, event_manager& _event_mgr, const event_obj_id& _recver, event_id _eid, std::vector<static_any<EMAS> >& _datas)
	{
	}

private:
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
		if (m_state_mgr.get_current_state() == (state_id)MSRS_RISE_UP)
			return;

		// attack info:
		//printf("msrid: [" << m_id << "] - " << "attack info -  dmg: " << atk->dmg() << ", type: " << atk->type() << ", dir: " << atk->dir() << std::endl;
		
		m_state_mgr.change_state((state_id)MSRS_FALL);
		m_afs_mgr.start_afs((afs_id)AFSMSRT_FALL, "fall", static_any<FSLCDS>(damage(atk->dmg(), (damage_type)atk->type(), atk->dir())));
	}
	

public:
	//!
	ORZ_PLUS_INLINE monster_id						get_id					() const
	{
		return m_id;
	}

	//!
	ORZ_PLUS_INLINE const event_obj_id&				get_event_obj_id		() const
	{
		return m_eoi;
	}

	//!
	ORZ_PLUS_INLINE const std::string&				get_name				() const
	{
		return m_name;
	}

	//!
	ORZ_PLUS_INLINE int32								run						(float64 _dt)
	{
		m_state_mgr.run();
		m_print_dt += _dt;
		if (m_print_dt > 3.0f)
		{
			m_print_dt = 0.0f;
			std::string face_dir(m_dir ? g_dirs[DIRT_RIGHT] : g_dirs[DIRT_LEFT]);
			//printf("monster(id: " << m_id << ") state - position: [ " << m_pos.x << ", " << m_pos.y 
					  //<< " ], current state: " << g_monster_states[m_state_mgr.get_current_state()] 
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

	ORZ_PLUS_INLINE void								plat_collision			(int _collision)
	{
	}

	ORZ_PLUS_INLINE collision_box&					get_collision_box		()
	{
		return m_col_box;
	}

	ORZ_PLUS_INLINE int32								get_collision			() const
	{
		return 0;
	}

//------------------------------------------------------------------------------------------------------------
private:		// 处理actions
	//! 
	ORZ_PLUS_INLINE void								beeline_move_handler	(action_id _actid, action_manager& _action_mgr, static_any<AMAS>& _sa, float64 _pt, float64 _dt)
	{
		beeline_move* bm = _sa.get_data<beeline_move>();
		if (!bm)
			return;
		if (bm->is_stopped())
		{
			_action_mgr.interrupt((action_id)amt_beeline_move);
			return;
		}
		float64 dx = bm->run();
		if (m_collision_type != ct_null)
		{
			m_pos.x += dx;
		}
		//update_position(dx);
		////printf("msrid: [" << m_id << "] - " << "beeline_move_handler - dx: " << dx << ", m_pos.x: " << m_pos.x << std::endl;
	}

	//! 
	ORZ_PLUS_INLINE void								falling_handler			(action_id _actid, action_manager& _action_mgr, static_any<AMAS>& _sa, float64 _pt, float64 _dt)
	{
		parabola* pba = _sa.get_data<parabola>();
		if (!pba)
			return;
		
		pos2d dpos = pba->run();
		if (m_collision_type != ct_null)
		{
			m_pos.x += dpos.x;
		}
		m_pos.y += dpos.y;
		////printf("msrid: [" << m_id << "] - " << "falling_handler - dpos.x: " << dpos.x << ". dpos.y: " << dpos.y << ", m_pos.x: " << m_pos.x << ", m_pos.y: " << m_pos.y << ", m_pos.z: " << m_pos.z << std::endl;
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
		////printf("msrid: [" << m_id << "] - " << "idle_fs_finish_handler ! is over! _past_time: " << _past_time << ", _is_break: " << _is_break << std::endl;
		if (!_is_break)
		{
			////printf("msrid: [" << m_id << "] - " << "idle_fs_finish_handler - !_is_break\n");
			_afs_mgr.start_afs((afs_id)AFSMSRT_IDLE, "idle");
		}
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
		////printf("msrid: [" << m_id << "] - " << "move_fs_finish_handler ! is over! _past_time: " << _past_time << ", _is_break: " << _is_break <<std::endl;
		if (!_is_break)
			_afs_mgr.start_afs((afs_id)AFSMSRT_MOVE, "move");
	}
	//------------------------------------------------------------------------------------------------------------

	// attack
	//------------------------------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE void								attack_fs_start_handler		(const std::string& _label, action_frame_sequence& _afs)
	{
		//printf("msrid: [" << m_id << "] - " << "\n attack_fs_start_handler: _label: " << _label << std::endl << std::endl;
		push_new_attack();
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
			////printf("msrid: [" << m_id << "] - " << "\n attack_fs_loop_handler - m_collision_type == ct_null\n");
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
			////printf("msrid: [" << m_id << "] - " << "attack_fs_loop_handler - _af.has_attack_zone() - obj type:" << m_eoi.get_obj_type() << ", obj id: " << m_eoi.get_obj_id() << std::endl;
			int dmg = 0;
			attack_type at= at_null;
			if (_af.get_label() == "attack1")
			{
				dmg = m_dmc->attack1_dmg;
				at = at_normal;
			}

			push_attack(_af.get_attack_zone(), m_dir, m_pos, _af.get_attack_y(), attack(dmg, at, m_dir ? DIRT_RIGHT:DIRT_LEFT));
		}
	}
	//!
	ORZ_PLUS_INLINE void								attack_fs_finish_handler	(frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt, bool _is_break)
	{
		if (_is_break)
			return;

		//printf("msrid: [" << m_id << "] - " << "attack over, change to idle state!\n");
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

		//printf("msrid: [" << m_id << "] - " << "fall_fs_start_handler - _label: " << _label << std::endl;

		if (_label == "fall")
		{
			// 检测攻击类型，如果是击飞则切换到fall1；如果不是，则根据击退距离播放fall1动画
			switch (dmg->type())
			{
			case dmgt_normal:		
				{
					// 根据attack的dmg值和自己的修正值设定被击退的最终距离
					int bt = get_beat_back_by_dmg(*dmg);
					// 根据fall持续的时间的一半，计算速度
					const cfg_key_frame* ckf = get_ckf(6, 0);
					if (ckf)
					{
						float64 unit_ft = m_dmc->frame_time / 1000000.0f;
						float64 v0 = dmg->dir() == DIRT_RIGHT ? (bt / (ckf->frame_times * unit_ft / 2.0f)) : -(bt / (ckf->frame_times * unit_ft / 2.0f));
						v0 /= (float64)m_dmc->client_fps;
						//printf("msrid: [" << m_id << "] - " << "fall_fs_start_handler - v0: " << v0 << std::endl;
						m_fs_action_mgr.awake((action_id)amt_beeline_move, static_any<AMAS>(beeline_move(v0, (float64)bt)));
						// send dmg msg
						broadcast_dmg_and_bm_msg(*dmg, v0, 0.0f, bt);
					}
				}break;
			case dmgt_serious:	
			case dmgt_rise:	
				_afs.change_to("fall1", *sa);	break;
			}
		}
		else if (_label == "fall1")
		{
			state_id sid = m_state_mgr.get_last_state();
			if ((pc_state)sid != PC_FALL && (pc_state)sid != PC_JUMP)
			{
				// 根据attack的dmg值和自己的修正值设定抛物线
				parabola pba = get_parabola_by_dmg(*dmg, (float64)m_dmc->g, m_dmc->move_speed/2.0f, 0.0f, m_dmc->client_fps);
				m_fs_action_mgr.awake((action_id)amt_falling, static_any<AMAS>(pba));
				// send dmg msg
				broadcast_dmg_and_pda_msg(*dmg, -pba.get_v0(), dmg->dir()?(m_dmc->move_speed/2.0f):-(m_dmc->move_speed/2.0f), 0.0f);
			}
			else
			{
				// 使用固定v0的空中挨揍的抛物线
				parabola pba = get_parabola_by_dmg(*dmg, (float64)m_dmc->g, m_dmc->move_speed/2.0f, 0.0f, m_dmc->client_fps);
				m_fs_action_mgr.awake((action_id)amt_falling, static_any<AMAS>(parabola(m_dmc->f_max_atk_y, 0.0f, 0.0f, m_dmc->g, m_dmc->client_fps)));
				// send dmg msg
				broadcast_dmg_and_pda_msg(*dmg, -pba.get_v0(), dmg->dir()?(m_dmc->move_speed/2.0f):-(m_dmc->move_speed/2.0f), 0.0f);
			}
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

		//printf("msrid: [" << m_id << "] - " << "fall_fs_loop_handler - label: " << label << std::endl;

		if (label == "fall1")
		{
		}
		else if (label == "fall2")
		{
			// 检测人物坐标是否落地
			if (m_pos.y >= m_pos.z)
			{
				m_pos.y = m_pos.z;
				m_fs_action_mgr.interrupt((action_id)amt_falling);

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

		//printf("msrid: [" << m_id << "] - " << "fall_fs_finish_handler - label: " << label << std::endl;

		if (label == "fall")
		{
			if (_is_break)
				m_fs_action_mgr.interrupt_all();
			m_fs_action_mgr.interrupt((action_id)amt_beeline_move);
			if (!_is_break)
				m_state_mgr.change_state((state_id)PC_IDLE);
		}
		else if (label == "fall1")
		{
			if (_is_break)
				m_fs_action_mgr.interrupt_all();
			_afs.change_to(std::string("fall2"), *sa);
		}
		else if (label == "fall2")
		{
			if (!_is_break)
			{
				// 检测人物坐标是否落地
				if (m_pos.y >= m_pos.z)
				{
					m_pos.y = m_pos.z;
					m_fs_action_mgr.interrupt((action_id)amt_falling);

					_afs.change_to(std::string("fall3"), *sa);
				}
				else
					_afs.change_to(std::string("fall2"), *sa);
			}
		}
		else if (label == "fall3")
		{
			if (_is_break)
				m_fs_action_mgr.interrupt_all();
			m_state_mgr.change_state((state_id)PC_RISE_UP);
			//_afs.change_to(std::string("riseup"), *sa);
		}
	}
	//------------------------------------------------------------------------------------------------------------

	// brith
	//------------------------------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE void								birth_fs_start_handler		(const std::string& _label, action_frame_sequence& _afs)
	{
	}
	//!
	ORZ_PLUS_INLINE void								birth_afs_change_fs_handler	(const std::string& _old, action_frame_sequence& _afs, const std::string& _new)
	{
	}
	//!
	ORZ_PLUS_INLINE void								birth_fs_loop_handler		(const action_frame& _af, frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt)
	{
	}
	//!
	ORZ_PLUS_INLINE void								birth_fs_finish_handler		(frame_segment& _fs, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, float64 _past_time, float64 _dt, bool _is_break)
	{
		////printf("msrid: [" << m_id << "] - " << "move_fs_finish_handler ! is over! _past_time: " << _past_time << ", _is_break: " << _is_break <<std::endl;
		if (!_is_break)
			_afs_mgr.start_afs((afs_id)AFSMSRT_IDLE, "idle");
	}
	//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
private:		// state 's handler
	//! begin state handler
	ORZ_PLUS_INLINE void								begin_state_handler		(state_id _stid, state_trigger& _state_mgr, float64 _dt)
	{
		//printf("monster begin state!\n");
		_state_mgr.change_state((state_id)MSRS_RISE_UP);
		m_afs_mgr.start_afs((afs_id)AFSMSRT_BIRTH, "birth");
	}

	//! state handler
	ORZ_PLUS_INLINE void								state_handler			(state_id _stid, state_trigger& _state_mgr, float64 _dt)
	{
		switch ((int)_stid)
		{
		case MSRS_IDLE:		idle_state_handler(_state_mgr, _dt);	break;
		case MSRS_MOVE:		move_state_handler(_state_mgr, _dt);	break;
		case MSRS_ATTACK_1:	attack1_state_handler(_state_mgr, _dt);	break;
		case MSRS_FALL:		fall_state_handler(_state_mgr, _dt);	break;
		case MSRS_RISE_UP:	rise_up_state_handler(_state_mgr, _dt);	break;
		default:													break;
		}
	}

	//!
	ORZ_PLUS_INLINE void								change_state_handler	(state_id _old, state_trigger& _state_mgr, state_id _new, std::vector<static_any<SMAS> >& _datas)
	{
		if (_old == (state_id)MSRS_IDLE && _new == (state_id)MSRS_MOVE)
		{
			if (_datas.empty())
				return;
			direction_type* pdt = _datas[0].get_data<direction_type>();
			if (!pdt)
				return;

			direction_type dt = *pdt;

			//printf("monster change to move state!\n");
			if (dt != DIRT_NULL)
			{
				broadcast_move_msg(dt, true);
				if (dt == DIRT_RIGHT || dt == DIRT_RIGHT_UP || dt == DIRT_RIGHT_DOWN)
					m_dir = true;
				else if (dt == DIRT_LEFT || dt == DIRT_LEFT_UP || dt == DIRT_LEFT_DOWN)
					m_dir = false;
			}
		}
		else if (_old == (state_id)MSRS_IDLE && _new == (state_id)MSRS_ATTACK_1)
		{
			//printf("monster change to attack 1 state!\n");
			//broadcast_skill_msg(SKT_1, m_dir ? DIRT_RIGHT : DIRT_LEFT);
			//clock_proxy cp = m_server->start_clock(SessionPxy(), boost::bind(&demo_monster::attack1_time_handler, this, _1, _2), data(), m_dmc->attack1_act_time);
			//m_clocks[cp.get_clock_id()] = cp;
		}
		else if (_old == (state_id)MSRS_MOVE && _new == (state_id)MSRS_IDLE)
		{
			broadcast_move_msg(m_dir ? DIRT_RIGHT : DIRT_LEFT, false);
			//printf("monster change to idle state!\n");
			m_mrj_sub_trigger.stop();
		}
		else if (_old == (state_id)MSRS_MOVE && _new == (state_id)MSRS_ATTACK_1)
		{
			//printf("monster change to attack 1 state!\n");
			broadcast_action_msg(AT_NORMAL_ATTACK, m_dir ? DIRT_RIGHT : DIRT_LEFT, AS_1, AS_NULL, true);
			//clock_proxy cp = m_server->start_clock(SessionPxy(), boost::bind(&demo_monster::attack1_time_handler, this, _1, _2), data(), m_dmc->attack1_act_time);
			//m_clocks[cp.get_clock_id()] = cp;
			m_mrj_sub_trigger.stop();
		}
		else if (_old == (state_id)MSRS_ATTACK_1 && _new == (state_id)MSRS_IDLE)
		{
			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSMSRT_IDLE, "idle");
		}
		else if (_old == (state_id)MSRS_FALL && _new == (state_id)MSRS_RISE_UP)
		{
			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSMSRT_IDLE, "birth");
		}
		else if (_old == (state_id)MSRS_FALL && _new == (state_id)MSRS_IDLE)
		{
			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSMSRT_IDLE, "idle");
		}
		else if (_old == (state_id)MSRS_RISE_UP && _new == (state_id)MSRS_IDLE)
		{
			// change afs manager
			m_afs_mgr.start_afs((afs_id)AFSMSRT_IDLE, "idle");
		}
	}

	//! common rum handler
	ORZ_PLUS_INLINE void								cr_handler				(state_id _stid, state_trigger& _state_mgr, float64 _dt)
	{
	}

	//! end state handler
	ORZ_PLUS_INLINE void								end_state_handler		(state_id _stid, state_trigger& _state_mgr, float64 _dt)
	{
		//printf("monster end state! now stop!" << std::endl << std::endl;
		_state_mgr.stop();
	}

//------------------------------------------------------------------------------------------------------------
private:		// key 's handler
	//!
	ORZ_PLUS_INLINE void								key_handler				(key_value _kv, key_manager& _key_mgr, key_state _ks, input_type _it)
	{
		if (_ks == KS_PUSHED || _ks == KS_RELEASED)
		{
			m_has_new_input = true;
			obj_input pi(_it, _ks);
			pi.now();
			m_undo_inputs.push(pi);
			////printf("key_handler get_input_time: " << m_undo_inputs.back().get_input_time() << std::endl;
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
	}

	//! 
	ORZ_PLUS_INLINE void								run_motion_handler		(motion_type _mt, motion_manager& _motion_mgr)
	{
	}

	//! 
	ORZ_PLUS_INLINE void								end_motion_handler		(motion_type _mt, motion_manager& _motion_mgr)
	{
	}


private:
	//------------------------------------------------------------------------------------------------------------
	//! MSRS_IDLE
	ORZ_PLUS_INLINE void								idle_state_handler			(state_trigger& _state_mgr, float64 _dt)
	{
		// #1 there is nothing we could do here, just let it go

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(MSRS_IDLE);
		if (pi.get_input() != IT_NULL)
		{
			// check input. 按键序列多的优先级大于少的
			motion_type mt = m_motion_mgr.check_motion(m_motion_mgr.match_input(m_dir, false, m_input_queue.get_inputs(g_min, KS_NULL)));
			if (mt != MT_NULL)
			{
				switch (mt)
				{
				case MT_1:	case MT_2:	case MT_3:	case MT_4:
					{
						static_any<SMAS> sa(get_direction_by_input(pi.get_input()));
						_state_mgr.change_state((state_id)MSRS_MOVE, sa);
						return;
					}break;
				}
			}

			// skill, attack, and so on
			// we use skill manager to deal with attack and skills
			skill_type skt = m_skill_mgr.check_skill(m_skill_mgr.match_input(m_dir, false, m_input_queue.get_inputs(g_min, KS_NULL)));
			if (skt != SKT_NULL)
			{
				switch (skt)
				{
				case SKT_1:		// J
					{
						_state_mgr.change_state((state_id)MSRS_ATTACK_1);
						return;
					}break;
				}
			}
		}

		// check key state
		if (m_key_mgr.get_key_state_by_kv(KV_RIGHT) == KS_PUSHED ||
			m_key_mgr.get_key_state_by_kv(KV_LEFT) == KS_PUSHED ||
			m_key_mgr.get_key_state_by_kv(KV_UP) == KS_PUSHED ||
			m_key_mgr.get_key_state_by_kv(KV_DOWN) == KS_PUSHED )
		{
			static_any<SMAS> sa(check_motion_direction());
			_state_mgr.change_state((state_id)MSRS_MOVE, sa);
			return;
		}
	}
	//------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------
	//! MSRS_MOVE
	ORZ_PLUS_INLINE void								move_state_handler			(state_trigger& _state_mgr, float64 _dt)
	{
		////printf("move state!\n");
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
				//printf("msrid: [" << m_id << "] - " << "	move_state_handler - ERROR! !cs_x || !cs_y\n");
				m_mrj_sub_trigger.start();
			}
			else
				m_mrj_sub_trigger.start(*cs_x, *cs_y);
		}
		
		m_mrj_sub_trigger.run(_dt);

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(MSRS_MOVE);
		if (pi.get_input() != IT_NULL)
		{
			// attack
			// we use skill manager to deal with attack and skills
			skill_type skt = m_skill_mgr.check_skill(m_skill_mgr.match_input(m_dir, false, m_input_queue.get_inputs(g_min, KS_PUSHED)));
			switch (skt)
			{
			case SKT_1:		// J
				{
					_state_mgr.change_state((state_id)MSRS_ATTACK_1);
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
			_state_mgr.change_state(MSRS_IDLE);
		}
	}
	//------------------------------------------------------------------------------------------------------------

	//! mss begin
		ORZ_PLUS_INLINE void								mrjss_begin_handler			(state_id _stid, state_trigger& _state_mgr, float64 _dt)
		{
			//printf("monster mrj's sub states begin!\n");

			float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
			float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
			if (!curr_speed_x || !curr_speed_y)
			{
				//printf("msrid: [" << m_id << "] - " << "mrjss_begin_handler - !curr_speed_x || !curr_speed_y\n");
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
			default:																	break;
			}
		}

		//! MSRS_MOVE's sub state
		ORZ_PLUS_INLINE void								mrjss_right_handler			(state_trigger& _state_mgr, float64 _dt)
		{
			if (m_collision_type == ct_null)
			{
				float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
				float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
				if (!curr_speed_x || !curr_speed_y)
				{
					//printf("msrid: [" << m_id << "] - " << "mrjss_right_handler - !curr_speed_x || !curr_speed_y\n");
					return;
				}

				float64 cs_x = *curr_speed_x;
				float64 cs_y = *curr_speed_y;
				float64 fix = _dt * m_dmc->client_fps;
				m_pos.x += fix * cs_x;
			}
			opinion_motion_direction();
		}
			//! MSRS_MOVE's sub state
			ORZ_PLUS_INLINE void								mrjss_right_up_handler		(state_trigger& _state_mgr, float64 _dt)
			{
				if (m_collision_type == ct_null)
				{
					float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
					float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
					if (!curr_speed_x || !curr_speed_y)
					{
						//printf("msrid: [" << m_id << "] - " << "mrjss_right_up_handler - !curr_speed_x || !curr_speed_y\n");
						return;
					}
		
					float64 cs_x = *curr_speed_x;
					float64 cs_y = *curr_speed_y;
					float64 fix = _dt * m_dmc->client_fps;
					m_pos.x += fix * cs_x;
					m_pos.y -= fix * cs_y;
					m_pos.z -= fix * cs_y;
				}
				opinion_motion_direction();
			}
		//! MSRS_MOVE's sub state
		ORZ_PLUS_INLINE void								mrjss_left_handler			(state_trigger& _state_mgr, float64 _dt)
		{
			if (m_collision_type == ct_null)
			{
				float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
				float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
				if (!curr_speed_x || !curr_speed_y)
				{
					//printf("msrid: [" << m_id << "] - " << "mrjss_left_handler - !curr_speed_x || !curr_speed_y\n");
					return;
				}
				float64 cs_x = *curr_speed_x;
				float64 cs_y = *curr_speed_y;
				float64 fix = _dt * m_dmc->client_fps;
				m_pos.x -= fix * cs_x;
			}
			opinion_motion_direction();
		}
			//! MSRS_MOVE's sub state
			ORZ_PLUS_INLINE void								mrjss_left_up_handler			(state_trigger& _state_mgr, float64 _dt)
			{
				if (m_collision_type == ct_null)
				{
					float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
					float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
					if (!curr_speed_x || !curr_speed_y)
					{
						//printf("msrid: [" << m_id << "] - " << "mrjss_left_up_handler - !curr_speed_x || !curr_speed_y\n");
						return;
					}
					float64 cs_x = *curr_speed_x;
					float64 cs_y = *curr_speed_y;
					float64 fix = _dt * m_dmc->client_fps;
					m_pos.x -= fix * cs_x;
					m_pos.y -= fix * cs_y;
					m_pos.z -= fix * cs_y;
				}
				opinion_motion_direction();
			}
		//! MSRS_MOVE's sub state
		ORZ_PLUS_INLINE void								mrjss_up_handler				(state_trigger& _state_mgr, float64 _dt)
		{
			if (m_collision_type == ct_null)
			{
				float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
				float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
				if (!curr_speed_x || !curr_speed_y)
				{
					//printf("msrid: [" << m_id << "] - " << "mrjss_up_handler - !curr_speed_x || !curr_speed_y\n");
					return;
				}
				float64 cs_x = *curr_speed_x;
				float64 cs_y = *curr_speed_y;
				float64 fix = _dt * m_dmc->client_fps;
				m_pos.y -= fix * cs_y;
				m_pos.z -= fix * cs_y;
			}
			opinion_motion_direction();
		}
			//! MSRS_MOVE's sub state
			ORZ_PLUS_INLINE void								mrjss_right_down_handler		(state_trigger& _state_mgr, float64 _dt)
			{
				if (m_collision_type == ct_null)
				{
					float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
					float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
					if (!curr_speed_x || !curr_speed_y)
					{
						//printf("msrid: [" << m_id << "] - " << "mrjss_right_down_handler - !curr_speed_x || !curr_speed_y\n");
						return;
					}
					float64 cs_x = *curr_speed_x;
					float64 cs_y = *curr_speed_y;
					float64 fix = _dt * m_dmc->client_fps;
					m_pos.x += fix * cs_x;
					m_pos.y += fix * cs_y;
					m_pos.z += fix * cs_y;
				}
				opinion_motion_direction();
			}
		//! MSRS_MOVE's sub state
		ORZ_PLUS_INLINE void								mrjss_down_handler			(state_trigger& _state_mgr, float64 _dt)
		{
			if (m_collision_type == ct_null)
			{
				float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
				float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
				if (!curr_speed_x || !curr_speed_y)
				{
					//printf("msrid: [" << m_id << "] - " << "mrjss_down_handler - !curr_speed_x || !curr_speed_y\n");
					return;
				}
				float64 cs_x = *curr_speed_x;
				float64 cs_y = *curr_speed_y;
				float64 fix = _dt * m_dmc->client_fps;
				m_pos.y += fix * cs_y;
				m_pos.z += fix * cs_y;
			}
			opinion_motion_direction();
		}
			//! MSRS_MOVE's sub state
			ORZ_PLUS_INLINE void								mrjss_left_down_handler		(state_trigger& _state_mgr, float64 _dt)
			{
				if (m_collision_type == ct_null)
				{
					float64* curr_speed_x = _state_mgr.get_lcd<float64>(0);
					float64* curr_speed_y = _state_mgr.get_lcd<float64>(1);
					if (!curr_speed_x || !curr_speed_y)
					{
						//printf("msrid: [" << m_id << "] - " << "mrjss_left_down_handler - !curr_speed_x || !curr_speed_y\n");
						return;
					}
					float64 cs_x = *curr_speed_x;
					float64 cs_y = *curr_speed_y;
					float64 fix = _dt * m_dmc->client_fps;
					m_pos.x -= fix * cs_x;
					m_pos.y += fix * cs_y;
					m_pos.z += fix * cs_y;
				}
				opinion_motion_direction();
			}

		//! mss end
		ORZ_PLUS_INLINE void								mrjss_end_handler				(state_id _stid, state_trigger& _state_mgr, float64 _dt)
		{
			_state_mgr.stop();
		}

	//------------------------------------------------------------------------------------------------------------
	//! MSRS_ATTACK_1
	ORZ_PLUS_INLINE void								attack1_state_handler		(state_trigger& _state_mgr, float64 _dt)
	{
		// #1 deal with attack1

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(MSRS_ATTACK_1);
		if (pi.get_input() != IT_NULL)
		{
		}
	}

	//------------------------------------------------------------------------------------------------------------
	//! PC_FALL
	ORZ_PLUS_INLINE void								fall_state_handler			(state_trigger& _state_mgr, float64 _dt)
	{
		////printf("pid: [" << m_pid << "] - " << "fall state!\n");
		//if (m_collision_type == ct_null)
		//	m_collision_type = check_collision(!m_dir?DIRT_RIGHT:DIRT_LEFT);
		
		// #1 deal with others

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(MSRS_FALL);
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
		////printf("pid: [" << m_pid << "] - " << "rise up state!\n");
		// #1 deal with others

		// #2 check state change condition
		// first check instant input
		obj_input pi = check_undo_inputs(MSRS_RISE_UP);
		if (pi.get_input() != IT_NULL)
		{
		}

		// second check keys state 

	}
	//------------------------------------------------------------------------------------------------------------

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
		zn.h = (unsigned int)((float64)zn.w * m_dmc->min_move_distance / m_dmc->move_speed);
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
		//if (sid == PC_MOVE)
		//	broadcast_move_msg(_dt, false);
		//else if (sid == PC_RUN)
		//	broadcast_run_msg(_dt, false);
		//if (ct != ct_null)
		//	//printf("\n	new collision ! pc state: " << m_state_mgr.get_current_state() << std::endl;
		//else /*if (m_state_mgr.get_current_state() == PC_SKILL)*/
		//	//printf("\n	new collision ! ct == ct_null p2.x: " << p2.x << ", p3.x: " << p3.x << std::endl;
		return ct;
	}

	//! send event  -to-  game
	ORZ_PLUS_INLINE void								push_attack					(const zone& _zone, bool _dir, const pos3d& _pos, unsigned int _atk_y, const attack& _atk)
	{
		//zone new_zn = _zone.convert_by_dir(_dir, (int)m_pos.x, (int)m_pos.y);
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
		pk->put("type=", 5);
		std::string tp("1");
		pk->put(tp.c_str(), tp.size());
		pk->put("#", 1);
		pk->put("msrid=", 9);
		NumStr pid(d2s(m_id));
		pk->put(pid.c_str(), pid.size());
		pk->put("|", 1);
		pk->put("direct=", 7);
		NumStr dt(d2s(_dmg.dir()));
		pk->put(dt.c_str(), dt.size());
		pk->put("|", 1);
		pk->put("dmgtype=", 8);
		NumStr dmt(d2s(_dmg.type()));
		pk->put(dmt.c_str(), dmt.size());

		// mt
		pk->put("|", 1);
		pk->put("mt=1", 4);

		// v0
		pk->put("&", 1);
		pk->put("v0=", 3);
		NumStr v0(f2s(_v0));
		pk->put(v0.c_str(), v0.size());

		// a
		pk->put(",", 1);
		pk->put("a=", 2);
		NumStr a(f2s(_a));
		pk->put(a.c_str(), a.size());

		// max dis
		pk->put(",", 1);
		pk->put("maxdis=", 7);
		NumStr maxdis(d2s(_max_dis));
		pk->put(maxdis.c_str(), maxdis.size());

		m_game_proxy->broadcast(pk);
	}

	//! broadcast 伤害+抛物线运动
	ORZ_PLUS_INLINE void								broadcast_dmg_and_pda_msg	(damage& _dmg, float64 _v0, float64 _spx, float64 _spy)
	{
		IPacketShrPtr pk = allocPacket();
		pk->setMsg(M_DAMAGE);
		pk->put("type=", 5);
		NumStr tp("1");
		pk->put(tp.c_str(), tp.size());
		pk->put("#", 1);
		pk->put("msrid=", 9);
		NumStr pid(d2s(m_id));
		pk->put(pid.c_str(), pid.size());
		pk->put("|", 1);
		pk->put("direct=", 7);
		NumStr dt(d2s(_dmg.dir()));
		pk->put(dt.c_str(), dt.size());
		pk->put("|", 1);
		pk->put("dmgtype=", 8);
		NumStr dmt(d2s(_dmg.type()));
		pk->put(dmt.c_str(), dmt.size());

		// mt
		pk->put("|", 1);
		pk->put("mt=2", 4);

		// v0
		pk->put("&", 1);
		pk->put("v0=", 3);
		NumStr v0(f2s(_v0));
		pk->put(v0.c_str(), v0.size());

		// spx
		pk->put(",", 1);
		pk->put("spx=", 4);
		NumStr spx(f2s(_spx));
		pk->put(spx.c_str(), spx.size());

		// spy
		pk->put(",", 1);
		pk->put("spy=", 4);
		NumStr spy(f2s(_spy));
		pk->put(spy.c_str(), spy.size());

		m_game_proxy->broadcast(pk);
	}

	//! broadcast move event
	ORZ_PLUS_INLINE void								broadcast_move_msg			(direction_type _dt, bool _state)
	{
		IPacketShrPtr pk = allocPacket();
		pk->setMsg(M_MOVE);
		pk->put("type=", 5);
		NumStr tp("2");
		pk->put(tp.c_str(), tp.size());
		pk->put("#", 1);
		pk->put("msrid=", 6);
		NumStr msrid(d2s(m_id));
		pk->put(msrid.c_str(), msrid.size());
		pk->put("|", 1);
		pk->put("direct=", 7);
		NumStr dt(d2s(_dt));
		pk->put(dt.c_str(), dt.size());
		pk->put("&", 1);
		pk->put("posx=", 5);
		NumStr posx(d2s((int)m_pos.x));
		pk->put(posx.c_str(), posx.size());
		pk->put("&", 1);
		pk->put("posy=", 5);
		NumStr posy(d2s((int)m_pos.y));
		pk->put(posy.c_str(), posy.size());
		pk->put("&", 1);
		pk->put("state=", 6);
		NumStr state(d2s(_state ? 0 : 1));
		pk->put(state.c_str(), state.size());

		m_game_proxy->broadcast(pk);
	}

	//! broadcast action event
	ORZ_PLUS_INLINE void								broadcast_action_msg		(action_type _at, direction_type _dt, action_step _cur_step, action_step _next_step, bool _value)
	{
		IPacketShrPtr pk = allocPacket();
		pk->setMsg(M_ACTION);
		pk->put("msrid=", 9);
		NumStr pid(d2s(m_id));
		pk->put(pid.c_str(), pid.size());
		pk->put("#", 1);
		pk->put("actiontype=", 11);
		NumStr action_type(d2s(_at));
		pk->put(action_type.c_str(), action_type.size());
		pk->put("|", 1);
		pk->put("direct=", 7);
		NumStr dt(d2s(_dt));
		pk->put(dt.c_str(), dt.size());
		pk->put("&", 1);
		pk->put("curstep=", 8);
		NumStr cur_step(d2s(_cur_step));
		pk->put(cur_step.c_str(), cur_step.size());
		pk->put("&", 1);
		pk->put("nextstep=", 9);
		NumStr next_step(d2s(_next_step));
		pk->put(next_step.c_str(), next_step.size());
		pk->put(",", 1);
		pk->put("value=", 6);
		NumStr value(d2s(_value ? 0 : 1));
		pk->put(value.c_str(), value.size());

		m_game_proxy->broadcast(pk);
	}

	//! 检查undo inputs
	ORZ_PLUS_INLINE obj_input						check_undo_inputs				(monster_state _cs)
	{
		obj_input pi;
		if (!m_undo_inputs.empty() && m_has_new_input)
		{
			m_has_new_input = false;
			pi = m_undo_inputs.front();
			m_undo_inputs.pop();

			input_type it = (input_type)pi.get_input();
			//printf("monster's input: [ " << g_inputs[it] << " ]\n");
			
			m_input_queue.push(pi);
		}
		return pi;
	}

	//! 检查undo inputs
	ORZ_PLUS_INLINE obj_input						check_undo_inputs_no_pop		(monster_state _cs)
	{
		obj_input pi;
		if (!m_undo_inputs.empty() && m_has_new_input)
		{
			pi = m_undo_inputs.front();
			input_type it = (input_type)pi.get_input();
			//printf("monster's input: [ " << g_inputs[it] << " ]\n");
		}
		return pi;
	}

	//! 
	ORZ_PLUS_INLINE direction_type					check_motion_direction			()
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
	ORZ_PLUS_INLINE void								opinion_motion_direction			(bool _is_send=true)
	{
		std::vector<key_value> range;
		range.push_back(KV_RIGHT);
		range.push_back(KV_LEFT);
		range.push_back(KV_UP);
		range.push_back(KV_DOWN);
		std::vector<key_value> key_time_order = m_key_mgr.get_order(KS_PUSHED, range);
		
		mrj_sub_state mss = MRJSS_NULL;
		if (key_time_order.empty())
			m_mrj_sub_trigger.change_to_end();
		else
		{
			if (key_time_order.size() == 1)
			{
				mss = get_mrj_sub_state_by_kv(key_time_order[0]);
				////printf("opinion_motion_direction - key_time_order[0]: " << key_time_order[0] << std::endl;
			}
			else
			{
				mss = get_mrj_sub_state_by_kv(key_time_order[0], key_time_order[1]);
				////printf("opinion_motion_direction - key_time_order[0]: " << key_time_order[0] << ", key_time_order[1]: " << key_time_order[1] << std::endl;
			}
			
			if (mss != MRJSS_NULL && m_mrj_sub_trigger.get_current_state() != (state_id)mss)
			{
				//printf("monster change to sub state: " << g_sub_states[mss] << ", mss: " << mss << std::endl;
				if (mss == MRJSS_RIGHT || mss == MRJSS_RIGHT_UP || mss == MRJSS_RIGHT_DOWN)
					m_dir = true;
				else if (mss == MRJSS_LEFT || mss == MRJSS_LEFT_UP || mss == MRJSS_LEFT_DOWN)
					m_dir = false;
				if (_is_send)
				{
					if (m_state_mgr.get_current_state() == MSRS_MOVE)
						broadcast_move_msg(get_direction_by_mrj_ss(mss), true);
				}
				m_mrj_sub_trigger.change_state(mss);
			}
		}
	}

	//! 根据给定的二位数组的indexs 返回对应的ckf
	ORZ_PLUS_INLINE const cfg_key_frame*				get_ckf						(unsigned int _i1, unsigned _i2)
	{
		if (_i1 >= m_dmc->cfg_key_frames.size() || _i2 >= m_dmc->cfg_key_frames[_i1].size())
			return 0;

		return &m_dmc->cfg_key_frames[_i1][_i2];
	}


	//------------------------------------------------------------------------------------------------------------


	ServiceManager*													m_svcMgr;
	monster_id														m_id;
	std::string														m_name;

	event_obj_id													m_eoi;

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
	event_manager*													m_event_mgr;

	//!
	action_manager													m_fs_action_mgr;

	//! action_frame_sequence manager 
	action_frame_sequence_manager									m_afs_mgr;
	std::vector<action_frame_sequence>								m_afss;

	//!
	obj_input_queue													m_input_queue;
	//! 当前未处理的输入事件
	std::queue<obj_input>											m_undo_inputs;

	// config
	demo_monster_config*											m_dmc;

	// test 
	float64															m_print_dt;
};

typedef boost::shared_ptr<demo_monster> demo_monster_ptr;

}

#endif
