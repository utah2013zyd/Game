#ifndef __ShrewMouseManager_H__
#define __ShrewMouseManager_H__
#include <Ogre.h>

class ShrewMouseManager
{
public:
	ShrewMouseManager(Ogre::SceneManager *sm);
	~ShrewMouseManager(void);
protected:
	//������Ҫһ��Ogre3D������������ָ�룬���ڵ�����Ϸ�ռ�
	Ogre::SceneManager * _sm;
};

#endif
