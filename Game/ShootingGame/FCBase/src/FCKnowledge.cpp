#include "FCKnowledge.h"
#include "FCMapInfo.h"
using namespace Orz;

template<> FCKnowledge* Singleton<FCKnowledge>::_singleton = NULL;
FCKnowledge::FCKnowledge(void)
{

}
FCKnowledge::~FCKnowledge(void)
{

}
FCMapInfo & FCKnowledge::mapInfo(void)
{
	return _map;
}

void FCKnowledge::setSceneName(const std::string sceneName)
{
	_sceneName = sceneName;
}

void FCKnowledge::setDirectorName(const std::string directorName)
{
	_directorName = directorName;
}

std::string FCKnowledge::getSceneName()
{
	return _sceneName;
}

std::string FCKnowledge::getDirectorName()
{
	return _directorName;
}

void FCKnowledge::setPlayerPosition(const Ogre::Vector3 pos)
{
	_playerPos = pos;
}

Ogre::Vector3 FCKnowledge::getPlayerPosition()
{
	return _playerPos;
}