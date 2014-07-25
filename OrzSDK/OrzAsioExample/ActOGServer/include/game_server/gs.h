#ifndef GS_H_
#define GS_H_

#include <orz/OrzAsio/OrzAsioConfig.h>
#include <common/game/game_common.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>

//#define __DEBUG___

namespace Orz
{

static char g_spts_array[] = {'#', '|', '&', ','};
static std::vector<char> g_spts(g_spts_array, g_spts_array + sizeof(g_spts_array) / sizeof(char));
static const std::string null_label = "null";

//ORZ_PLUS_INLINE void display_clock(const char* _str)
//{
//	printf(" ################## - display_clock: " << _str << " - [ " << now() << " ]\n");
//}

struct array2d
{
	array2d() : x(0), y(0) {}
	array2d(int _x, int _y) : x(_x), y(_y) {}
	~array2d() {}

	int			x;
	int			y;
};

struct zone
{
	zone() : x(0), y(0), w(0), h(0) {}
	zone(int _x, int _y, unsigned int _w, unsigned int _h) : x(_x), y(_y), w(_w), h(_h) {}
	~zone() {}

	//! 根据给定的方向（正为右，负为左）和zone所属的坐标系的原点在新坐标系中的坐标，
	//	产生一个新的以给定坐标点的坐标系为基准的zone
	ORZ_PLUS_INLINE zone			convert_by_dir		(bool _dir, int _px, int _py) const
	{
		if (_dir)
			return zone(x+_px, y+_py, w, h);
		else
			return zone(-x-w+_px, y+_py, w, h);
	}

	// 直接比较，假定给定的坐标与这个zone处于同一个坐标系中
	ORZ_PLUS_INLINE bool			is_in				(const int _cx, const int _cy)
	{
		if (_cx >= x && _cx <= x+(int)w && _cy >= y && _cy <= y+(int)h)
			return true;
		else
			return false;
	}

	//!
	ORZ_PLUS_INLINE bool			is_wrap				(const zone& _z)
	{
		// 左上角
		int x1 = _z.x;
		int y1 = _z.y;
		// 右上角
		int x2 = _z.x + _z.w;
		int y2 = _z.y;
		// 右下角
		int x3 = _z.x + _z.w;
		int y3 = _z.y + _z.h;
		// 左下角
		int x4 = _z.x;
		int y4 = _z.y + _z.h;

		if (is_in(x1, y1) || 
			is_in(x2, y2) || 
			is_in(x3, y3) || 
			is_in(x4, y4))
			return true;
		else
			return false;
	}

