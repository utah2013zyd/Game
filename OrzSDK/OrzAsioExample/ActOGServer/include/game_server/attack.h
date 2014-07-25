#ifndef ATTACK_H_
#define ATTACK_H_
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <game_server/gs.h>


namespace Orz
{


class attack
{
public:
	attack()
		: m_damage(0)
		, m_at(at_null)
		, m_dt(DIRT_NULL)
	{
	}
	attack(int _dmg, attack_type _at, direction_type _dt)
		: m_damage(_dmg)
		, m_at(_at)
		, m_dt(_dt)
	{
	}
	~attack()
	{
	}

public:
	//!
	ORZ_PLUS_INLINE int32				dmg				() const
	{
		return m_damage;
	}

	//!
	ORZ_PLUS_INLINE attack_type		type			() const
	{
		return m_at;
	}

	//!
	ORZ_PLUS_INLINE direction_type	dir				() const
	{
		return m_dt;
	}

private:
	//! 所能造成的伤害
	int						m_damage;
	//! 攻击类型
	attack_type				m_at;
	//! 攻击的方向
	direction_type			m_dt;
};

}

#endif

