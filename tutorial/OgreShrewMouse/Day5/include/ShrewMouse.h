#ifndef __ShrewMouse_H__
#define __ShrewMouse_H__
#include <Ogre.h>

class ShrewMouse
{
public:
	enum State
	{
		Alive,
		Dead,
		Sleep
	};
public:
	ShrewMouse(const std::string & name, Ogre::SceneManager * sm, Ogre::Vector3 & pos);
	~ShrewMouse(void);
	void update(float interval);
	void enable(void);

	State getState(void);
	bool hit(void);
private:

	Ogre::Entity * _ent;
	float _time;
	Ogre::AnimationState* _animState;

	State _state;

};

#endif