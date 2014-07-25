#include "Logic/GameFactoriesImpl.h"
#include <orz/Framework_Base/Logic/IFactory.h>
using namespace Orz;
GameFactoriesImpl::GameFactoriesImpl(void)
{

}
GameFactoriesImpl::~GameFactoriesImpl(void)
{

}
void GameFactoriesImpl::addDirectorFactory(DirectorFactory * factory)
{
	DirectorFactoryMap::iterator it = _directorFactoryMap.find(factory->getTypeName());
	if(it == _directorFactoryMap.end())
	{
		_directorFactoryMap.insert(std::make_pair(factory->getTypeName(), factory));
	}
	else
	{
		throw ORZ_EXCEPTION(Exception::ERR_DUPLICATE_ITEM);
	}
}
void GameFactoriesImpl::addSceneFactory(SceneFactory * factory)
{
	SceneFactoryMap::iterator it = _sceneFactoryMap.find(factory->getTypeName());
	if(it == _sceneFactoryMap.end())
	{
		_sceneFactoryMap.insert(std::make_pair(factory->getTypeName(), factory));
	}
	else
	{
		throw ORZ_EXCEPTION(Exception::ERR_DUPLICATE_ITEM);
	}
}
void GameFactoriesImpl::addActorFactory(ActorFactory * factory)
{
	ActorFactoryMap::iterator it = _actorFactoryMap.find(factory->getTypeName());
	if(it == _actorFactoryMap.end())
	{
		_actorFactoryMap.insert(std::make_pair(factory->getTypeName(), factory));
	}
	else
	{
		throw ORZ_EXCEPTION(Exception::ERR_DUPLICATE_ITEM);
	}
}
void GameFactoriesImpl::addTheaterFactory(TheaterFactory * factory)
{
	TheaterFactoryMap::iterator it = _theaterFactoryMap.find(factory->getTypeName());
	if(it == _theaterFactoryMap.end())
	{
		_theaterFactoryMap.insert(std::make_pair(factory->getTypeName(), factory));
	}
	else
	{
		throw ORZ_EXCEPTION(Exception::ERR_DUPLICATE_ITEM);
	}
}



void GameFactoriesImpl::removeDirectorFactory(DirectorFactory * factory)
{
	_directorFactoryMap.erase(factory->getTypeName());
}



void GameFactoriesImpl::removeActorFactory(ActorFactory * factory)
{
	_actorFactoryMap.erase(factory->getTypeName());
}



void GameFactoriesImpl::removeTheaterFactory(TheaterFactory * factory)
{
	_theaterFactoryMap.erase(factory->getTypeName());
}



void GameFactoriesImpl::removeSceneFactory(SceneFactory * factory)
{
	_sceneFactoryMap.erase(factory->getTypeName());
}



DirectorPtr GameFactoriesImpl::createDirector(const std::string & typeName, const std::string & name, NameValueList * parameter)
{
	DirectorFactoryMap::iterator it = _directorFactoryMap.find(typeName);
	if(it == _directorFactoryMap.end())
	{
		throw ORZ_EXCEPTION(Exception::ERR_ITEM_NOT_FOUND);
	}
	
	return it->second->createInstance(name, parameter);
	
}
ScenePtr GameFactoriesImpl::createScene(const std::string & typeName, const std::string & name, NameValueList * parameter)
{
	SceneFactoryMap::iterator it = _sceneFactoryMap.find(typeName);
	if(it == _sceneFactoryMap.end())
	{
		throw ORZ_EXCEPTION(Exception::ERR_ITEM_NOT_FOUND);
	}
	
	return it->second->createInstance(name, parameter);

}
ActorPtr GameFactoriesImpl::createActor(const std::string & typeName, const std::string & name, NameValueList * parameter)
{
	ActorFactoryMap::iterator it = _actorFactoryMap.find(typeName);
	if(it == _actorFactoryMap.end())
	{
		throw ORZ_EXCEPTION(Exception::ERR_ITEM_NOT_FOUND);
	}
	
	return it->second->createInstance(name, parameter);
}
TheaterPtr GameFactoriesImpl::createTheater(const std::string & typeName, const std::string & name, NameValueList * parameter)
{
	TheaterFactoryMap::iterator it = _theaterFactoryMap.find(typeName);
	if(it == _theaterFactoryMap.end())
	{
		throw ORZ_EXCEPTION(Exception::ERR_ITEM_NOT_FOUND);
	}
	
	return it->second->createInstance(name, parameter);
}