#ifndef __Orz_ToolkitBase_PreDeclare_H_
#define __Orz_ToolkitBase_PreDeclare_H_
namespace Orz
{
//IDManager
	class IDManager;
	typedef boost::shared_ptr<IDManager> IDManagerPtr;


//EventSystem
	class EventWorld;
	class Event;
	class EventChannel;
	class EventHandler;
	class EventSpace;
	class EventFactory;
	class EventSender;

	typedef boost::shared_ptr<EventWorld> EventWorldPtr;
	typedef boost::shared_ptr<EventHandler> EventHandlerPtr;
	typedef boost::weak_ptr<EventHandler> WeakEventHandlerPtr;
	typedef boost::shared_ptr<EventSpace> EventSpacePtr;
	typedef boost::shared_ptr<EventFactory> EventFactoryPtr;
	typedef boost::shared_ptr<EventSender> EventSenderPtr;
	

//DynLibManager	
	class DynLib;
	typedef boost::shared_ptr<DynLib> DynLibPtr;
	
	class Timer;
	typedef boost::shared_ptr<Timer> TimerPtr;
}

#endif
