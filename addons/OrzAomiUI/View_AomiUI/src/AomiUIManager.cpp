#include <orz/View_AomiUI/AomiUIManager.h>
#include "AomiUIManagerImpl.h"

using namespace Orz;


template<> AomiUIManager* Singleton<AomiUIManager>::_singleton = NULL;
AomiUIManager::AomiUIManager(void):_impl(new AomiUIManagerImpl())
{
	
}
AomiUIManager::~AomiUIManager(void)
{
	
}
bool AomiUIManager::init(void)
{
	return _impl->init();
}
void AomiUIManager::shutdown(void)
{
	_impl->shutdown();
}
bool AomiUIManager::update(TimeType i)
{
	return _impl->update(i);
}

//AomiView* AomiUIManager::createAomiView(const std::string & name, int width, int height)
//{
//	return _impl->createAomiView(name, width, height);
//}
OverlayView* AomiUIManager::createOverlayView(const std::string & name, int width, int height)
{
	return _impl->createOverlayView(name, width, height);
}



void AomiUIManager::destroyView(const std::string & name)
{
	_impl->destroyView(name);
}

