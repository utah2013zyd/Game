
#include "ModelBaseConfig.h"
#include "TheaterBase.h"
#include <orz/Framework_Base/Logic.h>
Orz::TheaterFactoryPtr theater = Orz::TheaterFactoryPtr(new Orz::TheaterBaseFactory());
extern "C" void dllStartPlugin(void)
{

	using namespace Orz;
	GameFactories::getInstance().addTheaterFactory(theater.get()); 
}

extern "C" void dllStopPlugin(void)
{
	using namespace Orz;
	GameFactories::getInstance().removeTheaterFactory(theater.get()); 
}

