
#include "DynLibManager/PluginsManagerImpl.h"
#include <orz/Toolkit_Base/DynLibManager/PluginsManager.h>

using namespace Orz;


template<> PluginsManager* Singleton<PluginsManager>::_singleton = NULL;

PluginsManager::PluginsManager(void):_impl(new PluginsManagerImpl())
{

}
PluginsManager::~PluginsManager(void)
{

}


bool PluginsManager::init(void)
{
	return _impl->init();
}
void PluginsManager::shutdown(void)
{
	_impl->shutdown();
}


void PluginsManager::clear()
{
	_impl->clear();
}
void PluginsManager::loadPlugin(const std::string & pluginName)
{
	_impl->loadPlugin(pluginName);
}

void PluginsManager::loadPluginFromFile(const std::string & fileName)
{
	_impl->loadPluginFromFile(fileName);
}


void PluginsManager::unloadPlugin(const std::string & pluginName)
{
	_impl->unloadPlugin(pluginName);
}
