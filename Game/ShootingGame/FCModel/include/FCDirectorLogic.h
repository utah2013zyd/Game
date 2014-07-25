#include "FCConfig.h"
#include <orz/Toolkit_Base/FSMLogic.h>
#pragma once

namespace Orz
{
	class FCDirector;
	class FCDirectorRunning;
	class FCDirectorWin;
	class FCDirectorLose;

	struct EvPlayerExecuted:sc::event<EvPlayerExecuted>{};
	struct EvEnimiesExecuted:sc::event<EvEnimiesExecuted>{};

	class FCDirectorLogic: public FSM::MainLogic<FCDirectorLogic, FCDirector, FCDirectorRunning>
	{
	};

	class FCDirectorRunning: public FSM::Logic<FCDirectorRunning, FCDirectorLogic>
	{
	public:
		typedef boost::mpl::list<sc::custom_reaction<EvPlayerExecuted>, sc::custom_reaction<EvEnimiesExecuted>> reactions;
		sc::result react(const EvPlayerExecuted & evt);
		sc::result react(const EvEnimiesExecuted & evt);

		FCDirectorRunning(void)
		{
		}

		~FCDirectorRunning(void)
		{
		}
	};

	class FCDirectorWin: public FSM::Logic<FCDirectorWin, FCDirectorLogic>
	{
	public:
		typedef boost::mpl::list<sc::custom_reaction<UpdateEvt>> reactions;
		sc::result react(const UpdateEvt & evt);
		FCDirectorWin(void)
		{
		}

		~FCDirectorWin(void)
		{
		}
	};

	class FCDirectorLose: public FSM::Logic<FCDirectorLose, FCDirectorLogic>
	{
	public:
		typedef boost::mpl::list<sc::custom_reaction<UpdateEvt>> reactions;
		sc::result react(const UpdateEvt & evt);
		FCDirectorLose(void)
		{
		}

		~FCDirectorLose(void)
		{
		}
	};
}