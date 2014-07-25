#include <orz/Toolkit_Base/EventSystem/EventHandler.h>
//#include "EventSystem/EventWorld.h"
#include <orz/Toolkit_Base/EventSystem/Event.h>
#include "EventSystem/EventHandlerImpl.h"
using namespace Orz;

EventHandler::EventHandler(void):
	_enable(false),
	_world(NULL),
	_channel(),
	_enableUpdate(false),
	_updateReceiveEvent(false)
{
	_channel.fill();
}


void EventHandler::execute(Event * evt)
{	
	
	assert(evt);
	if(_enable)
	{
		if(isUpdateEvent(evt))
		{
			if(_enableUpdate)
			{
				doFrame();
				if(_updateReceiveEvent)
					doExecute(evt);
			}
		}
		else
			doExecute(evt);
	}
		
}



void EventHandler::enableUpdate(bool receiveEvent)
{
	
	_updateReceiveEvent = receiveEvent;
	_enableUpdate = true;
}

void EventHandler::disableUpdate(void)
{
	_enableUpdate = false;
}
		

void EventHandler::enable(EventWorld * world)
{
	_world = world;
	_enable  = true;
	doEnable();
}
void EventHandler::disable(void)
{
	_enable = false;
	doDisable();
	_world = NULL;
}




/**
处理消息接口
*/
void EventHandler::doExecute(Event * evt)
{
}

bool EventHandler::isInTheWorld(EventWorld * world) const
{

	if(world)
	{
		return world == _world;
	}
	return false;

}



bool EventHandler::isInWorld(void) const
{
	
	return _world != NULL;

}

bool EventHandler::listening(const EventChannel & channel)
{
	return _channel.listening(channel);
}



void EventHandler::setChannel(const EventChannel & channel)
{
	_channel = channel;
}

const EventChannel EventHandler::getChannel(void) const
{
	return _channel;
}
void EventHandler::addChannel(const EventChannel & channel)
{
	_channel.add(channel);
}
void EventHandler::removeChannel(const EventChannel & channel)
{
	_channel.remove(channel);
}
void EventHandler::clearChannel(void)
{
	_channel.clear();
}
	
///得到世界的指针
EventWorld * EventHandler::getWorld(void)
{
	return _world;
}//{return _world;}

void EventHandler::doEnable(void)
{

}


void EventHandler::doDisable(void)
{

}

void EventHandler::doFrame(void)
{
}

//
//EventHandler::EventHandler(const EventHandler& eh):_impl(new EventHandlerImpl(*(eh._impl))),_enable(false)
//{
//	
//}
//void EventHandler::swap(EventHandler& eh)  throw()
//{
//	_impl.swap(eh._impl);
//}
//EventHandler& EventHandler::operator = (const EventHandler& eh)
//{
//	(*_impl) = *(eh._impl);
//	return (*this);
//}

EventHandler::~EventHandler(void)
{
	if(this->isInWorld())
		this->disable();
}


/*
void EventHandler::_everyFrame(void)
{
	
}*/
