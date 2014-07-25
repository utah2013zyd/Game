#include "FCBulletLogic.h"
#include "FCBullet.h"

using namespace Orz;

sc::result FCBulletInactive::react(const EvBulletFire & evt)
{
	FCBullet* bullet = getOwner();
	return transit<FCBulletActive>();
}

sc::result FCBulletActive::react(const UpdateEvt &evt)
{
	FCBullet* bullet = getOwner();
	bullet->fly();
	if(bullet->checkHit())
	{
		bullet->setInactive();
		return transit<FCBulletHit>();
	}
	else if(bullet->isOutReach())
	{
		bullet->setInactive();
		return transit<FCBulletInactive>();
	}
	return forward_event();
}

sc::result FCBulletHit::react(const UpdateEvt &evt)
{
	FCBullet* bullet = getOwner();
	bullet->explode();

	return transit<FCBulletInactive>();
}
