#include <orz/Framework_Base/System/ISoundManager.h>
#include "EnemyPlanes.h"
using namespace Orz;



//(Day5)这里是为了配合单件工具的设置。
template<> EnemyPlanes* Singleton<EnemyPlanes>::_singleton = NULL;


//(Day3)构造函数
EnemyPlanes::EnemyPlanes(Ogre::SceneManager * sm):
_all(),
_planes(),
_time(0.0f)
{

	//(Day3)创建根节点
	_all = sm->getRootSceneNode()->createChildSceneNode();

	//(Day3)遍历节点并初始化
	for(uint32 i = 0; i< _planes.size(); ++i)
	{

		//(Day3)创建节点
		_planes[i] = _all->createChildSceneNode();

		//(Day3)创建实体（注Ogre中实体不能有相同的名称"name"，所以我们通过i来作为名称的id，这里没有使用Ogre自身的字符串转换工具，是因为boost比较通用）
		Ogre::Entity *ent = sm->createEntity(std::string("head") + boost::lexical_cast<std::string>(i), "ogrehead.mesh");
		//(Day3)放置节点到正确的位置
		_planes[i]->translate((i%4)* 200.0f - 300.0f, 150.0f *(i/4), 0.0f);
		//(Day3)缩放节点
		_planes[i]->scale(2.0f, 2.0f, 2.0f);
		//(Day3)挂接实体
		_planes[i]->attachObject(ent);
		
		//(Day6)在这一课我们临时把包围盒显示关闭
		_planes[i]->showBoundingBox(false);

		//(Day5)设置碰撞掩码，这里与子弹碰撞检查掩码相同，表明可以被子弹击中物体。
		ent->setQueryFlags(0x2);
		//(Day5)通过Any类型设置ID，注意的是，这个ID一定要在本动态库释放前删除，否则无法正确释放。
		ent->setUserAny(Ogre::Any(static_cast<uint32>(i)));
	}
}

EnemyPlanes::~EnemyPlanes(void)
{

}

	
void EnemyPlanes::attackEnemy(uint32 id)
{

	//(Day6)这里我们使用了一个简单的播放音源接口，里面自动的载入，播放一遍，并在每一次循环内检查是否播放完毕，如果播放完毕释放并销毁。
	//(Day6)这种方法比较方便，但需要额外的系统消耗以及更少的可控性。
	ISoundManager::getSingleton().quickPlay("boom.wav");

	//(Day5)在这里如果被击中我们首先设为不显示，然后把碰撞掩码设为0，这样现在的物体既看不见，又打不倒。当然你也可以在这里开始播放爆炸动画。
	_planes[id]->setVisible(false);
	_planes[id]->getAttachedObject(0)->setQueryFlags(0);
}

//更新
void EnemyPlanes::update(void)
{

	//(Day3)每次更新，固定时间间隔0.015秒。
	_time += 0.015f;
	
	//(Day3)分配时间
	float move = 0.0f;
	if(_time < 1.0f)
	{
		move = 1.0f;
	}
	else if(_time < 3.0f)
	{
		move = -1.0f;
	}
	else if(_time < 4.0f)
	{
		move = 1.0f;
	}
	else
	{
		_time = 0.0f;
	}
		
	//(Day3)通过时间设置敌人移动状态
	for(uint32 i = 0; i< _planes.size(); ++i)
	{
		if((i/4) % 2 )
			_planes[i]->translate(move, 0.0f, 0.0f);
		else
			_planes[i]->translate(-move, 0.0f, 0.0f);
	}
	
}