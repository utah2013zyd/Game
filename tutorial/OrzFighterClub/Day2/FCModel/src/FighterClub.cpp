
#include "FCConfig.h"
#include "FCDirector.h"
#include "FCScene.h"
#include "FCFighter.h"
#include "FCWall.h"
#include <orz/Framework_Base/Logic.h>

///这个插件注册了若干 导演(游戏管理)工厂，场景工厂，角色工厂




Orz::GameRegisterPtr game;

extern "C" void dllStartPlugin(void)
{
 	using namespace Orz;
	game.reset(new GameRegisterList<boost::mpl::list<
		FCDirectorFactory, 
		FCSceneFactory,
		FCFighterFactory,
		FCWallFactory
	> >());
}

extern "C" void dllStopPlugin(void)
{
	game.reset();
}

