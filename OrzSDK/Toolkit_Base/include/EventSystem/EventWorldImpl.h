//#ifndef __Orz_ToolkitBase_EventWorldImpl_h__
//#define __Orz_ToolkitBase_EventWorldImpl_h__
//#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
//#include <orz/Toolkit_Base/EventSystem/EventWorld.h>
//
//namespace Orz
//{
//class _OrzToolkitBasePrivate EventWorldImpl:private boost::noncopyable
//{
//
//public:
//	typedef std::vector<Event*> EventList;
//	typedef std::vector<EventHandlerPtr> EventHandlerList;
//public:
//	EventWorldImpl(void);
//	~EventWorldImpl(void);
//	
//	void swapEvtList(void) throw();
//	
//	void update(void);
//	void updateEvent(void);
//	void comeIn(EventHandlerPtr handler);
//	void goOut(EventHandlerPtr handler);
//
//	bool init(void);
//	void shutdown(void);
//	void broadcasting(Event * evt);
//
//private:
//	bool addHandler(EventHandlerPtr handler);
//	void removeHandler(EventHandlerPtr handler);
//	
//	
//	EventHandlerList _handlers;
//	boost::scoped_ptr<EventList> _nowEvtList;
//	boost::scoped_ptr<EventList> _nextEvtList;
//	bool _isExit;
//	
//};
//}
//#endif
