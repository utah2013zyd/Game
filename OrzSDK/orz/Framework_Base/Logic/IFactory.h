#ifndef __Orz_FrameworkBase_IFactory_h__
#define __Orz_FrameworkBase_IFactory_h__
#include <orz/Framework_Base/FrameworkBaseConfig.h>



namespace Orz{


template <class T>
class _OrzFrameworkBaseExport IFactory: private boost::noncopyable
{
public:
	typedef boost::shared_ptr<T>  pointer_type;
	///定义参数类型
	typedef NameValueList * parameter_type;
	
	typedef NameValueList::iterator parameter_iterator;
public:
	IFactory(void){}
	virtual ~IFactory(void){}

	///返回工厂生产实体的类型
	virtual const std::string & getTypeName() const = 0;

	/** 创建一个实体
		@param instanceName 实例名
		@param parameter_type 创建参数
		@return 返回一个创建实体的智能指针
	*/
	virtual pointer_type createInstance(const std::string& instanceName, parameter_type parameter) = 0;
};


}
#endif