#ifndef __ShrewMouse_H__
#define __ShrewMouse_H__
#include <Ogre.h>

class ShrewMouse
{
public:
	ShrewMouse(const std::string & name, Ogre::SceneManager * sm, Ogre::Vector3 & pos);
	~ShrewMouse(void);
	void update(float interval);
	void enable(void);
	bool isEnable(void);
private:
	bool _isEnable;
	Ogre::Entity * _ent;
	float _time;

	Ogre::AnimationState* _animState;

};

#endif