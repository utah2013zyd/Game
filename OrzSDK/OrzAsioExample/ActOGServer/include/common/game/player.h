//////////////////////////////////////////////////////////////
//
//      FileName: player.h
//        Author: 熊小磊
//       Version: 1.0.0
//          Date: 2008-10-13
//          Time: 10:58:00
//   Description: Player类，抽象基类，所有“房间形式”的玩家均从此类继承
//
// History:
//      <author>    <time>      <version>   <descript>
//      熊小磊		2008-10-13  1.0.0       add
//////////////////////////////////////////////////////////////
#ifndef PLAYER_H_
#define PLAYER_H_

typedef unsigned int player_id;
typedef unsigned int account_id;

class string;
class pos3d;
class event_obj_id;


namespace Orz
{

class SessionPxy;
struct zone;
class collision_box;

class player
{
public:
	player() {}
	virtual ~player() {}

public:
	//!
	virtual player_id				get_id				() const = 0;

	//!
	virtual const event_obj_id&		get_event_obj_id	() const = 0;
	
	//! 
	virtual account_id				get_account_id		() const = 0;
	
	//!
	virtual const SessionPxy&	get_session_proxy	() const = 0;
	
	//! 
	virtual const std::string&		get_name			() const = 0;

	//!
	virtual const std::string&		get_nick_name		() const = 0;
	
	//!
	virtual int						get_user_type		() const = 0;

	//!
	virtual int						run					(float64 _dt) = 0;

	//!
	virtual int						set_input			(int _uipt) = 0;

	//!
	virtual const pos3d&			get_position		() const = 0;

	//!
	virtual const pos3d&			next_position		() = 0;

	//!
	virtual const bool				get_direction		() const = 0;

	//!
	virtual const zone*				get_current_iz		() const = 0;

	//!
	virtual const int				pop_event			() = 0;

	//!
	virtual void					plat_collision		(int _collision) = 0;

	//!
	virtual collision_box&			get_collision_box	() = 0;

	//!
	virtual int						get_collision		() const = 0;

};

typedef boost::shared_ptr<player> player_ptr;

}	// end of namespace Orz

#endif
