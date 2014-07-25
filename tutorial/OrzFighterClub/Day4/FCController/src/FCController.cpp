#include "FCController.h"
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


	LogicConfiger::ManualBuilder builder;

	
	builder.addPlugin("Model_Base");
	builder.addPlugin("FCModel");
	builder.setTheater("TheaterBase","FC");
	builder.addDirector("FCDirector", "FC");
	builder.setActiveDirector("FC");


	logic->building(builder);
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