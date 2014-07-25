#include "OrzAwesomiumDemo.h"
#include "AwesomiumDemoRuning.h"
#include <orz/Framework_Base/FrameworkBase.h>
#include <iostream>
#include "../../test/include/test.h"
int main(void)
{
	using namespace Orz;


#ifdef _smallsystem_
	SystemPtr system(new SystemList<boost::mpl::list<TimerManager, PluginsManager> >());
#else
	SystemPtr system(new SystemList<boost::mpl::list<SimpleLogManager, SimpleTimerManager, OgreGraphicsManager, OISInputManager, AomiUIManager> >());
#endif
	

	//LogicConfiger::ConfigerPtr logic(new LogicConfiger::Configer());
	//logic->readDate(LogicConfiger::LoaderPtr(new LogicConfigerXMLLoader("orzconfig.xml")));
	//logic->readDate(LogicConfiger::LoaderPtr(new LogicConfigerArgLoader(argc, argv)));
	
	Orz::SystemInterface::getSingleton().setParame<bool>("w32_mouse", true );
	
	Orz::SystemInterface::getSingleton().setParame<std::string>("AomiUI_MediaDirectory", "..\\..\\media" );
	
	if(system->init())
	{
		boost::shared_ptr<AwesomiumDemoRuning>  runing(new AwesomiumDemoRuning);

	
		Orz::TimerPtr timer = Orz::ITimerManager::getSingleton().createTimer(boost::bind(&AwesomiumDemoRuning::update, runing.get()));
	
		system->run();
		system->shutdown();
	}
	
	
	return 0;
}

