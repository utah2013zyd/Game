#include <orz/View_Fmod/FmodSoundManager.h>
#include "FmodSoundManagerImpl.h"
using namespace Orz;
FmodSoundManager::FmodSoundManager(void):_impl(new FmodSoundManagerImpl())
{
}
FmodSoundManager::~FmodSoundManager(void)
{

}
bool FmodSoundManager::init(void)
{
	return _impl->init();
}
void FmodSoundManager::shutdown(void)
{
	_impl->shutdown(); 
}

bool FmodSoundManager::update(TimeType interval)
{
	updateQuickPlay();
	return _impl->update(interval); 
}

SoundPlayerPtr FmodSoundManager::createPlayer(const std::string & fileName, bool stream)
{
	return _impl->createPlayer(getResourcesPath() +fileName, stream); 
}