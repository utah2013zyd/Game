
#include "FCConfig.h"
#include "FCDirector.h"
#include "FCScene.h"
#include "FCBullet.h"
#include "PlayerPlane.h"
#include "EnemyPlane.h"
#include "Obstacle.h"



#include <orz/Framework_Base/Logic.h>

Orz::GameRegisterPtr game;
extern "C" void dllStartPlugin(void)
{
 	using namespace Orz;
	game.reset(new GameRegisterList<boost::mpl::list<
		FCDirectorFactory, 
		FCSceneFactory,
		FCBulletFactory, 
		EnemyControllerFactory,
		PlayerControllerFactory,
		PlayerPlaneFactory, 
		EnemyPlaneFactory,
		ObstacleFactory
	> >());
	
}

extern "C" void dllStopPlugin(void)
{

	using namespace Orz;
	
	game.reset();
}

