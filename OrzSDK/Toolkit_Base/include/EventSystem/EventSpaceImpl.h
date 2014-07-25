//#ifndef __Orz_ToolkitBase_EventSpaceImpl_h__
//#define __Orz_ToolkitBase_EventSpaceImpl_h__
//#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
//#include <orz/Toolkit_Base/EventSystem/EventSpace.h>
//#include <orz/Toolkit_Base/EventSystem/EventFactory.h>
//#include <orz/Toolkit_Base/EventSystem/Event.h>
//namespace Orz
//{
//class _OrzToolkitBasePrivate EventSpaceImpl:private boost::noncopyable
//{
//public:
//	class Space
//	{
//	public:
//		inline Space(size_t begin, size_t end):_begin(begin), _end(end){}
//		inline ~Space(void){}
//		inline Event::message_type cover(Event * evt)  const
//		{
//			assert(evt);
//			return (evt->getMsg() - _begin);
//		}
//
//		inline Event::message_type uncover(Event::message_type msg) const
//		{
//
//			return (msg + _begin);
//		}
//		inline bool in(Event * evt) const
//		{
//			Event::message_type msg = evt->getMsg();
//
//			return (msg >= _begin) &&  (msg < _end);
//		}
//		inline bool equal(Event * evt, Event::message_type msg)  const
//		{
//			assert(evt);
//			return cover(evt) == msg;
//		}
//
//	private:
//		size_t _begin;
//		size_t _end;
//	};
//public:
//
//
//	
//	const std::string & getName(void)
//	{
//		return _name;
//	}
//	
//	Event * createEvent(Event::message_type msg)
//	{
//		Event * evt = EventFactory::getInstance().createEvent();
//		evt->setMsg(_space->uncover(msg));
//		return evt;
//	}
//
//	inline bool in(Event * evt) const
//	{
//		return _space->in(evt);
//	}
//
//	inline bool equal(Event * evt, Event::message_type msg)
//	{
//		return _space->equal(evt, msg);
//	}
//
//	inline Event::message_type getMsg(Event * evt) const
//	{
//		assert(evt);
//		return _space->cover(evt);
//	}
//
//	void setSpace(size_t shift)
//	{
//		_space.reset(new Space(shift, shift + amount()));
//	}
//
//	inline size_t amount()  const
//	{
//		return _amount;
//	}
//
//	EventSpaceImpl(const std::string & name, size_t amount):_space(),_name(name),_amount(amount)
//	{}
//	~EventSpaceImpl(void){}
//private:
//	boost::scoped_ptr<Space> _space;
//	std::string _name;
//	size_t _amount;
//};
//}
//#endif
