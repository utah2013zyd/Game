#ifndef OBJ_INPUT_QUEUE_H_
#define OBJ_INPUT_QUEUE_H_

#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <game_server/gs.h>
#include <game_server/key_manager.h>


namespace Orz
{

class obj_input_queue;

//! input
class obj_input
{
	friend class obj_input_queue;
public:
	obj_input()
		: input_time(0)
		, ipt(IT_NULL)
		, ks(KS_NULL)
	{
	}

	obj_input(input_type _ipt, key_state _ks)
		: input_time(0)
		, ipt(_ipt)
		, ks(_ks)
	{
	}
	~obj_input()
	{
	}
public:
	ORZ_PLUS_INLINE void			now						()
	{
		input_time = ::now();
	}

	ORZ_PLUS_INLINE ClockTime	get_input_time			() const
	{
		return input_time;
	}

	ORZ_PLUS_INLINE input_type	get_input				() const
	{
		return ipt;
	}

	ORZ_PLUS_INLINE key_state	get_key_state			() const
	{
		return ks;
	}

private:
	ClockTime			input_time;
	input_type			ipt;
	//! 这个输入产生时，对应按键的状态
	key_state			ks;
};

//! pc input queue
class obj_input_queue
{
public:
	obj_input_queue(float64 _queue_time, key_manager& _key_mgr)
		: m_queue_time((ClockTime)(_queue_time*1000000.0f))
		, m_key_mgr(_key_mgr)
	{
	}
	~obj_input_queue()
	{
	}
public:
	//!
	ORZ_PLUS_INLINE void						push					(const obj_input& _pi)
	{
		while (!m_pc_inputs.empty() && _pi.get_input_time() - m_pc_inputs.front().get_input_time() > m_queue_time)
			m_pc_inputs.pop_front();

		m_pc_inputs.push_back(_pi);
	}

	//!
	ORZ_PLUS_INLINE void						clear					()
	{
		m_pc_inputs.clear();
	}

	//! 
	ORZ_PLUS_INLINE bool						match_input				(const float64 _duration_time, bool _is_clear_queue, key_state _ks, input_type _i1)
	{
		std::vector<input_type> order;
		order.push_back(_i1);
		return match_input(_duration_time, _is_clear_queue, _ks, order);
	}

	//!
	ORZ_PLUS_INLINE bool						match_input				(const float64 _duration_time, bool _is_clear_queue, key_state _ks, input_type _i1, input_type _i2)
	{
		std::vector<input_type> order;
		order.push_back(_i1);
		order.push_back(_i2);
		return match_input(_duration_time, _is_clear_queue, _ks, order);
	}

	//!
	ORZ_PLUS_INLINE bool						match_input				(const float64 _duration_time, bool _is_clear_queue, key_state _ks, input_type _i1, input_type _i2, input_type _i3)
	{
		std::vector<input_type> order;
		order.push_back(_i1);
		order.push_back(_i2);
		order.push_back(_i3);
		return match_input(_duration_time, _is_clear_queue, _ks, order);
	}

	//!
	ORZ_PLUS_INLINE bool						match_input				(const float64 _duration_time, bool _is_clear_queue, key_state _ks, input_type _i1, input_type _i2, input_type _i3, input_type _i4)
	{
		std::vector<input_type> order;
		order.push_back(_i1);
		order.push_back(_i2);
		order.push_back(_i3);
		order.push_back(_i4);
		return match_input(_duration_time, _is_clear_queue, _ks, order);
	}

	//!
	ORZ_PLUS_INLINE bool						match_input				(const float64 _duration_time, bool _is_clear_queue, key_state _ks, input_type _i1, input_type _i2, input_type _i3, input_type _i4, input_type _i5)
	{
		std::vector<input_type> order;
		order.push_back(_i1);
		order.push_back(_i2);
		order.push_back(_i3);
		order.push_back(_i4);
		order.push_back(_i5);
		return match_input(_duration_time, _is_clear_queue, _ks, order);
	}

	//!
	ORZ_PLUS_INLINE bool						match_input				(const float64 _duration_time, bool _is_clear_queue, key_state _ks, input_type _i1, input_type _i2, input_type _i3, input_type _i4, input_type _i5, input_type _i6)
	{
		std::vector<input_type> order;
		order.push_back(_i1);
		order.push_back(_i2);
		order.push_back(_i3);
		order.push_back(_i4);
		order.push_back(_i5);
		order.push_back(_i6);
		return match_input(_duration_time, _is_clear_queue, _ks, order);
	}

