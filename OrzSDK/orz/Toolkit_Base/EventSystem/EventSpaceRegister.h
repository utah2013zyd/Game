

#ifndef __Orz_ToolkitBase_EventSpaceRegister_h__
#define __Orz_ToolkitBase_EventSpaceRegister_h__

#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/EventSystem/Event.h>

namespace Orz
{

template<class ES>
class EventSpaceRegister: public EventSpace
{
public:

	EventSpaceRegister():EventSpace(ES::ES_ID, ES::NUM)
	{
		EventFactory::getInstance().regEventSpace(this);
	}
	virtual ~EventSpaceRegister(void)
	{
		//EventFactory::getInstance().unregEventSpace(ES::ES_ID);
	}



	
	///通过本空间内的消息值创建消息
	static Event * createEvent(Event::message_type msg);

	
	///判断消息是否在本命名空间内和某个消息值相同
	static bool equal(Event * evt, Event::message_type msg);

	
	///得到消息在本消息空间内消息值
	static Event::message_type getMsg(Event * evt);

	
	///查看某消息是否再此空间中
	static bool in(Event * evt);

	
private:

};

template<class ES>
Event *  EventSpaceRegister<ES>::createEvent(Event::message_type msg)
{
	return ES::getPtr()->createEvent(msg);
}

template<class ES>
bool EventSpaceRegister<ES>::equal(Event * evt, Event::message_type msg)
{
	return ES::getPtr()->equal(evt, msg);
}

template<class ES>
Event::message_type EventSpaceRegister<ES>::getMsg(Event * evt)
{
	return ES::getPtr()->getMsg(evt);
}


template<class ES>
bool EventSpaceRegister<ES>::in(Event * evt)
{
	return ES::getPtr()->in(evt);
}











}

#define DEF_EVENT_BEGIN_WITH_EXPORT(Export, Space) \
class Export Space:public EventSpaceRegister<Space> \
{  \
public: \
	enum Event \
	{



#define DEF_EVENT_BEGIN(Space) \
class Space:public EventSpaceRegister<Space> \
{  \
public: \
	enum Event \
	{

#define DEF_EVENT(Event) Event,


#define DEF_EVENT_END(Space) \
		NUM \
	}; \
	static const std::string ES_ID; \
	static EventSpace * getPtr(void); \
	static EventSpace & getRef(void); \
private: \
	static Space _obj; \
};


#define DEF_EVENT_CPP(Space)  const std::string Space::ES_ID(#Space); \
Space Space::_obj; \
 \
EventSpace * Space::getPtr(void) \
{ \
	return &_obj; \
} \
 \
EventSpace & Space::getRef(void) \
{ \
	return *getPtr(); \
}

#endif
