#ifndef MOTION_MANAGER_H_
#define MOTION_MANAGER_H_

#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <game_server/gs.h>
#include <game_server/key_manager.h>


namespace Orz
{

class motion_manager;

class motion
{
public:
	motion(motion_type _skt)
		: m_id(_skt)
	{
	}

	motion(motion_type _skt, 
		  const boost::function<void (motion_type, motion_manager&)>& _begin_skill_handler,
		  const boost::function<void (motion_type, motion_manager&)>& _run_skill_handler, 
		  const boost::function<void (motion_type, motion_manager&)>& _end_skill_handler, 
		  motion_input_type _sit1)
		: m_id(_skt)
		, m_begin_skill_handler(_begin_skill_handler)
		, m_run_skill_handler(_run_skill_handler)
		, m_end_skill_handler(_end_skill_handler)
	{
		m_input_order.push_back(_sit1);
	}

	motion(motion_type _skt, 
		  const boost::function<void (motion_type, motion_manager&)>& _begin_skill_handler,
		  const boost::function<void (motion_type, motion_manager&)>& _run_skill_handler, 
		  const boost::function<void (motion_type, motion_manager&)>& _end_skill_handler, 
		  motion_input_type _sit1, motion_input_type _sit2)
		: m_id(_skt)
		, m_begin_skill_handler(_begin_skill_handler)
		, m_run_skill_handler(_run_skill_handler)
		, m_end_skill_handler(_end_skill_handler)
	{
		m_input_order.push_back(_sit1);
		m_input_order.push_back(_sit2);
	}

	motion(motion_type _skt, 
		  const boost::function<void (motion_type, motion_manager&)>& _begin_skill_handler,
		  const boost::function<void (motion_type, motion_manager&)>& _run_skill_handler, 
		  const boost::function<void (motion_type, motion_manager&)>& _end_skill_handler, 
		  motion_input_type _sit1, motion_input_type _sit2, motion_input_type _sit3)
		: m_id(_skt)
		, m_begin_skill_handler(_begin_skill_handler)
		, m_run_skill_handler(_run_skill_handler)
		, m_end_skill_handler(_end_skill_handler)
	{
		m_input_order.push_back(_sit1);
		m_input_order.push_back(_sit2);
		m_input_order.push_back(_sit3);
	}

	motion(motion_type _skt, 
		  const boost::function<void (motion_type, motion_manager&)>& _begin_skill_handler,
		  const boost::function<void (motion_type, motion_manager&)>& _run_skill_handler, 
		  const boost::function<void (motion_type, motion_manager&)>& _end_skill_handler, 
		  motion_input_type _sit1, motion_input_type _sit2, motion_input_type _sit3, motion_input_type _sit4)
		: m_id(_skt)
		, m_begin_skill_handler(_begin_skill_handler)
		, m_run_skill_handler(_run_skill_handler)
		, m_end_skill_handler(_end_skill_handler)
	{
		m_input_order.push_back(_sit1);
		m_input_order.push_back(_sit2);
		m_input_order.push_back(_sit3);
		m_input_order.push_back(_sit4);
	}

	motion(motion_type _skt, 
		  const boost::function<void (motion_type, motion_manager&)>& _begin_skill_handler,
		  const boost::function<void (motion_type, motion_manager&)>& _run_skill_handler, 
		  const boost::function<void (motion_type, motion_manager&)>& _end_skill_handler, 
		  motion_input_type _sit1, motion_input_type _sit2, motion_input_type _sit3, motion_input_type _sit4, motion_input_type _sit5)
		: m_id(_skt)
		, m_begin_skill_handler(_begin_skill_handler)
		, m_run_skill_handler(_run_skill_handler)
		, m_end_skill_handler(_end_skill_handler)
	{
		m_input_order.push_back(_sit1);
		m_input_order.push_back(_sit2);
		m_input_order.push_back(_sit3);
		m_input_order.push_back(_sit4);
		m_input_order.push_back(_sit5);
	}

	motion(motion_type _skt, 
		  const boost::function<void (motion_type, motion_manager&)>& _begin_skill_handler,
		  const boost::function<void (motion_type, motion_manager&)>& _run_skill_handler, 
		  const boost::function<void (motion_type, motion_manager&)>& _end_skill_handler, 
		  motion_input_type _sit1, motion_input_type _sit2, motion_input_type _sit3, motion_input_type _sit4, motion_input_type _sit5, motion_input_type _sit6)
		: m_id(_skt)
		, m_begin_skill_handler(_begin_skill_handler)
		, m_run_skill_handler(_run_skill_handler)
		, m_end_skill_handler(_end_skill_handler)
	{
		m_input_order.push_back(_sit1);
		m_input_order.push_back(_sit2);
		m_input_order.push_back(_sit3);
		m_input_order.push_back(_sit4);
		m_input_order.push_back(_sit5);
		m_input_order.push_back(_sit6);
	}

