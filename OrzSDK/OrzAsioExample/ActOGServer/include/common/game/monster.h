//////////////////////////////////////////////////////////////
//
//      FileName: monster.h
//        Author: 熊小磊
//       Version: 1.0.0
//          Date: 2008-12-25
//          Time: 14:07:00
//   Description: monster类，抽象基类，所有“房间形式”的怪物均从此类继承
//
// History:
//      <author>    <time>      <version>   <descript>
//      熊小磊		2008-12-25  1.0.0       add
//////////////////////////////////////////////////////////////
#ifndef MONSTER_H_
#define MONSTER_H_

typedef unsigned int monster_id;

class string;
class pos3d;
class event_obj_id;

namespace Orz
{

struct zone;
class collision_box;

class monster
{
public:
	monster() {}
	virtual ~monster() {}

public:
	//!
	virtual monster_id				get_id				() const = 0;

	//!
	virtual const event_obj_id&		get_event_obj_id	() const = 0;
	
	//! 
	virtual const std::string&		get_name			() const = 0;

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

typedef boost::shared_ptr<monster> monster_ptr;

}	// end of namespace Orz

#endif
