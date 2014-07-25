
#ifndef __Orz_Test_TestWebkit_h__
#define __Orz_Test_TestWebkit_h__

#include "TestWebkit.h"


#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/View_OIS/OISInputManager.h>
#include <orz/View_AomiUI/AomiUIManager.h>
#include <orz/View_AomiUI/Awesomium/WebCore.h>
#include <string>
#include <iostream>
using namespace Orz;

BOOST_AUTO_TEST_SUITE( TestTestWebkit )


struct F {
	F(void) { 
		
		system.reset(new SystemList<boost::mpl::list<OgreGraphicsManager, OISInputManager, AomiUIManager> >());
	
		
		Orz::SystemInterface::getSingleton().setParame<bool>("w32_mouse", true );
	
		Orz::SystemInterface::getSingleton().setParame<std::string>("AomiUI_MediaDirectory", "..\\..\\media" );
	}
    ~F(void)
	{ 
	
		system.reset();
	}
   
	
	SimpleLogManager logManager;
	SystemPtr system;
};

//#if TRACKER_SINGLE
BOOST_FIXTURE_TEST_CASE(_TestWebkit, F)
{
	if(system->init())
	{
		{

			JSBinding binding;
			TestWebkit web("a.html");
			web.setListener(&binding);
			Sleep(1000);
			web.executeJavascript("abc();");
			system->run();
/*for(size_t i = 0; i< logManager.size(); ++i)
			{
				std::cout<<logManager.get(i)<<std::endl;
			}*/
		}
	system->shutdown();
	}
		
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END()

#endif