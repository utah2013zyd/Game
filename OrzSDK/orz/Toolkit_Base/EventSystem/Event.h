
#ifndef __Orz_ToolkitBase_Event_h__
#define __Orz_ToolkitBase_Event_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/EventSystem/EventChannel.h>
#include <orz/Toolkit_Base/Exception.h>
#include <orz/Toolkit_Base/Variant.h>

namespace Orz
{

enum SystemEvent
{
	EVENT_REMOVE_HANDLER = -1,
	EVENT_ADD_HANDLER = -2,
	EVENT_UPDATE_HANDLER = -3
};

/**
事件对象，封装系统中的各种消息。
*/
class  Event
{
public:
	friend class EventWorldImpl;
	friend class EventWorld;
	friend class EventHandler;
	//typedef uint32 id_type;
	typedef uint32 message_type;
	//typedef AnyPtr any_type;
	typedef Variant data_type;
	
public:

	inline Event(void);
	inline Event(const Event & evt);
	inline Event& operator = (const Event& evt);
	inline void swap(Event& evt);

	inline ~Event(void);
	/**
	设置消息类型
	@param msg 通过一个整形数据来代表消息类型
	*/
	inline void setMsg(message_type msg);

	/**
	设置消息过滤频道
	@param channel 设置消息频道
	*/
	inline void setChannel(const EventChannel & channel);

	/**
	设置消息发送者
	@param sender 发送消息的处理器
	*/
	inline void setSender(EventHandlerPtr sender);

	
	/**
	设置消息接收者
	@param receiver 通过模板适配的任意类型数据
	*/
	inline void setReceiver(EventHandlerPtr receiver);


	/**
	设置消息传递数据
	@param data 通过模板适配的任意类型数据
	*/
	template<class DType>
	inline void setData(DType data);

	



	///得到此消息所属频道
	inline EventChannel getChannel(void) const;

	///得到此消息发送者ID
	inline EventHandlerPtr getSender(void) const;

	///得到此消息类型
	inline message_type getMsg(void) const;


	///得到消息传递数据
	template<class DType>
	inline DType getData(void);
	
	
	///得到消息接受者
	inline EventHandlerPtr getReceiver(void) const;


	
private:

	inline bool system(void) const;
	inline void setSystem(void);


	inline void init(const Event & evt);
	///频道，用于过滤消息
	EventChannel _channel;

	///发送者
	EventHandlerPtr _sender;

	///消息类型
	message_type _msg;

	///内部类型数据
	data_type _data;
	

	///接受者 如果没有设置就是广播，设置之后就只有接受者能收到消息，此时频道将失去功能。
	EventHandlerPtr _receiver;

	



};


inline Event::Event(void):
_channel(), 
_sender(),
_msg(0), 
_data(),
_receiver()
{
	_channel.fillUserChannel();
}

inline Event::~Event(void)
{}


inline Event::Event(const Event & evt)
{
	init(evt);
}
inline Event& Event::operator = (const Event& evt)
{
	init(evt);
	return (*this);
}
inline void Event::swap(Event& evt)
{
	using std::swap;
	_channel.swap(evt._channel);
	_sender.swap(evt._sender);
	swap(_msg, evt._msg);
	_data.swap(evt._data);
	_receiver.swap(evt._receiver);

}
inline void Event::init(const Event & evt)
{
	_channel = evt._channel;
	_sender = evt._sender;
	_msg = evt._msg;
	_data = evt._data;
	_receiver = evt._receiver;
}
inline void Event::setMsg(Event::message_type msg)
{
	_msg = msg;
}

inline EventHandlerPtr Event::getReceiver(void) const
{
	return _receiver;
}

inline void Event::setChannel(const EventChannel & channel)
{
	if(_channel.empty())
		throw  ORZ_EXCEPTION(Exception::ERR_INVALID_PARAMS);
	_channel = channel;
}

inline void Event::setSender(EventHandlerPtr sender)
{
	_sender = sender;
}

	
	
inline void Event::setReceiver(EventHandlerPtr handler)
{
	_receiver = handler;
}

inline EventChannel Event::getChannel(void) const
{
	return _channel;
}

///得到此消息发送者ID
inline EventHandlerPtr Event::getSender(void) const
{
	return _sender;
}

///得到此消息类型
inline Event::message_type Event::getMsg(void) const
{
	return _msg;
}


template<class DType>
inline void Event::setData(DType data)
{
	_data = VariantData<DType>::set(data);
	//_data = boost::any(data);
}



//template<class UserType>
//inline void Event::setUserData(UserType data)
//{
//	setData<AnyPtr>(make_any_ptr(data));
//}

//template<class UserType>
//inline UserType Event::getUserData(void)
//{
//	AnyPtr ptr = getData<AnyPtr>();
//	return boost::any_cast< UserType >(*ptr);
//}
template<class DType>
inline DType Event::getData(void)
{
	return VariantData<DType>::get(_data);
}



bool Event::system(void) const
{
	return _channel.empty();
}

void Event::setSystem(void)
{
	_channel.clear();
}

}
#endif

