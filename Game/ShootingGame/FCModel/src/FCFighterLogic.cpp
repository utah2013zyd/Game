#include "FCFighterLogic.h"
#include "FCFighter.h"

using namespace Orz;

sc::result FCFighterLive::react(const EvFighterCrush & evt)
{
	FCFighter* fighter = getOwner();
	fighter->crush();
	return transit<FCFighterDestroying>();
}

sc::result FCFighterLive::react(const UpdateEvt & evt)
{
	FCFighter* fighter = getOwner();
	fighter->checkHit();
	fighter->forward();
	return forward_event();
}

sc::result FCFighterDestroying::react(const Orz::UpdateEvt &evt)
{
	_elapsedTime += WORLD_UPDATE_INTERVAL;
	if(_elapsedTime >= 2.0)
	{
		FCFighter* fighter = getOwner();
		fighter->disappear();
		return transit<FCFighterDead>();
	}
	return forward_event();
}