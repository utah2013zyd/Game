#ifndef __Orz_ToolkitPlus_IOrzPlugin_h__
#define __Orz_ToolkitPlus_IOrzPlugin_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
#include <orz/Toolkit_Plus/Toolkit/Params.hpp>
namespace Orz
{
//! Orz����ӿ���
/*!
 *	\note Orz��������Զ�̬����ʽ�����غ����еģ�����Ҫ���Ա���ʹ���Լ����̣߳�
 *	\note ͨ��boost��������shutdown���رգ�join���ڵ���boostͬһ���߳��ںϲ��߳�
 */
class IOrzPlugin
{
public:
	IOrzPlugin() {}
	virtual ~IOrzPlugin() {}

public:
	virtual bool		boost				() = 0;
	virtual void		shutdown			() = 0;
	virtual void		join				() = 0;
};
}
typedef Orz::IOrzPlugin* (*DLL_START_ORZ_PLUGIN)(const Orz::Params<std::string>& _params);
typedef void (*DLL_STOP_ORZ_PLUGIN)(Orz::IOrzPlugin* _orzPlg);
#endif
