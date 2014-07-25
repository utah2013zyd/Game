#include "Bullets.h"
#include <orz/Framework_Base/System/ISoundManager.h>
using namespace Orz;
Bullet::BulletInfo::BulletInfo(float power, Ogre::MovableObject * object, int fighterID):_power(power),_object(object),_fighterID(fighterID)
{
}

float Bullet::BulletInfo::getPower(void) const
{
	return _power;
}


int Bullet::BulletInfo::getFighterID(void) const
{
	return _fighterID;
}






Bullet::Bullet(Ogre::SceneManager * sm, int query):
_force(),
_node(NULL),
_ent(NULL),
_active(false),
_sm(sm),
_query(NULL),
_object(NULL)
{
	
	static int i = 0;

	//3创建节点
	_node = sm->getRootSceneNode()->createChildSceneNode();
	//3这里临时用一个球体作为子弹模型，以后再美化
	_ent = sm->createEntity(std::string("bullet") + boost::lexical_cast<std::string>(i++), "sphere.mesh");

	//4这里把子弹的碰撞检测掩码设置为0，即子弹实体不必检查被别人碰撞
	_ent->setQueryFlags(0);
	//3挂接模型实体
	_node->attachObject(_ent);

	//3缩放一下
	_node->setScale(0.1f, 0.1f, 0.1f);
	//3先因藏着 备用
	_ent->setVisible(false);


	//4我们在这里创建一个球形场景查询器,用于查询场景中和子弹碰撞的物体,第二个参数是场景查询掩码.它将查询指定“and”的掩码的实体。
	_query =  _sm->createSphereQuery(Ogre::Sphere(), query);

	//5我们在这里创建音效
	_sound = ISoundManager::getSingleton().createPlayer("fire.wav");
	//5载入音源
	_sound->load();
}


Bullet::~Bullet(void)
{
	//5在这里释放音源
	_sound->unload();
	//4销毁球形查询器
	_sm->destroyQuery(_query);
}
void Bullet::update(void)
{
	//3进行一个位移，不是正确的物理模型，把力量当速度
	_node->translate(_force);

	//4当子弹的y值大于400的时候，我们认为子弹已经飞出屏幕，在这里收回子弹
	if(Ogre::Math::Abs(_node->getPosition().x) > 512.f||Ogre::Math::Abs(_node->getPosition().z) > 512.f)
	{
		_ent->setVisible(false);
		_active = false;
	}
}
bool Bullet::active(void)
{
	return _active;
}
void Bullet::fire(const Ogre::Vector3 & pos, const Ogre::Vector3 & force, float power)
{
	_power = power;
	//4开火激活子弹
	_node->setPosition(pos);
	float scale = force.length()/5.0f;
	scale = scale/2.0f +0.5f;
	scale = scale/10.f;
	if(scale > 0.f)
	{
		_node->setScale(scale, scale, scale);
	}
	_force = force;
	_ent->setVisible(true);
	_active = true;
	_object = NULL;
	//4播放一次 开火的声音
	_sound->play(1);
	
	
}

bool Bullet::flying(void)
{
	//4如果子弹仍然显示，我们认为仍然在飞行状态中
	return _ent->getVisible();
}

//4检查子弹是否击中敌人
bool Bullet::touch(void)
{

	//4设置一个圆心为子弹位置，半径极小的球体作为子弹碰撞体
	_query->setSphere(Ogre::Sphere(_node->getPosition(), 0.01f));

	//4执行碰撞，并返回结果
	Ogre::SceneQueryResult qres =  _query->execute();
	

	//4如果碰撞结果不是为空
	if(!qres.movables.empty())
    { 

		_object = (*qres.movables.begin());
		////4我们把第一个碰撞物体所储存的ID取出
		//const Ogre::Any& any = (*qres.movables.begin())->getUserAny();
		//
		////4保存ID
		//_fighterID = any.operator()<uint32>();
		//

		//4碰撞成功，返回true
		return true;
	 
	}
	//4否则碰撞失败返回false
	return false;
}
		
void Bullet::burst(void)
{
	//4子弹被引爆，这里我们直接释放子弹，但是我们也可以在这里设置开始播放爆炸动画
	_ent->setVisible(false);
	_active = false;
	
}

BulletManager::BulletManager(Ogre::SceneManager * sm, int n, int query)
{
	//先放置n个子弹
	for(int i = 0; i< n; ++i)
	{
		_bullets.push_back(BulletPtr(new Bullet(sm, query)));
	}
}
void BulletManager::update(void)
{
	//3更新所有激活的子弹（包括飞行核爆炸）
	for(BulletPtrList::iterator it = _bullets.begin(); it != _bullets.end(); ++it)
	{
		if((*it)->active())
			(*it)->update();
	}
}
Bullet * BulletManager::touch(void)
{
	//3遍历所有 飞行的子弹，看看有没有击中的
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
bool BulletManager::fire(const Ogre::Vector3 & pos, const Ogre::Vector3 & force, float power)
{
	//3开火，在没有激活的子弹中选择一个。如果没有子弹可用就返回false;
	for(BulletPtrList::iterator it = _bullets.begin(); it != _bullets.end(); ++it)
	{
		if(!(*it)->active())
		{
			(*it)->fire(pos, force, power);
			return true;
		}
	}
	return false;
}
		
