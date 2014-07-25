#include "ControllerBase.h"
#include <orz/Framework_Base/FrameworkBase.h>



int main(int argc, char **argv)
{
	using namespace Orz;

#ifdef _smallsystem_
	SystemPtr system(new SystemList<boost::mpl::list<SimpleTimerManager, PluginsManager> >());
#else
	SystemPtr system(new SystemList<boost::mpl::list<SimpleLogManager, SimpleTimerManager, OgreGraphicsManager, CEGUIManager, OISInputManager, FmodSoundManager, PluginsManager> >());
#endif

	LogicConfiger::LogicFacadePtr logic(new LogicConfiger::LogicFacade());

	if(argc == 1)
	{
		logic->building(LogicConfiger::XMLBuilder("orzconfig.xml"));
	}
	else
	{
		logic->building(LogicConfiger::ArgBuilder(argc, argv));
	}

	if(system->init())
	{
		if(logic->load())
		{

			system->run();
			logic->unload();
		}

		system->shutdown();
	}
	
	
	return 0;
}