
#ifndef __Orz_ToolkitBase_EventWorld_h__
#define __Orz_ToolkitBase_EventWorld_h__

#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/EventSystem/EventHandler.h>

namespace Orz
{



//class EventWorldImpl;
//template class _OrzToolkitBaseExport boost::scoped_ptr<EventWorldImpl>;

/** EventWorld 核心的消息管理对象，提供消息分发，消息处理器管理等功能
    @remarks
	   消息世界需要被在固定的帧速率下被更新
*/
class _OrzToolkitBaseExport EventWorld: private boost::noncopyable
{
private:
	
	typedef std::vector<Event*> EventList;
	typedef std::vector<EventHandlerPtr> EventHandlerList;
public:

	EventWorld(void);
	~EventWorld(void);
	/**
	更新世界，请用固定的速率该进行此造作，在某些时候可以通过这个接口实现消息世界的暂停、快放以及慢进等逻辑特效。
	*/
	void update(void);

	/**
	向世界注册消息管理器，
		@param handler 消息管理器的智能指针
	*/
	void comeIn(EventHandlerPtr handler);
	/**
	向世界注销消息管理器，
		@param handler 消息管理器的智能指针
	*/
	void goOut(EventHandlerPtr handler);

	/**
	对世界进行初始化操作
	*/
	bool init(void);

	/**
	对世界进行结束操作，这里会把剩余的消息依次广播
	*/
	void shutdown(void);
	
	/** 对一个消息Event进行广播操作
		@param evt 一个被广播消息的指针。
	*/
	void broadcasting(Event * evt);


	//void swap(EventWorld & world) throw();
private:
	//boost::scoped_ptr<EventWorldImpl> _impl;
	
	void _broadcasting(Event * evt);
	
	bool addHandler(EventHandlerPtr handler);
	void removeHandler(EventHandlerPtr handler);
	
	void swapEvtList(void) throw();
	void updateEvent(void);

	EventHandlerList _handlers;
	boost::scoped_ptr<EventList> _nowEvtList;
	boost::scoped_ptr<EventList> _nextEvtList;


};
}

#endif

