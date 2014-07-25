#ifndef __Orz_ToolkitPlus_PluginWrap_hpp__
#define __Orz_ToolkitPlus_PluginWrap_hpp__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
//! 插件包装类
struct PluginWrap
{
	PluginWrap() : plg(0) {}
	PluginWrap(IOrzPlugin* _plg)
		: plg(_plg)
	{
	}
	PluginWrap(MTDynLibShrPtr _dynLib, IOrzPlugin* _plg)
		: dynLib(_dynLib)
		, plg(_plg)
	{
	}

	MTDynLibShrPtr	dynLib;
	IOrzPlugin*		plg;
};
}
#endif
