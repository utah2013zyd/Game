#ifndef OBJECT_GENERATOR_MANAGER_H_
#define OBJECT_GENERATOR_MANAGER_H_

#include <orz/OrzAsio/OrzAsioConfig.h>
#include <common/game/pos3d.h>
#include <game_server/gs.h>

class data;

namespace Orz
{

class object_generator_manager;

struct object_common_data
{
	object_common_data(const pos3d& _pos)
		: pos(_pos)
	{
	}

	pos3d			pos;
};

class object_generator
{
	friend class object_generator_manager;
public:
	object_generator(object_type _ot, const object_common_data& _obj_cm_d, const boost::function<int (object_type, object_generator_manager&, object_common_data&, data&)>& _create_handler)
		: m_ot(_ot)
		, m_create_handler(_create_handler)
		, m_obj_common_data(_obj_cm_d)
		, m_is_open(true)
		, m_generate_num(0)
	{
	}
	~object_generator()
	{
	}

public:
	//!
	ORZ_PLUS_INLINE object_type					get_type					() const
	{
		return m_ot;
	}

	//! open
	ORZ_PLUS_INLINE void							open						()
	{
		m_is_open = true;
	}

	//! close
	ORZ_PLUS_INLINE void							close						()
	{
		m_is_open = false;
	}

	//!
	ORZ_PLUS_INLINE bool							is_open						() const
	{
		return m_is_open;
	}

	//!
	ORZ_PLUS_INLINE unsigned long				get_generate_num			() const
	{
		return m_generate_num;
	}

private:
	//!
	ORZ_PLUS_INLINE int32							invoke_create_handler		(object_generator_manager& _obj_genr_mgr, data& _data)
	{
		if (m_is_open)
		{
			int err = m_create_handler(m_ot, _obj_genr_mgr, m_obj_common_data, _data);

			//printf("invoke_create_handler - err == \n");
			++m_generate_num;
			return err;
		}
		return 1;
	}

private:
	//! 
	object_type														m_ot;
	//!
	boost::function<int (object_type, object_generator_manager&, object_common_data&, data&)>	
																	m_create_handler;
	//!
	object_common_data												m_obj_common_data;
	//!
	bool															m_is_open;
	//! 产生过多少次
	unsigned long													m_generate_num;

};

static bool object_generator_less(const object_generator& _o1, const object_generator& _o2)
{
	return _o1.get_generate_num() < _o2.get_generate_num();
}

//!
class object_generator_manager
{
public:
	object_generator_manager(const boost::function<object_generator* (object_type, std::vector<object_generator>&)>& _assign_handler)
		: m_assign_handler(_assign_handler)
	{
	}
	~object_generator_manager()
	{
	}

public:
	//!
	ORZ_PLUS_INLINE void							init						(const std::vector<object_generator>& _obj_genrs)
	{
		std::vector<object_generator>::const_iterator itr(_obj_genrs.begin());
		for (; itr!=_obj_genrs.end(); ++itr)
		{
			const object_generator& obj_genr = *itr;
			std::vector<object_generator> obj_genrs;
			obj_genrs.push_back(obj_genr);
			if (add2map(m_object_generators, obj_genr.get_type(), obj_genrs) == 0)
			{
				//printf("object_generator_manager:init - add2map == 0\n");
				m_object_generators[obj_genr.get_type()].push_back(obj_genr);
			}
		}
	}

public:
	//!
	ORZ_PLUS_INLINE int32							generate_object				(object_type _ot, data& _data)
	{
		std::map<object_type, std::vector<object_generator> >::iterator itr(m_object_generators.find(_ot));
		if (itr == m_object_generators.end())
		{
			printf("generate_object - itr == m_object_generators.end()!\n");
			return 1;
		}

		if (itr->second.empty())
		{
			printf("generate_object - itr == itr->second.empty()!\n");
			return 1;
		}

		object_generator* obj_genr = m_assign_handler(_ot, itr->second);
		if (obj_genr)
			return obj_genr->invoke_create_handler(*this, _data);

		printf("generate_object - itr == !obj_genr!\n");
		return 1;
	}

private:
	//! assign handler, 用户自己选择一个object_generator
	boost::function<object_generator* (object_type, std::vector<object_generator>&)>		m_assign_handler;
	//!
	std::map<object_type, std::vector<object_generator> >									m_object_generators;
};

}

#endif
