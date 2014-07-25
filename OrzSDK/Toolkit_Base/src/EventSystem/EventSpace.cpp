#include <orz/Toolkit_Base/EventSystem/EventSpace.h>
#include <orz/Toolkit_Base/EventSystem/EventFactory.h>
#include "EventSystem/EventSpaceImpl.h"
#include "EventSystem/EventFactoryImpl.h"


using namespace Orz;
class Space
{
public:
	inline Space(void):_begin(-1), _end(-1){}
	inline ~Space(void){}
	inline Event::message_type cover(Event * evt)  const
	{
		assert(evt);
		return (evt->getMsg() - _begin);
	}

	inline Event::message_type uncover(Event::message_type msg) const
	{

		return (msg + _begin);
	}
	inline bool in(Event * evt) const
	{
		Event::message_type msg = evt->getMsg();
		return (msg >= _begin) &&  (msg < _end);
	}
	inline bool equal(Event * evt, Event::message_type msg)  const
	{
		assert(evt);
		return cover(evt) == msg;
	}
	inline void reset(size_t begin, size_t end)
	{
		_begin = begin;
		_end = end;
	}
private:
	size_t _begin;
	size_t _end;
};


Event * EventSpace::createEvent(Event::message_type msg)
{
	Event * evt = EventFactory::getInstance().createEvent();
	evt->setMsg(_space.uncover(msg));
	return evt;
}



bool EventSpace::equal(Event * evt, Event::message_type msg) const
{
	return _space.equal(evt, msg);
}
Event::message_type EventSpace::getMsg(Event * evt) const
{
	assert(evt);
	return _space.cover(evt);
}


bool EventSpace::in(Event * evt) const
{
	return _space.in(evt);
}

const std::string & EventSpace::getName(void) const
{
	return _name;
}
size_t EventSpace::amount() const
{
	return _amount;
}

void EventSpace::setSpace(size_t shift)
{
	
	_space.reset(shift, shift + amount());
	//_impl->setSpace(shift);
}
EventSpace::EventSpace(const std::string & name, size_t amount):_space(), _name(name), _amount(amount)//: _impl(new EventSpaceImpl(name, amount))
{

}
EventSpace::~EventSpace(void)
{

}
