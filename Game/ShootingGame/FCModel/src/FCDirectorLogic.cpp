#include "FCDirectorLogic.h"
#include "FCDirector.h"
#include <iostream>
using namespace Orz;

sc::result FCDirectorRunning::react(const EvPlayerExecuted & evt)
{
	CEGUIManager::getSingleton();
	std::cout << "player lose"<<std::endl;
	return transit<FCDirectorLose>();
}
sc::result FCDirectorRunning::react(const EvEnimiesExecuted & evt)
{
	std::cout << "player win" << std::endl;
	return transit<FCDirectorWin>();
}

sc::result FCDirectorWin::react(const UpdateEvt & evt)
{
	return forward_event();
}

sc::result FCDirectorLose::react(const UpdateEvt & evt)
{
	return forward_event();
}