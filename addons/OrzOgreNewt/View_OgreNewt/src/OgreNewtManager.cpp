
#include <orz/View_OgreNewt/OgreNewtManager.h>
#include <orz/Framework_Base/System.h>
#include "OgreNewt_World.h"
#include <iostream>
using namespace Orz;




template<> OgreNewtManager* Singleton<OgreNewtManager>::_singleton = NULL;


OgreNewtManager::OgreNewtManager(void):_time(0.f),_speed(1.f)
{

}
OgreNewtManager::~OgreNewtManager(void)
{

}
bool OgreNewtManager::init(void)
{
	_time = 0.f;
	if(Orz::SystemInterface::getSingleton().hasParame("physics_speed"))
	{
		_speed = Orz::SystemInterface::getSingleton().getParame<float>("physics_speed");
	}
	else 
		_speed = 1.f;
	_world.reset(new OgreNewt::World());
	return true;
}
void OgreNewtManager::shutdown(void)
{
	_world.reset();
}
bool OgreNewtManager::update(TimeType i)
{
	//ding zhen
	_time += i * _speed;
	while(_time > 0.01f)
	{
		_world->update(100);
		_time -= 0.01;
	}
	std::cout << "newton update"<<std::endl;
	return true;
}



OgreNewt::World * OgreNewtManager::getNewtWorld(void)
{
	return _world.get();
}