	motion(motion_type _skt, 
		  const boost::function<void (motion_type, motion_manager&)>& _begin_skill_handler,
		  const boost::function<void (motion_type, motion_manager&)>& _run_skill_handler, 
		  const boost::function<void (motion_type, motion_manager&)>& _end_skill_handler, 
		  motion_input_type _sit1, motion_input_type _sit2, motion_input_type _sit3, motion_input_type _sit4, motion_input_type _sit5, motion_input_type _sit6, motion_input_type _sit7)
		: m_id(_skt)
		, m_begin_skill_handler(_begin_skill_handler)
		, m_run_skill_handler(_run_skill_handler)
		, m_end_skill_handler(_end_skill_handler)
	{
		m_input_order.push_back(_sit1);
		m_input_order.push_back(_sit2);
		m_input_order.push_back(_sit3);
		m_input_order.push_back(_sit4);
		m_input_order.push_back(_sit5);
		m_input_order.push_back(_sit6);
		m_input_order.push_back(_sit7);
	}

	~motion()
	{
	}

public:
	bool							operator==					(motion_type _mt)
	{
		if (m_id == _mt)
			return true;
		else
			return false;
	}

public:
	ORZ_PLUS_INLINE motion_type				get_id						()
	{
		return m_id;
	}

	//!
	ORZ_PLUS_INLINE std::vector<motion_input_type>&	get_input_order		()
	{
		return m_input_order;
	}

	//!
	ORZ_PLUS_INLINE void						invoke_begin_handler		(motion_manager& _motion_mgr)
	{
		m_begin_skill_handler(m_id, _motion_mgr);
	}

	//!
	ORZ_PLUS_INLINE void						invoke_run_handler			(motion_manager& _motion_mgr)
	{
		m_run_skill_handler(m_id, _motion_mgr);
	}

	//!
	ORZ_PLUS_INLINE void						invoke_end_handler			(motion_manager& _motion_mgr)
	{
		m_end_skill_handler(m_id, _motion_mgr);
	}

	//! 取得按键序列按键的数量
	ORZ_PLUS_INLINE unsigned					get_input_num				()
	{
		return (unsigned int)m_input_order.size();
	}

private:
	motion_type												m_id;
	std::vector<motion_input_type>							m_input_order;
	boost::function<void (motion_type, motion_manager&)>	m_begin_skill_handler;
	boost::function<void (motion_type, motion_manager&)>	m_run_skill_handler;
	boost::function<void (motion_type, motion_manager&)>	m_end_skill_handler;
};

//!
class motion_manager
{
public:
	//! _motion_usable_condition_handler: 判定技能能否使用的handler
	motion_manager(const boost::function<bool (motion_type, motion_manager&)>& _motion_usable_condition_handler)
		: m_motion_usable_condition_handler(_motion_usable_condition_handler)
		, m_current_motion(0)
	{
	}
	~motion_manager()
	{
	}

public:
	//! init motions
	ORZ_PLUS_INLINE void							init						(std::vector<motion>& _motions)
	{
		std::vector<motion>::iterator itr(_motions.begin());
		for (; itr!=_motions.end(); ++itr)
		{
			motion& mt = *itr;
			unsigned in = mt.get_input_num();
			while (in >= m_motions.size())
			{
				m_motions.push_back(std::vector<motion>());
			}
			m_motions[in].push_back(mt);
		}
	}

	//!
	ORZ_PLUS_INLINE motion_type					match_input					(bool _dir, bool _check_order, input_type _it1)
	{
		std::vector<input_type> its;
		its.push_back(_it1);
		return match_input(_dir, _check_order, its);
	}
	//!
	ORZ_PLUS_INLINE motion_type					match_input					(bool _dir, bool _check_order, input_type _it1, input_type _it2)
	{
		std::vector<input_type> its;
		its.push_back(_it1);
		its.push_back(_it2);
		return match_input(_dir, _check_order, its);
	}
	//!
	ORZ_PLUS_INLINE motion_type					match_input					(bool _dir, bool _check_order, input_type _it1, input_type _it2, input_type _it3)
	{
		std::vector<input_type> its;
		its.push_back(_it1);
		its.push_back(_it2);
		its.push_back(_it3);
		return match_input(_dir, _check_order, its);
	}
	//!
	ORZ_PLUS_INLINE motion_type					match_input					(bool _dir, bool _check_order, input_type _it1, input_type _it2, input_type _it3, input_type _it4)
	{
		std::vector<input_type> its;
		its.push_back(_it1);
		its.push_back(_it2);
		its.push_back(_it3);
		its.push_back(_it4);
		return match_input(_dir, _check_order, its);
	}
	//!
	ORZ_PLUS_INLINE motion_type					match_input					(bool _dir, bool _check_order, input_type _it1, input_type _it2, input_type _it3, input_type _it4, input_type _it5)
	{
		std::vector<input_type> its;
		its.push_back(_it1);
		its.push_back(_it2);
		its.push_back(_it3);
		its.push_back(_it4);
		its.push_back(_it5);
		return match_input(_dir, _check_order, its);
	}
	//!
	ORZ_PLUS_INLINE motion_type					match_input					(bool _dir, bool _check_order, input_type _it1, input_type _it2, input_type _it3, input_type _it4, input_type _it5, input_type _it6)
	{
		std::vector<input_type> its;
		its.push_back(_it1);
		its.push_back(_it2);
		its.push_back(_it3);
		its.push_back(_it4);
		its.push_back(_it5);
		its.push_back(_it6);
		return match_input(_dir, _check_order, its);
	}

