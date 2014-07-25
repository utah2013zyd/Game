#include "FCConfig.h"

#include <orz/Toolkit_Base/FSMLogic.h>
#pragma once
namespace Orz
{
	class FCFighter;
	class FCFighterLive;
	class FCFighterDestroying;
	class FCFighterDead;

	struct EvFighterCrush:sc::event<EvFighterCrush>{};

	class FCFighterLogic : public FSM::MainLogic<FCFighterLogic, FCFighter, FCFighterLive>
	{
	};

	class FCFighterLive: public FSM::Logic<FCFighterLive, FCFighterLogic>
	{
	public:
		typedef boost::mpl::list<sc::custom_reaction<EvFighterCrush>, sc::custom_reaction<UpdateEvt>> reactions;
		sc::result react(const EvFighterCrush & evt);
		sc::result react(const UpdateEvt & evt);
		FCFighterLive(void)
		{
		}

		~FCFighterLive(void)
		{
		}
	};

	class FCFighterDestroying: public FSM::Logic<FCFighterDestroying, FCFighterLogic>
	{
	public:
		typedef boost::mpl::list<sc::custom_reaction<UpdateEvt>> reactions;
		sc::result react(const UpdateEvt & evt);
		FCFighterDestroying(void)
		{
			_elapsedTime = 0.0;
		}

		~FCFighterDestroying(void)
		{
		}
	private:
		double _elapsedTime;
	};

	class FCFighterDead: public FSM::Logic<FCFighterDead, FCFighterLogic>
	{
	public:
		FCFighterDead(void)
		{
		}

		~FCFighterDead(void)
		{
		}
	};

}