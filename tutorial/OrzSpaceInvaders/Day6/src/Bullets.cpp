#include "Bullets.h"
#include <orz/Framework_Base/System/ISoundManager.h>
using namespace Orz;


Bullet::Bullet(Ogre::SceneManager * sm):
_force(),
_node(NULL),
_ent(NULL),
_active(false),
_sm(sm),
_query(NULL),
_enemyID(0)
{
	
	static int i = 0;

	//(Day4)创建节点
	_node = sm->getRootSceneNode()->createChildSceneNode();
	//(Day4)这里临时用一个球体作为子弹模型，以后再美化
	_ent = sm->createEntity(std::string("bullet") + boost::lexical_cast<std::string>(i++), "sphere.mesh");

	//(Day5)这里把子弹的碰撞检测掩码设置为0，即子弹实体不必检查被别人碰撞
	_ent->setQueryFlags(0);
	//(Day4)挂接模型实体
	_node->attachObject(_ent);

	//(Day4)缩放一下
	_node->setScale(0.1f, 0.1f, 0.1f);
	//(Day4)先因藏着 备用
	_ent->setVisible(false);


	//(Day5)我们在这里创建一个球形场景查询器,用于查询场景中和子弹碰撞的物体,第二个参数是场景查询掩码.它将查询指定“and”的掩码的实体。
	_query =  _sm->createSphereQuery(Ogre::Sphere(), 0x2);

	//(Day6)我们在这里创建音效
	_sound = ISoundManager::getSingleton().createPlayer("fire.wav");
	//(Day6)载入音源
	_sound->load();
}


Bullet::~Bullet(void)
{
	//(Day6)在这里释放音源
	_sound->unload();
	//(Day5)销毁球形查询器
	_sm->destroyQuery(_query);
}
void Bullet::update(void)
{
	//(Day4)进行一个位移，不是正确的物理模型，把力量当速度
	_node->translate(_force);

	//(Day5)当子弹的y值大于400的时候，我们认为子弹已经飞出屏幕，在这里收回子弹
	if(_node->getPosition().y > 400)
	{
		_ent->setVisible(false);
		_active = false;
	}
}
bool Bullet::active(void)
{
	return _active;
}
void Bullet::fire(const Ogre::Vector3 & pos, const Ogre::Vector3 & force)
{

	//(Day5)开火激活子弹
	_node->setPosition(pos);
	_force = force;
	_ent->setVisible(true);
	_active = true;

	//(Day5)播放一次 开火的声音
	_sound->play(1);
	
	
}

bool Bullet::flying(void)
{
	//(Day5)如果子弹仍然显示，我们认为仍然在飞行状态中
	return _ent->getVisible();
}

//(Day5)检查子弹是否击中敌人
bool Bullet::touch(void)
{

	//(Day5)设置一个圆心为子弹位置，半径极小的球体作为子弹碰撞体
	_query->setSphere(Ogre::Sphere(_node->getPosition(), 0.01f));

	//(Day5)执行碰撞，并返回结果
	Ogre::SceneQueryResult qres =  _query->execute();
	

	//(Day5)如果碰撞结果不是为空
	if(!qres.movables.empty())
    { 
		//(Day5)我们把第一个碰撞物体所储存的ID取出
		const Ogre::Any& any = (*qres.movables.begin())->getUserAny();
		
		//(Day5)保存ID
		_enemyID = any.operator()<uint32>();
		

		//(Day5)碰撞成功，返回true
		return true;
	 
	}
	//(Day5)否则碰撞失败返回false
	return false;
}
		
void Bullet::burst(void)
{
	//(Day5)子弹被引爆，这里我们直接释放子弹，但是我们也可以在这里设置开始播放爆炸动画
	_ent->setVisible(false);
	_active = false;
}

BulletManager::BulletManager(Ogre::SceneManager * sm, int n)
{
	//(Day4)先放置n个子弹
	for(int i = 0; i< n; ++i)
	{
		_bullets.push_back(BulletPtr(new Bullet(sm)));
	}
}
void BulletManager::update(void)
{
	//(Day4)更新所有激活的子弹（包括飞行核爆炸）
	for(BulletPtrList::iterator it = _bullets.begin(); it != _bullets.end(); ++it)
	{
		if((*it)->active())
			(*it)->update();
	}
}
Bullet * BulletManager::touch(void)
{
	//(Day4)遍历所有 飞行的子弹，看看有没有击中的
	for(BulletPtrList::iterator it = _bullets.begin(); it != _bullets.end(); ++it)
	{
		if((*it)->flying())
			if((*it)->touch())
			{
				(*it)->burst();
				return (*it).get();
			}
	}
	return NULL;

}
BulletManager::~BulletManager(void)
{
	
}
bool BulletManager::fire(const Ogre::Vector3 & pos, const Ogre::Vector3 & force)
{
	//(Day4)开火，在没有激活的子弹中选择一个。如果没有子弹可用就返回false;
	for(BulletPtrList::iterator it = _bullets.begin(); it != _bullets.end(); ++it)
	{
		if(!(*it)->active())
		{
			(*it)->fire(pos, force);
			return true;
		}
	}
	return false;
}
		
