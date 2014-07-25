#include <Plugin/MTPluginMgrImpl.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Toolkit/DynShrLock.hpp>
#include <orz/Toolkit_Plus/DynLib/MTDynLibMgr.h>
#include <orz/Toolkit_Plus/DynLib/MTDynLib.h>
#include <orz/Toolkit_Plus/Plugin/IOrzPlugin.h>
using namespace Orz;

MTPluginMgrImpl::MTPluginMgrImpl(uint32 _plgsHash, MTDynLibMgr* _dynLibMgr)
	: m_plugins(_plgsHash)
	, m_dynLibMgr(_dynLibMgr)
	, m_plgIdr(1000, 1)
{
	m_plugins.setHash();
}

MTPluginMgrImpl::~MTPluginMgrImpl()
{
	uint32 size = m_plugins.arraySize();
	for (uint32 i=0; i<size; ++i)
	{
		ThreadShrMap<OrzPluginId, PluginWrap>& tMap = m_plugins.threadMap(i);
		DynShrLock lock(tMap.mutex());
		std::map<OrzPluginId, PluginWrap>& m = tMap.map();
		std::map<OrzPluginId, PluginWrap>::iterator itr = m.begin();
		for (; itr!=m.end(); ++itr)
		{
			if (itr->second.plg)
			{
				unloadPlugin(itr->second);
			}
		}
	}
}

PluginWrap MTPluginMgrImpl::loadPlugin(const char* _pluginName, const Params<std::string>& _params)
{
	if (!_pluginName)
	{
		return PluginWrap();
	}

	if (!m_dynLibMgr)
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", " !m_dynLibMgr!", 0);
		return PluginWrap();
	}

	MTDynLibShrPtr dynLib = m_dynLibMgr->load(_pluginName);
	if (!dynLib)
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", " !dynLib!", 0);
		return PluginWrap();
	}

	DLL_START_ORZ_PLUGIN dllStartOrzPlgFunc = (DLL_START_ORZ_PLUGIN)dynLib->getSymbol("dllStartOrzPlgFunc");
	if (!dllStartOrzPlgFunc)
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", " !dllStartOrzPlgFunc!", 0);
		m_dynLibMgr->unload(dynLib);
		return PluginWrap();
	}

	IOrzPlugin* plg = dllStartOrzPlgFunc(_params);
	if (!plg)
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", " !plg!", 0);
		m_dynLibMgr->unload(dynLib);
		return PluginWrap();
	}

	sLogger->out(OL_INFO, " - Plugin[Name: ", _pluginName, "] load success!", 0);
	return PluginWrap(dynLib, plg);
}

bool MTPluginMgrImpl::unloadPlugin(PluginWrap _plgWrap)
{
	if (!_plgWrap.dynLib || !_plgWrap.plg)
	{
		return false;
	}

	if (!m_dynLibMgr)
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", " !m_dynLibMgr!", 0);
		return false;
	}

	DLL_STOP_ORZ_PLUGIN dllStopOrzPlgFunc = (DLL_STOP_ORZ_PLUGIN)_plgWrap.dynLib->getSymbol("dllStopOrzPlgFunc");
	if (!dllStopOrzPlgFunc)
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", " !dllStopOrzPlgFunc!", 0);
		return false;
	}

	dllStopOrzPlgFunc(_plgWrap.plg);
	m_dynLibMgr->unload(_plgWrap.dynLib);
	return true;
}

OrzPluginId MTPluginMgrImpl::addPlugin(PluginWrap _plgWrap)
{
	OrzPluginId plgId = (OrzPluginId)m_plgIdr.generate();
	if (plgId == (OrzPluginId)m_plgIdr.errObj())
	{
		return 0;
	}
	m_plugins.add(plgId, _plgWrap, plgId);
	return plgId;
}

PluginWrap MTPluginMgrImpl::removePlugin(OrzPluginId _id)
{
	DynShrLock lock(m_plugins.mutex(_id, _id));
	PluginWrap plgWrap = m_plugins.get(_id, _id, false);
	m_plugins.erase(_id, _id, false);
	m_plgIdr.release(_id);
	return plgWrap;
}

bool MTPluginMgrImpl::boost()
{
	uint32 size = m_plugins.arraySize();
	for (uint32 i=0; i<size; ++i)
	{
		ThreadShrMap<OrzPluginId, PluginWrap>& tMap = m_plugins.threadMap(i);
		DynShrLock lock(tMap.mutex());
		std::map<OrzPluginId, PluginWrap>& m = tMap.map();
		std::map<OrzPluginId, PluginWrap>::iterator itr = m.begin();
		for (; itr!=m.end(); ++itr)
		{
			if (itr->second.plg)
			{
				if (!itr->second.plg->boost())
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool MTPluginMgrImpl::boostPlugin(OrzPluginId _plgId)
{
	DynShrLock lock(m_plugins.mutex(_plgId, _plgId));
	PluginWrap plgWrap = m_plugins.get(_plgId, _plgId, false);
	if (plgWrap.plg)
	{
		return plgWrap.plg->boost();
	}
	return false;
}

void MTPluginMgrImpl::shutdownPlugin(OrzPluginId _plgId)
{
	DynShrLock lock(m_plugins.mutex(_plgId, _plgId));
	PluginWrap plgWrap = m_plugins.get(_plgId, _plgId, false);
	if (plgWrap.plg)
	{
		plgWrap.plg->shutdown();
	}
}

void MTPluginMgrImpl::joinPlugin(OrzPluginId _plgId)
{
	DynShrLock lock(m_plugins.mutex(_plgId, _plgId));
	PluginWrap plgWrap = m_plugins.get(_plgId, _plgId, false);
	if (plgWrap.plg)
	{
		plgWrap.plg->join();
	}
}

void MTPluginMgrImpl::shutdown()
{
	uint32 size = m_plugins.arraySize();
	for (uint32 i=0; i<size; ++i)
	{
		ThreadShrMap<OrzPluginId, PluginWrap>& tMap = m_plugins.threadMap(i);
		DynShrLock lock(tMap.mutex());
		std::map<OrzPluginId, PluginWrap>& m = tMap.map();
		std::map<OrzPluginId, PluginWrap>::iterator itr = m.begin();
		for (; itr!=m.end(); ++itr)
		{
			if (itr->second.plg)
			{
				itr->second.plg->shutdown();
			}
		}
	}
}

void MTPluginMgrImpl::join()
{
	uint32 size = m_plugins.arraySize();
	for (uint32 i=0; i<size; ++i)
	{
		ThreadShrMap<OrzPluginId, PluginWrap>& tMap = m_plugins.threadMap(i);
		DynShrLock lock(tMap.mutex());
		std::map<OrzPluginId, PluginWrap>& m = tMap.map();
		std::map<OrzPluginId, PluginWrap>::iterator itr = m.begin();
		for (; itr!=m.end(); ++itr)
		{
			if (itr->second.plg)
			{
				itr->second.plg->join();
			}
		}
	}
}
