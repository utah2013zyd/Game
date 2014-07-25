
#include "FCConfig.h"
#include "FCDirector.h"
#include "FCScene.h"
#include "FCFighter.h"
#include "FCBullet.h"
#include "PlayerController.h"


#include <orz/Framework_Base/Logic.h>

Orz::GameRegisterPtr game;
extern "C" void dllStartPlugin(void)
{
 	using namespace Orz;
	game.reset(new GameRegisterList<boost::mpl::list<
		FCDirectorFactory, 
		FCSceneFactory,
		FCFighterFactory,
		FCBulletFactory, 
		EnemyControllerFactory,
		PlayerControllerFactory
	> >());
	
}

extern "C" void dllStopPlugin(void)
{

	using namespace Orz;
	
	game.reset();
}

