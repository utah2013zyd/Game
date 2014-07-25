#ifndef __Orz_FrameworkBase_PreDeclare_H_
#define __Orz_FrameworkBase_PreDeclare_H_
#include <orz/Toolkit_Base/Variant.h>
namespace Orz
{
class Entity;
class Actor;
class Scene;
class Director;
class Theater;
class GameFactories;

template <class T> class IFactory; 


typedef IFactory<Actor> ActorFactory;
typedef IFactory<Scene> SceneFactory;
typedef IFactory<Director> DirectorFactory;
typedef IFactory<Theater> TheaterFactory;


typedef boost::shared_ptr<ActorFactory> ActorFactoryPtr;
typedef boost::shared_ptr<SceneFactory> SceneFactoryPtr;
typedef boost::shared_ptr<DirectorFactory> DirectorFactoryPtr;
typedef boost::shared_ptr<TheaterFactory> TheaterFactoryPtr;



typedef boost::shared_ptr<Entity> EntityPtr;
typedef boost::shared_ptr<Actor> ActorPtr;
typedef boost::shared_ptr<Scene> ScenePtr;
typedef boost::shared_ptr<Director> DirectorPtr;
typedef boost::shared_ptr<Theater> TheaterPtr;
typedef boost::shared_ptr<GameFactories> GameFactoriesPtr;

typedef std::map<std::string, Variant > NameValueList;




class SystemInterface;
typedef boost::shared_ptr<SystemInterface> SystemPtr;

class WindowInterface;
typedef boost::shared_ptr<WindowInterface> WindowPtr;
typedef boost::weak_ptr<WindowInterface> WeakWindowPtr;

class SoundPlayer;


typedef boost::shared_ptr<SoundPlayer> SoundPlayerPtr;
}

#endif