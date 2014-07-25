#ifndef DEMOMAP_H_
#define DEMOMAP_H_

#include <orz/OrzAsio/OrzAsioConfig.h>
#include <common/game/rect.h>
#include <game_server/config.h>
#include <game_server/gs.h>
#include <game_server/collision_box.h>

namespace Orz
{

class demo_map
{
public:
	demo_map(demo_map_config& _dmc)
		: m_dmc(&_dmc)
		, m_game_zone(zone(_dmc.game_zone.x, _dmc.game_zone.y, _dmc.game_zone.w, _dmc.game_zone.h))
	{
		std::vector<cfg_zone>::const_iterator itr(m_dmc->block_zones.begin());
		for (; itr!=m_dmc->block_zones.end(); ++itr)
		{
			const cfg_zone& dr = *itr;
			zone zn(dr.x, dr.y, dr.w, dr.h);
			m_block_zones.push_back(zone(dr.x, dr.y, dr.w, dr.h));
		}
	}
	~demo_map()
	{
	}

public:
	//!
	ORZ_PLUS_INLINE const zone&					get_game_zone					() const
	{
		return m_game_zone;
	}

	ORZ_PLUS_INLINE bool							check_raw_box					(const zone& _raw_box) 
	{
		// check map zone
		if (_raw_box.x <= m_game_zone.x ||
			_raw_box.y <= m_game_zone.y ||
			_raw_box.x+_raw_box.w >= m_game_zone.x+m_game_zone.w ||
			_raw_box.y+_raw_box.h >= m_game_zone.y+m_game_zone.h)
			return true;
		
		std::vector<zone>::iterator itr(m_block_zones.begin());
		for (; itr!=m_block_zones.end(); ++itr)
		{
			zone& zn = *itr;
			if (zn.is_wrap(_raw_box))
				return true;
		}
		return false;
	}

	//!
	ORZ_PLUS_INLINE collision_type				check_collision					(direction_type _dt, const collision_box& _cb)
	{
		std::vector<pos2d> check_ps = _cb.get_check_points_by_dir(_dt);
		std::vector<pos2d>::iterator itr(check_ps.begin());
		for (; itr!=check_ps.end(); ++itr)
		{
			pos2d& p = *itr;
			
			if (!m_game_zone.is_in((int)p.x, (int)p.y))
				return ct_mb;

			std::vector<zone>::iterator zitr(m_block_zones.begin());
			for (; zitr!=m_block_zones.end(); ++zitr)
			{
				zone& zn = *zitr;
				if (zn.is_in((int)p.x, (int)p.y))
					return ct_mb;
			}
		}
		return ct_null;
	}

private:
	demo_map_config*								m_dmc;
	zone											m_game_zone;
	std::vector<zone>								m_block_zones;
};

}

#endif
