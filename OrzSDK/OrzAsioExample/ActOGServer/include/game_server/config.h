#ifndef CONFIG_H_
#define CONFIG_H_

#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <common/game/pos3d.h>

namespace Orz
{


struct cfg_zone
{
	cfg_zone() {}

	int							x;
	int							y;
	unsigned					w;
	unsigned					h;
};

//! config中的关键帧类型
struct cfg_key_frame
{
	cfg_key_frame() {}

	unsigned int			key_frame_id;
	std::string				label;
	int						dx;
	int						dy;
	unsigned int			frame_times;

	int						has_iz;
	cfg_zone				injure_zone;

	int						has_az;
	cfg_zone				attack_zone;

	unsigned int			attack_y;
};

struct monster_mind_config
{
	monster_mind_config() {}

	//! client 's fps
	unsigned					client_fps;

	//! 警戒状态下怪物随机移动的间隔时间
	float64						guard_move_interval_time;

	//!
	float64						short_side_move_time;
	//!
	float64						long_side_move_time;
};

struct demo_monster_config
{
	demo_monster_config() {}

	//! fps
	unsigned					run_fps;

	cfg_zone					raw_collision_box;
	//float64						colboxr;
	//! y方向的移动速度
	float64						min_move_distance;
	//! x移动速度
	float64						move_speed;

	//! client 's fps
	unsigned					client_fps;

	//! 攻击1持续时间
	float64						attack1_act_time;

	//!
	int							attack1_dmg;

	//! 动画单位关键针时间
	unsigned 					frame_time;

	//! 重力加速度
	float64						g;

	//! 空中被攻击，抛物线最大y向位移
	unsigned int				f_max_atk_y;

	std::vector<std::vector<cfg_key_frame> >		
								cfg_key_frames;
};


struct demo_map_config
{
	demo_map_config() {}

	//! 地图大小
	int							size_x;
	int							size_y;

	//! 地图可移动区域
	cfg_zone					game_zone;

	//! 阻挡区域个数
	unsigned					block_zone_num;
	//! 所有阻挡区域信息
	std::vector<cfg_zone>		block_zones;
};

struct demo_player_config
{
	demo_player_config() {}

	cfg_zone					raw_collision_box;

	//! fps
	unsigned					run_fps;

	//float64						colboxr;
	//! y方向的移动速度
	float64						min_move_distance;
	//! x移动速度
	float64						move_speed;
	//!y方向的奔跑距离
	float64						min_run_distance;
	//! x奔跑速度
	float64						run_speed;
	//!
	float64						jump_high;

	//! client 's fps
	unsigned					client_fps;
	//! 技能出发需要的时间（按键序列最长时间）
	float64						skill_act_time;

	//!
	int							attack1_dmg;
	//!
	int							attack2_dmg;
	//!
	int							attack3_dmg;
	//!
	int							skt10_dmg;
	//!
	int							skt13_dmg;

	//! 动画单位关键针时间
	unsigned 					frame_time;

	//! 重力加速度
	float64						g;
	//! 空中被攻击，抛物线最大y向位移
	unsigned int				f_max_atk_y;

