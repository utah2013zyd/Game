
#include <orz/Framework_Base/Logic/GameFactories.h>
#include "Logic/GameFactoriesImpl.h"
#include <orz/Framework_Base/Logic/IFactory.h>
using namespace Orz;


//template<> GameFactories* Singleton<GameFactories>::_singleton = NULL;
GameFactories::GameFactories(void):_impl(new GameFactoriesImpl())
{

}
GameFactories::~GameFactories(void)
{

}



void GameFactories::addDirectorFactory(DirectorFactory * factory)
{
	_impl->addDirectorFactory(factory);
}
void GameFactories::addSceneFactory(SceneFactory * factory)
{
	_impl->addSceneFactory(factory);
}
void GameFactories::addActorFactory(ActorFactory * factory)
{
	_impl->addActorFactory(factory);

}
void GameFactories::addTheaterFactory(TheaterFactory * factory)
{
	_impl->addTheaterFactory(factory);
}




void GameFactories::removeDirectorFactory(DirectorFactory * factory)
{
	_impl->removeDirectorFactory(factory);
}

void GameFactories::removeSceneFactory(SceneFactory * factory)
{
	_impl->removeSceneFactory(factory);
}

void GameFactories::removeActorFactory(ActorFactory * factory)
{
	_impl->removeActorFactory(factory);
}

void GameFactories::removeTheaterFactory(TheaterFactory * factory)
{
	_impl->removeTheaterFactory(factory);
}

DirectorPtr GameFactories::createDirector(const std::string & typeName, const std::string & name, NameValueList * parameter)
{
	return _impl->createDirector(typeName, name, parameter);
}
ScenePtr GameFactories::createScene(const std::string & typeName, const std::string & name, NameValueList * parameter)
{

	return _impl->createScene(typeName, name, parameter);
}
ActorPtr GameFactories::createActor(const std::string & typeName, const std::string & name, NameValueList * parameter)
{

	return _impl->createActor(typeName, name, parameter);
}
	



TheaterPtr GameFactories::createTheater(const std::string & typeName, const std::string & name, NameValueList * parameter)
{
	return _impl->createTheater(typeName, name, parameter);
}


GameFactories & GameFactories::getInstance()
{
	return *(getInstancePtr());
}
GameFactories * GameFactories::getInstancePtr()
{
	static GameFactories instance;
	return &instance;
}
