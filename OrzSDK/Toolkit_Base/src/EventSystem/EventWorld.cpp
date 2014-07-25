#include <orz/Toolkit_Base/EventSystem/EventWorld.h>
#include <orz/Toolkit_Base/EventSystem/Event.h>
#include <orz/Toolkit_Base/EventSystem/EventHandler.h>
#include <orz/Toolkit_Base/EventSystem/EventFactory.h>


#include "EventSystem/EventFactoryImpl.h"

using namespace Orz;


EventWorld::EventWorld(void):
_handlers(),
_nowEvtList(new EventList()), 
_nextEvtList(new EventList())
{}

EventWorld::~EventWorld(void)
{}




void EventWorld::update()
{
	Event * evt = EventFactory::getInstance().createEvent();
	evt->setMsg(EVENT_UPDATE_HANDLER);
	evt->setSystem();
	_broadcasting(evt);
	updateEvent();
}

void EventWorld::comeIn(EventHandlerPtr handler)
{
	handler->enable(this);
	
	Event * evt = EventFactory::getInstance().createEvent();
	evt->setMsg(EVENT_ADD_HANDLER);
	evt->setSystem();
	evt->setData<EventHandlerPtr>(handler);
	_broadcasting(evt);
	
}

void EventWorld::goOut(EventHandlerPtr handler)
{

	
	if(!handler->isInTheWorld(this))
		return;
	handler->disable();
	
	
	Event * evt = EventFactory::getInstance().createEvent();
	evt->setMsg(EVENT_REMOVE_HANDLER);
	evt->setSystem();
	evt->setData<EventHandlerPtr>(handler);
	_broadcasting(evt);
	
}


bool EventWorld::init()
{
	return true;
}


void EventWorld::shutdown()
{
	while(!(_nowEvtList->empty() && _nextEvtList->empty()))
	{
		updateEvent();
	}
}


void EventWorld::broadcasting(Event * evt)
{
	if(evt->system())
		throw ORZ_EXCEPTION(Exception::ERR_INVALID_PARAMS)<<errno_info_text("Can't broadcasting System Event!");
	_broadcasting(evt);
}
void EventWorld::_broadcasting(Event * evt)
{
	_nextEvtList->push_back(evt);
}

bool EventWorld::addHandler(EventHandlerPtr handler)
{
	if(std::find(_handlers.begin(), _handlers.end(), handler) == _handlers.end())
	{
		_handlers.push_back(handler);
	}
	else
	{
		throw ORZ_EXCEPTION(Exception::ERR_DUPLICATE_ITEM);
	}
	return true;
}
void EventWorld::removeHandler(EventHandlerPtr handler)
{
	//handler->disable();
	_handlers.erase(std::remove(_handlers.begin(), _handlers.end(), handler), _handlers.end());
}


//
//void EventWorld::swap(EventWorld & world) throw()
//{
//	return _impl.swap(world._impl);
//}



void EventWorld::updateEvent(void)
{
	EventList::iterator iEvt;
	for(iEvt = _nowEvtList->begin(); iEvt != _nowEvtList->end(); ++iEvt)
	{
		if((*iEvt)->system())
		{

			switch((*iEvt)->getMsg())
			{
			case EVENT_ADD_HANDLER:
				addHandler((*iEvt)->getData<EventHandlerPtr>());
				break;
			case EVENT_REMOVE_HANDLER:
				removeHandler((*iEvt)->getData<EventHandlerPtr>());
				break;
			case EVENT_UPDATE_HANDLER:
				{
					EventHandlerList::iterator iHandler;
					for(iHandler = _handlers.begin(); iHandler != _handlers.end(); ++iHandler)
					{
						
						(*iHandler)->execute(*iEvt);
					}
				}
				break;
			}
		
			
		}
		else
		{
			EventHandlerPtr receiver= (*iEvt)->getReceiver();
			if(receiver)
			{
				receiver->execute(*iEvt);
			}else
			{
				EventHandlerList::iterator iHandler;
				for(iHandler = _handlers.begin(); iHandler != _handlers.end(); ++iHandler)
				{
					if((*iHandler)->getChannel().listening((*iEvt)->getChannel()))
						(*iHandler)->execute(*iEvt);
				}
			}
		}
		EventFactory::getInstance().destroyEvent(*iEvt);
	}

	
	_nowEvtList->clear();
	//_channel.clear();
	swapEvtList();
	
}

void EventWorld::swapEvtList(void) throw()
{

	//_channel.swap(_channelBackup);
	_nowEvtList.swap(_nextEvtList);

}
