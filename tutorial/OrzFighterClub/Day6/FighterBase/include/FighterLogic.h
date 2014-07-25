#ifndef __Orz_FighterBase_FCFighterLogic_h__
#define __Orz_FighterBase_FCFighterLogic_h__

#include "FighterBaseConfig.h"
#include <orz/Toolkit_Base/FSMLogic.h>
class ActionOverEvt : public boost::statechart::event<ActionOverEvt> {};

namespace Orz
{
	/*
	typedef FiniteStateMachine<GameLogic, Owner> LogicType---- 游戏根逻辑 （orz\Toolkit_Base\FSMLogic\FSMLogic.h文件内，被隐藏）
	FSM<FCFighterLogic, FighterBase, FCFighterFall>::MainLogic---- 游戏主逻辑
	typedef boost::statechart::simple_state<GameLogic, Owner, InnerInitial, historyMode> Logic---- 子逻辑（orz\Toolkit_Base\FSMLogic\FSMLogic.h文件内）
											  自己		父亲  儿子(第一个激活的逻辑)
	*/
	class FighterBase;
	class FCFighterFall;
	class FCFighterRun;
	class FCFighterTest;
	class FCFighterAction;
	class FCFighterLogic: public FSM::MainLogic<FCFighterLogic, FighterBase, FCFighterFall> 
	{
	
	};
                                   
	class FCFighterFall: public FSM::Logic<FCFighterFall/*自己*/, FCFighterLogic/*父亲*/>
	{
	public:

		typedef boost::mpl::list< sc::custom_reaction< UpdateEvt > > reactions;
		sc::result react(const UpdateEvt & evt);
	

	};

	//                                                                
	class FCFighterRun: public FSM::Logic<FCFighterRun/*自己*/, FCFighterLogic/*父亲*/, FCFighterTest/*儿子*/>
	{
	};

	
	class FCFighterTest: public FSM::Logic<FCFighterTest, FCFighterRun>
	{
	public:

		typedef boost::mpl::list< sc::custom_reaction< UpdateEvt > > reactions;
		sc::result react(const UpdateEvt & evt);
		
	
	};



	class FCFighterAction: public FSM::Logic<FCFighterAction, FCFighterRun>
	{
	public:
		typedef boost::mpl::list< sc::custom_reaction< ActionOverEvt > > reactions;
		sc::result react(const ActionOverEvt & evt);

	};
}
#endif