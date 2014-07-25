#ifndef __Orz_OrzAsio_SSLAcceptorService_h__
#define __Orz_OrzAsio_SSLAcceptorService_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/IOService.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
namespace Orz
{
//! SSLAcceptorService
/*!
 *	\note 不建议用户使用，这个类供系统内部使用
 */
class _OrzOrzAsioExport SSLAcceptorService
	: public IOService
{
public:
	SSLAcceptorService(IServiceManager* _svcMgr, uint32 _threadNum, bool _isIp4=true);
	virtual ~SSLAcceptorService();

public:
	TcpAcceptorShrPtr	createAcceptor			(NetPort _port);
	int32				accept					(TcpAcceptorShrPtr _acptr, IConnection* _connection, ContextId _contextId, uint32 _syn=1, uint32 _delayTime=5);

private:
	void				handleAccept			(TcpAcceptorShrPtr _acptr, SSLSocketShrPtr _sslSkt, IConnection* _connection, 
												 ContextId _contextId, const boost::system::error_code& _error, uint32 _delayTime);
	void				delayAccept				(DlTimerShrPtr _tmr, TcpAcceptorShrPtr _acptr, IConnection* _connection, ContextId _contextId, uint32 _delayTime);
	void				checkAccept				(DlTimerShrPtr _tmr, TcpAcceptorShrPtr _acptr, IConnection* _connection, ContextId _contextId, uint32 _delayTime);

private:
	bool										m_isIp4;
};
}
#endif
