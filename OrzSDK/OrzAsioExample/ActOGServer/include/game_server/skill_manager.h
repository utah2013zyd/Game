#ifndef SKILL_MANAGER_H_
#define SKILL_MANAGER_H_
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <game_server/gs.h>
#include <game_server/key_manager.h>

namespace Orz
{

class skill_manager;

class skill
{
public:
	skill(skill_type _skt)
		: m_id(_skt)
	{
	}

	skill(skill_type _skt, 
		  const boost::function<void (skill_type, skill_manager&)>& _begin_skill_handler,
		  const boost::function<void (skill_type, skill_manager&)>& _run_skill_handler, 
		  const boost::function<void (skill_type, skill_manager&)>& _end_skill_handler, 
		  skill_input_type _sit1)
		: m_id(_skt)
		, m_begin_skill_handler(_begin_skill_handler)
		, m_run_skill_handler(_run_skill_handler)
		, m_end_skill_handler(_end_skill_handler)
	{
		m_input_order.push_back(_sit1);
	}

	skill(skill_type _skt, 
		  const boost::function<void (skill_type, skill_manager&)>& _begin_skill_handler,
		  const boost::function<void (skill_type, skill_manager&)>& _run_skill_handler, 
		  const boost::function<void (skill_type, skill_manager&)>& _end_skill_handler, 
		  skill_input_type _sit1, skill_input_type _sit2)
		: m_id(_skt)
		, m_begin_skill_handler(_begin_skill_handler)
		, m_run_skill_handler(_run_skill_handler)
		, m_end_skill_handler(_end_skill_handler)
	{
		m_input_order.push_back(_sit1);
		m_input_order.push_back(_sit2);
	}

	skill(skill_type _skt, 
		  const boost::function<void (skill_type, skill_manager&)>& _begin_skill_handler,
		  const boost::function<void (skill_type, skill_manager&)>& _run_skill_handler, 
		  const boost::function<void (skill_type, skill_manager&)>& _end_skill_handler, 
		  skill_input_type _sit1, skill_input_type _sit2, skill_input_type _sit3)
		: m_id(_skt)
		, m_begin_skill_handler(_begin_skill_handler)
		, m_run_skill_handler(_run_skill_handler)
		, m_end_skill_handler(_end_skill_handler)
	{
		m_input_order.push_back(_sit1);
		m_input_order.push_back(_sit2);
		m_input_order.push_back(_sit3);
	}

	skill(skill_type _skt, 
		  const boost::function<void (skill_type, skill_manager&)>& _begin_skill_handler,
		  const boost::function<void (skill_type, skill_manager&)>& _run_skill_handler, 
		  const boost::function<void (skill_type, skill_manager&)>& _end_skill_handler, 
		  skill_input_type _sit1, skill_input_type _sit2, skill_input_type _sit3, skill_input_type _sit4)
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

	skill(skill_type _skt, 
		  const boost::function<void (skill_type, skill_manager&)>& _begin_skill_handler,
		  const boost::function<void (skill_type, skill_manager&)>& _run_skill_handler, 
		  const boost::function<void (skill_type, skill_manager&)>& _end_skill_handler, 
		  skill_input_type _sit1, skill_input_type _sit2, skill_input_type _sit3, skill_input_type _sit4, skill_input_type _sit5)
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

	skill(skill_type _skt, 
		  const boost::function<void (skill_type, skill_manager&)>& _begin_skill_handler,
		  const boost::function<void (skill_type, skill_manager&)>& _run_skill_handler, 
		  const boost::function<void (skill_type, skill_manager&)>& _end_skill_handler, 
		  skill_input_type _sit1, skill_input_type _sit2, skill_input_type _sit3, skill_input_type _sit4, skill_input_type _sit5, skill_input_type _sit6)
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

	skill(skill_type _skt, 
		  const boost::function<void (skill_type, skill_manager&)>& _begin_skill_handler,
		  const boost::function<void (skill_type, skill_manager&)>& _run_skill_handler, 
		  const boost::function<void (skill_type, skill_manager&)>& _end_skill_handler, 
		  skill_input_type _sit1, skill_input_type _sit2, skill_input_type _sit3, skill_input_type _sit4, skill_input_type _sit5, skill_input_type _sit6, skill_input_type _sit7)
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

