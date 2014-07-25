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

	if(enemy)
	{

		if(!_init)
		{
			enemy->nextPosition();
			_init = true;
		}
		Ogre::Vector3 from = enemy->getPosition();
		Ogre::Vector3 to =  enemy->getTarget();

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
			return transit<FCFighterRun>();
		}
		enemy->setPosition(from);
	}
	return forward_event();
}
