//////////////////////////////////////////////////////////////
//
//      FileName: key_manager.h
//        Author: 熊小磊
//       Version: 1.0.0
//          Date: 2008-12-15
//          Time: 15:39:00
//   Description: 按键管理器
//
// History:
//      <author>    <time>      <version>   <descript>
//      熊小磊		2008-12-15  1.0.0       add
//////////////////////////////////////////////////////////////
#ifndef KEY_MANAGER_H_
#define KEY_MANAGER_H_

#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Toolkit/MultiMap.hpp>
#include <game_server/gs.h>


namespace Orz
{

class key_manager;

class key
{
public:
	key(input_type _it1, input_type _it2, key_value _id, 
		const boost::function<void (key_value, key_manager&, key_state, input_type)>& _key_push_handler, 
		const boost::function<void (key_value, key_manager&, key_state, input_type)>& _key_release_handler, 
		const boost::function<void (key_value, key_manager&, key_state, input_type)>& _key_forbiddon_handler, 
		const boost::function<void (key_value, key_manager&, key_state, input_type)>& _key_renew_handler)
		: m_it1(_it1)
		, m_it2(_it2)
		, m_id(_id)
		, m_state(KS_RELEASED)
		, m_key_push_handler(_key_push_handler)
		, m_key_release_handler(_key_release_handler)
		, m_key_forbiddon_handler(_key_forbiddon_handler)
		, m_key_renew_handler(_key_renew_handler)
	{
	}
	~key()
	{
	}

public:
	//!
	ORZ_PLUS_INLINE key_value			get_id					() const
	{
		return m_id;
	}

	//!
	ORZ_PLUS_INLINE input_type			get_it1					() const
	{
		return m_it1;
	}

	//!
	ORZ_PLUS_INLINE input_type			get_it2					() const
	{
		return m_it2;
	}

	//! 
	ORZ_PLUS_INLINE key_state			get_state				() const
	{
		return m_state;
	}

	//!
	ORZ_PLUS_INLINE ClockTime			get_last_act_time		() const
	{
		return m_last_act_time;
	}

	//! 0 正常 1失败
	ORZ_PLUS_INLINE int32					push					()
	{
		if (m_state == KS_FORBIDDON)
			return 1;

		m_state = KS_PUSHED;
		m_last_act_time = now();
		return 0;
	}

	//!
	ORZ_PLUS_INLINE int32					release					()
	{
		if (m_state == KS_FORBIDDON)
			return 1;

		m_state = KS_RELEASED;
		m_last_act_time = now();
		return 0;
	}

	//!
	ORZ_PLUS_INLINE void					forbiddon				()
	{
		m_state = KS_FORBIDDON;
		m_last_act_time = now();
	}

	//!
	ORZ_PLUS_INLINE void					renew					()
	{
		m_state = KS_RELEASED;
		m_last_act_time = now();
	}

	//!
	ORZ_PLUS_INLINE void					invoke_push_handler		(key_manager& _key_mgr, input_type _it)
	{
		m_key_push_handler(m_id, _key_mgr, m_state, _it);
	}

	//!
	ORZ_PLUS_INLINE void					invoke_release_handler	(key_manager& _key_mgr, input_type _it)
	{
		m_key_release_handler(m_id, _key_mgr, m_state, _it);
	}

	//!
	ORZ_PLUS_INLINE void					invoke_forbiddon_handler(key_manager& _key_mgr)
	{
		input_type it = IT_NULL;
		m_key_forbiddon_handler(m_id, _key_mgr, m_state, it);
	}

