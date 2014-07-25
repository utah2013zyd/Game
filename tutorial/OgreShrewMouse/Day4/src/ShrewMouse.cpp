#include "ShrewMouse.h"


ShrewMouse::ShrewMouse(const std::string & name, Ogre::SceneManager * sm, Ogre::Vector3 & pos):_isEnable(false)
{
	using namespace Ogre;
	_ent = sm->createEntity(name, "ogrehead.mesh");

	Ogre::Animation  * anim;
	Ogre::NodeAnimationTrack * track;

	Ogre::SceneNode * sn = sm->getRootSceneNode()->createChildSceneNode(pos);
	// Add entity to the root scene node
	sn->attachObject(_ent);
	//_ent->setVisible(false);
	sn->setScale(Ogre::Vector3(0.5f, 0.5f, 0.5f));


	 anim = sm->createAnimation(name.c_str(), 3.f);
	// Spline it for nice curves
	anim->setInterpolationMode(Animation::IM_SPLINE);
	// Create a track to animate the camera's node
	track = anim->createNodeTrack(0, sn);



	// Setup keyframes
	TransformKeyFrame* key = track->createNodeKeyFrame(0); // startposition
	key->setScale(Ogre::Vector3(0.5f, 0.5f, 0.5f));
	key->setTranslate(pos);
	key = track->createNodeKeyFrame(1.f);
	key->setScale(Ogre::Vector3(1.f, 1.f, 1.f));
	key->setTranslate(pos);



	key = track->createNodeKeyFrame(2.f);
	key->setScale(Ogre::Vector3(1.f, 1.f, 1.f));
	key->setTranslate(pos);



	
	key = track->createNodeKeyFrame(3.f);
	key->setScale(Ogre::Vector3(0.5f, 0.5f, 0.5f));
	key->setTranslate(pos);


	_animState = sm->createAnimationState(name.c_str());
	_animState->setEnabled(false);
	_animState->setLoop(true);
}

//���º��������������ﵹ��ʱ��������ʱΪ0��ʱ����ʵ������
void ShrewMouse::update(float interval)
{
	//ÿ�ε��õ�ʱ��
	_time-=interval;
	_animState->setTimePosition(3.f- _time);
	//������ʱС�ڵ���0��ʱ��
	if(_time <=0.f)
	{
		_isEnable = false;
		//���ص����ͷ
		//_ent->setVisible(false);
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
		
		_animState->setEnabled(true);
		_animState->setTimePosition(0.f);
		_isEnable = true;
		//_ent->setVisible(true);
		_time = 3.f;
	}
}

bool ShrewMouse::isEnable(void)
{
	return _isEnable;
} 
