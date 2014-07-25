#ifndef GAME_H_
#define GAME_H_

class pos2d;

namespace Orz
{

typedef unsigned int game_id;
typedef unsigned int player_id;
typedef unsigned int account_id;

class SessionPxy;
class account;
typedef boost::shared_ptr<account> account_ptr;
class player;
typedef boost::shared_ptr<player> player_ptr;
//class packet;
class IPacket;
typedef boost::shared_ptr<IPacket> IPacketShrPtr;


class game
{
public:
	game() {}
	virtual ~game() {}

public:
	//!
	virtual game_id				get_id					() = 0;

	//! 
	virtual const player_ptr	get_master				() = 0;

	//! 判断房间能否加入
	virtual bool				is_allow_join			() = 0;

	//! 0 成功，1 失败
	virtual int					join					(const account_ptr _pAct) = 0;

	//! 0 成功，1 失败
	virtual int					leave					(const SessionPxy& _sn) = 0;

	//!
	virtual int					set_ready				(player_id _pid, bool _r) = 0;

	//! 开始游戏 0 成功 1 失败
	virtual int					start_game				() = 0;

	virtual void				updatePosition			(const SessionPxy& _sid, int32 _x, int32 _y) = 0;

	//!
	virtual bool				is_gaming				() = 0;

	//!
	virtual bool				is_end					() = 0;

	//!
	virtual int					end_game				() = 0;

	//!
	virtual const player_ptr	get_player				(player_id _pid) = 0;

	virtual const player_id		get_player_id_by_aid	(account_id _aid) = 0;

	virtual const player_id		get_player_id_by_sn		(const SessionPxy& _sid) = 0;

	virtual int					get_grid_xy				(player_id _pid, pos2d& _pos) = 0;

	virtual int					get_player_frame		(player_id _pid, unsigned int& _frame) = 0;

	//!
	virtual unsigned			get_player_num			() = 0;

	//!
	virtual int					get_type				() = 0;

	//!
	virtual int					run						(float64 _dt) = 0;

	//!
	virtual int					set_input				(player_id _pid, int _uipt) = 0;

	//!
	virtual int					broadcast				(IPacketShrPtr _pk, const SessionPxy* _eraser) = 0;

	//!
	virtual void				joinThread				() = 0;

};

typedef boost::shared_ptr<game> game_ptr;

}	// end of namespace Orz

#endif