	~skill()
	{
	}

public:
	bool							operator==					(skill_type _st)
	{
		if (m_id == _st)
			return true;
		else
			return false;
	}

public:
	ORZ_PLUS_INLINE skill_type				get_id						()
	{
		return m_id;
	}

	//!
	ORZ_PLUS_INLINE std::vector<skill_input_type>&	
									get_input_order				()
	{
		return m_input_order;
	}

	//!
	ORZ_PLUS_INLINE void						invoke_begin_handler		(skill_manager& _skill_mgr)
	{
		m_begin_skill_handler(m_id, _skill_mgr);
	}

	//!
	ORZ_PLUS_INLINE void						invoke_run_handler			(skill_manager& _skill_mgr)
	{
		m_run_skill_handler(m_id, _skill_mgr);
	}

	//!
	ORZ_PLUS_INLINE void						invoke_end_handler			(skill_manager& _skill_mgr)
	{
		m_end_skill_handler(m_id, _skill_mgr);
	}

	//! 取得按键序列按键的数量
	ORZ_PLUS_INLINE unsigned					get_input_num				()
	{
		return (unsigned int)m_input_order.size();
	}

private:
	skill_type											m_id;
	std::vector<skill_input_type>						m_input_order;
	boost::function<void (skill_type, skill_manager&)>	m_begin_skill_handler;
	boost::function<void (skill_type, skill_manager&)>	m_run_skill_handler;
	boost::function<void (skill_type, skill_manager&)>	m_end_skill_handler;
};

class skill_manager
{
public:
	//! _skill_usable_condition_handler: 判定技能能否使用的handler
	skill_manager(const boost::function<bool (skill_type, skill_manager&)>& _skill_usable_condition_handler)
		: m_skill_usable_condition_handler(_skill_usable_condition_handler)
		, m_current_skill(0)
	{
	}
	~skill_manager()
	{
	}

public:
	//! init skills
	ORZ_PLUS_INLINE void							init						(std::vector<skill>& _skills)
	{
		std::vector<skill>::iterator itr(_skills.begin());
		for (; itr!=_skills.end(); ++itr)
		{
			skill& sk = *itr;
			unsigned in = sk.get_input_num();
			while (in >= m_skills.size())
			{
				m_skills.push_back(std::vector<skill>());
			}
			m_skills[in].push_back(sk);
		}
	}

	//!
	ORZ_PLUS_INLINE skill_type					match_input					(bool _dir, bool _check_order, input_type _it1)
	{
		std::vector<input_type> its;
		its.push_back(_it1);
		return match_input(_dir, _check_order, its);
	}
	//!
	ORZ_PLUS_INLINE skill_type					match_input					(bool _dir, bool _check_order, input_type _it1, input_type _it2)
	{
		std::vector<input_type> its;
		its.push_back(_it1);
		its.push_back(_it2);
		return match_input(_dir, _check_order, its);
	}
	//!
	ORZ_PLUS_INLINE skill_type					match_input					(bool _dir, bool _check_order, input_type _it1, input_type _it2, input_type _it3)
	{
		std::vector<input_type> its;
		its.push_back(_it1);
		its.push_back(_it2);
		its.push_back(_it3);
		return match_input(_dir, _check_order, its);
	}
	//!
	ORZ_PLUS_INLINE skill_type					match_input					(bool _dir, bool _check_order, input_type _it1, input_type _it2, input_type _it3, input_type _it4)
	{
		std::vector<input_type> its;
		its.push_back(_it1);
		its.push_back(_it2);
		its.push_back(_it3);
		its.push_back(_it4);
		return match_input(_dir, _check_order, its);
	}
	//!
	ORZ_PLUS_INLINE skill_type					match_input					(bool _dir, bool _check_order, input_type _it1, input_type _it2, input_type _it3, input_type _it4, input_type _it5)
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
	ORZ_PLUS_INLINE skill_type					match_input					(bool _dir, bool _check_order, input_type _it1, input_type _it2, input_type _it3, input_type _it4, input_type _it5, input_type _it6)
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

