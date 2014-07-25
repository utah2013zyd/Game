#ifndef __Orz_FC_FCFighterLogic_h__
#define __Orz_FC_FCFighterLogic_h__

#include "FCConfig.h"
#include <orz/Toolkit_Base/FSMLogic.h>
namespace Orz
{
	class FCFighter;
	class FCFighterFall;
	class FCFighterRun;
	class FCFighterTest;
	class FCFighterLogic: public FSM::MainLogic<FCFighterLogic, FCFighter, FCFighterFall>
	{
	
	};

	class FCFighterFall: public FSM::Logic<FCFighterFall, FCFighterLogic>
	{
	public:

		typedef boost::mpl::list< sc::custom_reaction< UpdateEvt > > reactions;
		sc::result react(const UpdateEvt & evt);
	

	};


	class FCFighterRun: public FSM::Logic<FCFighterRun, FCFighterLogic, FCFighterTest>
	{
	};

	
	class FCFighterTest: public FSM::Logic<FCFighterTest, FCFighterRun>
	{
	public:

		typedef boost::mpl::list< sc::custom_reaction< UpdateEvt > > reactions;
		sc::result react(const UpdateEvt & evt);
		
	
	};

	class FCFighterMove: public FSM::Logic<FCFighterMove, FCFighterRun>
	{
	public:
		typedef boost::mpl::list< sc::custom_reaction< UpdateEvt > > reactions;
		sc::result react(const UpdateEvt & evt);
		
	};


	class FCFighterFire: public FSM::Logic<FCFighterFire, FCFighterRun>
	{
	public:
		typedef boost::mpl::list< sc::custom_reaction< UpdateEvt > > reactions;
		sc::result react(const UpdateEvt & evt);
//target
		
	};
}
#endif