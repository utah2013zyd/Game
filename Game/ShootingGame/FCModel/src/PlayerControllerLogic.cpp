#include "PlayerControllerLogic.h"
#include <stdio.h>
using namespace Orz;

sc::result Stable::react(const EvKeyAPress & evt)
{
	printf("key A is down!\n");
	return transit<TurnLeft>();
}

sc::result Stable::react(const EvKeyDPress & evt)
{
	printf("key D is down! \n");
	return transit<TurnRight>();
}

sc::result Stable::react(const EvKeyQPress & evt)
{
	printf("key Q is down! \n");
	return transit<BankLeft>();
}

sc::result Stable::react(const EvKeyEPress &evt)
{
	printf("key E is down! \n");
	return transit<BankRight>();
}

sc::result Stable::react(const EvKeyWPress &evt)
{
	printf("key W is down! \n");
	return transit<PitchForward>();
}

sc::result Stable::react(const EvKeySPress &evt)
{
	printf("key S is down! \n");
	return transit<PitchBackward>();
}


sc::result TurnLeft::react(const EvKeyARelease &evt)
{
	printf("key A is up! \n");
	return transit<Stable>();
}

sc::result TurnRight::react(const EvKeyDRelease &evt)
{
	printf("key D is up! \n");
	return transit<Stable>();
}

sc::result BankLeft::react(const EvKeyQRelease &evt)
{
	printf("key Q is up! \n");
	return transit<Stable>();
}

sc::result BankRight::react(const EvKeyERelease &evt)
{
	printf("key E is up! \n");
	return transit<Stable>();
}

sc::result PitchForward::react(const EvKeyWRelease &evt)
{
	printf("key W is up! \n");
	return transit<Stable>();
}

sc::result PitchBackward::react(const EvKeySRelease &evt)
{
	printf("key S is up! \n");
	return transit<Stable>();
}