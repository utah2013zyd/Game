#ifndef __Orz_ToolkitPlus_PluginsManagerImpl_h__
#define __Orz_ToolkitPlus_PluginsManagerImpl_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
#include <orz/Toolkit_Plus/Toolkit/HashMap.hpp>
#include <orz/Toolkit_Plus/Toolkit/FastIdr.hpp>
#include <orz/Toolkit_Plus/Plugin/PluginWrap.hpp>
#include <orz/Toolkit_Plus/Toolkit/Params.hpp>
namespace Orz 
{
class _OrzToolkitPlusPrivate MTPluginMgrImpl
{
public:
	explicit MTPluginMgrImpl(uint32 _plgsHash, MTDynLibMgr* _dynLibMgr);
	~MTPluginMgrImpl();

public:
	PluginWrap		loadPlugin			(const char* _pluginName, const Params<std::string>& _params);
	bool			unloadPlugin		(PluginWrap _plgWrap);
	OrzPluginId		addPlugin			(PluginWrap _plgWrap);
	PluginWrap		removePlugin		(OrzPluginId _id);
	bool			boost				();
	bool			boostPlugin			(OrzPluginId _plgId);
	void			shutdownPlugin		(OrzPluginId _plgId);
	void			joinPlugin			(OrzPluginId _plgId);
	void			shutdown			();
	void			join				();

private:
	HashShrMap<OrzPluginId, PluginWrap>		m_plugins;
	MTDynLibMgr*							m_dynLibMgr;
	FastIdr									m_plgIdr;
};
}
#endif
