#include "Bullets.h"

using namespace Orz;


Bullet::Bullet(Ogre::SceneManager * sm):
_force(),
_node(NULL),
_ent(NULL),
_active(false)
{
	static int i = 0;

	//(Day4)创建节点
	_node = sm->getRootSceneNode()->createChildSceneNode();
	//(Day4)这里临时用一个球体作为子弹模型，以后再美化
	_ent = sm->createEntity(std::string("bullet") + boost::lexical_cast<std::string>(i++), "sphere.mesh");

	//(Day4)挂接模型实体
	_node->attachObject(_ent);

	//(Day4)缩放一下
	_node->setScale(0.1f, 0.1f, 0.1f);
	//(Day4)先隐藏着 备用
	_ent->setVisible(false);
}


Bullet::~Bullet(void)
{
}
void Bullet::update(void)
{
	//(Day4)进行一个位移，不是正确的物理模型，把力量当速度
	_node->translate(_force);
}
bool Bullet::active(void)
{
	return _active;
}
void Bullet::fire(const Ogre::Vector3 & pos, const Ogre::Vector3 & force)
{
	_node->setPosition(pos);
	_force = force;
	_node->setVisible(true);
	_active = true;
}

bool Bullet::flying(void)
{
	
	return _ent->isVisible();
}
bool Bullet::touch(void)
{
	return true;
}
		
void Bullet::burst(void)
{
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
		
