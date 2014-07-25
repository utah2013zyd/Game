#include "ShrewMouse.h"

//�ڹ��캯���д��������ڵ��ģ��
ShrewMouse::ShrewMouse(const std::string & name, Ogre::SceneManager * sm, Ogre::Vector3 & pos):_isEnable(false),_time(0.f)
{
	using namespace Ogre;
	//ͨ�����ƺ�ģ����Ϊ����������һ������ʵ��
	_ent = sm->createEntity(name, "ogrehead.mesh");
	//�ӳ����ĸ��ڵ��ϴ���һ���ӽڵ�
	Ogre::SceneNode * sn = sm->getRootSceneNode()->createChildSceneNode(pos);
	//�ѵ���ʵ��ҽӵ�����ڵ�����
	sn->attachObject(_ent);
	//��ʱ���������ʵ�����أ��ڵض����棩
	_ent->setVisible(false);
}

//���º��������������ﵹ��ʱ��������ʱΪ0��ʱ����ʵ������
void ShrewMouse::update(float interval)
{
	//ÿ�ε��õ�ʱ��
	_time-=interval;
	//������ʱС�ڵ���0��ʱ��
	if(_time <=0.f)
	{
		_isEnable = false;
		//���ص����ͷ
		_ent->setVisible(false);
	}
}
ShrewMouse::~ShrewMouse(void)
{
	
}




//�á�������ʾ���������ѵ����ڵ���ĵ���ʱ����Ϊ1��
void ShrewMouse::enable(void)
{
	if(!isEnable())
	{
		_isEnable = true;
		//��ʾ������
		_ent->setVisible(true);
		_time = 1.f;
	}
}
bool ShrewMouse::isEnable(void)
{

	return _isEnable;
} 
