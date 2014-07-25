#ifndef __Orz_UnitTest_LogManager__
#define __Orz_UnitTest_LogManager__
BOOST_AUTO_TEST_SUITE( LogManagerTest )

struct F {
	F(void) { 
		
		std::cout<<"a"<<std::endl; 
		
		
	
	}
    ~F(void)
	{ 
	
	
	}
   
};

//#if TRACKER_SINGLE
BOOST_FIXTURE_TEST_CASE(simple, F)
{
	
		
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END()

#endif