#include "FCConfig.h"
#include <orz/Toolkit_Base/FSMLogic.h>
#pragma once

namespace Orz
{
	class FCBullet;
	class FCBulletInactive;
	class FCBulletActive;
	class FCBulletHit;
	
	struct EvBulletFire:sc::event<EvBulletFire>{};

	class FCBulletLogic: public FSM::MainLogic<FCBulletLogic, FCBullet, FCBulletInactive>
	{
	};

	class FCBulletInactive: public FSM::Logic<FCBulletInactive, FCBulletLogic>
	{
	public:
		typedef boost::mpl::list<sc::custom_reaction<EvBulletFire>> reactions;
		sc::result react(const EvBulletFire & evt);
		FCBulletInactive(void)
		{
		}

		~FCBulletInactive(void)
		{
		}
	};

	class FCBulletActive: public FSM::Logic<FCBulletActive, FCBulletLogic>
	{
	public:
		typedef boost::mpl::list<sc::custom_reaction<UpdateEvt>> reactions;
		sc::result react(const UpdateEvt &evt);

		FCBulletActive(void)
		{
		}

		~FCBulletActive(void)
		{
		}
	};

	class FCBulletHit: public FSM::Logic<FCBulletHit, FCBulletLogic>
	{
	public:
		typedef boost::mpl::list<sc::custom_reaction<UpdateEvt>> reactions;
		sc::result react(const UpdateEvt &evt);
		FCBulletHit(void)
		{
		}
		
		~FCBulletHit(void)
		{
		}
	};

}