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
	
	///ж�ز�����ر��߼���ص�����
	bool unload(void);
	
	///����������ʼ���߼���ص�����
	bool load(void);

	//�õ����й����߼���Ҫ����Ϣ��
	bool building(const Builder & builder);

private:
	boost::scoped_ptr<LogicFacadeImpl> _impl;
};

typedef boost::shared_ptr<LogicFacade> LogicFacadePtr;

}

}


#endif 