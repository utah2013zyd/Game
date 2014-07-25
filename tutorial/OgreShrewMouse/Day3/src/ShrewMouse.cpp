#include "ShrewMouse.h"

//在构造函数中创建场景节点和模型
ShrewMouse::ShrewMouse(const std::string & name, Ogre::SceneManager * sm, Ogre::Vector3 & pos):_isEnable(false),_time(0.f)
{
	using namespace Ogre;
	//通过名称和模型作为参数，创建一个地鼠实体
	_ent = sm->createEntity(name, "ogrehead.mesh");
	//从场景的跟节点上创建一个子节点
	Ogre::SceneNode * sn = sm->getRootSceneNode()->createChildSceneNode(pos);
	//把地鼠实体挂接到这个节点上面
	sn->attachObject(_ent);
	//暂时让这个地鼠实体隐藏（在地洞里面）
	_ent->setVisible(false);
}

//更新函数，我们在这里倒计时，当倒计时为0的时候让实体隐藏
void ShrewMouse::update(float interval)
{
	//每次调用的时候
	_time-=interval;
	//当倒计时小于等于0的时候
	if(_time <=0.f)
	{
		_isEnable = false;
		//隐藏地鼠的头
		_ent->setVisible(false);
	}
}
ShrewMouse::~ShrewMouse(void)
{
	
}




//让“地鼠”显示出来，并把地鼠在地面的倒计时设置为1秒
void ShrewMouse::enable(void)
{
	if(!isEnable())
	{
		_isEnable = true;
		//显示“地鼠”
		_ent->setVisible(true);
		_time = 1.f;
	}
}
bool ShrewMouse::isEnable(void)
{

	return _isEnable;
} 
