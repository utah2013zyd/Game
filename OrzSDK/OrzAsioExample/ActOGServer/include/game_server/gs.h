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

	//! ���ݸ����ķ�����Ϊ�ң���Ϊ�󣩺�zone����������ϵ��ԭ����������ϵ�е����꣬
	//	����һ���µ��Ը�������������ϵΪ��׼��zone
	ORZ_PLUS_INLINE zone			convert_by_dir		(bool _dir, int _px, int _py) const
	{
		if (_dir)
			return zone(x+_px, y+_py, w, h);
		else
			return zone(-x-w+_px, y+_py, w, h);
	}

	// ֱ�ӱȽϣ��ٶ����������������zone����ͬһ������ϵ��
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
		// ���Ͻ�
		int x1 = _z.x;
		int y1 = _z.y;
		// ���Ͻ�
		int x2 = _z.x + _z.w;
		int y2 = _z.y;
		// ���½�
		int x3 = _z.x + _z.w;
		int y3 = _z.y + _z.h;
		// ���½�
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

	//! x �� y �ǳ����ε����ϵ�
	int				x;
	int				y;
	unsigned int	w;
	unsigned int	h;
};

//! �����˶�������
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
	at_normal,						//			��ͨ�˺�
	at_serious,						//			�����˺�
	at_rise,						//			�����˺�
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
	ct_mb,							//			��ͼ�赲
	ct_total				
};

//!
enum event_type
{
	et_null = 0,					// - 0
	et_pc_attack,					//			��ҵĹ���
	et_pc_new_attack,				//			�µ���ҹ���
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
	AFSPCT_FALL,					//		��Ȱ���ˤ���������
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
	AFSMSRT_FALL,					//		��Ȱ���ˤ���������
	AFSMSRT_BIRTH,					//		��������
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

//! ��������
enum object_type
{
	OT_NULL = 0,						// - 0	��
	OT_PC,								//		���
	OT_MSR,								//		����
	OT_TOTAL
};

//! ������Щ��
enum key_value
{
	KV_NULL = 0,						// - 0	
	KV_RIGHT,							//		��
	KV_LEFT,							//		��
	KV_UP,								//		��
	KV_DOWN,							//		��
	KV_ATTACK,							// - 5	��ͨ����
	KV_JUMP,							//		��Ծ
	KV_SKILL,							//		����
	KV_TOTAL
};

//! ������״̬
enum key_state
{
	KS_NULL = 0,						// - 0
	KS_PUSHED,							//		���� 
	KS_RELEASED,						//		̧��
	KS_FORBIDDON,						//		����
	KS_TOTAL
};

//! �ͻ��˷��͸���������������Ϣ
enum input_type
{
	IT_NULL = 0,				// - 0
	IT_PUSH_UP,					//			����
	IT_PUSH_RIGHT,				//			����	
	IT_PUSH_DOWN,				//			����
	IT_PUSH_LEFT,				//			����
	IT_RELEASE_UP,				// - 5		̧����
	IT_RELEASE_RIGHT,			//			̧����
	IT_RELEASE_DOWN,			//			̧����
	IT_RELEASE_LEFT,			//			̧����
	IT_PUSH_ATTACK,				//			���¹���
	IT_PUSH_JUMP,				// - 10		������
	IT_PUSH_SKILL,				//			���¼���
	IT_RELEASE_ATTACK,			//			̧�𹥻�
	IT_RELEASE_JUMP,			//			̧����
	IT_RELEASE_SKILL,			//			̧����
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
	MIT_PUSH_UP,				//			����
	MIT_PUSH_RIGHT,				//			����	
	MIT_PUSH_DOWN,				//			����
	MIT_PUSH_LEFT,				//			����
	MIT_RELEASE_UP,				// - 5		̧����
	MIT_RELEASE_RIGHT,			//			̧����
	MIT_RELEASE_DOWN,			//			̧����
	MIT_RELEASE_LEFT,			//			̧����
	MIT_PUSH_ATTACK,			//			���¹���
	MIT_PUSH_JUMP,				// - 10		������
	MIT_PUSH_SKILL,				//			���¼���
	MIT_RELEASE_ATTACK,			//			̧�𹥻�
	MIT_RELEASE_JUMP,			//			̧����
	MIT_RELEASE_SKILL,			//			̧����
	MIT_TOTAL
};

//! �˷���
enum direction_type
{
	DIRT_NULL = 0,				// - 0		
	DIRT_UP,						//			��
	DIRT_RIGHT_UP,				//			����
	DIRT_RIGHT,					//			��
	DIRT_RIGHT_DOWN,				//			����
	DIRT_DOWN,					// - 5		��
	DIRT_LEFT_DOWN,				//			����
	DIRT_LEFT,					//			��
	DIRT_LEFT_UP,					//			����
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

//! ��ɫ״̬ 
//	�л�״̬��ԭ��ÿ��״̬��ֻ�����л����������״̬��������״̬�����ᴦ��״̬����״̬�л������������������룬����Բ�����
//	
//			   |--- mrj_sub_state
//	pc_state ��|---
//			   |---
enum pc_state
{
	PC_NULL = 0,				// - 0					
	PC_IDLE,					//			����
	PC_MOVE,					//			����
	PC_RUN,						//			����
	PC_JUMP,					//			��Ծ
	PC_ATTACK_1,				// - 5		����һ��
	PC_ATTACK_2,				//			��������
	PC_ATTACK_3,				//			��������
	PC_SKILL,					//			����
	PC_FALL,					//			����
	PC_RISE_UP,					// - 10		���׺�����
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
	MSRS_IDLE,					//			����
	MSRS_MOVE,					//			����
	MSRS_ATTACK_1,				//			����һ��
	MSRS_FALL,					//			����
	MSRS_RISE_UP,				// - 10		���׺�����
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
	MSRMS_GUARD,				//			����
	MSRMS_COMBAT,				//			����
	MSRMS_TOTAL
};

static std::string g_msr_mind_states[MSRMS_TOTAL] = 
{
	"MSRMS_NULL",
	"MSRMS_GUARD",
	"MSRMS_COMBAT"
};

//! monster ��actions
enum monster_action
{
	MSRACT_NULL = 0,			// - 0
	MSRACT_GUARD,				//			���������ƶ����֮��ľ�ֹ״̬
	MSRACT_A_TO_B,				//			���ﾯ��״̬�������ƶ���һ����
	MSRACT_B_TO_C,				//			���ﾯ��״̬�������ƶ���һ����
	MSRACT_C_TO_A,				//			���ﾯ��״̬�������ƶ���һ����
	MSRACT_TOTAL
};


//! PC/MSRS_MOVE��PC/MSRS_RUN��PC/MSRS_JUMP�Ĺ�ͬ����״̬
enum mrj_sub_state
{
	MRJSS_NULL = 0,				// - 0
	MRJSS_RIGHT,				//			������/��/��
	MRJSS_LEFT,					//			������/��/��
	MRJSS_UP,					//			������/��/��
	MRJSS_DOWN,					//			������/��/��
	MRJSS_RIGHT_UP,				// - 5		��������/��/��
	MRJSS_LEFT_UP,				//			��������/��/��
	MRJSS_RIGHT_DOWN,			//			��������/��/��
	MRJSS_LEFT_DOWN,			//			��������/��/��
	MRJSS_JUMP_IDLE,			//			jump�޷���
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

//! action type ��������������
enum action_type
{
	AT_NULL = 0,				// - 0		
	AT_NORMAL_ATTACK,			//			��ͨ����
	AT_TOTAL
};

//! ���������Ľ׶�����
enum action_step
{
	AS_NULL = 0,				// - 0		
	AS_1,						//			��һ�׶�
	AS_2,						//			�ڶ��׶�
	AS_3,						//			�����׶�
	AS_TOTAL
};

//! skill type
enum skill_type
{
	SKT_NULL = 0,				// - 0		
	SKT_10,						//			����		+ J
	SKT_13,						//			������		+ J
	SKT_1,						//			J
	SKT_2,
	SKT_3,
	SKT_4,
	SKT_5,
	SKT_6,
	SKT_7,
	SKT_8,
	SKT_9,
	
