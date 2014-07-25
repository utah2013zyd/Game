
#ifndef __Orz_Test_Toolkit_Script_h__
#define __Orz_Test_Toolkit_Script_h__

#include "UnitTest.h"
#include "TestActor.h"

#include <orz/Toolkit_Base/EventSystem.h>

#include <orz/Framework_Base/FrameworkBase.h>
#include "TestLogManager.h"
using namespace Orz;

BOOST_AUTO_TEST_SUITE( Toolkit_Script )


struct F {
	F(void)  { 
		
		std::cout<<"a"<<std::endl; 
		
		game_setup.reset(new GameRegisterList<boost::mpl::list<TestActorFactory> >());

		
	
	}
    ~F(void)
	{ 
	
		game_setup.reset();
	}
   
	GameRegisterPtr game_setup;
	TestLogManager logManager;
};

//#if TRACKER_SINGLE
BOOST_FIXTURE_TEST_CASE(ScriptInterfaceTest, F)
{
	EventWorld ew;
	
	Orz::ScriptInterfacePtr si(new ScriptInterface);
	ew.comeIn(si);

	si->createActor("test", "temp");
	BOOST_CHECK(logManager.get(0)== std::wstring(L"ScriptInterface: We create Actor, type is test, name is temp.") );
	
	si->enableActor("temp");
	
	BOOST_CHECK(logManager.get(1)== std::wstring(L"ScriptInterface: We enable Actor, name is temp.") );
	si->disableActor("temp");

	BOOST_CHECK(logManager.get(2)== std::wstring(L"ScriptInterface: We disable Actor, name is temp.") );
	si->destroyActor("temp");
	
	BOOST_CHECK(logManager.get(3)== std::wstring(L"ScriptInterface: We destroy Actor, name is temp.") );
	ew.goOut(si);
	logManager.clear();
		
}


//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END()

#endif