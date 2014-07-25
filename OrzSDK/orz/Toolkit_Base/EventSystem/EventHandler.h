
#ifndef __Orz_ToolkitBase_EventHandler_h__
#define __Orz_ToolkitBase_EventHandler_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/EventSystem/Event.h>
#include "EventChannel.h"


namespace Orz
{

	//class EventHandlerImpl;
	//template class _OrzToolkitBaseExport boost::scoped_ptr<EventHandlerImpl>;
	class _OrzToolkitBaseExport EventHandler: public boost::enable_shared_from_this<EventHandler>
	{

		
		friend class EventWorldImpl;
		friend class EventWorld;
	public:
		EventHandler(void);
		virtual ~EventHandler(void);

	/*	EventHandler(const EventHandler& eh);
		virtual void swap(EventHandler& eh) throw();
		virtual EventHandler& operator = (const EventHandler& eh);*/

		/**
		检查是否监听此频道
		*/
		
		bool listening(const EventChannel & channel);//{return channel.listeneing(_channel);}

		///设置处理频道
		void setChannel(const EventChannel & channel);

		///得到当前处理频道
		const EventChannel getChannel(void) const;

		///增加新的处理频道
		void addChannel(const EventChannel & channel);
		///移除处理频道
		void removeChannel(const EventChannel & channel);

		///清空处理频道
		void clearChannel(void);

		/**
			检查是否在本world中
			
		*/
		bool isInTheWorld(EventWorld * world) const;	
		/**
			否则检查是否在world中
		*/
		bool isInWorld(void) const;		

		///打开更新
		void enableUpdate(bool receiveEvent = false);
		///关闭更新
		void disableUpdate(void);


		///得到世界的指针
		EventWorld * getWorld(void);//{return _world;}
		
		/**
		处理消息接口
		 Non-Virtual Interface
		*/
		void execute(Event * evt);
		inline bool isUpdateEvent(Event * evt)
		{
			return evt->system();
		}
	private:
		
		
		/**
		当进入世界时候调用，通常在这个时候构建图像信息
		 Non-Virtual Interface
		*/
		void enable(EventWorld * world);
		
		/**
		当离开世界的时候调用，通常在这个时候关闭图像信息
		 Non-Virtual Interface
		*/
		void disable(void);
		
	



		///重载，被用于处理消息调用
		virtual void doExecute(Event * evt);
		
		
		///重载，被用于进入消息管理调用
		virtual void doEnable(void);
		
		
		///重载，被用于离开消息管理调用
		virtual void doDisable(void);

		///重载，被用于更新调用
		virtual void doFrame(void);




	private:
	//	boost::scoped_ptr<EventHandlerImpl> _impl;

		bool _enable;

			
		EventWorld * _world;
		EventChannel _channel;
		bool _enableUpdate;
		bool _updateReceiveEvent;
	};
}

#endif

