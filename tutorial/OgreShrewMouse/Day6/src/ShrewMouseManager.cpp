#include "ShrewMouseManager.h"
#include "ShrewMouse.h"


bool ShrewMouseManager::hit(int num)
{
	return _mice[num]->hit();
}
ShrewMouseManager::ShrewMouseManager(Ogre::SceneManager * sm):_sm(sm), _time(0.f)
{
	using namespace Ogre;
	
	for(int i=0; i<3; ++i)
	{
	
		for(int j =0; j<3; ++j)
		{
			int n = i+j*3;
			_mice[n] = new ShrewMouse("head"+Ogre::StringConverter::toString(i+j*3), _sm, Ogre::Vector3(100 *i, 0, 100 *j));
		
		}
	}

}
ShrewMouseManager::~ShrewMouseManager(void)
{ 
	for(int i=0; i<9; ++i)
	{
		if(_mice[i])
		{
			delete _mice[i];
			_mice[i] = NULL;
		}
	}
}



void ShrewMouseManager::update(float interval)
{


	_time+=interval;
	while(_time >= 1.0f)
	{
		_time -= 1.f;
		_mice[rand()%9]->enable();
	
	}
	for(int i=0; i<9; ++i)
	{
		if(_mice[i]->getState() != ShrewMouse::Sleep)
		{
			_mice[i]->update(interval);
		}
	}
}