	//! _check_order 真表示正序,否表示倒序(所谓序，指得按照按键序列的按键数量由小到大排列的motion们)
	ORZ_PLUS_INLINE motion_type					match_input					(bool _dir, bool _check_order, const std::vector<input_type>& _order)
	{
		std::vector<motion_input_type> mits;
		std::vector<input_type>::const_iterator iitr(_order.begin());
		for (; iitr!=_order.end(); ++iitr)
		{
			mits.push_back(convert_to_motion_input_type(_dir, *iitr));
		}
		motion_type st = MT_NULL;
		
		if (_check_order)
		{
			std::vector<std::vector<motion> >::iterator aitr(m_motions.begin());
			for (; aitr!=m_motions.end(); ++aitr)
			{
				std::vector<motion>& _motions = *aitr;

				std::vector<motion>::iterator mtitr(_motions.begin());
				for (; mtitr!=_motions.end(); ++mtitr)
				{
					motion& mt = *mtitr;
					std::vector<motion_input_type>& mtits = mt.get_input_order();
					if (mtits.size() > mits.size())
						continue;

					// match
					bool is_matched = true;
					std::vector<motion_input_type>::reverse_iterator sitr(mits.rbegin());
					std::vector<motion_input_type>::reverse_iterator ditr(mtits.rbegin());
					for (; ditr!=mtits.rend(); ++sitr,++ditr)
					{
						motion_input_type spi = *sitr;
						motion_input_type dpi = *ditr;
						if (dpi != spi)
						{
							is_matched = false;
							break;
						}
					}
					if (mtits.empty())
						is_matched = false;
					if (is_matched)
					{
						st = mt.get_id();
						goto OVER;
					}
				}
			}
		}
		else
		{
			std::vector<std::vector<motion> >::reverse_iterator aitr(m_motions.rbegin());
			for (; aitr!=m_motions.rend(); ++aitr)
			{
				std::vector<motion>& _motions = *aitr;

				std::vector<motion>::iterator mtitr(_motions.begin());
				for (; mtitr!=_motions.end(); ++mtitr)
				{
					motion& mt = *mtitr;
					std::vector<motion_input_type>& mtits = mt.get_input_order();
					if (mtits.size() > mits.size())
						continue;
					// match
					bool is_matched = true;
					std::vector<motion_input_type>::reverse_iterator sitr(mits.rbegin());
					std::vector<motion_input_type>::reverse_iterator ditr(mtits.rbegin());
					for (; ditr!=mtits.rend(); ++sitr,++ditr)
					{
						motion_input_type spi = *sitr;
						motion_input_type dpi = *ditr;
						if (dpi != spi)
						{
							is_matched = false;
							break;
						}
					}
					if (mtits.empty())
						is_matched = false;
					if (is_matched)
					{
						st = mt.get_id();
						goto OVER;
					}
				}
			}
		}
OVER:
		return st;
	}

	//! 检查给定的技能是否满足条件
	ORZ_PLUS_INLINE motion_type					check_motion				(motion_type _mt)
	{
		if (m_motion_usable_condition_handler(_mt, *this))
			return _mt;
		else
			return MT_NULL;
	}

	//!
	ORZ_PLUS_INLINE void							start_motion				(motion_type _mt)
	{
		std::vector<std::vector<motion> >::iterator aitr(m_motions.begin());
		for (; aitr!=m_motions.end(); ++aitr)
		{
			std::vector<motion>& _motions = *aitr;
			std::vector<motion>::iterator itr(std::find(_motions.begin(), _motions.end(), _mt));
			if (itr != _motions.end())
				m_current_motion = &(*itr);
		}

		if (m_current_motion)
			m_current_motion->invoke_begin_handler(*this);
	}

	//!
	ORZ_PLUS_INLINE void							run_motion					()
	{
		if (m_current_motion)
			m_current_motion->invoke_run_handler(*this);
	}

	//!
	ORZ_PLUS_INLINE void							end_motion					()
	{
		if (m_current_motion)
			m_current_motion->invoke_end_handler(*this);
		m_current_motion = 0;
	}

private:
	std::vector<std::vector<motion> >								m_motions;
	boost::function<bool (motion_type, motion_manager&)>			m_motion_usable_condition_handler;
	motion*															m_current_motion;
};

}

#endif
