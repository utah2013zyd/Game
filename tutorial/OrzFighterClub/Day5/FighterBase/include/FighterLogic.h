#ifndef __Orz_FighterBase_FCFighterLogic_h__
#define __Orz_FighterBase_FCFighterLogic_h__

#include "FighterBaseConfig.h"
#include <orz/Toolkit_Base/FSMLogic.h>
class ActionOverEvt : public boost::statechart::event<ActionOverEvt> {};

namespace Orz
{
	/*
	typedef FiniteStateMachine<GameLogic, Owner> LogicType---- ��Ϸ���߼� ��orz\Toolkit_Base\FSMLogic\FSMLogic.h�ļ��ڣ������أ�
	FSM<FCFighterLogic, FighterBase, FCFighterFall>::MainLogic---- ��Ϸ���߼�
	typedef boost::statechart::simple_state<GameLogic, Owner, InnerInitial, historyMode> Logic---- ���߼���orz\Toolkit_Base\FSMLogic\FSMLogic.h�ļ��ڣ�
											  �Լ�		����  ����(��һ��������߼�)
	*/
	class FighterBase;
	class FCFighterFall;
	class FCFighterRun;
	class FCFighterTest;
	class FCFighterAction;
	class FCFighterLogic: public FSM::MainLogic<FCFighterLogic, FighterBase, FCFighterFall> 
	{
	
	};
                                   
	class FCFighterFall: public FSM::Logic<FCFighterFall/*�Լ�*/, FCFighterLogic/*����*/>
	{
	public:

		typedef boost::mpl::list< sc::custom_reaction< UpdateEvt > > reactions;
		sc::result react(const UpdateEvt & evt);
	

	};

	//                                                                
	class FCFighterRun: public FSM::Logic<FCFighterRun/*�Լ�*/, FCFighterLogic/*����*/, FCFighterTest/*����*/>
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