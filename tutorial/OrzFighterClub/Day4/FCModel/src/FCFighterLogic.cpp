#include "FCFighterLogic.h"
#include "FCFighter.h"
#include "FCWall.h"
#include "FCKnowledge.h"
#include "FCEvents.h"
using namespace Orz;

sc::result FCFighterFall::react(const UpdateEvt & evt)
{
	FCFighter * fighter = getOwner();
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
	FCFighter * fighter = getOwner();
	
	if(fighter->getTargetBlocker())
		return transit<FCFighterFire>();
	
	return transit<FCFighterMove>();
}




sc::result FCFighterMove::react(const UpdateEvt & evt)
{

	FCFighter * fighter = getOwner();

	if(fighter)
	{

		Ogre::Vector3 from = fighter->getPosition();
		Ogre::Vector3 to =  fighter->getTarget();

		if(to.x > from.x)
			from.x += 0.3f;
		else
			from.x -= 0.3f;

		
		if(to.z > from.z)
			from.z += 0.3f;
		else
			from.z -= 0.3f;
		if(Ogre::Math::Abs(to.x - from.x)<0.4 && Ogre::Math::Abs(to.z - from.z)<0.4)
		{
			fighter->nextPosition();
			return transit<FCFighterTest>();
		}
		fighter->setPosition(from);
	}
	return forward_event();
}

sc::result FCFighterFire::react(const UpdateEvt & evt)
{

	FCFighter * fighter = getOwner();
	Orz::EventHandler * blocker = fighter->getTargetBlocker();
	if(blocker)
	{

		Event * evt = FCEvents::createEvent(FCEvents::SHOOTING);
		evt->setSender(fighter->shared_from_this());
		evt->setReceiver(blocker->shared_from_this());

		fighter->getWorld()->broadcasting(evt);
		
	}
	return transit<FCFighterTest>();
}