	//! x 和 y 是长方形的左上点
	int				x;
	int				y;
	unsigned int	w;
	unsigned int	h;
};

//! 用于运动的类型
enum action_move_type
{
	amt_null = 0,					// - 0
	amt_beeline_move,				// 
	amt_falling,					// 
	amt_total
};

//!
enum attack_type
{
	at_null = 0,					// - 0
	at_normal,						//			普通伤害
	at_serious,						//			严重伤害
	at_rise,						//			浮空伤害
	at_total						//			
};

enum damage_type
{
	dmgt_null = 0,					// - 0
	dmgt_normal,					//
	dmgt_serious,					//
	dmgt_rise,						//
	dmgt_total						//
};

enum collision_type
{
	ct_null = 0,					// - 0
	ct_mb,							//			地图阻挡
	ct_total				
};

//!
enum event_type
{
	et_null = 0,					// - 0
	et_pc_attack,					//			玩家的攻击
	et_pc_new_attack,				//			新的玩家攻击
	et_total
};

//! pc's afs
enum afs_pc_type
{
	AFSPCT_IDLE = 0,				// - 0	idle afs
	AFSPCT_MOVE,					//		move afs
	AFSPCT_RUN,						//		run afs
	AFSPCT_ATTACK,					// 
	AFSPCT_SKT10,					// 
	AFSPCT_SKT13,					// - 5
	AFSPCT_FALL,					//		轻度挨打，摔倒和起身等
	AFSPCT_JUMP,					//		jump afs
	AFSPCT_TOTAL
};

static std::string g_apcts[AFSPCT_TOTAL] = 
{
	"IDLE",
	"MOVE",
	"RUN",
	"ATTACK",
	"SKT10",
	"SKT13",
	"FALL",
	"JUMP"
};

//! monster 's afs
enum afs_msr_type
{
	AFSMSRT_IDLE = 0,				// - 0	idle afs
	AFSMSRT_MOVE,					//		move afs
	AFSMSRT_ATTACK,					// 
	AFSMSRT_FALL,					//		轻度挨打，摔倒和起身等
	AFSMSRT_BIRTH,					//		出生动画
	AFSMSRT_TOTAL
};

static std::string g_amsrts[AFSMSRT_TOTAL] = 
{
	"IDLE",
	"MOVE",
	"ATTACK",
	"FALL",
	"BIRTH"
};

//! 对象类型
enum object_type
{
	OT_NULL = 0,						// - 0	空
	OT_PC,								//		玩家
	OT_MSR,								//		怪物
	OT_TOTAL
};

//! 都有哪些键
enum key_value
{
	KV_NULL = 0,						// - 0	
	KV_RIGHT,							//		右
	KV_LEFT,							//		左
	KV_UP,								//		上
	KV_DOWN,							//		下
	KV_ATTACK,							// - 5	普通攻击
	KV_JUMP,							//		跳跃
	KV_SKILL,							//		技能
	KV_TOTAL
};

//! 按键的状态
enum key_state
{
	KS_NULL = 0,						// - 0
	KS_PUSHED,							//		按下 
	KS_RELEASED,						//		抬起
	KS_FORBIDDON,						//		被禁
	KS_TOTAL
};

//! 客户端发送给服务器的输入信息
enum input_type
{
	IT_NULL = 0,				// - 0
	IT_PUSH_UP,					//			按上
	IT_PUSH_RIGHT,				//			按右	
	IT_PUSH_DOWN,				//			按下
	IT_PUSH_LEFT,				//			按左
	IT_RELEASE_UP,				// - 5		抬起上
	IT_RELEASE_RIGHT,			//			抬起右
	IT_RELEASE_DOWN,			//			抬起下
	IT_RELEASE_LEFT,			//			抬起左
	IT_PUSH_ATTACK,				//			按下攻击
	IT_PUSH_JUMP,				// - 10		按下跳
	IT_PUSH_SKILL,				//			按下技能
	IT_RELEASE_ATTACK,			//			抬起攻击
	IT_RELEASE_JUMP,			//			抬起跳
	IT_RELEASE_SKILL,			//			抬起技能
	IT_TOTAL
};

static std::string g_inputs[IT_TOTAL] = 
{
	"IT_NULL",
	"IT_PUSH_UP",
	"IT_PUSH_RIGHT",
	"IT_PUSH_DOWN",
	"IT_PUSH_LEFT",
	"IT_RELEASE_UP",
	"IT_RELEASE_RIGHT",
	"IT_RELEASE_DOWN",
	"IT_RELEASE_LEFT",
	"IT_PUSH_ATTACK",
	"IT_PUSH_JUMP",
	"IT_PUSH_SKILL",
	"IT_RELEASE_ATTACK",
	"IT_RELEASE_JUMP",
	"IT_RELEASE_SKILL"
};

//! motion type
enum motion_type
{
	MT_NULL = 0,				// - 0		
	MT_1,						//			walk up
	MT_2,						//			walk right
	MT_3,						//			walk down
	MT_4,						//			walk left
	MT_5,						//			run up
	MT_6,						//			run right
	MT_7,						//			run down
	MT_8,						//			run left
	MT_9,						//			jump
	MT_TOTAL								
};

//! 
enum motion_input_type
{
	MIT_NULL = 0,				// - 0
	MIT_PUSH_UP,				//			按上
	MIT_PUSH_RIGHT,				//			按右	
	MIT_PUSH_DOWN,				//			按下
	MIT_PUSH_LEFT,				//			按左
	MIT_RELEASE_UP,				// - 5		抬起上
	MIT_RELEASE_RIGHT,			//			抬起右
	MIT_RELEASE_DOWN,			//			抬起下
	MIT_RELEASE_LEFT,			//			抬起左
	MIT_PUSH_ATTACK,			//			按下攻击
	MIT_PUSH_JUMP,				// - 10		按下跳
	MIT_PUSH_SKILL,				//			按下技能
	MIT_RELEASE_ATTACK,			//			抬起攻击
	MIT_RELEASE_JUMP,			//			抬起跳
	MIT_RELEASE_SKILL,			//			抬起技能
	MIT_TOTAL
};

//! 八方向
enum direction_type
{
	DIRT_NULL = 0,				// - 0		
	DIRT_UP,						//			上
	DIRT_RIGHT_UP,				//			右上
	DIRT_RIGHT,					//			右
	DIRT_RIGHT_DOWN,				//			右下
	DIRT_DOWN,					// - 5		下
	DIRT_LEFT_DOWN,				//			左下
	DIRT_LEFT,					//			左
	DIRT_LEFT_UP,					//			左上
	DIRT_TOTAL
};

static std::string g_dirs[DIRT_TOTAL] = 
{
	"DIRT_NULL",
	"DIRT_UP",
	"DIRT_RIGHT_UP",
	"DIRT_RIGHT",
	"DIRT_RIGHT_DOWN",
	"DIRT_DOWN",
	"DIRT_LEFT_DOWN",
	"DIRT_LEFT",
	"DIRT_LEFT_UP"
};

//! 角色状态 
//	切换状态的原则：每个状态机只负责切换属于自身的状态，例如子状态机不会处理父状态机的状态切换事务，遇到这样的输入，则忽略不处理
//	
//			   |--- mrj_sub_state
//	pc_state —|---
//			   |---
enum pc_state
{
	PC_NULL = 0,				// - 0					
	PC_IDLE,					//			待机
	PC_MOVE,					//			行走
	PC_RUN,						//			奔跑
	PC_JUMP,					//			跳跃
	PC_ATTACK_1,				// - 5		攻击一段
	PC_ATTACK_2,				//			攻击二段
	PC_ATTACK_3,				//			攻击三段
	PC_SKILL,					//			技能
	PC_FALL,					//			挨揍
	PC_RISE_UP,					// - 10		到底后起身
	PC_TOTAL
};

static std::string g_pc_states[PC_TOTAL] = 
{
	"PC_NULL",
	"PC_IDLE",
	"PC_MOVE",
	"PC_RUN",
	"PC_JUMP",
	"PC_ATTACK_1",
	"PC_ATTACK_2",
	"PC_ATTACK_3",
	"PC_SKILL",
	"PC_FALL",
	"PC_RISE_UP"
};

// monster state
enum monster_state
{
	MSRS_NULL = 0,				// - 0					
	MSRS_IDLE,					//			待机
	MSRS_MOVE,					//			行走
	MSRS_ATTACK_1,				//			攻击一段
	MSRS_FALL,					//			挨揍
	MSRS_RISE_UP,				// - 10		到底后起身
	MSRS_TOTAL
};

static std::string g_monster_states[MSRS_TOTAL] = 
{
	"MSRS_NULL",
	"MSRS_IDLE",
	"MSRS_MOVE",
	"MSRS_ATTACK_1"
	"MSRS_FALL",
	"MSRS_RISE_UP"
};

// monster mind state
enum msr_mind_state
{
	MSRMS_NULL = 0,				// - 0		
	MSRMS_GUARD,				//			待机
	MSRMS_COMBAT,				//			行走
	MSRMS_TOTAL
};

static std::string g_msr_mind_states[MSRMS_TOTAL] = 
{
	"MSRMS_NULL",
	"MSRMS_GUARD",
	"MSRMS_COMBAT"
};

//! monster 的actions
enum monster_action
{
	MSRACT_NULL = 0,			// - 0
	MSRACT_GUARD,				//			两次三角移动间隔之间的静止状态
	MSRACT_A_TO_B,				//			怪物警戒状态下三角移动的一部分
	MSRACT_B_TO_C,				//			怪物警戒状态下三角移动的一部分
	MSRACT_C_TO_A,				//			怪物警戒状态下三角移动的一部分
	MSRACT_TOTAL
};


//! PC/MSRS_MOVE、PC/MSRS_RUN、PC/MSRS_JUMP的共同的子状态
enum mrj_sub_state
{
	MRJSS_NULL = 0,				// - 0
	MRJSS_RIGHT,				//			向右走/跑/跳
	MRJSS_LEFT,					//			向左走/跑/跳
	MRJSS_UP,					//			向上走/跑/跳
	MRJSS_DOWN,					//			向下走/跑/跳
	MRJSS_RIGHT_UP,				// - 5		向右上走/跑/跳
	MRJSS_LEFT_UP,				//			向左上走/跑/跳
	MRJSS_RIGHT_DOWN,			//			向右下走/跑/跳
	MRJSS_LEFT_DOWN,			//			向左下走/跑/跳
	MRJSS_JUMP_IDLE,			//			jump无方向
	MRJSS_TOTAL
};

static std::string g_sub_states[MRJSS_TOTAL] = 
{
	"MRJSS_NULL",
	"MRJSS_RIGHT",
	"MRJSS_LEFT",
	"MRJSS_UP",
	"MRJSS_DOWN",
	"MRJSS_RIGHT_UP",
	"MRJSS_LEFT_UP",
	"MRJSS_RIGHT_DOWN",
	"MRJSS_LEFT_DOWN"
};

//! action type 连续动作的类型
enum action_type
{
	AT_NULL = 0,				// - 0		
	AT_NORMAL_ATTACK,			//			普通攻击
	AT_TOTAL
};

//! 连续动作的阶段类型
enum action_step
{
	AS_NULL = 0,				// - 0		
	AS_1,						//			第一阶段
	AS_2,						//			第二阶段
	AS_3,						//			第三阶段
	AS_TOTAL
};

//! skill type
enum skill_type
{
	SKT_NULL = 0,				// - 0		
	SKT_10,						//			↓→		+ J
	SKT_13,						//			→↓→		+ J
	SKT_1,						//			J
	SKT_2,
	SKT_3,
	SKT_4,
	SKT_5,
	SKT_6,
	SKT_7,
	SKT_8,
	SKT_9,
	
