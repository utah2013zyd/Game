#include <orz/View_OIS/OISInputManager.h>
#include "OISInputManagerImpl.h"
using namespace Orz;
OISInputManager::OISInputManager(void):_oisImpl(new OISInputManagerImpl())
{

}
bool OISInputManager::init(void)
{
	
	return _oisImpl->init(this);
}
void OISInputManager::shutdown(void)
{
	_oisImpl->shutdown();
}
bool OISInputManager::update(TimeType interval)
{
	return _oisImpl->update(interval);
}
OISInputManager::~OISInputManager(void)
{

}


OISInputManager& OISInputManager::getSingleton(void)
{
	return static_cast<OISInputManager&>(IInputManager::getSingleton());
}
OISInputManager* OISInputManager::getSingletonPtr(void)
{
	return static_cast<OISInputManager*>(IInputManager::getSingletonPtr());
}
