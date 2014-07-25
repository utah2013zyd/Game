 #include <orz/View_OGRE3D/CEGUIManager.h>
 #include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/Framework_Base/System/IInputManager.h>

#include "CEGUIManagerImpl.h"
using namespace Orz;
template<> CEGUIManager* Singleton<CEGUIManager>::_singleton = NULL;

CEGUIManager::CEGUIManager(void):_impl(new CEGUIManagerImpl())
{
	
}
CEGUIManager::~CEGUIManager(void)
{

}

bool CEGUIManager::init(void)
{
	
	assert(IInputManager::getSingletonPtr());
	return _impl->init();
}
void CEGUIManager::shutdown(void)
{
	_impl->shutdown();
}


CEGUI::Renderer * CEGUIManager::getRenderer(void)
{
	return _impl->getRenderer();
}
CEGUI::System * CEGUIManager::getSystem(void)
{
	return _impl->getSystem();
}



void CEGUIManager::mouseEvent(bool enable)
{
	_impl->mouseEvent(enable);
}
void CEGUIManager::keyEvent(bool enable)
{
	_impl->keyEvent(enable);
}