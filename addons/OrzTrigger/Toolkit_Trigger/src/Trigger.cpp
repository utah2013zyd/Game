//
//
//#include <orz/Toolkit_Trigger/Trigger.h>
//using namespace Orz;
//Trigger::Trigger(void):/*_isEnable(false),*/_callBack(NULL), _userData(NULL), _loop(true){}
//Trigger::~Trigger(void){}
//void Trigger::setLoop(bool loop){_loop = loop;}
//void Trigger::setCallback(TriggerCallback * callBack, void * userData)
//{
//	_callBack = callBack;
//	_userData = NULL;
//}
//void Trigger::stop(void)
//{
//	EventWorld * world = getWorld();
//	if(world)
//	{
//		world->goOut(shared_from_this());
//	}
//}
//void Trigger::restart(void)
//{
//	EventWorld * world = getWorld();
//	if(world)
//	{
//		world->goOut(shared_from_this());
//		world->comeIn(shared_from_this());
//	}
//}
//
//
#include <orz/Toolkit_Trigger/Trigger.h>

using namespace Orz;


Trigger::Trigger(int id):_id(id),_isEnable(false)
{}
Trigger::~Trigger(void)
{
	clearEventHandler();
}

	
void Trigger::addEventHandler(TriggerEventHandlerPtr eh, EventWorld * world)
{
	eh->setCallback(this);
	_eventHandlers.push_back(std::make_pair(eh, world));
	if(_isEnable)
		world->comeIn(eh);
}
void Trigger::addAction(TriggerActionPtr action)
{
	_actions.push_back(action);
}

	
void Trigger::addConditions(TriggerConditionsPtr conditions)
{
	_conditionses.push_back(conditions);
}
void Trigger::clearEventHandler(void)
{
	_stop();
	_eventHandlers.clear();
}


void Trigger::execute(Event * evt)
{
	BOOST_FOREACH(TriggerConditionsPtr conditions, _conditionses)
	{
		if(!(*conditions)(evt))
			return;
	}
	_action(evt);
}
	
void Trigger::_start(void)
{
	BOOST_FOREACH(EventHandlerPair eh, _eventHandlers)
	{
		eh.second->comeIn(eh.first);
	}
}
void Trigger::_stop(void)
{
	BOOST_FOREACH(EventHandlerPair eh, _eventHandlers)
	{
		eh.second->goOut(eh.first);
	}
}
void Trigger::_action(Event * evt)
{
	BOOST_FOREACH(TriggerActionPtr action, _actions)
	{
		(*action)(_id, evt);
	}

}

void Trigger::enable(void)
{
	_isEnable = true;
	_start();
}
void Trigger::disable(void)
{
	_stop();
	_isEnable = false;
}