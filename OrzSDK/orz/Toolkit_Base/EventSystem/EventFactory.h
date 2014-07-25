
#ifndef __Orz_ToolkitBase_EventFactory_h__
#define __Orz_ToolkitBase_EventFactory_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>





namespace Orz
{

class EventFactoryImpl;
template class _OrzToolkitBaseExport boost::scoped_ptr<EventFactoryImpl>;



class _OrzToolkitBaseExport EventFactory : private boost::noncopyable
{

public:

	///销毁消息
	void destroyEvent(Event * evt);

	///创建消息
	Event * createEvent(void);

	///注册消息空间
	void regEventSpace(EventSpace * es);

	///通过名称得到消息空间
	EventSpace * getEventSpace(const std::string & name) const;

	///搜索消息所在空间，如果不存在返回空指针
	EventSpace * findEventSpace(Event * evt) const;

	///注销消息空间
	void unregEventSpace(const std::string & name);

	///得到单件引用
	static EventFactory & getInstance();

	///得到单件指针
	static EventFactory * getInstancePtr();
	
private:
	EventFactory(void);
	~EventFactory();


	boost::scoped_ptr<EventFactoryImpl> _impl;




};


}

#endif
