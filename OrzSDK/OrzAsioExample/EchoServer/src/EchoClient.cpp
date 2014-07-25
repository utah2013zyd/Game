#include <EchoClient.h>
#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/OrzAsio/NetEngine/PacketFreeList.h>
#include <orz/OrzAsio/NetEngine/SessionDeputy.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
using namespace Orz;

EchoClient::EchoClient()
{
}

EchoClient::EchoClient(ServiceManager* _svcMgr, const SessionPxy& _snPxy)
	: IClient(_svcMgr, _snPxy)
	, m_bHasInited(false)
{
}

EchoClient::~EchoClient()
{
}

void EchoClient::setup(bool _bHasInited)
{
	m_bHasInited = true;
}

void EchoClient::clean()
{
	m_bHasInited = false;
}

void EchoClient::logoff()
{
}

int32 EchoClient::processPacket(IPacketShrPtr _pk)
{
	if (checkPacket(_pk) != EP_NORMAL)
		return 1;

	//int32 err = 0;
	//DateSys dt;
	//_pk->getDT(dt);
	//StringPtr str = _pk->readStr(err);
	//if (!err)
	//	printf("[%s]: %s", dt.formatString().c_str(), str.ptr());

	//if (sn().getSocketType() == SKTP_TCP)
	//	printf(" - [TCP]\n");
	//else if  (sn().getSocketType() == SKTP_UDP)
	//	printf(" - [UDP]\n");
	//else
	//	printf("\n");

	skp()->send(sn(), _pk);

	return 0;
}

IClient::ErrPkt EchoClient::checkPacket(IPacketShrPtr _pk)
{
	return EP_NORMAL;
}
