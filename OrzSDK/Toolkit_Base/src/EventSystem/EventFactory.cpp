#include <orz/Toolkit_Base/EventSystem/EventFactory.h>

#include "EventSystem/EventFactoryImpl.h"

using namespace Orz;

void EventFactory::destroyEvent(Event * evt)
{
	assert(evt);
	_impl->destroyEvent(evt);

}
Event * EventFactory::createEvent(void)
{
	return _impl->createEvent();
}

void EventFactory::regEventSpace(EventSpace * es)
{
	return _impl->regEventSpace(es);
}
EventSpace * EventFactory::getEventSpace(const std::string & name) const
{
	return _impl->getEventSpace(name);
}

EventSpace * EventFactory::findEventSpace(Event * evt) const
{
	return _impl->findEventSpace(evt);
}

void EventFactory::unregEventSpace(const std::string & name)
{
	_impl->unregEventSpace(name);
}

EventFactory & EventFactory::getInstance()
{
	return *(getInstancePtr());
}
EventFactory * EventFactory::getInstancePtr()
{
	static EventFactory instance;
	return &instance;
}


EventFactory::EventFactory(void):_impl(new EventFactoryImpl())
{

}
EventFactory::~EventFactory()
{

}