	//! 对比给定状态的事件（输入）序列 如果是KS_NULL，则判断全部状态的输入
	ORZ_PLUS_INLINE bool						match_input				(const float64 _duration_time, bool _is_clear_queue, key_state _ks, std::vector<input_type>& _order)
	{
		if (_order.size() > m_pc_inputs.size())
			return false;

		std::deque<obj_input> ks_inputs;
		std::deque<obj_input>::iterator itr(m_pc_inputs.begin());
		if (_ks != KS_NULL)
		{
			for (; itr!=m_pc_inputs.end(); ++itr)
			{
				obj_input& pi = *itr;
				key_state ks = pi.get_key_state();
				if (ks == _ks)
					ks_inputs.push_back(pi);
			}
			if (ks_inputs.empty())
				return false;
		}
		else
			ks_inputs = m_pc_inputs;

		float64 dut = _duration_time * 1000000.0f;
		ClockTime dt = (ClockTime)dut;
		ClockTime last_time = ks_inputs.back().get_input_time();

		// find duration inputs
		std::deque<obj_input*> match_inputs;
		std::deque<obj_input>::reverse_iterator ccitr(ks_inputs.rbegin());
		for (; ccitr!=ks_inputs.rend(); ++ccitr)
		{
			obj_input& pi = *ccitr;
			if (last_time - pi.get_input_time() <= dt)
				match_inputs.push_back(&pi);
			else
				break;
		}

		if (match_inputs.size() < _order.size())
			return false;
		
		// match
		std::vector<input_type>::reverse_iterator oitr(_order.rbegin());
		std::deque<obj_input*>::reverse_iterator pitr(match_inputs.rbegin());
		for (; oitr!=_order.rend(); ++pitr,++oitr)
		{
			input_type opi = *oitr;
			obj_input* pi = *pitr;
			if (pi->get_input() != opi)
				return false;
		}

		if (_is_clear_queue)
			m_pc_inputs.clear();
		return true;
	}

	//! 根据给定的时间和按键状态返回输入序列 如果_ks == KS_NULL则表示挑选在给定时间内的所有状态的按键
	ORZ_PLUS_INLINE std::vector<input_type>	get_inputs				(const float64 _duration_time, key_state _ks)
	{
		std::vector<input_type> its;
		std::deque<obj_input> ks_inputs;

		//printf("get_inputs - begin-----------------------\n");

		std::deque<obj_input>::iterator itr(m_pc_inputs.begin());
		if (_ks != KS_NULL)
		{
			for (; itr!=m_pc_inputs.end(); ++itr)
			{
				obj_input& pi = *itr;
				key_state ks = pi.get_key_state();
				//printf("get_inputs - pi: " << pi.get_input() << ", ks: " << ks << std::endl;
				if (ks == _ks)
					ks_inputs.push_back(pi);
			}
		}
		else
			ks_inputs = m_pc_inputs;

		//for (unsigned i=0; i < ks_inputs.size(); ++i)
		//{
		//	printf("get_inputs - ks_inputs[" << i << "]: " << ks_inputs[i].get_input() << std::endl;
		//}
		//printf("get_inputs - end-----------------------\n");

		if (ks_inputs.empty())
			return its;

		float64 dut = _duration_time * 1000000.0f;
		ClockTime dt = (ClockTime)dut;
		ClockTime last_time = ks_inputs.back().get_input_time();

		//printf("get_inputs dt: " << dt << ", last_time: " << last_time << std::endl;

		// find duration inputs
		std::deque<input_type> rits;
		std::deque<obj_input>::reverse_iterator citr = ks_inputs.rbegin();
		for (; citr!=ks_inputs.rend(); ++citr)
		{
			obj_input& pi = *citr;
			//printf("pi.last_time - pi.get_input_time(): " << last_time - pi.get_input_time() << std::endl;
			if (last_time - pi.get_input_time() <= dt)
				rits.push_front(pi.get_input());
			else
				break;
		}
		std::deque<input_type>::iterator ritr(rits.begin());
		for (; ritr!=rits.end(); ++ritr)
		{
			its.push_back(*ritr);
	//		printf("get_inputs - correct input: " << *ritr << std::endl;
		}
		//printf("get_inputs - over \n");
		return its;
	}

private:
	std::deque<obj_input>			m_pc_inputs;
	//! 队列保留多少时间内的输入
	const ClockTime				m_queue_time;
	//!
	key_manager&					m_key_mgr;
};

}

#endif
