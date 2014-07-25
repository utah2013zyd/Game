#include <orz/OrzAsio/AsioManager.h>
#include <AsioManagerImpl.h>
using namespace Orz;

AsioManager::AsioManager(uint32 _dynLibMgrHash, uint32 _pluginsMgrHash)
	: m_impl(new AsioManagerImpl(_dynLibMgrHash, _pluginsMgrHash))
{
}
AsioManager::~AsioManager(void)
{
	if (m_impl)
	{
		delete m_impl;
	}
}

bool AsioManager::initialize(IServiceManager* _svcMgr)
{
	return m_impl->initialize(_svcMgr);
}

bool AsioManager::boost(bool _isBlock)
{
	return m_impl->boost(_isBlock);
}

IServiceManager* AsioManager::getSvcMgr()
{
	return m_impl->getSvcMgr();
}

MTDynLibMgr* AsioManager::getDynLibMgr()
{
	return m_impl->getDynLibMgr();
}

MTPluginMgr* AsioManager::getPluginsMgr()
{
	return m_impl->getPluginsMgr();
}

void AsioManager::join()
{
	m_impl->join();
}

void AsioManager::shutdown(void)
{
	m_impl->shutdown();
}
