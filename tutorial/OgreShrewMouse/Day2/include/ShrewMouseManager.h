#ifndef __ShrewMouseManager_H__
#define __ShrewMouseManager_H__
#include <Ogre.h>

class ShrewMouseManager
{
public:
	ShrewMouseManager(Ogre::SceneManager *sm);
	~ShrewMouseManager(void);
protected:
	//这里需要一个Ogre3D场景管理器的指针，用于调度游戏空间
	Ogre::SceneManager * _sm;
};

#endif
