 #include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include "OgreGraphicsManagerImpl.h"
#pragma warning(push)
#pragma warning(disable:4819)
#include <Ogre/Ogre.h>
#pragma warning(pop)
using namespace Orz;
template<> OgreGraphicsManager* Singleton<OgreGraphicsManager>::_singleton = NULL;

OgreGraphicsManager::OgreGraphicsManager(void):_impl(new OgreGraphicsManagerImpl())
{
	
}
OgreGraphicsManager::~OgreGraphicsManager(void)
{
	
}
bool OgreGraphicsManager::update(TimeType interval)
{
	return _impl->update(interval);
}
bool OgreGraphicsManager::init(void)
{
	return _impl->init();
}
void OgreGraphicsManager::shutdown(void)
{
	_impl->shutdown();
}
Ogre::Camera * OgreGraphicsManager::getCamera(void)
{
	return _impl->getCamera();
}
Ogre::SceneManager * OgreGraphicsManager::getSceneManager(void)
{
	return _impl->getSceneManager();
}
Ogre::RenderWindow * OgreGraphicsManager::getRenderWindow(void)
{
	return _impl->getRenderWindow();
}