	//! _check_order 真表示正序,否表示倒序(所谓序，指得按照按键序列的按键数量由小到大排列的skill们)
	ORZ_PLUS_INLINE skill_type					match_input					(bool _dir, bool _check_order, const std::vector<input_type>& _order)
	{
		std::vector<skill_input_type> sits;
		std::vector<input_type>::const_iterator iitr(_order.begin());
		for (; iitr!=_order.end(); ++iitr)
		{
			sits.push_back(convert_to_skill_input_type(_dir, *iitr));
		}
		skill_type st = SKT_NULL;
		if (sits.empty())
			return st;
		
		if (_check_order)
		{
			std::vector<std::vector<skill> >::iterator aitr(m_skills.begin());
			for (; aitr!=m_skills.end(); ++aitr)
			{
				std::vector<skill>& _skills = *aitr;

				std::vector<skill>::iterator skitr(_skills.begin());
				for (; skitr!=_skills.end(); ++skitr)
				{
					skill& sk = *skitr;
					std::vector<skill_input_type>& skits = sk.get_input_order();
					if (skits.empty())
						continue;

					if (skits.size() > sits.size())
						continue;

					//if (sk.get_input_num() == 1)
					//{
					//	printf("	skill manager::match_input - sk.get_input_num() == 1!\n");
					//	for (unsigned i=0; i<skits.size(); ++i)
					//	{
					//		printf("	skits[" << i << "]: " << skits[i] << std::endl;
					//	}
					//	for (unsigned i=0; i<sits.size(); ++i)
					//	{
					//		printf("	sits[" << i << "]: " << sits[i] << std::endl;
					//	}
					//}
					
					// 检查skits中每个元素是否在sits存在，如果不存在，则匹配直接失败；
					// 如果都存在，则根据记录的元素在sits中的位置，来判断，如果不是按照skits中的顺序，则表明失败
					//if (skits.front() != sits.front())
					//	continue;
					
					// match
					bool is_matched = true;
					std::vector<unsigned> compares;
					std::vector<skill_input_type>::reverse_iterator skits_itr(skits.rbegin());
					for (; skits_itr!=skits.rend(); ++skits_itr)
					{
						bool has_exist = false;
						unsigned sits_or = (unsigned int)sits.size()-1;
						std::vector<skill_input_type>::reverse_iterator sits_itr(sits.rbegin());
						for (; sits_itr!=sits.rend(); --sits_or, ++sits_itr)
						{
							if (*skits_itr == *sits_itr)
							{
								compares.push_back(sits_or);
								has_exist = true;
								break;
							}
						}
						if (!has_exist)
						{
							is_matched = false;
							break;
						}
					}
					if (is_matched)
					{
						std::vector<unsigned>::iterator cpr_itr(compares.begin());
						unsigned last_sit = *cpr_itr;
						for (; cpr_itr!=compares.end(); ++cpr_itr)
						{
							if (*cpr_itr > last_sit)
							{
								is_matched = false;
								break;
							}
							else
								last_sit = *cpr_itr;
						}
					}
					
					if (is_matched)
					{
						st = sk.get_id();
						goto OVER;
					}
				}
			}
		}
		else
		{
			std::vector<std::vector<skill> >::reverse_iterator aitr(m_skills.rbegin());
			for (; aitr!=m_skills.rend(); ++aitr)
			{
				std::vector<skill>& _skills = *aitr;

				std::vector<skill>::iterator skitr(_skills.begin());
				for (; skitr!=_skills.end(); ++skitr)
				{
					skill& sk = *skitr;
					
					std::vector<skill_input_type>& skits = sk.get_input_order();

					if (skits.empty())
						continue;

					if (skits.size() > sits.size())
						continue;

					//if (sk.get_input_num() == 1)
					//{
					//	printf("	skill manager::match_input - sk.get_input_num() == 1!\n");
					//	for (unsigned i=0; i<skits.size(); ++i)
					//	{
					//		printf("	skits[" << i << "]: " << skits[i] << std::endl;
					//	}
					//	for (unsigned i=0; i<sits.size(); ++i)
					//	{
					//		printf("	sits[" << i << "]: " << sits[i] << std::endl;
					//	}
					//}
					
					// 检查skits中每个元素是否在sits存在，如果不存在，则匹配直接失败；
					// 如果都存在，则根据记录的元素在sits中的位置，来判断，如果不是按照skits中的顺序，则表明失败
					//if (skits.front() != sits.front())
					//	continue;
					
					// match
					bool is_matched = true;
					std::vector<unsigned> compares;
					std::vector<skill_input_type>::reverse_iterator skits_itr(skits.rbegin());
					for (; skits_itr!=skits.rend(); ++skits_itr)
					{
						bool has_exist = false;
						unsigned sits_or = (unsigned int)sits.size()-1;
						std::vector<skill_input_type>::reverse_iterator sits_itr(sits.rbegin());
						for (; sits_itr!=sits.rend(); --sits_or, ++sits_itr)
						{
							if (*skits_itr == *sits_itr)
							{
								compares.push_back(sits_or);
								*sits_itr = SIT_NULL;
								has_exist = true;
								break;
							}
						}
						if (!has_exist)
						{
							is_matched = false;
							break;
						}
					}
					if (is_matched)
					{
						std::vector<unsigned>::iterator cpr_itr(compares.begin());
						unsigned last_sit = *cpr_itr;
						for (; cpr_itr!=compares.end(); ++cpr_itr)
						{
							if (*cpr_itr > last_sit)
							{
								is_matched = false;
								break;
							}
							else
								last_sit = *cpr_itr;
						}
					}
					
					if (is_matched)
					{
						st = sk.get_id();
						goto OVER;
					}
				}
			}
		}
OVER:
		return st;
	}

