#include "FCConfig.h"

#include <orz/Toolkit_Base/FSMLogic.h>
#pragma once
namespace Orz
{
	class EnemyController;
	class EnemyControllerApproach;
	class EnemyControllerFire;
	class EnemyControllerHide;

	//struct EvFighterCrush:sc::event<EvFighterCrush>{};

	class EnemyControllerLogic : public FSM::MainLogic<EnemyControllerLogic, EnemyController, EnemyControllerApproach>
	{
	};

	class EnemyControllerApproach: public FSM::Logic<EnemyControllerApproach, EnemyControllerLogic>
	{
	public:
		typedef boost::mpl::list<sc::custom_reaction<UpdateEvt> > reactions;
		sc::result react(const UpdateEvt & evt);
		double _elapsedTime;
		EnemyControllerApproach(void): _elapsedTime(0.0)
		{
		}

		~EnemyControllerApproach(void)
		{
		}
	};

	class EnemyControllerFire: public FSM::Logic<EnemyControllerFire, EnemyControllerLogic>
	{
	public:
		typedef boost::mpl::list<sc::custom_reaction<UpdateEvt> > reactions;
		sc::result react(const UpdateEvt & evt);
		EnemyControllerFire(void)
		{
		}

		~EnemyControllerFire(void)
		{
		}
	};

	class EnemyControllerHide: public FSM::Logic<EnemyControllerHide, EnemyControllerLogic>
	{
	public:
		EnemyControllerHide(void)
		{
		}

		~EnemyControllerHide(void)
		{
		}
	};

}