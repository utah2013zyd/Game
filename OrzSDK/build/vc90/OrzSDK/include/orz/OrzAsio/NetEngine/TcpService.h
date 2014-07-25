#ifndef __Orz_OrzAsio_TcpService_h__
#define __Orz_OrzAsio_TcpService_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/MemString.hpp>
#include <orz/OrzAsio/NetEngine/ProtocolService.h>
namespace Orz
{
class _OrzOrzAsioExport TcpService
	: public ProtocolService
{
public:
	TcpService(IServiceManager* _svcMgr, bool _isIp4=true);
	virtual ~TcpService();

public:
	void				send					(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick);
	void				halt					(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr);
	void				connect					(IpString _ip, NetPort _port, IConnection* _connection, void* _tag, uint32 _delayTime=5);
	void				accepted				(TcpSocketShrPtr _tcpSkt, NetPort _port, IConnection* _connection, 
												 const boost::system::error_code& _error);

private:
	void				processSend				(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick);
	void				processHalt				(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr);
	void				processConnect			(IpString _ip, NetPort _port, IConnection* _connection, void* _tag, uint32 _delayTime);
	void				processAccepted			(TcpSocketShrPtr _tcpSkt, NetPort _port, IConnection* _connection, const boost::system::error_code& _error);
	void				handleConnect			(TcpSocketShrPtr _tcpSkt, IpString _ip, NetPort _port, IConnection* _connection, void* _tag, 
												 const boost::system::error_code& _error, boost::asio::ip::tcp::resolver::iterator _itr);
	void				delayConnect			(DlTimerShrPtr _tmr, IpString _ip, NetPort _port, IConnection* _connection, void* _tag, uint32 _delayTime);
	void				checkConnect			(DlTimerShrPtr _tmr, IpString _ip, NetPort _port, IConnection* _connection, void* _tag, uint32 _delayTime);
};
}
#endif
