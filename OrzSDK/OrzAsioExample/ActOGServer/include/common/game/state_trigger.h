//////////////////////////////////////////////////////////////
//
//      FileName: state_trigger.h
//        Author: 熊小磊
//       Version: 1.0.0
//          Date: 2008-12-10
//          Time: 15:55:00
//   Description: state_trigger类，状态触发器
//
// History:
//      <author>    <time>      <version>   <descript>
//      熊小磊		2008-12-10  1.0.0       add
//////////////////////////////////////////////////////////////
#ifndef STATETRIGGER_H_
#define STATETRIGGER_H_

#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <common/static_any.h>
using namespace Orz;

#define SMAS 32

typedef unsigned int state_id;			// 状态id

class state_trigger;

//! class state - 状态类，描述状态
class state
{
public:
	state(state_id _sid, const std::vector<static_any<SMAS> >& _lcds, 
		  const boost::function<void (state_id, state_trigger&, float64)>& _state_handler)
		: m_sid(_sid)
		, m_life_cycle_datas(_lcds)
		, m_state_handler(_state_handler)
	{
	}

	state(state_id _sid, const boost::function<void (state_id, state_trigger&, float64)>& _state_handler)
		: m_sid(_sid)
		, m_state_handler(_state_handler)
	{
	}

	state(state_id _sid, const static_any<SMAS>& _lcd1, 
		  const boost::function<void (state_id, state_trigger&, float64)>& _state_handler)
		: m_sid(_sid)
		, m_state_handler(_state_handler)
	{
		m_life_cycle_datas.push_back(_lcd1);
	}

	state(state_id _sid, const static_any<SMAS>& _lcd1, const static_any<SMAS>& _lcd2, 
		  const boost::function<void (state_id, state_trigger&, float64)>& _state_handler)
		: m_sid(_sid)
		, m_state_handler(_state_handler)
	{
		m_life_cycle_datas.push_back(_lcd1);
		m_life_cycle_datas.push_back(_lcd2);
	}

	state(state_id _sid, const static_any<SMAS>& _lcd1, const static_any<SMAS>& _lcd2, const static_any<SMAS>& _lcd3, 
		  const boost::function<void (state_id, state_trigger&, float64)>& _state_handler)
		: m_sid(_sid)
		, m_state_handler(_state_handler)
	{
		m_life_cycle_datas.push_back(_lcd1);
		m_life_cycle_datas.push_back(_lcd2);
		m_life_cycle_datas.push_back(_lcd3);
	}

	state(state_id _sid, const static_any<SMAS>& _lcd1, const static_any<SMAS>& _lcd2, const static_any<SMAS>& _lcd3, const static_any<SMAS>& _lcd4, 
		  const boost::function<void (state_id, state_trigger&, float64)>& _state_handler)
		: m_sid(_sid)
		, m_state_handler(_state_handler)
	{
		m_life_cycle_datas.push_back(_lcd1);
		m_life_cycle_datas.push_back(_lcd2);
		m_life_cycle_datas.push_back(_lcd3);
		m_life_cycle_datas.push_back(_lcd4);
	}

	~state()
	{
	}

public:
	ORZ_PLUS_INLINE state_id		get_id					() const
	{
		return m_sid;
	}

	template<typename LCD_TYPE>
	ORZ_PLUS_INLINE LCD_TYPE*	get_rlcd				(unsigned int _index)
	{
		if (_index >= m_life_cycle_datas.size())
			return 0;
		return m_life_cycle_datas[_index].get_data<LCD_TYPE>();
	}

	template <typename LCD_TYPE>
	ORZ_PLUS_INLINE void			set_lcd					(unsigned int _index, const LCD_TYPE& _lcd)
	{
		if (_index >= m_life_cycle_datas.size())
			return;
		m_life_cycle_datas[_index].set_data<LCD_TYPE>(_lcd);
	}

	ORZ_PLUS_INLINE static_any<SMAS>*	
						get_lcd					(unsigned int _index)
	{
		if (_index >= m_life_cycle_datas.size())
		{
			printf("get_lcd: _index >= m_life_cycle_datas.size() - m_sid: %d\n", m_sid);
			return 0;
		}
		return &m_life_cycle_datas[_index];
	}

