#ifndef __Orz_EchoServer_EchoClient_h__
#define __Orz_EchoServer_EchoClient_h__
#include <IClient.h>
namespace Orz
{
class EchoClient
	: public IClient
{
public:
	EchoClient();
	EchoClient(ServiceManager* _svcMgr, const SessionPxy& _snPxy);
	virtual ~EchoClient();

	void						setup							(bool _bHasInited);
	void						clean							();

public:
	void						logoff							();
	int32						processPacket					(IPacketShrPtr _pk);
	IClient::ErrPkt				checkPacket						(IPacketShrPtr _pk);

private:
	bool						m_bHasInited;
};
}
#endif
