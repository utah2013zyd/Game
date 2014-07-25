
#ifndef __Orz_Test_TestLogManager_h__
#define __Orz_Test_TestLogManager_h__

#include <orz/Toolkit_Base/LogManager.h>
#include <string>
#include <iostream>
using namespace Orz;

BOOST_AUTO_TEST_SUITE( TestTestLogManager )


struct F {
	F(void) { 
		
	
	
	}
    ~F(void)
	{ 
	
	}
   

	Orz::TestLogManager logManager;
};

//#if TRACKER_SINGLE
BOOST_FIXTURE_TEST_CASE(simple, F)
{
	BOOST_CHECK_EQUAL(logManager.size(), 0);
	Orz::ILogManager::getSingleton().utf8("abc");
	BOOST_CHECK_EQUAL(logManager.size(), 1);
	BOOST_CHECK(logManager.get(0)== std::wstring(L"abc") );

	
	Orz::ILogManager::getSingleton().unicode(L"test");
	
	BOOST_CHECK_EQUAL(logManager.size(), 2);
	
	BOOST_CHECK(logManager.get(1)== std::wstring(L"test") );
	logManager.clear();
	
	BOOST_CHECK_EQUAL(logManager.size(), 0);
		
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END()

#endif