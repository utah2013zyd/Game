#include "Bullets.h"

using namespace Orz;


Bullet::Bullet(Ogre::SceneManager * sm):
_force(),
_node(NULL),
_ent(NULL),
_active(false)
{
	static int i = 0;

	//(Day4)�����ڵ�
	_node = sm->getRootSceneNode()->createChildSceneNode();
	//(Day4)������ʱ��һ��������Ϊ�ӵ�ģ�ͣ��Ժ�������
	_ent = sm->createEntity(std::string("bullet") + boost::lexical_cast<std::string>(i++), "sphere.mesh");

	//(Day4)�ҽ�ģ��ʵ��
	_node->attachObject(_ent);

	//(Day4)����һ��
	_node->setScale(0.1f, 0.1f, 0.1f);
	//(Day4)�������� ����
	_ent->setVisible(false);
}


Bullet::~Bullet(void)
{
}
void Bullet::update(void)
{
	//(Day4)����һ��λ�ƣ�������ȷ������ģ�ͣ����������ٶ�
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
	//(Day4)�ȷ���n���ӵ�
	for(int i = 0; i< n; ++i)
	{
		_bullets.push_back(BulletPtr(new Bullet(sm)));
	}
}
void BulletManager::update(void)
{
	//(Day4)�������м�����ӵ����������к˱�ը��
	for(BulletPtrList::iterator it = _bullets.begin(); it != _bullets.end(); ++it)
	{
		if((*it)->active())
			(*it)->update();
	}
}
Bullet * BulletManager::touch(void)
{
	//(Day4)�������� ���е��ӵ���������û�л��е�
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
	//(Day4)������û�м�����ӵ���ѡ��һ�������û���ӵ����þͷ���false;
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
		
