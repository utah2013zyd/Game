//////////////////////////////////////////////////////////////
//
//      FileName: monster_mind.h
//        Author: 熊小磊
//       Version: 1.0.0
//          Date: 2008-12-26
//          Time: 16:06:00
//   Description: monster ai类
//
// History:
//      <author>    <time>      <version>   <descript>
//      熊小磊		2008-12-26  1.0.0       add
//////////////////////////////////////////////////////////////
#ifndef MONSTER_MIND_H_
#define MONSTER_MIND_H_

#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <common/game/state_trigger.h>
#include <common/game/monster.h>
#include <game_server/gs.h>
#include <common/game/area.h>
#include <common/game/action_manager.h>
#include <game_server/key_manager.h>
#include <game_server/demo_map.h>


namespace Orz
{

#define DELAY_INPUT_TIME 0.05


class monster_mind
{
public:
	monster_mind(ServiceManager* _svcMgr, event_manager& _event_mgr, monster* _monster, game_proxy& _game_proxy, monster_mind_config& _mmc, demo_map& _demo_map)
		: m_svcMgr(_svcMgr)
		, m_monster(_monster)
		, m_game_proxy(&_game_proxy)
		, m_state_mgr(boost::bind(&monster_mind::begin_state_handler, this, _1, _2, _3), 
					  boost::bind(&monster_mind::change_state_handler, this, _1, _2, _3, _4), 
					  boost::bind(&monster_mind::cr_handler, this, _1, _2, _3),
					  boost::bind(&monster_mind::end_state_handler, this, _1, _2, _3))
		, m_event_mgr(&_event_mgr)
		, m_guard_area(ator(180), 300.0f)
		, m_combat_area(ator(360), 400.0f)
		, m_mmc(&_mmc)
		, m_demo_map(&_demo_map)
	{
		//printf("monster_mind - m_mmc->guard_move_interval_time: %f, m_mmc->short_side_move_time: " << m_mmc->short_side_move_time << ", m_mmc->long_side_move_time: " << m_mmc->long_side_move_time << std::endl;
		// init states
		std::vector<state> states;
		for (int i=(int)MSRMS_NULL; i<(int)MSRMS_TOTAL; ++i)
		{
			states.push_back(state((state_id)i, boost::bind(&monster_mind::state_handler, this, _1, _2, _3)));
		}
		m_state_mgr.init(states);
		m_state_mgr.start();

		// init keys
		std::vector<key> keys;
		boost::function<void (key_value, key_manager&, key_state, input_type)> key_hdr = boost::bind(&monster_mind::key_handler, this, _1, _2, _3, _4);
		keys.push_back(key(IT_PUSH_RIGHT,	IT_RELEASE_RIGHT,	KV_RIGHT,	key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_LEFT,	IT_RELEASE_LEFT,	KV_LEFT,	key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_UP,		IT_RELEASE_UP,		KV_UP,		key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_DOWN,	IT_RELEASE_DOWN,	KV_DOWN,	key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_ATTACK,	IT_RELEASE_ATTACK,	KV_ATTACK,	key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_JUMP,	IT_RELEASE_JUMP,	KV_JUMP,	key_hdr, key_hdr, key_hdr, key_hdr));
		keys.push_back(key(IT_PUSH_SKILL,	IT_RELEASE_SKILL,	KV_SKILL,	key_hdr, key_hdr, key_hdr, key_hdr));
		m_key_mgr.init(keys);

		// init actions
		
	}
	~monster_mind()
	{
	}

public:
	//!
	ORZ_PLUS_INLINE int32								run						(float64 _dt)
	{
		m_state_mgr.run();
		return 0;
	}

//------------------------------------------------------------------------------------------------------------
private:		// key 's handler
	//!
	ORZ_PLUS_INLINE void								key_handler				(key_value _kv, key_manager& _key_mgr, key_state _ks, input_type _it)
	{
		m_monster->set_input((int)_it);
	}

//------------------------------------------------------------------------------------------------------------
private:		// time handle

	//!
	//ORZ_PLUS_INLINE void								delay_input_time_handler(ClockId _cid, data& _data)
	//{
	//	m_clocks.erase(_cid);
	//	input_type* it = _data.get_data<input_type>();
	//	if (!it)
	//		return;
	//	m_monster->set_input((int)(*it));
	//}

//------------------------------------------------------------------------------------------------------------
private:		// action 's handler
	
	
//------------------------------------------------------------------------------------------------------------
private:		// state 's handler
	//! begin state handler
	ORZ_PLUS_INLINE void								begin_state_handler		(state_id _stid, state_trigger& _state_mgr, float64 _dt)
	{
		printf("monster mind begin state!\n");
		_state_mgr.change_state((state_id)MSRMS_GUARD);
	}

	//! state handler
	ORZ_PLUS_INLINE void								state_handler			(state_id _stid, state_trigger& _state_mgr, float64 _dt)
	{
		switch ((int)_stid)
		{
		case MSRMS_GUARD:	guard_state_handler(_state_mgr, _dt);	break;
		case MSRMS_COMBAT:	combat_state_handler(_state_mgr, _dt);	break;
		default:													break;
		}
	}

	//!
	ORZ_PLUS_INLINE void								change_state_handler	(state_id _old, state_trigger& _state_mgr, state_id _new, std::vector<static_any<SMAS> >& _datas)
	{
	}

	//! common rum handler
	ORZ_PLUS_INLINE void								cr_handler				(state_id _stid, state_trigger& _state_mgr, float64 _dt)
	{
	}

	//! end state handler
	ORZ_PLUS_INLINE void								end_state_handler		(state_id _stid, state_trigger& _state_mgr, float64 _dt)
	{
		printf("monster mind end state! now stop!\n");
		_state_mgr.stop();
	}

//------------------------------------------------------------------------------------------------------------
private:
	//------------------------------------------------------------------------------------------------------------
	//! MSRMS_GUARD
	ORZ_PLUS_INLINE void								guard_state_handler			(state_trigger& _state_mgr, float64 _dt)
	{
	}
	//------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------
	//! MSRMS_COMBAT
	ORZ_PLUS_INLINE void								combat_state_handler		(state_trigger& _state_mgr, float64 _dt)
	{
	}
	//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
private:	// 工具函数
	//!
	ORZ_PLUS_INLINE void								delay_input					(float64 _delay_time, input_type _it)
	{
		//boost::function<void (ClockId, data&)> hdr = boost::bind(&monster_mind::delay_input_time_handler, this, _1, _2);
	}

private:
	ServiceManager*							m_svcMgr;
	monster*								m_monster;
	game_proxy*								m_game_proxy;

	//! 状态触发器
	state_trigger							m_state_mgr;

	//!

	//!
	event_manager*							m_event_mgr;

	//!
	key_manager								m_key_mgr;

	//! 怪物的警戒区域
	area									m_guard_area;
	//! 怪物的战斗区域
	area									m_combat_area;

	demo_map*								m_demo_map;


	// config
	monster_mind_config*					m_mmc;
};

typedef boost::shared_ptr<monster_mind> monster_mind_ptr;

}

#endif
