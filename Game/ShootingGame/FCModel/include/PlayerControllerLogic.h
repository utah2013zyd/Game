#pragma once

#include "FCConfig.h"
#include <orz/Toolkit_Base/FSMLogic.h>

namespace sc = boost::statechart;
namespace mpl = boost::mpl;
namespace Orz{
	struct EvKeyAPress:sc::event<EvKeyAPress>{};
	struct EvKeyDPress:sc::event<EvKeyDPress>{};
	struct EvKeyWPress:sc::event<EvKeyWPress>{};
	struct EvKeySPress:sc::event<EvKeySPress>{};
	struct EvKeyQPress:sc::event<EvKeyQPress>{};
	struct EvKeyEPress:sc::event<EvKeyEPress>{};
	struct EvKeyARelease:sc::event<EvKeyARelease>{};
	struct EvKeyDRelease:sc::event<EvKeyDRelease>{};
	struct EvKeyWRelease:sc::event<EvKeyWRelease>{};
	struct EvKeySRelease:sc::event<EvKeySRelease>{};
	struct EvKeyQRelease:sc::event<EvKeyQRelease>{};
	struct EvKeyERelease:sc::event<EvKeyERelease>{};
	
	class PlayerController;	
	class Stable;
	class TurnLeft;
	class TurnRight;
	class BankLeft;
	class BankRight;
	class PitchForward;
	class PitchBackward;
	class PlayerControllerLogic: public FSM::MainLogic<PlayerControllerLogic, PlayerController, Stable>
	{
	};

	class Stable: public FSM::Logic<Stable, PlayerControllerLogic>
	{
	public:
		typedef mpl::list<sc::custom_reaction<EvKeyAPress>, 
							sc::custom_reaction<EvKeyDPress>,
							sc::custom_reaction<EvKeyQPress>,
							sc::custom_reaction<EvKeyEPress>,
							sc::custom_reaction<EvKeyWPress>,
							sc::custom_reaction<EvKeySPress>> reactions;
		sc::result react(const EvKeyAPress & evt);
		sc::result react(const EvKeyDPress & evt);
		sc::result react(const EvKeyQPress & evt);
		sc::result react(const EvKeyEPress & evt);
		sc::result react(const EvKeyWPress & evt);
		sc::result react(const EvKeySPress & evt);

	};
	class TurnLeft: public FSM::Logic<TurnLeft, PlayerControllerLogic>
	{
	public:
		typedef mpl::list<sc::custom_reaction<EvKeyARelease>> reactions;
		sc::result react(const EvKeyARelease & evt);
	};
	class TurnRight: public FSM::Logic<TurnRight, PlayerControllerLogic>
	{
	public:
		typedef mpl::list<sc::custom_reaction<EvKeyDRelease>> reactions;
		sc::result react(const EvKeyDRelease & evt);
	};
	class BankLeft: public FSM::Logic<BankLeft, PlayerControllerLogic>
	{
	public:
		typedef mpl::list<sc::custom_reaction<EvKeyQRelease>> reactions;
		sc::result react(const EvKeyQRelease &evt);
	};
	class BankRight: public FSM::Logic<BankRight, PlayerControllerLogic>
	{
	public:
		typedef mpl::list<sc::custom_reaction<EvKeyERelease>> reactions;
		sc::result react(const EvKeyERelease &evt);
	};
	class PitchForward: public FSM::Logic<PitchForward, PlayerControllerLogic>
	{
	public:
		typedef mpl::list<sc::custom_reaction<EvKeyWRelease>> reactions;
		sc::result react(const EvKeyWRelease &evt);
	};
	class PitchBackward: public FSM::Logic<PitchBackward, PlayerControllerLogic>
	{
	public:
		typedef mpl::list<sc::custom_reaction<EvKeySRelease>> reactions;
		sc::result react(const EvKeySRelease &evt);
	};

}