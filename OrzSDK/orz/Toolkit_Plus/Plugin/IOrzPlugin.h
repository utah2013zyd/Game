#ifndef __Orz_ToolkitPlus_IOrzPlugin_h__
#define __Orz_ToolkitPlus_IOrzPlugin_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
#include <orz/Toolkit_Plus/Toolkit/Params.hpp>
namespace Orz
{
//! Orz插件接口类
/*!
 *	\note Orz插件都是以动态库形式来加载和运行的，而且要求自必须使用自己的线程；
 *	\note 通过boost来启动，shutdown来关闭，join来在调用boost同一个线程内合并线程
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
