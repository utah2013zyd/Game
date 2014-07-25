#include <orz/Toolkit_Plus/Plugin/MTPluginMgr.h>
#include <Plugin/MTPluginMgrImpl.h>
using namespace Orz;

MTPluginMgr::MTPluginMgr(uint32 _plgsHash, MTDynLibMgr* _dynLibMgr)
	: m_impl(new MTPluginMgrImpl(_plgsHash, _dynLibMgr))
{
}

MTPluginMgr::~MTPluginMgr()
{
	if (m_impl)
	{
		delete m_impl;
	}
}

PluginWrap MTPluginMgr::loadPlugin(const char* _pluginName, const Params<std::string>& _params)
{
	return m_impl->loadPlugin(_pluginName, _params);
}

bool MTPluginMgr::unloadPlugin(PluginWrap _plgWrap)
{
	return m_impl->unloadPlugin(_plgWrap);
}

OrzPluginId MTPluginMgr::addPlugin(PluginWrap _plgWrap)
{
	return m_impl->addPlugin(_plgWrap);
}

PluginWrap MTPluginMgr::removePlugin(OrzPluginId _id)
{
	return m_impl->removePlugin(_id);
}

bool MTPluginMgr::boost()
{
	return m_impl->boost();
}

bool MTPluginMgr::boostPlugin(OrzPluginId _plgId)
{
	return m_impl->boostPlugin(_plgId);
}

void MTPluginMgr::shutdownPlugin(OrzPluginId _plgId)
{
	m_impl->shutdownPlugin(_plgId);
}

void MTPluginMgr::joinPlugin(OrzPluginId _plgId)
{
	m_impl->joinPlugin(_plgId);
}

void MTPluginMgr::shutdown()
{
	return m_impl->shutdown();
}

void MTPluginMgr::join()
{
	return m_impl->join();
}
