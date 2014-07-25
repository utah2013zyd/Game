//#include <orz/Toolkit_Base/EventSystem/EventWorld.h>
//#include <orz/Toolkit_Base/EventSystem/Event.h>
//#include <orz/Toolkit_Base/EventSystem/EventHandler.h>
//#include <orz/Toolkit_Base/EventSystem/EventFactory.h>
//#include <orz/Toolkit_Base/Exception.h>
//
//#include "EventSystem/EventWorldImpl.h"
//
//#include "EventSystem/EventHandlerImpl.h"
//#include "EventSystem/EventFactoryImpl.h"
//#include "EventSystem/EventSpaceImpl.h"
//
////namespace Orz
////{
////template<>
////inline void Event::setData<EventHandlerPtr>(EventHandlerPtr handler)
////{
////	any_type ptr(new boost::any(handler));
////	setData(ptr);
////}
////
////template<>
////inline EventHandlerPtr Event::getData<EventHandlerPtr>(void)
////{
////	any_type ptr = getData<any_type>();
////	EventHandlerPtr handler;
////	try{
////		handler = boost::any_cast< EventHandlerPtr >(*ptr);
////	}
////	catch(boost::bad_any_cast &)
////	{
////		throw ORZ_EXCEPTION(Exception::ERR_ITEM_NOT_FOUND);//<<errno_info_text("the Event have no data with type std::string!");
////	}
////	return handler;
////}
////
////}
//using namespace Orz;
//
//
//EventWorldImpl::EventWorldImpl(void):
//_nowEvtList(new EventList()), 
//_nextEvtList(new EventList()),
//_isExit(false)
//{}
//EventWorldImpl::~EventWorldImpl(void){}
//	
//
//
//		
//void EventWorldImpl::update(void)
//{
//	Event * evt = EventFactory::getInstance().createEvent();
//	evt->setMsg(EVENT_UPDATE_HANDLER);
//	evt->setSystem();
//	broadcasting(evt);
//	//popEvent();
//
//	return updateEvent();
//}
//
//void EventWorldImpl::comeIn(EventHandlerPtr handler)
//{
//
//	Event * evt = EventFactory::getInstance().createEvent();
//	evt->setMsg(EVENT_ADD_HANDLER);
//	evt->setSystem();
//	evt->setData<EventHandlerPtr>(handler);
//	broadcasting(evt);
//		 
//}
//
//bool EventWorldImpl::addHandler(EventHandlerPtr handler)
//{
//	if(std::find(_handlers.begin(), _handlers.end(), handler) == _handlers.end())
//	{
//		_handlers.push_back(handler);
//	}
//	else
//	{
//		throw ORZ_EXCEPTION(Exception::ERR_DUPLICATE_ITEM);
//	}
//	return true;
//}
//void EventWorldImpl::removeHandler(EventHandlerPtr handler)
//{
//	//handler->disable();
//	_handlers.erase(std::remove(_handlers.begin(), _handlers.end(), handler), _handlers.end());
//}
//
//void EventWorldImpl::goOut(EventHandlerPtr handler)
//{
//
//	Event * evt = EventFactory::getInstance().createEvent();
//	evt->setMsg(EVENT_REMOVE_HANDLER);
//	evt->setSystem();
//	evt->setData<EventHandlerPtr>(handler);
//	broadcasting(evt);
//
//}
//
//bool EventWorldImpl::init(void)
//{
//
//
//
//	
//	return true;
//}
//void EventWorldImpl::shutdown(void)
//{
//
//	//abc//this->broadcasting(evt);
//	
//	while(!(_nowEvtList->empty() && _nextEvtList->empty()))
//	{
//		updateEvent();
//	}
//		//updateEvent();
//	/*
//	if(_recorder)
//	{
//		_recorder->shutdown();
//		_recorder = NULL;
//	}
//	*/
//	//_theater->shutdown();
//	//goOut(_theater);
//	/*
//	EventHandlerList::iterator iHandler;
//	for(iHandler = _handlers.begin(); iHandler != _handlers.end(); ++iHandler)
//	{
//		goOut(*iHandler);//(*iHandler)->execute(*iEvt);
//	}
//	*/
//
//}
//
//void EventWorldImpl::broadcasting(Event * evt)
//{
//	//_channelBackup.add(evt->getChannel());
//	_nextEvtList->push_back(evt);
//}
///*
//void EventWorldImpl::addRecorder(EventRecorder * recorder)
//{}
//void EventWorldImpl::removeRecorder(EventRecorder * recorder)
//{}
//*/
//
//
//void EventWorldImpl::updateEvent(void)
//{
//	EventList::iterator iEvt;
//	for(iEvt = _nowEvtList->begin(); iEvt != _nowEvtList->end(); ++iEvt)
//	{
//		if((*iEvt)->system())
//		{
//
//			switch((*iEvt)->getMsg())
//			{
//			case EVENT_ADD_HANDLER:
//				addHandler((*iEvt)->getData<EventHandlerPtr>());
//				break;
//			case EVENT_REMOVE_HANDLER:
//				removeHandler((*iEvt)->getData<EventHandlerPtr>());
//				break;
//			case EVENT_UPDATE_HANDLER:
//				{
//					EventHandlerList::iterator iHandler;
//					for(iHandler = _handlers.begin(); iHandler != _handlers.end(); ++iHandler)
//					{
//						(*iHandler)->execute(*iEvt);
//					}
//				}
//				break;
//			}
//		
//			
//		}
//		else
//		{
//			EventHandlerPtr receiver= (*iEvt)->getReceiver();
//			if(receiver)
//			{
//				receiver->execute(*iEvt);
//			}else
//			{
//				EventHandlerList::iterator iHandler;
//				for(iHandler = _handlers.begin(); iHandler != _handlers.end(); ++iHandler)
//				{
//					if((*iEvt)->system() || (*iHandler)->getChannel().listening((*iEvt)->getChannel()))
//						(*iHandler)->execute(*iEvt);
//				}
//			}
//		}
//		EventFactory::getInstance().destroyEvent(*iEvt);
//	}
//
//	
//	_nowEvtList->clear();
//	//_channel.clear();
//	swapEvtList();
//	
//}
//
//void EventWorldImpl::swapEvtList(void) throw()
//{
//
//	//_channel.swap(_channelBackup);
//	_nowEvtList.swap(_nextEvtList);
//
//}
