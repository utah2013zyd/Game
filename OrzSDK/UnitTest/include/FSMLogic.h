#ifndef __Orz_UnitTest_FSMLogic__
#define __Orz_UnitTest_FSMLogic__

class Logic;
class Owner//: private Orz::FSMLogic<Owner, Logic>
{
public:
	Owner(void)
	{
		
		std::cout<<"!!!!!!!!!!!!!2313"<<std::endl;
		_logic.init(this);
		//_logic_initiate();
	}
	~Owner(void)
	{
		_logic.shutdown();
	}
public:
	Orz::FSMLogicHandler<Owner, Logic> _logic;
};


class Logic:public Orz::FSM::MainLogic<Logic, Owner>
{
public:
	Logic()
	{
		std::cout<<"!!!!!!!!!!!!!"<<std::endl;
	}
};
//____________________________________________________________________________//

// most frequently you implement test cases as a free functions with automatic registration
BOOST_AUTO_TEST_CASE( test1 )
{
	Owner a;
    // reports 'error in "test1": test 2 == 1 failed'
    //BOOST_CHECK( 2 == 1 );
}

//____________________________________________________________________________//

// each test file may contain any number of test cases; each test case has to have unique name
BOOST_AUTO_TEST_CASE( test2 )
{
    int i = 0;

    // reports 'error in "test2": check i == 2 failed [0 != 2]'
    //BOOST_CHECK_EQUAL( i, 2 );

    //BOOST_CHECK_EQUAL( i, 0 );
}
#endif