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

	//3�����ڵ�
	_node = sm->getRootSceneNode()->createChildSceneNode();
	//3������ʱ��һ��������Ϊ�ӵ�ģ�ͣ��Ժ�������
	_ent = sm->createEntity(std::string("bullet") + boost::lexical_cast<std::string>(i++), "sphere.mesh");

	//4������ӵ�����ײ�����������Ϊ0�����ӵ�ʵ�岻�ؼ�鱻������ײ
	_ent->setQueryFlags(0);
	//3�ҽ�ģ��ʵ��
	_node->attachObject(_ent);

	//3����һ��
	_node->setScale(0.1f, 0.1f, 0.1f);
	//3������� ����
	_ent->setVisible(false);


	//4���������ﴴ��һ�����γ�����ѯ��,���ڲ�ѯ�����к��ӵ���ײ������,�ڶ��������ǳ�����ѯ����.������ѯָ����and���������ʵ�塣
	_query =  _sm->createSphereQuery(Ogre::Sphere(), query);

	//5���������ﴴ����Ч
	_sound = ISoundManager::getSingleton().createPlayer("fire.wav");
	//5������Դ
	_sound->load();
}


Bullet::~Bullet(void)
{
	//5�������ͷ���Դ
	_sound->unload();
	//4�������β�ѯ��
	_sm->destroyQuery(_query);
}
void Bullet::update(void)
{
	//3����һ��λ�ƣ�������ȷ������ģ�ͣ����������ٶ�
	_node->translate(_force);

	//4���ӵ���yֵ����400��ʱ��������Ϊ�ӵ��Ѿ��ɳ���Ļ���������ջ��ӵ�
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
	//4���𼤻��ӵ�
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
	//4����һ�� ���������
	_sound->play(1);
	
	
}

bool Bullet::flying(void)
{
	//4����ӵ���Ȼ��ʾ��������Ϊ��Ȼ�ڷ���״̬��
	return _ent->getVisible();
}

//4����ӵ��Ƿ���е���
bool Bullet::touch(void)
{

	//4����һ��Բ��Ϊ�ӵ�λ�ã��뾶��С��������Ϊ�ӵ���ײ��
	_query->setSphere(Ogre::Sphere(_node->getPosition(), 0.01f));

	//4ִ����ײ�������ؽ��
	Ogre::SceneQueryResult qres =  _query->execute();
	

	//4�����ײ�������Ϊ��
	if(!qres.movables.empty())
    { 

		_object = (*qres.movables.begin());
		////4���ǰѵ�һ����ײ�����������IDȡ��
		//const Ogre::Any& any = (*qres.movables.begin())->getUserAny();
		//
		////4����ID
		//_fighterID = any.operator()<uint32>();
		//

		//4��ײ�ɹ�������true
		return true;
	 
	}
	//4������ײʧ�ܷ���false
	return false;
}
		
void Bullet::burst(void)
{
	//4�ӵ�����������������ֱ���ͷ��ӵ�����������Ҳ�������������ÿ�ʼ���ű�ը����
	_ent->setVisible(false);
	_active = false;
	
}

BulletManager::BulletManager(Ogre::SceneManager * sm, int n, int query)
{
	//�ȷ���n���ӵ�
	for(int i = 0; i< n; ++i)
	{
		_bullets.push_back(BulletPtr(new Bullet(sm, query)));
	}
}
void BulletManager::update(void)
{
	//3�������м�����ӵ����������к˱�ը��
	for(BulletPtrList::iterator it = _bullets.begin(); it != _bullets.end(); ++it)
	{
		if((*it)->active())
			(*it)->update();
	}
}
Bullet * BulletManager::touch(void)
{
	//3�������� ���е��ӵ���������û�л��е�
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
	//3������û�м�����ӵ���ѡ��һ�������û���ӵ����þͷ���false;
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
		
