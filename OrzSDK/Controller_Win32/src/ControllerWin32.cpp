#include "ControllerWin32.h"
#include <orz/Framework_Base/FrameworkBase.h>
#include "windows.h"
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
{
	using namespace Orz;

	

#ifdef _smallsystem_
	SystemPtr system(new SystemList<boost::mpl::list<SimpleTimerManager, PluginsManager> >());
#else
	SystemPtr system(new SystemList<boost::mpl::list<SimpleLogManager, AsioTimerManager, OgreGraphicsManager, CEGUIManager,  OISInputManager, FmodSoundManager,  PluginsManager> >());
#endif
	LogicConfiger::LogicFacadePtr logic(new LogicConfiger::LogicFacade());

	
	logic->building(LogicConfiger::XMLBuilder("orzconfig.xml"));
	

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