	//! 检查给定的技能是否满足条件
	ORZ_PLUS_INLINE skill_type					check_skill					(skill_type _st)
	{
		if (m_skill_usable_condition_handler(_st, *this))
			return _st;
		else
			return SKT_NULL;
	}

	//!
	ORZ_PLUS_INLINE void							start_skill					(skill_type _st)
	{
		std::vector<std::vector<skill> >::iterator aitr(m_skills.begin());
		for (; aitr!=m_skills.end(); ++aitr)
		{
			std::vector<skill>& _skills = *aitr;
			std::vector<skill>::iterator itr(std::find(_skills.begin(), _skills.end(), _st));
			if (itr != _skills.end())
				m_current_skill = &(*itr);
		}

		if (m_current_skill)
			m_current_skill->invoke_begin_handler(*this);
	}

	//!
	ORZ_PLUS_INLINE void							run_skill					()
	{
		if (m_current_skill)
			m_current_skill->invoke_run_handler(*this);
	}

	//!
	ORZ_PLUS_INLINE void							end_skill					()
	{
		if (m_current_skill)
			m_current_skill->invoke_end_handler(*this);
		m_current_skill = 0;
	}

private:
	//! 根据给定的方向转换 input_type --> skill_input_type  m_dir - true right, false left
	ORZ_PLUS_INLINE skill_input_type				convert_to_skill_input_type	(bool _dir, input_type _it)
	{
		if (_dir)
		{
			switch (_it)
			{
			case IT_PUSH_RIGHT:		return SIT_PUSH_FORWARD;
			case IT_RELEASE_RIGHT:	return SIT_RELEASE_FORWARD;
			case IT_PUSH_LEFT:		return SIT_PUSH_BACK;
			case IT_RELEASE_LEFT:	return SIT_RELEASE_BACK;
			}
		}
		else
		{
			switch (_it)
			{
			case IT_PUSH_RIGHT:		return SIT_PUSH_BACK;
			case IT_RELEASE_RIGHT:	return SIT_RELEASE_BACK;
			case IT_PUSH_LEFT:		return SIT_PUSH_FORWARD;
			case IT_RELEASE_LEFT:	return SIT_RELEASE_FORWARD;
			}
		}
		return (skill_input_type)_it;
	}

	std::vector<std::vector<skill> >								m_skills;
	boost::function<bool (skill_type, skill_manager&)>				m_skill_usable_condition_handler;
	skill*															m_current_skill;
};

}

#endif
