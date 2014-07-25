#ifndef __Orz_Test_Md5Checker_h__
#define __Orz_Test_Md5Checker_h__
#include "config.h"




BOOST_AUTO_TEST_SUITE( Md5Checker_ts )

struct F {
	F() {
		using namespace Orz;
		
	}
	~F() { }


	Orz::Md5Checker checker;
};
//#if TRACKER_SINGLE

BOOST_FIXTURE_TEST_CASE(CHECKER, F)
{

checker.checkerFile("asdf", "sdf");




}

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END()

#endif