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

//更新函数，我们在这里倒计时，当倒计时为0的时候让实体隐藏
void ShrewMouse::update(float interval)
{
	//每次调用的时候
	_time-=interval;
	_animState->setTimePosition(3.f- _time);
	//当倒计时小于等于0的时候
	if(_time <=0.f)
	{
		_isEnable = false;
		//隐藏地鼠的头
		//_ent->setVisible(false);
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
