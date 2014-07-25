#include "EnemyControllerLogic.h"
#include "EnemyController.h"

using namespace Orz;


sc::result EnemyControllerApproach::react(const UpdateEvt & evt)
{
	EnemyController* fighter = getOwner();
	fighter->approach();
	_elapsedTime +=  WORLD_UPDATE_INTERVAL;
	if(fighter->withinReach() && _elapsedTime > 1.0)
	{
		_elapsedTime = 0;
		return transit<EnemyControllerFire>();
	}
	return forward_event();
}

sc::result EnemyControllerFire::react(const UpdateEvt &evt)
{
	EnemyController* fighter = getOwner();
	fighter->fire();
	return transit<EnemyControllerApproach>();
	
}