	ORZ_PLUS_INLINE int32			invoke_handler			(state_id _sid, state_trigger& _stgr, float64 _dt)
	{
		m_state_handler(_sid, _stgr, _dt);
		return 1;
	}

private:
	state_id										m_sid;
	boost::function<void (state_id, state_trigger&, float64)>
												m_state_handler;
	std::vector<static_any<SMAS> >				m_life_cycle_datas;
};

//! class state_triigger - 状态触发器，用于处理状态机的管理
class state_trigger
{
public:
	state_trigger(const boost::function<void (state_id, state_trigger&, float64)>& _begin_state_hdr, 
				  const boost::function<void (state_id, state_trigger&, state_id, std::vector<static_any<SMAS> >&)>& _change_state_hdr,
				  const boost::function<void (state_id, state_trigger&, float64)>& _common_run_hdr,
				  const boost::function<void (state_id, state_trigger&, float64)>& _end_state_hdr)
		: m_current_state(0)
		, m_pause_state(0)
		, m_begin_state(0, _begin_state_hdr)
		, m_end_state(0, _end_state_hdr)
		, m_last_state_id(0)
		, m_is_start(false)
		, m_common_run_handler(_common_run_hdr)
		, m_change_state_handler(_change_state_hdr)
		, m_state_mgr_time(now())
	{
	}

	~state_trigger() 
	{
	}

	ORZ_PLUS_INLINE void				init					(const std::vector<state>& _states)
	{
		m_states = _states;
	}

public:
	//! get current state 0 error
	ORZ_PLUS_INLINE state_id			get_current_state		() const
	{
		if (m_current_state)
			return m_current_state->get_id();
		else
			return 0;
	}

	//!
	ORZ_PLUS_INLINE state_id			get_last_state			() const
	{
		return m_last_state_id;
	}

	template <typename LCD_TYPE>
	ORZ_PLUS_INLINE LCD_TYPE*		get_curr_state_lcd		(unsigned int _index)
	{
		if (m_current_state)
			return m_current_state->get_rlcd<LCD_TYPE>(_index);
		else
			return 0;
	}
	
	ORZ_PLUS_INLINE static_any<SMAS>*
							get_cs_lcd				(unsigned int _index)
	{
		if (m_current_state)
			return m_current_state->get_lcd(_index);
		else
			return 0;
	}

	template <typename LCD_TYPE>
	ORZ_PLUS_INLINE LCD_TYPE*		get_lcd					(unsigned int _index)
	{
		if (_index >= m_life_cycle_datas.size())
			return 0;
		return m_life_cycle_datas[_index].get_data<LCD_TYPE>();
	}

	template <typename LCD_TYPE>
	ORZ_PLUS_INLINE void				set_cs_lcd				(unsigned int _index, const LCD_TYPE& _lcd)
	{
		if (m_current_state)
			m_current_state->set_lcd<LCD_TYPE>(_index, _lcd);
	}

	//! return 1 failed
	ORZ_PLUS_INLINE int32				change_state			(state_id _sid, std::vector<static_any<SMAS> >& _datas)
	{
		if (_sid >= m_states.size())
			return 1;

		m_last_state_id = 0;
		if (m_current_state)
			m_last_state_id = m_current_state->get_id();
		else
			printf("[ERROR]state_trigger::change_state - m_current_state == 0!\n");

		m_current_state = &m_states[_sid];
		m_change_state_handler(m_last_state_id, *this, _sid, _datas);
		return 0;
	}

	ORZ_PLUS_INLINE int32				change_state			(state_id _sid)
	{
		std::vector<static_any<SMAS> > datas;
		return change_state(_sid, datas);
	}

