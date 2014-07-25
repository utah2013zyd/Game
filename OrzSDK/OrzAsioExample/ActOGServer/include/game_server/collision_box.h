#ifndef COLLISION_BOX_H_
#define COLLISION_BOX_H_

#include <orz/OrzAsio/OrzAsioConfig.h>
#include <common/game/pos2d.h>
#include <game_server/gs.h>

namespace Orz
{

//! 
class collision_box
{
public:
	//! _center为碰撞盒的中心 _radius为碰撞盒“半径”（如果是圆形则表示圆的半径，如果是正多形则表示中心到每个边的距离） 目前碰撞盒为正八边形
	collision_box(const zone& _zn)
		: m_raw_box(_zn)
	{
	}

	~collision_box() 
	{
	}

public:
	//! 把给定的点作为raw box的中心点，来更新raw box的坐标
	ORZ_PLUS_INLINE void			update_raw_box		(const pos2d& _curr_pos)
	{
		_curr_pos;
		m_raw_box.x = (int)(_curr_pos.x - m_raw_box.w/2.0f);
		m_raw_box.y = (int)(_curr_pos.y - m_raw_box.h/2.0f);
	}

	/* 坐标系
		  		 |
		  		 |
		  		 |
		---------|--------> x
		  		 |
		  		 |
		  		\|/
				  y
	*/
	//! 八边形
	/*
			8	1

		7			2

		6			3

			5	4
	*/
	ORZ_PLUS_INLINE void			set_bound			(const pos2d& _p1, const pos2d& _p2, const pos2d& _p3, const pos2d& _p4, 
											 const pos2d& _p5, const pos2d& _p6, const pos2d& _p7, const pos2d& _p8)
	{
		m_points[0] = _p1;	m_points[1] = _p2;	m_points[2] = _p3;	m_points[3] = _p4;
		m_points[4] = _p5;	m_points[5] = _p6;	m_points[6] = _p7;	m_points[7] = _p8;
	}

	const zone&			get_curr_raw_box	() const
	{
		return m_raw_box;
	}

	//!
	ORZ_PLUS_INLINE std::vector<pos2d>
						get_check_points_by_dir(direction_type _dt) const
	{
		std::vector<pos2d> rs;
		switch(_dt)
		{
		case DIRT_UP:			{ rs.push_back(m_points[0]);	rs.push_back(m_points[7]);	return rs; }
		case DIRT_RIGHT_UP:	{ rs.push_back(m_points[0]);	rs.push_back(m_points[1]);	return rs; }
		case DIRT_RIGHT:		{ rs.push_back(m_points[1]);	rs.push_back(m_points[2]);	return rs; }
		case DIRT_RIGHT_DOWN:	{ rs.push_back(m_points[2]);	rs.push_back(m_points[3]);	return rs; }
		case DIRT_DOWN:		{ rs.push_back(m_points[3]);	rs.push_back(m_points[4]);	return rs; }
		case DIRT_LEFT_DOWN:	{ rs.push_back(m_points[4]);	rs.push_back(m_points[5]);	return rs; }
		case DIRT_LEFT:		{ rs.push_back(m_points[5]);	rs.push_back(m_points[6]);	return rs; }
		case DIRT_LEFT_UP:	{ rs.push_back(m_points[6]);	rs.push_back(m_points[7]);	return rs; }
		}
		return rs;
	}
	
private:
	pos2d			m_points[8];
	zone			m_raw_box;
};

}

#endif
