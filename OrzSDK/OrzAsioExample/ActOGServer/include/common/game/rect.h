//////////////////////////////////////////////////////////////
//
//      FileName: rect.h
//        Author: ��С��
//       Version: 1.0.0
//          Date: 2008-12-22
//          Time: 15:40:00
//   Description: box�࣬��z��=0ʱ���˻�Ϊһ��rect
//
// History:
//      <author>    <time>      <version>   <descript>
//      ��С��		2008-12-22  1.0.0       add
//////////////////////////////////////////////////////////////
#ifndef RECT_H_
#define RECT_H_

#include <common/game/pos2d.h>
using namespace Orz;

class rect
{
public:
	rect()
		: m_length(0.0f)
		, m_width(0.0f)
	{
	}

	rect(const rect& _other)
		: m_pos(_other.m_pos)
		, m_length(_other.m_length)
		, m_width(_other.m_width)
	{
	}

	rect(float64 _length, float64 _width)
		: m_length(_length)
		, m_width(_width)
	{
	}

	rect(const pos2d& _pos, float64 _length, float64 _width)
		: m_pos(_pos)
		, m_length(_length)
		, m_width(_width)
	{
	}

	~rect()
	{
	}

public:
	//!
	const pos2d&			get_position		() const
	{
		return m_pos;
	}

	float64					get_length			() const
	{
		return m_length;
	}

	float64					get_width			() const
	{
		return m_width;
	}

	//! �ж������ĵ��Ƿ����Լ��ڲ�
	ORZ_PLUS_INLINE bool				is_in				(const pos2d& _left_up, const pos2d& _check_point)
	{
		return true;
	}

	//! �жϸ�����rect�Ƿ����ص�
	ORZ_PLUS_INLINE bool				is_wrap				(const pos2d& _left_up, const pos2d& _other_left_up, const rect& _check_rect)
	{
		return true;
	}

private:
	pos2d					m_pos;
	//! ����x�ᣩ
	float64					m_length;
	//! �ߣ�y�ᣩ
	float64					m_width;

};

#endif
