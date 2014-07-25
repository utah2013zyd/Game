#ifndef __Orz_Test_OrzAR_h__
#define __Orz_Test_OrzAR_h__
#include "config.h"
#include <iostream>

#include <orz/Framework_Base/FrameworkBase.h>
#include <orz/Framework_Base/System.h>
#include <orz/Toolkit_Base/TimerManager.h>
#include <orz/Toolkit_Base/DynLibManager.h>
#include <orz/Toolkit_Base/LogManager.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/View_OGRE3D/CEGUIManager.h>
#include <orz/View_OIS/OISInputManager.h>
#include <orz/View_Fmod/FmodSoundManager.h>
#include <orz/Toolkit_Base/Singleton.h>




BOOST_AUTO_TEST_SUITE( SingleChipManager_ts )

struct F {
	F() {
		using namespace Orz;
		system.reset(new SystemList<boost::mpl::list<SimpleLogManager, SimpleTimerManager, OgreGraphicsManager, SingleChipManager, OISInputManager> >());
		
	}
	~F() { }


	Orz::SystemPtr system;
};
//#if TRACKER_SINGLE

BOOST_FIXTURE_TEST_CASE(simple, F)
{


	Orz::SystemInterface::getSingleton().setParame<std::string>("SingleChip_Port", std::string("COM3"));
	Orz::SystemInterface::getSingleton().setParame<int>("SingleChip_BaudRate", 115200);
	if(system->init())
	{
		unsigned char c = 0;
		
		//boost::shared_ptr<OgreNewtRunning>  runing(new OgreNewtRunning());

		//Orz::TimerPtr timer(new Orz::Timer("DemoRuning", boost::bind(&OgreNewtRunning::update, runing.get() )));
		//	timer->start(WORLD_UPDATE_INTERVAL);

		system->run();



		//runing.reset();

		system->shutdown();
	}





}

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END()

#endif