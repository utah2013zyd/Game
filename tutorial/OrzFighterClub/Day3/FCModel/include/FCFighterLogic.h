#ifndef __Orz_FC_FCFighterLogic_h__
#define __Orz_FC_FCFighterLogic_h__

#include "FCConfig.h"
#include <orz/Toolkit_Base/FSMLogic.h>
namespace Orz
{
	class FCFighter;
	class FCFighterFall;
	class FCFighterRun;
	class FCFighterLogic: public FSM::MainLogic<FCFighterLogic, FCFighter, FCFighterFall>
	{
	
	};

	class FCFighterFall: public FSM::Logic<FCFighterFall, FCFighterLogic>
	{
	public:

		typedef boost::mpl::list< sc::custom_reaction< UpdateEvt > > reactions;
		sc::result react(const UpdateEvt & evt);
		FCFighterFall(void)
		{

		}

		~FCFighterFall(void)
		{
			
		}

	};


	class FCFighterRun: public FSM::Logic<FCFighterRun, FCFighterLogic>
	{
	public:

		typedef boost::mpl::list< sc::custom_reaction< UpdateEvt > > reactions;
		sc::result react(const UpdateEvt & evt);
		FCFighterRun(void):_init(false)
		{

		}

		~FCFighterRun(void)
		{
			
		}
	private:
		bool _init;
	};
}
#endif