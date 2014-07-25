#include <orz/Framework_Base/System/ISoundManager.h>
//#include "System/ISoundManagerImpl.h"
using namespace Orz;
template<> ISoundManager* Singleton<ISoundManager>::_singleton = NULL;



ISoundManager::ISoundManager(void)//:_impl(new ISoundManagerImpl())
{

}
ISoundManager::~ISoundManager(void)
{

}
//
//bool ISoundManager::init(void)
//{
//	return doInit();
//}
//void ISoundManager::shutdown(void)
//{
//	doShutdown();
//}

//void ISoundManager::updateQuickPlay(void)
//{
//	_impl->updateQuickPlay();
//}
//
//void ISoundManager::setResourcesPath(const std::string & path)
//{
//	_impl->setResourcesPath(path);
//}
//SoundPlayerPtr ISoundManager::createPlayer(const std::string & fileName, bool stream)
//{
//	return doCreatePlayer(_impl->getResourcesPath() + fileName, stream);
//}

void ISoundManager::setResourcesPath(const std::string & path)
{
	_path = path;
}

const std::string & ISoundManager::getResourcesPath(void) const
{
	return _path;
}

void ISoundManager::quickPlay(const std::string & fileName, bool stream)
{
	SoundPlayerPtr player =  createPlayer(fileName, stream);
	player->load();
	player->play(1);
	_players.push_back(player);
}
void ISoundManager::updateQuickPlay(void)
{
	SoundPlayerList::iterator it;
	for(it  = _players.begin(); it != _players.end();)
	{
		if((*it)->playing())
		{
			++it;
		}
		else
		{
			(*it)->unload();
			it = _players.erase(it);
		}
	}
}


SoundPlayer::SoundPlayer(const std::string & name, bool stream):
		_name(name),
		_stream(stream)
{}
//bool SoundPlayer::play(uint32 times)
//{
//	return doPlay(times);
//	
//}
//
//bool SoundPlayer::load(void)
//{
//	
//	return doLoad();
//
//}
//bool SoundPlayer::unload(void)
//{
//	return doUnload();
//
//}
//bool SoundPlayer::stop(void)
//{
//	return doStop();
//	
//}
//bool SoundPlayer::playing(void)
//{
//	return isPlaying();
//}
//bool SoundPlayer::setPaused(bool pause)
//{
//	return doSetPaused(pause);
//
//}
//bool SoundPlayer::setVolume(float volume)
//{
//	return doSetVolume(volume);
//	
//}
//bool SoundPlayer::setPosition(float posi)
//{
//	return doSetPosition(posi);
//	
//}
//float SoundPlayer::getPosition(void)
//{
//	return doGetPosition();
//	
//}
//float SoundPlayer::getLength(void)
//{
//	return doGetLength();
//	
//}
SoundPlayer::~SoundPlayer(void)
{
	
}             