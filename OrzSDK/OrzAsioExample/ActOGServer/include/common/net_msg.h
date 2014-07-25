#ifndef __Orz_ActOGServer_net_msg_h__
#define __Orz_ActOGServer_net_msg_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/MemString.hpp>
namespace Orz
{
typedef MemString<PKT_TYPE_SIZE+1> MsgStr;
// msg type
enum msg_id
{
	M_NULL = 0,			// - 0	默认，表示空
	M_INPUT,			//		客户端的输入
	M_MOVE,				//		移动
	M_RUN,				//		奔跑
	M_JUMP,				//		跳跃
	M_SKILL,			// - 5	技能
	M_ACTION,			//		连续技
	M_DAMAGE,			//		伤害
	M_POS,				//		位置信息
	M_LO,				//		
	M_NEWOBJ,			// - 10
	M_LG,				// 
	M_JG,				//
	M_PCJ,				//
	M_TOTAL
};

static MsgStr g_msgs[M_TOTAL] = 
{
	"NULL",			// - 0	M_NULL
	"INPUT",		//		M_INPUT
	"MOVE",			//		M_MOVE
	"RUN",			//		M_RUN
	"JUMP",			//		M_JUMP
	"SKILL",		// - 5	M_SKILL
	"ACTION",		//		M_ACTION
	"DAMAGE",		//		M_DAMAGE
	"POS",			//		M_POS
	"LO",			//		M_LO
	"NEWOBJ",		// 
	"LG",			//
	"JG",
	"PCJ"
};

ORZ_PLUS_INLINE msg_id get_msg(MsgStr _msg)
{
	if (_msg.empty())
		return M_TOTAL;

	if (_msg[0] != '0')
	{
		if (_msg == g_msgs[M_NULL]) return M_NULL;
		if (_msg == g_msgs[M_INPUT]) return M_INPUT;
		if (_msg == g_msgs[M_MOVE]) return M_MOVE;
		if (_msg == g_msgs[M_RUN]) return M_RUN;
		if (_msg == g_msgs[M_JUMP]) return M_JUMP;
		if (_msg == g_msgs[M_SKILL]) return M_SKILL;
		if (_msg == g_msgs[M_ACTION]) return M_ACTION;
		if (_msg == g_msgs[M_DAMAGE]) return M_DAMAGE;
		if (_msg == g_msgs[M_POS]) return M_POS;
		if (_msg == g_msgs[M_LO]) return M_LO;
		if (_msg == g_msgs[M_NEWOBJ]) return M_NEWOBJ;
		if (_msg == g_msgs[M_LG]) return M_LG;
		if (_msg == g_msgs[M_JG]) return M_JG;
		if (_msg == g_msgs[M_PCJ]) return M_PCJ;
	}
	else
	{
		uint32 offset = 0;
		for (uint32 i=0; i<PKT_TYPE_SIZE; ++i)
		{
			if (_msg[i] == '0')
				++offset;
		}
		MsgStr msg(_msg.raw() + offset, PKT_TYPE_SIZE - offset);
		return get_msg(msg);
	}

	return M_TOTAL;
}
}
#endif
