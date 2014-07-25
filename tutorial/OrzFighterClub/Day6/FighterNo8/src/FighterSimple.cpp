#include <iostream>
#include "Fighter.h"
Orz::ActorFactoryPtr fighterFact = Orz::ActorFactoryPtr(new Orz::FCFighterFactory());
extern "C" void dllStartPlugin(void)
{

	
 	using namespace Orz;
	GameFactories::getInstance().addActorFactory(fighterFact.get()); 
}

extern "C" void dllStopPlugin(void)
{


	using namespace Orz;

	GameFactories::getInstance().removeActorFactory(fighterFact.get()); 

}

