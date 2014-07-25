#ifndef ACTIONMANAGER_H_
#define ACTIONMANAGER_H_

#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <common/static_any.h>
using namespace Orz;

#define AMAS 128

typedef int action_id;				// 动作类型标识

class action_manager;

class action
{
public:
	action()
		: m_actid(0)
		, m_interrupt(true)
		, m_current_time(now())
		, m_start_time(now())
	{
	}

	action(action_id _actid, const boost::function<void (action_id, action_manager&, static_any<AMAS>&, float64, float64)>& _event_handler)
		: m_actid(_actid)
		, m_event_handler(_event_handler)
		, m_interrupt(true)
		, m_current_time(now())
		, m_start_time(now())
	{
	}

	~action()
	{
	}

public:
	//!
	bool						operator==				(action_id _actid) const
	{
		if (m_actid == _actid)
			return true;
		else
			return false;
	}

public:
	ORZ_PLUS_INLINE action_id			get_id					()
	{
		return m_actid;
	}

	ORZ_PLUS_INLINE int32					invoke_handler			(action_manager& _action_mgr)
	{
		ClockTime current_time = now();
		float64 dt = (float64)(current_time - m_current_time) / 1000000.0f;
		float64 past_time = (float64)(current_time - m_start_time) / 1000000.0f;
		m_current_time = current_time;

		m_event_handler(m_actid, _action_mgr, m_sa, past_time, dt);
		return 0;
	}

	ORZ_PLUS_INLINE void					interrupt				()
	{
		m_interrupt = true;
		m_sa.clear();
	}

	ORZ_PLUS_INLINE void					awake					(const static_any<AMAS>& _sa)
	{
		m_interrupt = false;
		m_current_time = now();
		m_start_time = now();
		m_sa = _sa;
	}

	ORZ_PLUS_INLINE bool					is_interrupted			()
	{
		return m_interrupt;
	}

private:
	action_id											m_actid;									
	boost::function<void (action_id, action_manager&, static_any<AMAS>&, float64, float64)>
														m_event_handler;
	bool												m_interrupt;
	ClockTime											m_current_time;
	ClockTime											m_start_time;
	static_any<AMAS>									m_sa;
};

class action_manager
{
public:
	action_manager() {}
	~action_manager() {}

public:
	//!
	ORZ_PLUS_INLINE void					create_action			(action_id _actid, const boost::function<void (action_id, action_manager&, static_any<AMAS>&, float64, float64)>& _event_handler)
	{
		m_actions.push_back(action(_actid, _event_handler));
	}

public:
	//!
	ORZ_PLUS_INLINE int32					simulate				()
	{
		std::vector<action>::iterator itr(m_actions.begin());
		for (; itr!=m_actions.end(); ++itr)
		{
			action& act = *itr;
			if (act.is_interrupted())
				continue;

			act.invoke_handler(*this);
		}

		return 0;
	}

	//!
	ORZ_PLUS_INLINE int32					interrupt				(action_id _actid)
	{
		std::vector<action>::iterator itr(std::find(m_actions.begin(), m_actions.end(), _actid));
		if (itr == m_actions.end())
			return 1;

		(*itr).interrupt();
		return 0;
	}

	//! 
	ORZ_PLUS_INLINE int32					interrupt_all			()
	{
		std::vector<action>::iterator itr(m_actions.begin());
		for (; itr!=m_actions.end(); ++itr)
		{
			action& act = *itr;
			act.interrupt();
		}
		return 0;
	}

	//!
	ORZ_PLUS_INLINE int32					awake					(action_id _actid, const static_any<AMAS>& _sa)
	{
		std::vector<action>::iterator itr(std::find(m_actions.begin(), m_actions.end(), _actid));
		if (itr == m_actions.end())
			return 1;

		(*itr).awake(_sa);
		return 0;
	}

	//!
	ORZ_PLUS_INLINE bool					is_interrupted			(action_id _actid)
	{
		std::vector<action>::iterator itr(std::find(m_actions.begin(), m_actions.end(), _actid));
		if (itr == m_actions.end())
			return false;
		
		return (*itr).is_interrupted();
	}

private:
	std::vector<action>					m_actions;
};


#endif
