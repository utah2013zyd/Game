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

	//(Day4)�����ڵ�
	_node = sm->getRootSceneNode()->createChildSceneNode();
	//(Day4)������ʱ��һ��������Ϊ�ӵ�ģ�ͣ��Ժ�������
	_ent = sm->createEntity(std::string("bullet") + boost::lexical_cast<std::string>(i++), "sphere.mesh");

	//(Day5)������ӵ�����ײ�����������Ϊ0�����ӵ�ʵ�岻�ؼ�鱻������ײ
	_ent->setQueryFlags(0);
	//(Day4)�ҽ�ģ��ʵ��
	_node->attachObject(_ent);

	//(Day4)����һ��
	_node->setScale(0.1f, 0.1f, 0.1f);
	//(Day4)������� ����
	_ent->setVisible(false);


	//(Day5)���������ﴴ��һ�����γ�����ѯ��,���ڲ�ѯ�����к��ӵ���ײ������,�ڶ��������ǳ�����ѯ����.������ѯָ����and���������ʵ�塣
	_query =  _sm->createSphereQuery(Ogre::Sphere(), 0x2);

	//(Day6)���������ﴴ����Ч
	_sound = ISoundManager::getSingleton().createPlayer("fire.wav");
	//(Day6)������Դ
	_sound->load();
}


Bullet::~Bullet(void)
{
	//(Day6)�������ͷ���Դ
	_sound->unload();
	//(Day5)�������β�ѯ��
	_sm->destroyQuery(_query);
}
void Bullet::update(void)
{
	//(Day4)����һ��λ�ƣ�������ȷ������ģ�ͣ����������ٶ�
	_node->translate(_force);

	//(Day5)���ӵ���yֵ����400��ʱ��������Ϊ�ӵ��Ѿ��ɳ���Ļ���������ջ��ӵ�
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

	//(Day5)���𼤻��ӵ�
	_node->setPosition(pos);
	_force = force;
	_ent->setVisible(true);
	_active = true;

	//(Day5)����һ�� ���������
	_sound->play(1);
	
	
}

bool Bullet::flying(void)
{
	//(Day5)����ӵ���Ȼ��ʾ��������Ϊ��Ȼ�ڷ���״̬��
	return _ent->getVisible();
}

//(Day5)����ӵ��Ƿ���е���
bool Bullet::touch(void)
{

	//(Day5)����һ��Բ��Ϊ�ӵ�λ�ã��뾶��С��������Ϊ�ӵ���ײ��
	_query->setSphere(Ogre::Sphere(_node->getPosition(), 0.01f));

	//(Day5)ִ����ײ�������ؽ��
	Ogre::SceneQueryResult qres =  _query->execute();
	

	//(Day5)�����ײ�������Ϊ��
	if(!qres.movables.empty())
    { 
		//(Day5)���ǰѵ�һ����ײ�����������IDȡ��
		const Ogre::Any& any = (*qres.movables.begin())->getUserAny();
		
		//(Day5)����ID
		_enemyID = any.operator()<uint32>();
		

		//(Day5)��ײ�ɹ�������true
		return true;
	 
	}
	//(Day5)������ײʧ�ܷ���false
	return false;
}
		
void Bullet::burst(void)
{
	//(Day5)�ӵ�����������������ֱ���ͷ��ӵ�����������Ҳ�������������ÿ�ʼ���ű�ը����
	_ent->setVisible(false);
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
		
