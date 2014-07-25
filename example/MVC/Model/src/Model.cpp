#include "Model.h"
#include "ExampleActor.h"
#include "ExampleDirector.h"
#include "ExampleScene.h"

#include <orz/Framework_Base/Logic.h>

Orz::GameRegisterPtr game_register;
extern "C" void dllStartPlugin(void)
{
	using namespace Orz;
	game_register.reset(new GameRegisterList<boost::mpl::list<ExampleDirectorFactory, ExampleActorFactory, ExampleSceneFactory> >());
}

extern "C" void dllStopPlugin(void)
{
	game_register.reset();
}



