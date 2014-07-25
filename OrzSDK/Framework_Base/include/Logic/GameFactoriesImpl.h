#ifndef __Orz_FrameworkBase_GameFactoriesImpl_h__
#define __Orz_FrameworkBase_GameFactoriesImpl_h__
#include <orz/Framework_Base/FrameworkBaseConfig.h>
#include <orz/Toolkit_Base/IDManager/IDManager.h>

namespace Orz{

class _OrzToolkitBasePrivate GameFactoriesImpl: private boost::noncopyable
{

	typedef std::map<std::string, DirectorFactory *> DirectorFactoryMap;
	typedef std::map<std::string, SceneFactory *> SceneFactoryMap;
	typedef std::map<std::string, ActorFactory *> ActorFactoryMap;
	typedef std::map<std::string, TheaterFactory *> TheaterFactoryMap;
public:


	GameFactoriesImpl(void);
	~GameFactoriesImpl(void);
	
	void addDirectorFactory(DirectorFactory * factory);

	
	void addSceneFactory(SceneFactory * factory);
	
	void addActorFactory(ActorFactory * factory);

	void addTheaterFactory(TheaterFactory * factory);


	void removeDirectorFactory(DirectorFactory * factory);
	void removeSceneFactory(SceneFactory * factory);
	
	void removeActorFactory(ActorFactory * factory);
	void removeTheaterFactory(TheaterFactory * factory);


	DirectorPtr createDirector(const std::string & typeName, const std::string & name = IDManager::BLANK, NameValueList * parameter = NULL);
	ScenePtr createScene(const std::string & typeName, const std::string & name = IDManager::BLANK, NameValueList * parameter = NULL);
	ActorPtr createActor(const std::string & typeName, const std::string & name = IDManager::BLANK, NameValueList * parameter = NULL);
	TheaterPtr createTheater(const std::string & typeName, const std::string & name = IDManager::BLANK, NameValueList * parameter = NULL);//with world


private:
	DirectorFactoryMap _directorFactoryMap;
	SceneFactoryMap _sceneFactoryMap;
	ActorFactoryMap _actorFactoryMap;
	TheaterFactoryMap _theaterFactoryMap;
	
};

}


#endif