#ifndef __Orz_RotateAnimation_h__
#define __Orz_RotateAnimation_h__
#include <Ogre/Ogre.h>
namespace Orz
{
class RotateAnimation
{
public:
	RotateAnimation(const std::string & name,Ogre::SceneNode * sn, Ogre::SceneManager * sm,  bool time):_anim(NULL)
	{
		using namespace Ogre;
		// set up spline animation of node
		_anim = sm->createAnimation(name.c_str(), 360);
        // Spline it for nice curves
        _anim->setInterpolationMode(Animation::IM_LINEAR);
        // Create a track to animate the camera's node
        _track = _anim->createNodeTrack(0, sn);
        // Setup keyframes
        TransformKeyFrame* key = _track->createNodeKeyFrame(0); // startposition
        key = _track->createNodeKeyFrame(90);
      /*  key->setTranslate(Vector3(500,500,-1000));
        key = track->createNodeKeyFrame(5);
        key->setTranslate(Vector3(-1500,1000,-600));
        key = track->createNodeKeyFrame(7.5);
        key->setTranslate(Vector3(0,-100,0));
        key = track->createNodeKeyFrame(10);
        key->setTranslate(Vector3(0,0,0));*/
		Ogre::Quaternion q;
		q.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3::UNIT_Y);
		key->setRotation(q);

		key = _track->createNodeKeyFrame(180);
		q.FromAngleAxis(Ogre::Degree(180), Ogre::Vector3::UNIT_Y);
		key->setRotation(q);


		
		key = _track->createNodeKeyFrame(270);
		q.FromAngleAxis(Ogre::Degree(270), Ogre::Vector3::UNIT_Y);
		key->setRotation(q);


		
		key = _track->createNodeKeyFrame(360);
		q.FromAngleAxis(Ogre::Degree(360), Ogre::Vector3::UNIT_Y);
		key->setRotation(q);


        // Create a new animation state to track this
        _animState = sm->createAnimationState(name.c_str());
        _animState->setEnabled(false);
		_animState->setLoop(true);
		_all =  0;
	}
	
	void play(Ogre::Real r, Ogre::Real speed = 1.0f)
	{
		 _animState->setEnabled(true);
		 _all = r + 360.f * 3;
	}
	void update()
	{
		if( _animState->getEnabled() && _all >0.f)
		{
			_all -= 0.015f * 60.f;
			_animState->addTime(0.015f * 60.f);
		}
		else
		{
			 _animState->setEnabled(false);
		}
	}
	~RotateAnimation(void)
	{
		
	}
private:
	Ogre::Animation * _anim;
	Ogre::NodeAnimationTrack * _track;
	Ogre::AnimationState* _animState;
	Ogre::Real _all;

};
}

#endif