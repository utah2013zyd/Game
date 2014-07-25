#ifndef __Orz_ToolkitBase_PluginsManager_h__
#define __Orz_ToolkitBase_PluginsManager_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/Singleton.h>



namespace Orz {

class PluginsManagerImpl;
template class _OrzToolkitBaseExport boost::scoped_ptr<PluginsManagerImpl>;
class _OrzToolkitBaseExport PluginsManager: public Singleton<PluginsManager>
{


public:
	PluginsManager(void);
	~PluginsManager(void);

public:
	///初始化插件管理器
	bool init(void);

	///关闭插件管理器
	void shutdown(void);

	///写在所有已经载入插件
	void clear();

	///载入插件
	void loadPlugin(const std::string & pluginName);

	///从文件中载入一些插件
	void loadPluginFromFile(const std::string & fileName);

	///卸载插件
	void unloadPlugin(const std::string & pluginName);

private:
	boost::scoped_ptr<PluginsManagerImpl> _impl;
};

}



//\
//SYSTEMUNIT_FACTORY(Interface, PluginsManager)


#endif
