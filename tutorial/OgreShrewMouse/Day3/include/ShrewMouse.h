#ifndef __ShrewMouse_H__
#define __ShrewMouse_H__
#include <Ogre.h>

class ShrewMouse
{
public:
	//构造函数，参数为 地鼠实例的名称，场景管理器，地鼠在三维空间的位置。
	ShrewMouse(const std::string & name, Ogre::SceneManager * sm, Ogre::Vector3 & pos);
	~ShrewMouse(void);
	//更新函数，用于让地鼠做相应的动作。参数为两次调用的时间间隔。
	void update(float interval);
	//启动，换句话说让地鼠从洞里面钻出来
	void enable(void);
	//检查是否在第洞外面
	bool isEnable(void);
private:
	//地鼠是否在洞外面
	bool _isEnable;
	//地鼠的实体
	Ogre::Entity * _ent;
	//一个时间值，当这个时间为零的时候地鼠回洞里面。
	float _time;

};

#endif