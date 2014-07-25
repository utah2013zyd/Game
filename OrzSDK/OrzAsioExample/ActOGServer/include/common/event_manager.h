//////////////////////////////////////////////////////////////
//
//      FileName: event_manager.h
//        Author: 熊小磊
//       Version: 1.0.0
//          Date: 2008-12-31
//          Time: 10:51:00
//   Description: 事件管理器，用于事件的触发、回调函数的管理
//
// History:
//      <author>    <time>      <version>   <descript>
//      熊小磊		2008-12-31  1.0.0       add
//////////////////////////////////////////////////////////////
#ifndef EVENT_MANAGER_H_
#define EVENT_MANAGER_H_

#include <orz/OrzAsio/OrzAsioConfig.h>
#include <common/static_any.h>

#define EMAS 64

typedef unsigned int event_id;			// 事件id（类型）
typedef unsigned int obj_type;			// 对象类型
typedef unsigned int obj_id;			// 对象id

class event_manager;


//! 用于标识事件对象的id
class event_obj_id
{
public:
	event_obj_id()
		: m_ot(0)
		, m_oi(0)
	{
	}

	event_obj_id(obj_type _ot, obj_id _oi)
		: m_ot(_ot)
		, m_oi(_oi)
	{
	}
	~event_obj_id()
	{
	}

public:
	bool				operator==			(const event_obj_id& _other) const
	{
		if (this == &_other)
			return true;

		if (m_ot == _other.m_ot && m_oi == _other.m_oi)
			return true;
		else
			return false;
	}

	inline obj_type		get_obj_type		() const
	{
		return m_ot;
	}

	inline obj_id		get_obj_id			() const
	{
		return m_oi;
	}

	inline bool			is_valid			() const
	{
		if (m_ot == 0 || m_oi == 0)
			return false;
		else
			return true;
	}

private:
	obj_type					m_ot;
	obj_id						m_oi;
};

class event_obj_id_less
{
public:
	bool operator() (const event_obj_id& _eoi1, const event_obj_id& _eoi2) const
	{
		if (_eoi1.get_obj_type() < _eoi2.get_obj_type())
			return true;
		else if (_eoi1.get_obj_type() == _eoi2.get_obj_type())
		{
			if (_eoi1.get_obj_id() < _eoi2.get_obj_id())
				return true;
			else
				return false;
		}
		else
			return false;
	}
};

//! 事件对象
class event_obj
{
public:
	event_obj(const event_obj_id& _id, 
			  const boost::function<void (const event_obj_id&, event_manager&, const event_obj_id&, event_id, std::vector<static_any<EMAS> >&)>& _recv_event_handler)
		: m_id(_id)
		, m_recv_event_handler(_recv_event_handler)
	{
	}
	~event_obj()
	{
	}

public:
	//!
	inline const event_obj_id&					get_id						() const
	{
		return m_id;
	}

	//!
	inline void									invoke_handler				(const event_obj_id& _sender, event_manager& _event_mgr, event_id _eid, std::vector<static_any<EMAS> >& _datas)
	{
		m_recv_event_handler(_sender, _event_mgr, m_id, _eid, _datas);
	}

private:
	event_obj_id								m_id;
	boost::function<void (const event_obj_id&, event_manager&, const event_obj_id&, event_id, std::vector<static_any<EMAS> >&)>		
												m_recv_event_handler;
};

//!
class event_manager
{
public:
	event_manager(const boost::function<void (const event_obj_id&, event_manager&, event_id, std::vector<static_any<EMAS> >&)>& _master_recv_handler)
		: m_master_recv_handler(_master_recv_handler)
	{
	}
	~event_manager()
	{
	}

public:
	//!
	inline void						create_event_obj			(const event_obj_id& _id, 
																 const boost::function<void (const event_obj_id&, event_manager&, const event_obj_id&, event_id, std::vector<static_any<EMAS> >&)>& _recv_event_handler)
	{
		event_obj eo(_id, _recv_event_handler);
		if (add2map(m_event_objs, _id, eo) == 0)
			std::cout << "create_event_obj - add2map failed!" << std::endl;
	}

	//!
	inline void						erase_event_obj				(const event_obj_id& _id)
	{
		rmv4mMap(m_event_objs, _id);
	}

	//! slave to master
	inline void						push_event					(const event_obj_id& _sender, event_id _eid, std::vector<static_any<EMAS> >& _datas)
	{
		std::map<event_obj_id, event_obj, event_obj_id_less>::iterator itr(m_event_objs.find(_sender));
		if (itr == m_event_objs.end())
		{
			std::cout << "push_event - find(_sender) failed! m_event_objs.size(): " << m_event_objs.size() << std::endl;
			return;
		}

		m_master_recv_handler(_sender, *this, _eid, _datas);
	}

public:
	//!
	inline void						send						(const event_obj_id& _sender, const event_obj_id& _target, event_id _eid, std::vector<static_any<EMAS> >& _datas)
	{
		std::map<event_obj_id, event_obj, event_obj_id_less>::iterator itr(m_event_objs.find(_target));
		if (itr == m_event_objs.end())
			return;

		itr->second.invoke_handler(_sender, *this, _eid, _datas);
	}

	//! 
	inline void						broadcast					(const event_obj_id& _sender, event_id _eid, std::vector<static_any<EMAS> >& _datas, const std::vector<event_obj_id>& _erases)
	{
		std::map<event_obj_id, event_obj, event_obj_id_less>::iterator itr(m_event_objs.begin());
		for (; itr!=m_event_objs.end(); ++itr)
		{
			const event_obj_id& eoi = itr->first;
			std::vector<event_obj_id>::const_iterator citr(std::find(_erases.begin(), _erases.end(), eoi));
			if (citr != _erases.end())
				continue;
			else
				send(_sender, eoi, _eid, _datas);
		}
	}
	
private:
	std::map<event_obj_id, event_obj, event_obj_id_less>		m_event_objs;
	boost::function<void (const event_obj_id&, event_manager&, event_id, std::vector<static_any<EMAS> >&)>
																m_master_recv_handler;
};


#endif
