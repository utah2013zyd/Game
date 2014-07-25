
#ifndef DAMAGE_H_
#define DAMAGE_H_

#include <common/game/physics.h>
#include <game_server/gs.h>

namespace Orz
{

class damage
{
public:
	damage()
		: m_damage(0)
		, m_dmgt(dmgt_null)
		, m_dt(DIRT_NULL)
	{
	}
	damage(int _dmg, damage_type _dmgt, direction_type _dt)
		: m_damage(_dmg)
		, m_dmgt(_dmgt)
		, m_dt(_dt)
	{
	}
	~damage()
	{
	}

public:
	//!
	ORZ_PLUS_INLINE int32				dmg				() const
	{
		return m_damage;
	}

	//!
	ORZ_PLUS_INLINE damage_type		type			() const
	{
		return m_dmgt;
	}

	//!
	ORZ_PLUS_INLINE direction_type	dir				() const
	{
		return m_dt;
	}
private:
	//! 所能造成的伤害
	int						m_damage;
	//! 类型
	damage_type				m_dmgt;
	//! 伤害的方向
	direction_type			m_dt;
};

//! normal damage's beat back distance
ORZ_PLUS_INLINE int32 get_beat_back_by_dmg(const damage& _dmg)
{
	int flag = 0;
	switch (_dmg.type())
	{
	case dmgt_normal:		flag = 20;	break;
	case dmgt_serious:		flag = 20;	break;
	case dmgt_rise:			flag = 1;	break;
	}
	//printf("attack get_beat_back_by_dmg -  flag: " << flag << ", _dmg.type(): " << _dmg.type() << ", _dmg.dmg(): " << _dmg.dmg() << std::endl;
	return _dmg.dmg() + (int)_dmg.type()*flag * 2;
}

//! serious damage 's 抛物线参数 _dt保留参数（现在只有左右，将来可能加入全8个方向）
ORZ_PLUS_INLINE parabola get_parabola_by_dmg(const damage& _dmg, float64 _g, float64 _speed_x, float64 _speed_y, unsigned int _tfps)
{
	//int bt = get_beat_back_by_dmg(_dmg, _at, _dt);
	bool dir = true;
	if (_dmg.dir() != DIRT_RIGHT)
		dir = false;

	_speed_x = fabs(_speed_x);
	_speed_y = fabs(_speed_y);

	if (!dir)
		_speed_x = -_speed_x;

	switch (_dmg.type())
	{
	case dmgt_normal:	return parabola(0.0f, 0.0f, 0.0f, _g, _tfps);
	case dmgt_serious:	return parabola(40.0f, _speed_x, _speed_y, _g, _tfps);
	case dmgt_rise:		return parabola(100.0f, _speed_x, _speed_y, _g, _tfps);
	}
	return parabola(0.0f, 0.0f, 0.0f, _g, _tfps);
}

}

#endif