	ORZ_PLUS_INLINE int32				change_state			(state_id _sid, const static_any<SMAS>& _d1)
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(_d1);
		return change_state(_sid, datas);
	}

	ORZ_PLUS_INLINE int32				change_state			(state_id _sid, const static_any<SMAS>& _d1, const static_any<SMAS>& _d2)
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(_d1);
		datas.push_back(_d2);
		return change_state(_sid, datas);
	}

	ORZ_PLUS_INLINE int32				change_state			(state_id _sid, const static_any<SMAS>& _d1, const static_any<SMAS>& _d2, const static_any<SMAS>& _d3)
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(_d1);
		datas.push_back(_d2);
		datas.push_back(_d3);
		return change_state(_sid, datas);
	}

	ORZ_PLUS_INLINE int32				change_state			(state_id _sid, const static_any<SMAS>& _d1, const static_any<SMAS>& _d2, const static_any<SMAS>& _d3, const static_any<SMAS>& _d4)
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(_d1);
		datas.push_back(_d2);
		datas.push_back(_d3);
		datas.push_back(_d4);
		return change_state(_sid, datas);
	}

	//! 
	ORZ_PLUS_INLINE int32				change_to_begin			(std::vector<static_any<SMAS> >& _datas)
	{
		m_last_state_id = 0;
		if (m_current_state)
			m_last_state_id = m_current_state->get_id();
		m_current_state = &m_begin_state;
		m_change_state_handler(m_last_state_id, *this, 0, _datas);

		return 0;
	}

	ORZ_PLUS_INLINE int32				change_to_begin			()
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(static_any<SMAS>(true));
		return change_to_begin(datas);
	}

	ORZ_PLUS_INLINE int32				change_to_begin			(const static_any<SMAS>& _d1)
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(static_any<SMAS>(true));
		datas.push_back(_d1);
		return change_to_begin(datas);
	}

	ORZ_PLUS_INLINE int32				change_to_begin			(const static_any<SMAS>& _d1, const static_any<SMAS>& _d2)
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(static_any<SMAS>(true));
		datas.push_back(_d1);
		datas.push_back(_d2);
		return change_to_begin(datas);
	}

	ORZ_PLUS_INLINE int32				change_to_begin			(const static_any<SMAS>& _d1, const static_any<SMAS>& _d2, const static_any<SMAS>& _d3)
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(static_any<SMAS>(true));
		datas.push_back(_d1);
		datas.push_back(_d2);
		datas.push_back(_d3);
		return change_to_begin(datas);
	}

	ORZ_PLUS_INLINE int32				change_to_begin			(const static_any<SMAS>& _d1, const static_any<SMAS>& _d2, const static_any<SMAS>& _d3, const static_any<SMAS>& _d4)
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(static_any<SMAS>(true));
		datas.push_back(_d1);
		datas.push_back(_d2);
		datas.push_back(_d3);
		datas.push_back(_d4);
		return change_to_begin(datas);
	}

	//!
	ORZ_PLUS_INLINE bool				is_start				() const
	{
		return m_is_start;
	}

	//!
	ORZ_PLUS_INLINE int32				change_to_end			(std::vector<static_any<SMAS> >& _datas)
	{
		m_last_state_id = 0;
		if (m_current_state)
			m_last_state_id = m_current_state->get_id();
		m_current_state = &m_end_state;
		m_change_state_handler(m_last_state_id, *this, 0, _datas);

		return 0;
	}

	ORZ_PLUS_INLINE int32				change_to_end			()
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(static_any<SMAS>(true));
		return change_to_end(datas);
	}

	ORZ_PLUS_INLINE int32				change_to_end			(const static_any<SMAS>& _d1)
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(static_any<SMAS>(true));
		datas.push_back(_d1);
		return change_to_end(datas);
	}

	ORZ_PLUS_INLINE int32				change_to_end			(const static_any<SMAS>& _d1, const static_any<SMAS>& _d2)
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(static_any<SMAS>(true));
		datas.push_back(_d1);
		datas.push_back(_d2);
		return change_to_end(datas);
	}

	ORZ_PLUS_INLINE int32				change_to_end			(const static_any<SMAS>& _d1, const static_any<SMAS>& _d2, const static_any<SMAS>& _d3)
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(static_any<SMAS>(true));
		datas.push_back(_d1);
		datas.push_back(_d2);
		datas.push_back(_d3);
		return change_to_end(datas);
	}

	ORZ_PLUS_INLINE int32				change_to_end			(const static_any<SMAS>& _d1, const static_any<SMAS>& _d2, const static_any<SMAS>& _d3, const static_any<SMAS>& _d4)
	{
		std::vector<static_any<SMAS> > datas;
		datas.push_back(static_any<SMAS>(true));
		datas.push_back(_d1);
		datas.push_back(_d2);
		datas.push_back(_d3);
		datas.push_back(_d4);
		return change_to_end(datas);
	}

	//! loop, 0 正常, 1 pause, 2 error
	ORZ_PLUS_INLINE int32				run						()
	{
		ClockTime state_mgr_time = now();
		float64 dt = (float64)(state_mgr_time - m_state_mgr_time) / 1000000.0f;
		m_state_mgr_time = state_mgr_time;

		//if (dt > 0.05f)
		//	printf("	state_trigger::run()! beyond dt: " << dt << ", current_state: " << m_current_state->get_id() << std::endl;

		if (!m_current_state)
			printf("[ERROR]state_trigger::run - !m_current_state \n");

		if (m_current_state && m_is_start)
		{
			m_common_run_handler(m_current_state->get_id(), *this, dt);
			m_current_state->invoke_handler(m_current_state->get_id(), *this, dt);
			return 0;
		}
		else if (m_is_start && m_pause_state)
			return 1;
		else 
			return 2;
	}

	//! loop, 0 正常, 1 pause, 2 error
	ORZ_PLUS_INLINE int32				run						(float64 _dt)
	{
		float64 dt = _dt;
		m_state_mgr_time = now();

		//if (dt > 0.05f)
		//	printf("	state_trigger::run(float64 _dt)! beyond dt: " << dt << ", current_state: " << m_current_state->get_id() << std::endl;

		if (m_current_state && m_is_start)
		{
			m_common_run_handler(m_current_state->get_id(), *this, dt);
			m_current_state->invoke_handler(m_current_state->get_id(), *this, dt);
			return 0;
		}
		else if (m_is_start && m_pause_state)
			return 1;
		else 
			return 2;
	}

	//! 启动初始状态 _old_dt: 用于父状态机当前的dt衔接
	ORZ_PLUS_INLINE int32				start					(const std::vector<static_any<SMAS> >& _datas)
	{
		m_life_cycle_datas = _datas;
		m_current_state = &m_begin_state;
		m_is_start = true;
		m_state_mgr_time = now();

		return 0;
	}

	//!
	ORZ_PLUS_INLINE int32				start					()
	{
		std::vector<static_any<SMAS> > sds;
		start(sds);
		return 0;
	}

	//!
	ORZ_PLUS_INLINE int32				start					(const static_any<SMAS>& _d1)
	{
		std::vector<static_any<SMAS> > sds;
		sds.push_back(_d1);
		start(sds);
		return 0;
	}

	//!
	ORZ_PLUS_INLINE int32				start					(const static_any<SMAS>& _d1, const static_any<SMAS>& _d2)
	{
		std::vector<static_any<SMAS> > sds;
		sds.push_back(_d1);
		sds.push_back(_d2);
		start(sds);
		return 0;
	}

	//!
	ORZ_PLUS_INLINE int32				start					(const static_any<SMAS>& _d1, const static_any<SMAS>& _d2, const static_any<SMAS>& _d3)
	{
		std::vector<static_any<SMAS> > sds;
		sds.push_back(_d1);
		sds.push_back(_d2);
		sds.push_back(_d3);
		start(sds);
	}

	//!
	ORZ_PLUS_INLINE int32				start					(const static_any<SMAS>& _d1, const static_any<SMAS>& _d2, const static_any<SMAS>& _d3, const static_any<SMAS>& _d4)
	{
		std::vector<static_any<SMAS> > sds;
		sds.push_back(_d1);
		sds.push_back(_d2);
		sds.push_back(_d3);
		sds.push_back(_d4);
		start(sds);
	}

	//! pause
	ORZ_PLUS_INLINE int32				pause					()
	{
		m_pause_state = m_current_state;
		m_current_state = 0;
		return 0;
	}

	//! continue;
	ORZ_PLUS_INLINE int32				go_on					()
	{
		m_current_state = m_pause_state;
		m_pause_state = 0;
		return 0;
	}

	//! 停止
	ORZ_PLUS_INLINE int32				stop					()
	{
		m_is_start = false;
		m_current_state = 0;
		return 0;
	}

public:
	std::vector<state>					m_states;
	bool								m_is_start;
	state*								m_current_state;
	state*								m_pause_state;
	state								m_begin_state;
	state								m_end_state;
	state_id								m_last_state_id;

	//!
	boost::function<void (state_id, state_trigger&, float64)>
										m_common_run_handler;

	//!
	boost::function<void (state_id, state_trigger&, state_id, std::vector<static_any<SMAS> >&)>
										m_change_state_handler;

	// 事件dt，用于计算所有state_mgr调用的dt的函数的时间点
	ClockTime							m_state_mgr_time;

	std::vector<static_any<SMAS> >		m_life_cycle_datas;
};

#endif
