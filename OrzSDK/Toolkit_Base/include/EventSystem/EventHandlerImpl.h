//#ifndef __Orz_ToolkitBase_EventHandlerImpl_h__
//#define __Orz_ToolkitBase_EventHandlerImpl_h__
//#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
//#include <orz/Toolkit_Base/EventSystem/EventHandler.h>
//
//namespace Orz
//{
//class _OrzToolkitBasePrivate EventHandlerImpl
//{
//public:
//
//
//	inline EventHandlerImpl(void):
//	_world(NULL),
//	_channel(),
//	_enableUpdate(false)
//	{
//		_channel.fill();
//	}
//	inline EventHandlerImpl::~EventHandlerImpl(void)
//	{
//		
//	}
//
//
//	inline EventHandlerImpl(const EventHandlerImpl & ehImpl)
//	{
//		init(ehImpl);
//	}
//	inline EventHandlerImpl& EventHandlerImpl::operator = (const EventHandlerImpl & ehImpl)
//	{
//		init(ehImpl);
//		return (*this);
//	}
//
//		
//		
//		
//		
//		
//	inline void setChannel(const EventChannel & channel){_channel = channel;}
//	inline EventChannel & getChannel(void){return _channel;}
//	inline const EventChannel getChannel(void) const{return _channel;}
//
//
//
//	
//		
//	///得到世界的指针
//	inline EventWorld * getWorld(void) const {return _world;}
//	
//
//
//	/**
//	当进入世界时候调用，通常在这个时候构建图像信息
//	 
//	*/
//	inline void enable(EventWorld * world)
//	{
//		_world = world;
//	}
//			
//	/**
//	当离开世界的时候调用，通常在这个时候关闭图像信息
//	*/
//	inline void disable(void)
//	{
//		_world = NULL;
//	}
//	inline void enableUpdate()
//	{
//		_enableUpdate = true;
//	}
//	inline void disableUpdate()
//	{
//		_enableUpdate = false;
//	}
//	inline bool isUpdate(void) const
//	{
//		return _enableUpdate;
//	}
//private:
//	inline void init(const EventHandlerImpl & ehImpl)
//	{
//		_world = ehImpl._world;
//		_channel = ehImpl._channel;
//	}
//
//
//
//
//	EventWorld * _world;
//	EventChannel _channel;
//	bool _enableUpdate;
//};
//}
//#endif
