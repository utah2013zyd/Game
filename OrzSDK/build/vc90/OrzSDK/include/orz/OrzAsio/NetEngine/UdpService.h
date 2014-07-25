#ifndef __Orz_OrzAsio_UdpService_h__
#define __Orz_OrzAsio_UdpService_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/ProtocolService.h>
#include <orz/Toolkit_Plus/Toolkit/MemString.hpp>
#include <orz/OrzAsio/NetEngine/UdpSubSocket.h>
namespace Orz
{
class _OrzOrzAsioExport UdpService
	: public ProtocolService
{
public:
	UdpService(IServiceManager* _svcMgr, uint32 _threadNum=1, bool _isIp4=true);
	virtual ~UdpService();

public:
	void				send					(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick);
	void				halt					(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr);

	void				sendto					(UdpSocketShrPtr _udpSocket, NetPort _port, IpString _ip, 
												 const IPacketShrPtr _pk, IConnection* _connection, void* _tag, uint32 _lifecycle);

	void				accepted				(UdpAcceptorService* _udpAcptrSvc, UdpAcceptorShrPtr _acptr, UdpSubSocketShrPtr _udpSubSkt, 
												 IConnection* _connection, const boost::system::error_code& _error);
	void				sended					(UdpSubSocketShrPtr _skt, UdpSubSocket::Type _type, const SessionPxy& _snPxy, const IPacketShrPtr _pk, 
												 bool _isKick, const boost::system::error_code& _error, std::size_t _bytes_transferred);
	void				recved					(UdpSubSocketShrPtr _udpSubSkt, const SessionPxy& _snPxy, StringPtr _buffer, const boost::system::error_code& _error);

private:
	void				processSend				(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick);
	void				processHalt				(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr);
	void				processSendto			(UdpSocketShrPtr _udpSocket, NetPort _port, IpString _ip, 
												 const IPacketShrPtr _pk, IConnection* _connection, void* _tag, uint32 _lifecycle);
	void				processAccepted			(UdpAcceptorService* _udpAcptrSvc, UdpAcceptorShrPtr _acptr, UdpSubSocketShrPtr _udpSubSkt, 
												 IConnection* _connection, uint32 _lifecycle, const boost::system::error_code& _error);
	void				processSended			(UdpSubSocketShrPtr _udpSubSkt, UdpSubSocket::Type _type, SessionPxy _snPxy, const IPacketShrPtr _pk, 
												 bool _isKick, const boost::system::error_code& _error, std::size_t _bytes_transferred);
	void				processRecved			(UdpSubSocketShrPtr _udpSubSkt, const SessionPxy& _snPxy, RecvBuffer _buffer, const boost::system::error_code& _error);

private:
};
}
#endif
