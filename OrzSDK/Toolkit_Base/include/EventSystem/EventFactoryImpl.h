#ifndef __Orz_ToolkitBase_EventFactoryImpl_h__
#define __Orz_ToolkitBase_EventFactoryImpl_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/Exception.h>
#include <orz/Toolkit_Base/EventSystem/EventSpace.h>
#include <orz/Toolkit_Base/EventSystem/Event.h>
#include "EventSystem/EventSpaceImpl.h"
namespace Orz
{
class _OrzToolkitBasePrivate EventFactoryImpl:private boost::noncopyable
{
typedef std::map<std::string,  EventSpace *> EventSpacesMap;
public:
	inline EventFactoryImpl(void):
	_epool(),
	_spaces(),
	_amount(0)
	//,_n(0)
	{}

	inline ~EventFactoryImpl(void)
	{
	}
	

	inline void destroyEvent(Event * evt);
	inline Event * createEvent(void);

	inline void regEventSpace(EventSpace * es);
	inline EventSpace * getEventSpace(const std::string & name) const;
	inline EventSpace * findEventSpace(Event * evt) const;
	inline void unregEventSpace(const std::string & name);

private:
	boost::object_pool<Event> _epool;
	EventSpacesMap _spaces;
	size_t _amount;
	//size_t _n;
};


inline Event * EventFactoryImpl::createEvent(void)
{
	//_n++;
	return _epool.construct();
}


void EventFactoryImpl::destroyEvent(Event * evt)
{
	//_n--;
	_epool.destroy(evt);
}


void EventFactoryImpl::regEventSpace(EventSpace * es)
{
	EventSpacesMap::iterator it = _spaces.find(es->getName());
	if(it != _spaces.end())
	{
		throw ORZ_EXCEPTION(Exception::ERR_DUPLICATE_ITEM)<<errno_info_text("You are repeat register a EventSpace!");
	}
	es->setSpace(_amount);
	_amount += es->amount();

	_spaces.insert(std::make_pair(es->getName(), es));

}
EventSpace * EventFactoryImpl::getEventSpace(const std::string & name) const
{
	EventSpacesMap::const_iterator it = _spaces.find(name);
	if(it == _spaces.end())
	{
		throw ORZ_EXCEPTION(Exception::ERR_ITEM_NOT_FOUND)<<errno_info_text("Can't find the EventSpace!");
	}
	return it->second;
}

EventSpace * EventFactoryImpl::findEventSpace(Event * evt) const
{
	EventSpacesMap::const_iterator it;
	for(it = _spaces.begin(); it != _spaces.end(); ++it)
	{
		if(it->second->in(evt))
			return it->second;
	}
	return NULL;

}
void EventFactoryImpl::unregEventSpace(const std::string & name)
{
	EventSpacesMap::iterator it = _spaces.find(name);
	if(it == _spaces.end())
	{
		throw ORZ_EXCEPTION(Exception::ERR_ITEM_NOT_FOUND)<<errno_info_text("Can't find the EventSpace!");
	}
	_spaces.erase(it);
}

}
#endif
