#ifndef __Orz_FrameworkBase_LogicConfig_LogicFacade_h__
#define __Orz_FrameworkBase_LogicConfig_LogicFacade_h__


#include <orz/Framework_Base/FrameworkBaseConfig.h>

namespace Orz
{
namespace LogicConfiger
{
class Builder;
class LogicFacadeImpl;
template class _OrzFrameworkBaseExport boost::scoped_ptr<LogicFacadeImpl>;


class _OrzFrameworkBaseExport LogicFacade
{

public:
	LogicFacade(void);
	~LogicFacade(void);
	
	///卸载插件，关闭逻辑相关的类型
	bool unload(void);
	
	///载入插件，初始化逻辑相关的类型
	bool load(void);

	//得到所有构造逻辑需要的信息。
	bool building(const Builder & builder);

private:
	boost::scoped_ptr<LogicFacadeImpl> _impl;
};

typedef boost::shared_ptr<LogicFacade> LogicFacadePtr;

}

}


#endif 