	std::vector<std::vector<cfg_key_frame> >		
								cfg_key_frames;
};

//!
ORZ_PLUS_INLINE int32 load_key_frame(const char* _str, cfg_key_frame& _ckf)
{
	std::vector<std::string> str;
	separateStr(_str, (unsigned int)strlen(_str), '\"', str);
	if (str.size() < 2)
	{
		printf("load_key_frame: str.empty()!\n");
		return 1;
	}

	std::string attrs(str[0]);
	str.clear();
	separateStr(attrs.c_str(), (unsigned int)attrs.size(), '|', str);

	if (str.size() < 6)
	{
		printf("load_key_frame: str.size() < 6!\n");
		return 1;
	}

	// #1 load index
	_ckf.key_frame_id = boost::lexical_cast<unsigned int>(str[0]);

	// #2 load label
	_ckf.label = str[1];

	// #3 load dx dy
	std::string dx;
	std::string dy;
	std::vector<std::string> dxy;
	separateStr(str[2].c_str(), (unsigned int)str[2].size(), ',', dxy);
	if (dxy.size() < 2)
	{
		printf("load_key_frame: dxy.size() < 2!\n");
		return 1;
	}

	for (unsigned i=0; i<dxy[0].size(); ++i)
	{
		if (dxy[0][i] == '(')
			continue;
		else 
			dx.push_back(dxy[0][i]);
	}
	for (unsigned i=0; i<dxy[1].size(); ++i)
	{
		if (dxy[1][i] == ')')
			continue;
		else 
			dy.push_back(dxy[1][i]);
	}
	_ckf.dx = boost::lexical_cast<int>(dx);
	_ckf.dy = boost::lexical_cast<int>(dy);

	// #4 load times
	_ckf.frame_times = boost::lexical_cast<unsigned int>(str[3]);

	// #5 load iz
	std::vector<std::string> iz_v;
	separateStr(str[4].c_str(), (unsigned int)str[4].size(), '&', iz_v);
	if (iz_v.size() < 2)
	{
		printf("load_key_frame: iz_v.size() < 2!\n");
		return 1;
	}

	_ckf.has_iz = boost::lexical_cast<int>(iz_v[0]);

	std::vector<std::string> iz_z_v;
	separateStr(iz_v[1].c_str(), (unsigned int)iz_v[1].size(), ',', iz_z_v);
	if (iz_z_v.size() < 4)
	{
		printf("load_key_frame: iz_z_v.size() < 4!\n");
		return 1;
	}

	std::string iz_x;
	for (unsigned i=0; i<iz_z_v[0].size(); ++i)
	{
		if (iz_z_v[0][i] == '(')
			continue;
		else 
			iz_x.push_back(iz_z_v[0][i]);
	}
	std::string iz_y = iz_z_v[1];
	std::string iz_w = iz_z_v[2];
	std::string iz_h;
	for (unsigned i=0; i<iz_z_v[3].size(); ++i)
	{
		if (iz_z_v[3][i] == ')')
			continue;
		else 
			iz_h.push_back(iz_z_v[3][i]);
	}
	_ckf.injure_zone.x = boost::lexical_cast<int>(iz_x);
	_ckf.injure_zone.y = boost::lexical_cast<int>(iz_y);
	_ckf.injure_zone.w = boost::lexical_cast<unsigned int>(iz_w);
	_ckf.injure_zone.h = boost::lexical_cast<unsigned int>(iz_h);

	// #6 load az
	std::vector<std::string> az_v;
	separateStr(str[5].c_str(), (unsigned int)str[5].size(), '&', az_v);
	if (az_v.size() < 3)
	{
		printf("load_key_frame: az_v.size() < 3!\n");
		return 1;
	}

	_ckf.has_iz = boost::lexical_cast<int>(az_v[0]);

	std::vector<std::string> az_z_v;
	separateStr(az_v[1].c_str(), (unsigned int)az_v[1].size(), ',', az_z_v);
	if (az_z_v.size() < 4)
	{
		printf("load_key_frame: az_z_v.size() < 4!\n");
		return 1;
	}

	std::string az_x;
	for (unsigned i=0; i<az_z_v[0].size(); ++i)
	{
		if (az_z_v[0][i] == '(')
			continue;
		else 
			az_x.push_back(az_z_v[0][i]);
	}
	std::string az_y = az_z_v[1];
	std::string az_w = az_z_v[2];
	std::string az_h;
	for (unsigned i=0; i<az_z_v[3].size(); ++i)
	{
		if (az_z_v[3][i] == ')')
			continue;
		else 
			az_h.push_back(az_z_v[3][i]);
	}

	_ckf.attack_zone.x = boost::lexical_cast<int>(az_x);
	_ckf.attack_zone.y = boost::lexical_cast<int>(az_y);
	_ckf.attack_zone.w = boost::lexical_cast<unsigned int>(az_w);
	_ckf.attack_zone.h = boost::lexical_cast<unsigned int>(az_h);
	_ckf.attack_y = boost::lexical_cast<unsigned int>(az_v[2]);

	return 0;
}

//
//! load key frames
ORZ_PLUS_INLINE int32 load_key_frames(FILE* _fp, const char* _ignore_str, std::vector<cfg_key_frame>& _ckfs)
{
	if (_fp == NULL || _ignore_str == NULL)
		return 1;

	size_t i_num = strlen(_ignore_str);

	unsigned int frame_num = 0;
	fseek(_fp, (long)i_num, SEEK_CUR);
	fscanf(_fp, "_FRAME_NUM %u\n",						&frame_num);

	//printf("frame_num: " << frame_num << std::endl;

	_ckfs.clear();
	
	char lc[512] = "\0";
	for (unsigned i=0; i<frame_num; ++i)
	{
		cfg_key_frame ckf;
		memset(lc, '\0', 512);
		fscanf(_fp, "<key info=\"%s\n", lc);
		load_key_frame(lc, ckf);
		_ckfs.push_back(ckf);
	}
	return 0;
}

struct demo_game_config
{
	demo_game_config() {}

	//PlatInitAttrs				pias;
	unsigned int				max_player;
	//! fps
	unsigned					run_fps;

	pos3d						bornPoint1;
	pos3d						bornPoint2;

	//! 重力加速度
	float64						g;
	//! 空中被攻击，抛物线最大y向位移
	unsigned int				f_max_atk_y;

	//! 动画单位关键针时间
	unsigned 					frame_time;

	// player config
	demo_player_config			dpc;
	//!
	demo_monster_config			dmc;
	//!
	monster_mind_config			mmc;
	//!
	demo_map_config				dmapc;
};


struct gs_gc_config
{
	gs_gc_config() {}

	//! 这个服务器最多能建立多少个游戏副本
	unsigned					max_game_num;

	// game config
	demo_game_config			dgc;
};


struct game_server_config
{
	game_server_config() : run_fps(20) {}

	//! fps
	unsigned					run_fps;

	//! game ctrl
	gs_gc_config				ggc;
};

}

#endif
