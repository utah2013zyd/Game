#include <orz/OrzAsio/NetEngine/IOService.h>
#include <NetEngine/IOServiceImpl.h>
using namespace Orz;

IOService::IOService(IServiceManager* _svcMgr, uint32 _threadNum)
	: m_impl(new IOServiceImpl(_svcMgr, _threadNum))
{
}

IOService::~IOService()
{
	delete m_impl;
}

void IOService::boost()
{
	m_impl->boost();
}

void IOService::shutdown()
{
	m_impl->shutdown();
}

void IOService::join()
{
	m_impl->join();
}

IOEngine& IOService::ioEngine()
{
	return m_impl->ioEngine();
}

IServiceManager* IOService::svcMgr()
{
	return m_impl->svcMgr();
}
