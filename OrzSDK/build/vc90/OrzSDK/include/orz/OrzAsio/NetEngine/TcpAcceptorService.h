#ifndef __Orz_OrzAsio_TcpAcceptorService_h__
#define __Orz_OrzAsio_TcpAcceptorService_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/IOService.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
namespace Orz
{
class _OrzOrzAsioExport TcpAcceptorService
	: public IOService
{
public:
	TcpAcceptorService(IServiceManager* _svcMgr, uint32 _threadNum, bool _isIp4=true);
	virtual ~TcpAcceptorService();

public:
	TcpAcceptorShrPtr	createAcceptor			(NetPort _port);
	int32				accept					(TcpAcceptorShrPtr _acptr, IConnection* _connection, uint32 _syn=1, uint32 _delayTime=5);

private:
	void				handleAccept			(TcpAcceptorShrPtr _acptr, TcpSocketShrPtr _tcpSkt, IConnection* _connection, 
												 const boost::system::error_code& _error, uint32 _delayTime);
	void				delayAccept				(DlTimerShrPtr _tmr, TcpAcceptorShrPtr _acptr, IConnection* _connection, uint32 _delayTime);
	void				checkAccept				(DlTimerShrPtr _tmr, TcpAcceptorShrPtr _acptr, IConnection* _connection, uint32 _delayTime);

private:
	bool										m_isIp4;
};
}
#endif
