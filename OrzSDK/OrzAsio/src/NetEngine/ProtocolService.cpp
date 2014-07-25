#include <orz/OrzAsio/NetEngine/ProtocolService.h>
#include <NetEngine/ProtocolServiceImpl.h>
using namespace Orz;

ProtocolService::ProtocolService(IServiceManager* _svcMgr, uint32 _threadNum, bool _isIp4)
	: IOService(_svcMgr, _threadNum)
	, m_impl(new ProtocolServiceImpl(_isIp4))
{
}

ProtocolService::~ProtocolService()
{
	delete m_impl;
}

bool ProtocolService::isIp4()
{
	return m_impl->isIp4();
}

void ProtocolService::send(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick)
{
}

void ProtocolService::halt(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr)
{
}
