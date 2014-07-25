#include <orz/Toolkit_Base/DynLibManager.h>



int main(void)
{
	using namespace Orz;
	{
		DynLibPtr lib(new DynLib("Plugin"));//
		lib->load();
		std::cout<<lib->getName()<<std::endl;
		typedef void (*Fun)(void);
		Fun fun = (Fun)lib->getSymbol("testFunction");
		(*fun)();

		lib->unload();
	
	}
	{
		PluginsManager pm;


		
		pm.init();
		pm.loadPlugin("Plugin");
		
		pm.shutdown();
		pm.unloadPlugin("Plugin");
		

	}
	return 0;
}