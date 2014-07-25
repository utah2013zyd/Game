
#ifndef __Orz_FrameworkBase_SystemTools_h__
#define __Orz_FrameworkBase_SystemTools_h__
#include <orz/Toolkit_Base/Exception.h>


namespace Orz{

template<class T>
class SystemFactory
{
public:
	static boost::shared_ptr<T> createInstance(void)
	{
		return boost::shared_ptr<T>(new T());
	}
};

template<class Base, class T>
class SystemUnit : public Base
{
private:
	virtual ~SystemUnit(){}
	inline SystemUnit(){}
};


#define SYSTEMUNIT_BEGIN(T)\
template<class Base> \
class SystemUnit<Base, T> : public Base\
{\
private:\
	boost::shared_ptr<T> _value;\
public:\
	inline SystemUnit()\
	{\
		_value = SystemFactory<T>::createInstance();\
	}/*SYSTEMU	NIT_BEGIN*/


#define SYSTEMUNIT_UPDATE\
	protected:\
	virtual bool update(TimeType interval)\
	{\
		return Base::update(interval) && _value->update(interval);\
	}/*SYSTEMUNIT_UPDATE*/



#define SYSTEMUNIT_INIT\
	public:\
	virtual bool init()\
	{\
		return Base::init() && _value->init();\
	}/*SYSTEMUNIT_INIT*/

#define SYSTEMUNIT_SHUTDOWN\
	public:\
	virtual void shutdown()\
	{\
		_value->shutdown();\
		Base::shutdown();\
	}/*SYSTEMUNIT_SHUTDOWN*/

#define SYSTEMUNIT_END(T)\
	public:\
	virtual ~SystemUnit(){}\
};/*SYSTEMUNIT_END*/




template<class TList>
class SystemList:public boost::mpl::inherit_linearly< TList, Orz::SystemUnit<boost::mpl::placeholders::_, boost::mpl::placeholders::_>, Orz::SystemInterface>::type {};

}


namespace Orz
{


class SimpleTimerManager;
SYSTEMUNIT_BEGIN(SimpleTimerManager) 
SYSTEMUNIT_UPDATE 
SYSTEMUNIT_END(SimpleTimerManager) 




class AsioTimerManager;
SYSTEMUNIT_BEGIN(AsioTimerManager) 
SYSTEMUNIT_UPDATE 
SYSTEMUNIT_END(AsioTimerManager) 


class SimpleLogManager;
SYSTEMUNIT_BEGIN(SimpleLogManager) 
SYSTEMUNIT_END(SimpleLogManager) 

class TestLogManager;
SYSTEMUNIT_BEGIN(TestLogManager) 
SYSTEMUNIT_END(TestLogManager) 


class PluginsManager;
SYSTEMUNIT_BEGIN(PluginsManager) 
SYSTEMUNIT_INIT 
SYSTEMUNIT_SHUTDOWN 
SYSTEMUNIT_END(PluginsManager)

}
#endif