	//!
	ORZ_PLUS_INLINE void					invoke_renew_handler	(key_manager& _key_mgr)
	{
		input_type it = IT_NULL;
		m_key_renew_handler(m_id, _key_mgr, m_state, it);
	}

private:
	key_value											m_id;
	input_type											m_it1;
	input_type											m_it2;
	key_state											m_state;
	//! 最近一次操作的时间
	ClockTime											m_last_act_time;
	//! 
	boost::function<void (key_value, key_manager&, key_state, input_type)>		
														m_key_push_handler;
	boost::function<void (key_value, key_manager&, key_state, input_type)>		
														m_key_release_handler;
	boost::function<void (key_value, key_manager&, key_state, input_type)>		
														m_key_forbiddon_handler;
	boost::function<void (key_value, key_manager&, key_state, input_type)>		
														m_key_renew_handler;
};


static bool key_time_greater(const key& _s1, const key& _s2)
{
	return _s1.get_last_act_time() > _s2.get_last_act_time();
}


class key_manager
{
public:
	key_manager()
	{
	}
	~key_manager()
	{
	}

public:
	//! 初始化有哪些按键
	ORZ_PLUS_INLINE void							init						(const std::vector<key>& _keys)
	{
		std::vector<key>::const_iterator itr(_keys.begin());
		for (; itr!=_keys.end(); ++itr)
		{
			const key& k = *itr;
			m_keys.add(k.get_it1(), k.get_it2(), k.get_id(), k);
		}
	}

	//!
	ORZ_PLUS_INLINE int32							push						(input_type _it1)
	{
		key* k = m_keys.getById1(_it1);
		if (!k)
			return 1;
		int err = k->push();
		k->invoke_push_handler(*this, _it1);
		return err;
	}

	//!
	ORZ_PLUS_INLINE int32							release						(input_type _it2)
	{
		key* k = m_keys.getById2(_it2);
		if (!k)
			return 1;
		int err = k->release();
		k->invoke_release_handler(*this, _it2);
		return err;
	}

	//!
	ORZ_PLUS_INLINE key_value					get_kv_by_it1				(input_type _it1)
	{
		key* k = m_keys.getById1(_it1);
		if (!k)
			return KV_NULL;

		return k->get_id();
	}

	//!
	ORZ_PLUS_INLINE key_value					get_kv_by_it2				(input_type _it2)
	{
		key* k = m_keys.getById2(_it2);
		if (!k)
			return KV_NULL;

		return k->get_id();
	}

	//!
	ORZ_PLUS_INLINE int32							forbiddon_by_kv				(key_value _kv)
	{
		key* k = m_keys.getById3(_kv);
		if (!k)
			return 1;

		k->forbiddon();
		k->invoke_forbiddon_handler(*this);
		return 0;
	}
	
	//!
	ORZ_PLUS_INLINE int32							renew_by_kv					(key_value _kv)
	{
		key* k = m_keys.getById3(_kv);
		if (!k)
			return 1;

		k->renew();
		k->invoke_renew_handler(*this);
		return 0;
	}

	//!
	ORZ_PLUS_INLINE key_state					get_key_state_by_kv			(key_value _kv)
	{
		key* k = m_keys.getById3(_kv);
		if (!k)
			return KS_NULL;

		return k->get_state();
	}

	//! -1 error
	ORZ_PLUS_INLINE ClockTime					get_last_act_time_by_kv		(key_value _kv)
	{
		key* k = m_keys.getById3(_kv);
		if (!k)
			return 0;

		return k->get_last_act_time();
	}

	//! 根据按键的事件时间排列指定状态的按键的序列（从大到小）
	ORZ_PLUS_INLINE std::vector<key_value>		get_order					(key_state _ks, std::vector<key_value>& _range)
	{
		std::vector<key> ks;
		std::map<input_type, key>::iterator kitr(m_keys.begin());
		for (; kitr!=m_keys.end(); ++kitr)
		{
			key* k = &kitr->second;
			std::vector<key_value>::iterator itr(std::find(_range.begin(), _range.end(), k->get_id()));
			if (itr == _range.end())
				continue;
			if (k->get_state() == _ks)
				ks.push_back(*k);
		}
		std::sort(ks.begin(), ks.end(), key_time_greater);
		std::vector<key_value> kvs;
		std::vector<key>::iterator itr(ks.begin());
		for (; itr!=ks.end(); ++itr)
		{
			key& k = *itr;
			kvs.push_back(k.get_id());
		}
		return kvs;
	}

	//! 取得指定状态的按键的数量
	ORZ_PLUS_INLINE unsigned						get_key_num					(key_state _ks)
	{
		unsigned kn = 0;
		std::map<input_type, key>::iterator itr(m_keys.begin());
		for (; itr!=m_keys.end(); ++itr)
		{
			key* k = &itr->second;
			if (k->get_state() == _ks)
				++kn;
		}
		return kn;
	}

private:
	TrebleMap<input_type, input_type, key_value, key>				m_keys;
};

}

#endif
