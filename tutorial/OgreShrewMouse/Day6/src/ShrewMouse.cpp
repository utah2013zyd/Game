#include "ShrewMouse.h"


ShrewMouse::ShrewMouse(const std::string & name, Ogre::SceneManager * sm, Ogre::Vector3 & pos):_time(0.f),_state(Sleep)
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


void ShrewMouse::update(float interval)
{

	
	_time -= interval;
	
	_animState->setTimePosition(3.f- _time);
	if(_time <=0.f)
	{
		
			
		_ent->getSubEntity(0)->setMaterialName("Ogre/Eyes");
		_ent->getSubEntity(1)->setMaterialName("Ogre/Skin");
		_ent->getSubEntity(2)->setMaterialName("Ogre/Earring");
		_ent->getSubEntity(3)->setMaterialName("Ogre/Tusks");

		_state = Sleep;
		//_ent->setVisible(false);
	}
}
ShrewMouse::~ShrewMouse(void)
{
//	_sm->destroyAnimation(_name.c_str());
}


void ShrewMouse::enable(void)
{
	if(_state == Sleep)
	{
		_animState->setEnabled(true);
		_animState->setTimePosition(0.f);
		_state = Alive;
		//_ent->setVisible(true);
		_time = 3.f;
	}
}


ShrewMouse::State ShrewMouse::getState(void)
{
	return _state;
}

bool ShrewMouse::hit(void)
{
	if(_state == Alive)
	{
		_ent->setMaterialName("Examples/EnvMappedRustySteel");
		_state = Dead;

		return true;
	}

	return false;
}