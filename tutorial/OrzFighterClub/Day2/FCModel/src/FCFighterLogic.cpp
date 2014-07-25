#include "FCFighterLogic.h"
#include "FCFighter.h"
#include "FCKnowledge.h"

using namespace Orz;

sc::result FCFighterFall::react(const UpdateEvt & evt)
{
	FCFighter * enemy = getOwner();
	if(enemy)
	{
		float d = FCKnowledge::getSingleton().mapInfo().getGround();
	
		Ogre::Vector3 pos = enemy->getPosition();
		if(pos.y > d + 0.31f)
		{
			pos.y = pos.y -0.3f;
			enemy->setPosition(pos);
		}
		else
		{
			pos.y = d;

			return transit<FCFighterRun>();
			
		}


	}
	return forward_event();
}


sc::result FCFighterRun::react(const UpdateEvt & evt)
{
	FCFighter * enemy = getOwner();

	
	return forward_event();
}
