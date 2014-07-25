#ifndef __Orz_OrzAsio_UdpAcceptorService_h__
#define __Orz_OrzAsio_UdpAcceptorService_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/IOService.h>
#include <orz/OrzAsio/NetEngine/UdpSubSocket.h>
namespace Orz
{
class _OrzOrzAsioExport UdpAcceptorService
	: public IOService
{
public:
	UdpAcceptorService(IServiceManager* _svcMgr, uint32 _threadNum, bool _isIp4=true);
	virtual ~UdpAcceptorService();

public:
	UdpAcceptorShrPtr	createAcceptor			(NetPort _port, uint32 _lifecycle);
	void				accept					(UdpAcceptorShrPtr _acptr, IConnection* _connection, uint32 _syn=1, uint32 _delayTime=5);
	void				send					(UdpAcceptorShrPtr _acptr, UdpService* _udpSvc, UdpSubSocketShrPtr _skt, boost::asio::ip::udp::endpoint& _ep, 
												 UdpSubSocket::Type _type, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick);

private:
	void				handleAccept			(UdpAcceptorShrPtr _acptr, UdpSubSocketShrPtr _udpSubSkt, IConnection* _connection, 
												 const boost::system::error_code& _error, std::size_t _bytes_transferred, uint32 _delayTime=5);
	void				handleSend				(UdpService* _udpSvc, UdpSubSocketShrPtr _skt, UdpSubSocket::Type _type, SessionPxy _snPxy, 
												 const IPacketShrPtr _pk, bool _isKick, const boost::system::error_code& _error, std::size_t _bytes_transferred);
	void				delayAccept				(DlTimerShrPtr _tmr, UdpAcceptorShrPtr _acptr, IConnection* _connection, uint32 _delayTime);
	void				checkAccept				(DlTimerShrPtr _tmr, UdpAcceptorShrPtr _acptr, IConnection* _connection, uint32 _delayTime);
private:
	bool										m_isIp4;
};
}
#endif
