#include <orz/OrzAsio/NetEngine/NetInfoManager.h>
#include <NetEngine/NetInfoManagerImpl.h>
using namespace Orz;

NetInfoManager::NetInfoManager(uint32 _cBase)
	: m_impl(new NetInfoManagerImpl(_cBase))
{
}

NetInfoManager::~NetInfoManager()
{
}

uint32 NetInfoManager::getSocketNum() const
{
	return m_impl->getSocketNum();
}

uint32 NetInfoManager::getInConnectionNum() const
{
	return m_impl->getInConnectionNum();
}

uint32 NetInfoManager::getOutConnectionNum() const
{
	return m_impl->getOutConnectionNum();
}

uint32 NetInfoManager::getInConnectionNum(NetPort _port) const
{
	return m_impl->getInConnectionNum(_port);
}

uint32 NetInfoManager::getOutConnectionNum(NetPort _port) const
{
	return m_impl->getOutConnectionNum(_port);
}

boost::shared_ptr<NetInfoManagerImpl> NetInfoManager::impl()
{
	return m_impl;
}