	SKT_11,						//			������		+ J	x
	SKT_12,						//			��������	+ J
	
	SKT_14,						//			��������	+ J x
	SKT_15,						//			��������	+ J x
	SKT_TOTAL								
};

//! 
enum skill_input_type
{
	SIT_NULL = 0,				// - 0
	SIT_PUSH_UP,				//			����
	SIT_PUSH_FORWARD,			//			��ǰ	
	SIT_PUSH_DOWN,				//			����
	SIT_PUSH_BACK,				//			����
	SIT_RELEASE_UP,				// - 5		̧����
	SIT_RELEASE_FORWARD,		//			̧��ǰ
	SIT_RELEASE_DOWN,			//			̧����
	SIT_RELEASE_BACK,			//			̧���
	SIT_PUSH_ATTACK,			//			���¹���
	SIT_PUSH_JUMP,				// - 10		������
	SIT_PUSH_SKILL,				//			���¼���
	SIT_RELEASE_ATTACK,			//			̧�𹥻�
	SIT_RELEASE_JUMP,			//			̧����
	SIT_RELEASE_SKILL,			//			̧����
	SIT_TOTAL
};

//! ��ͼ���������
enum GridType
{
	GDT_NL = 0,					// - 0		��ͨ
	GDT_BK,						//			�赲
	GDT_TOTAL		
};

//! ���������״̬����
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

//! ��������� ת��Ϊ ����
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
//! ���������״̬����
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

//! ���������״̬����
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

//! ���ݸ����ķ���ת�� input_type --> motion_input_type  m_dir - true right, false left
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
