#ifndef __Orz_OrzAsio_SSLService_h__
#define __Orz_OrzAsio_SSLService_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/MemString.hpp>
#include <orz/OrzAsio/NetEngine/ProtocolService.h>
namespace Orz
{
//! SSLService
/*!
 *	\note 不建议用户使用，这个类供系统内部使用
 */
class _OrzOrzAsioExport SSLService
	: public ProtocolService
{
public:
	SSLService(IServiceManager* _svcMgr, bool _isIp4=true);
	virtual ~SSLService();

public:
	void				send					(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick);
	void				halt					(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr);
	void				connect					(IpString _ip, NetPort _port, IConnection* _connection, ContextId _contextId, void* _tag, uint32 _delayTime=5);
	void				accepted				(SSLSocketShrPtr _sslSkt, NetPort _port, IConnection* _connection, 
												 const boost::system::error_code& _error);

private:
	void				processSend				(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick);
	void				processHalt				(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr);
	void				processConnect			(IpString _ip, NetPort _port, IConnection* _connection, ContextId _contextId, void* _tag, uint32 _delayTime);
	void				processAccepted			(SSLSocketShrPtr _sslSkt, NetPort _port, IConnection* _connection, const boost::system::error_code& _error);
	void				handleConnect			(SSLSocketShrPtr _sslSkt, IpString _ip, NetPort _port, IConnection* _connection, void* _tag, 
												 const boost::system::error_code& _error, boost::asio::ip::tcp::resolver::iterator _itr);
	void				delayConnect			(DlTimerShrPtr _tmr, IpString _ip, NetPort _port, IConnection* _connection, ContextId _contextId, void* _tag, uint32 _delayTime);
	void				checkConnect			(DlTimerShrPtr _tmr, IpString _ip, NetPort _port, IConnection* _connection, ContextId _contextId, void* _tag, uint32 _delayTime);
};
}
#endif
