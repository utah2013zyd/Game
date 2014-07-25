

#ifndef __Orz_ToolkitBase_EventSpace_h__
#define __Orz_ToolkitBase_EventSpace_h__

#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/EventSystem/Event.h>

namespace Orz
{



//class EventSpaceImpl;
class EventFactoryImpl;
//template class _OrzToolkitBaseExport boost::scoped_ptr<EventSpaceImpl>;

class _OrzToolkitBaseExport EventSpace:private boost::noncopyable
{
private:
	class Space
	{
	public:
		inline Space(void):_begin(-1), _end(-1){}
		inline ~Space(void){}
		inline Event::message_type cover(Event * evt)  const
		{
			assert(evt);
			return (evt->getMsg() - _begin);
		}

		inline Event::message_type uncover(Event::message_type msg) const
		{

			return (msg + _begin);
		}
		inline bool in(Event * evt) const
		{
			Event::message_type msg = evt->getMsg();
			return (msg >= _begin) &&  (msg < _end);
		}
		inline bool equal(Event * evt, Event::message_type msg)  const
		{
			assert(evt);
			return cover(evt) == msg;
		}
		inline void reset(size_t begin, size_t end)
		{
			_begin = begin;
			_end = end;
		}
	private:
		size_t _begin;
		size_t _end;
	};
	friend class EventFactoryImpl;
public:

	///通过本空间内的消息值创建消息
	Event * createEvent(Event::message_type msg);



	///判断消息是否在本命名空间内和某个消息值相同
	bool equal(Event * evt, Event::message_type msg) const;

	///得到消息在本消息空间内消息值
	Event::message_type getMsg(Event * evt) const;

	///得到空间名称
	const std::string & getName(void) const;

	///得到本空间内管理消息数量
	size_t amount() const;

	///查看某消息是否再此空间中
	bool in(Event * evt) const;
	
protected:
	EventSpace(const std::string & name, size_t amount);
	virtual ~EventSpace(void);
private:
	void setSpace(size_t shift);
	private:
	Space _space;
	std::string _name;
	size_t _amount;
};


}

#endif
