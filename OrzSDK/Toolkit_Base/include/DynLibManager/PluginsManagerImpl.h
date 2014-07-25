#ifndef __Orz_ToolkitBase_PluginsManagerImpl_h__
#define __Orz_ToolkitBase_PluginsManagerImpl_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/Singleton.h>



namespace Orz {
class DynLib;

class _OrzToolkitBasePrivate PluginsManagerImpl
{

typedef std::vector<DynLibPtr> DynLibList;
public:
	PluginsManagerImpl(void);
	~PluginsManagerImpl(void);

public:
	bool init(void);

	void shutdown(void);

public:
	void clear(void);

	void loadPluginFromFile(const std::string & fileName);
	void loadPlugin(const std::string & pluginName);
	void unloadPlugin(const std::string & pluginName);

private:
	
	DynLibList _pluginLibs;
	//boost::scoped_ptr<DynLibManager> _dynLibManager;
	bool _isInitialised;

	//typedef std::map<std::string, DynLibPtr> DynLibList;
	//DynLibList _libList;
};

}

#endif
