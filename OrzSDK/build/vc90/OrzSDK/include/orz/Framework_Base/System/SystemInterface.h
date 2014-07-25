
#ifndef __Orz_FrameworkBase_SystemInterface_h__
#define __Orz_FrameworkBase_SystemInterface_h__
#include <orz/Framework_Base/FrameworkBaseConfig.h>
#include <orz/Toolkit_Base/Singleton.h>

namespace Orz{

class SystemInterfaceImpl;
template class _OrzFrameworkBaseExport boost::scoped_ptr<SystemInterfaceImpl>;

class _OrzFrameworkBaseExport SystemInterface: public Singleton<SystemInterface>
{

public:
	SystemInterface(void);
	virtual ~SystemInterface(void);
	
	///启动系统循环
	void run(void);
	
	///推出系统运行
	void exit(void);

	
	void debugParame(void) const;
	
	///检查是否有某个（提供给子系统使用的）参数
	bool hasParame(const std::string & key) const;

	///得到某个（提供给子系统使用的）参数
	template<class T>
	T getParame(const std::string & key) const
	{
		return VariantData<T>::get(_getParame(key));
	}

	///设置某个（提供给子系统使用的）参数
	template<class T>
	void setParame(const std::string & key, const T & value)
	{
		_setParame(key, VariantData<T>::set(value));
	}
	
public:// to have something custom-made 

	///检查是否系统在运行
	bool running(void) const;
	///得到两次调用的时间间隔
	TimeType getInterval(void);
	///重置时间
	void reset(void);

public:

	///依次初始化每个子系统
	virtual bool init(void);
	///依次关闭每个子系统
	virtual void shutdown(void);
	///依次更新所有子系统，参数是两次调用的时间间隔
	virtual bool update(TimeType interval);

	bool isInit(void);

private:
	
	void _setParame(const std::string & key, const Variant & value);
	const Variant & _getParame(const std::string & key) const;
	boost::scoped_ptr<SystemInterfaceImpl> _impl;
	bool _init;
};
}
#include <orz/Framework_Base/System/SystemTools.h>
#endif
