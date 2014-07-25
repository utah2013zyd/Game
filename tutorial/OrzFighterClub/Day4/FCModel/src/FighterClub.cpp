
#include "FCConfig.h"
#include "FCDirector.h"
#include "FCScene.h"
#include "FCFighter.h"
#include "FCWall.h"
#include <orz/Framework_Base/Logic.h>

///������ע�������� ����(��Ϸ����)������������������ɫ����




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

