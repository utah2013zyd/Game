#ifndef __Orz_OrzAsio_UdpSocket_h__
#define __Orz_OrzAsio_UdpSocket_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
class _OrzOrzAsioExport UdpSocket
{
public:
	UdpSocket();
	UdpSocket(UdpService* _io, NetPort _port, bool _isIp4=true);
	~UdpSocket();

	bool							init				(UdpService* _io, NetPort _port, bool _isIp4=true);
	void							clean				();

public:
	boost::asio::ip::udp::socket*	socket				();
	UdpService*						getUdpSvc			();
	bool							hasInited			() const;

public:
	UdpService*						m_io;
	boost::asio::ip::udp::socket*	m_socket;
	bool							m_hasInited;
};
}
#endif
