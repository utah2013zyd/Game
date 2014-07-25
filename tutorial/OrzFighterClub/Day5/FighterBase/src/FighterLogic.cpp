#include "FighterLogic.h"
#include "FighterBase.h"

#include "FCKnowledge.h"

using namespace Orz;

sc::result FCFighterFall::react(const UpdateEvt & evt)
{
	FighterBase * fighter = getOwner();
	if(fighter)
	{

		
		float d = FCKnowledge::getSingleton().mapInfo().getGround();
		
		Ogre::Vector3 pos = fighter->getPosition();
		if(pos.y > d + 0.31f)
		{
			pos.y = pos.y -0.3f;
			fighter->setPosition(pos);
		}
		else
		{
			pos.y = d;
			fighter->nextPosition();
			return transit<FCFighterRun>();
			
		}


	}
	return forward_event();
}


sc::result FCFighterTest::react(const UpdateEvt & evt)
{
	FighterBase * fighter = getOwner();
	

	
	FighterBase::ActionReturnPtr ar = fighter->thinking(fighter->getThePower());
	fighter->sendFightingEvent(ar);
	switch(ar->get<0>())
	{
	case FighterBase::Fire:
		fighter->fire(ar->get<1>(), ar->get<2>());
		fighter->addPower(10.f);
		return transit<FCFighterAction>();
	case FighterBase::Move:
		fighter->move(ar->get<1>(), ar->get<2>());
		fighter->addPower(10.f);
		return transit<FCFighterAction>();
	case FighterBase::Sleep:
		fighter->move(FighterBase::East, 0.f, true);
		fighter->addPower(25.f);
		return transit<FCFighterAction>();
	
	}

	return forward_event();
}


sc::result FCFighterAction::react(const ActionOverEvt & evt)
{
	return transit<FCFighterTest>();
}
