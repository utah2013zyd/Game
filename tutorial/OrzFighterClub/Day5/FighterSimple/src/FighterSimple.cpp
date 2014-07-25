#include <iostream>
#include "Fighter.h"


Orz::GameRegisterPtr game;
extern "C" void dllStartPlugin(void)
{

	
 	using namespace Orz;

	game.reset(new GameRegisterList<boost::mpl::list<
		FCFighterFactory
	> >());
}

extern "C" void dllStopPlugin(void)
{

	game.reset();
}

