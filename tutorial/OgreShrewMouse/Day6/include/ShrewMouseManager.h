#ifndef __ShrewMouseManager_H__
#define __ShrewMouseManager_H__
#include <Ogre.h>

class ShrewMouse;
class ShrewMouseManager
{
public:
	ShrewMouseManager(Ogre::SceneManager *sm) ;
	~ShrewMouseManager(void);
	void update(float interval);
	bool hit(int num);
protected:
	Ogre::SceneManager * _sm;
	ShrewMouse* _mice[9];
	float _time;
	
};

#endif