	SKT_11,						//			→↓←		+ J	x
	SKT_12,						//			↓→↓→	+ J
	
	SKT_14,						//			↓←↓→	+ J x
	SKT_15,						//			↓→↓←	+ J x
	SKT_TOTAL								
};

//! 
enum skill_input_type
{
	SIT_NULL = 0,				// - 0
	SIT_PUSH_UP,				//			按上
	SIT_PUSH_FORWARD,			//			按前	
	SIT_PUSH_DOWN,				//			按下
	SIT_PUSH_BACK,				//			按后
	SIT_RELEASE_UP,				// - 5		抬起上
	SIT_RELEASE_FORWARD,		//			抬起前
	SIT_RELEASE_DOWN,			//			抬起下
	SIT_RELEASE_BACK,			//			抬起后
	SIT_PUSH_ATTACK,			//			按下攻击
	SIT_PUSH_JUMP,				// - 10		按下跳
	SIT_PUSH_SKILL,				//			按下技能
	SIT_RELEASE_ATTACK,			//			抬起攻击
	SIT_RELEASE_JUMP,			//			抬起跳
	SIT_RELEASE_SKILL,			//			抬起技能
	SIT_TOTAL
};

//! 地图网格的类型
enum GridType
{
	GDT_NL = 0,					// - 0		普通
	GDT_BK,						//			阻挡
	GDT_TOTAL		
};

//! 决定方向的状态代码
ORZ_PLUS_INLINE static mrj_sub_state					get_mrj_sub_state_by_kv		(key_value _kv)
{
	if (_kv > KV_DOWN)
		return MRJSS_NULL;

	switch (_kv)
	{
	case KV_RIGHT:		return MRJSS_RIGHT;
	case KV_LEFT:		return MRJSS_LEFT;
	case KV_UP:			return MRJSS_UP;
	case KV_DOWN:		return MRJSS_DOWN;
	}
	return MRJSS_NULL;
}

//! 方向键输入 转换为 方向
ORZ_PLUS_INLINE direction_type				get_direction_by_input		(input_type _it)
{
	if (_it == IT_NULL || _it > IT_PUSH_LEFT)
		return DIRT_NULL;

	switch (_it)
	{
	case IT_PUSH_RIGHT:		return DIRT_RIGHT;
	case IT_PUSH_LEFT:		return DIRT_LEFT;
	case IT_PUSH_UP:		return DIRT_UP;
	case IT_PUSH_DOWN:		return DIRT_DOWN;
	}
	return DIRT_NULL;
}
//! 决定方向的状态代码
ORZ_PLUS_INLINE direction_type				get_direction_by_mrj_ss		(mrj_sub_state _mrj_ss)
{
	switch (_mrj_ss)
	{
	case MRJSS_RIGHT:		return DIRT_RIGHT;
	case MRJSS_LEFT:		return DIRT_LEFT;
	case MRJSS_UP:			return DIRT_UP;
	case MRJSS_DOWN:		return DIRT_DOWN;
	case MRJSS_RIGHT_UP:	return DIRT_RIGHT_UP;
	case MRJSS_LEFT_UP:		return DIRT_LEFT_UP;
	case MRJSS_RIGHT_DOWN:	return DIRT_RIGHT_DOWN;
	case MRJSS_LEFT_DOWN:	return DIRT_LEFT_DOWN;
	}
	return DIRT_NULL;
}

//! 决定方向的状态代码
ORZ_PLUS_INLINE mrj_sub_state					get_mrj_sub_state_by_kv		(key_value _kv1, key_value _kv2)
{
	if (_kv1 > KV_DOWN || _kv2 > KV_DOWN)
		return MRJSS_NULL;

	if ((_kv1 == KV_RIGHT && _kv2 == KV_UP) || (_kv2 == KV_RIGHT && _kv1 == KV_UP))
		return MRJSS_RIGHT_UP;
	if ((_kv1 == KV_LEFT && _kv2 == KV_UP) || (_kv2 == KV_LEFT && _kv1 == KV_UP))
		return MRJSS_LEFT_UP;
	if ((_kv1 == KV_RIGHT && _kv2 == KV_DOWN) || (_kv2 == KV_RIGHT && _kv1 == KV_DOWN))
		return MRJSS_RIGHT_DOWN;
	if ((_kv1 == KV_LEFT && _kv2 == KV_DOWN) || (_kv2 == KV_LEFT && _kv1 == KV_DOWN))
		return MRJSS_LEFT_DOWN;

	return MRJSS_NULL;
}

//! 根据给定的方向转换 input_type --> motion_input_type  m_dir - true right, false left
ORZ_PLUS_INLINE motion_input_type				convert_to_motion_input_type(bool _dir, input_type _it)
{
	if (_dir)
	{
		switch (_it)
		{
		case IT_PUSH_RIGHT:		return MIT_PUSH_RIGHT;
		case IT_RELEASE_RIGHT:	return MIT_RELEASE_RIGHT;
		case IT_PUSH_LEFT:		return MIT_PUSH_LEFT;
		case IT_RELEASE_LEFT:	return MIT_RELEASE_LEFT;
		}
	}
	else
	{
		switch (_it)
		{
		case IT_PUSH_RIGHT:		return MIT_PUSH_LEFT;
		case IT_RELEASE_RIGHT:	return MIT_RELEASE_LEFT;
		case IT_PUSH_LEFT:		return MIT_PUSH_RIGHT;
		case IT_RELEASE_LEFT:	return MIT_RELEASE_RIGHT;
		}
	}
	return (motion_input_type)_it;
}

}	// end of namespace Orz

#endif
