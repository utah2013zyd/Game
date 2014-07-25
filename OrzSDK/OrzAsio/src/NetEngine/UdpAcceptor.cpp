#include <orz/OrzAsio/NetEngine/UdpAcceptor.h>
#include <orz/OrzAsio/NetEngine/IOEngine.h>
using namespace Orz;

UdpAcceptor::UdpAcceptor(IOEngine& _io, NetPort _port, uint32 _lifecycle, bool _isIp4)
	: m_udpSubSockets(3)
	, m_socket(new boost::asio::ip::udp::socket(_io.ioService(), boost::asio::ip::udp::endpoint(_isIp4 ? boost::asio::ip::udp::v4() : boost::asio::ip::udp::v6(), _port)))
	, m_lifecycle(_lifecycle)
{
	m_udpSubSockets.setHash();
}

UdpAcceptor::~UdpAcceptor()
{
	delete m_socket;
}

boost::shared_mutex& UdpAcceptor::mutex(const UdpSdrEndpoint& _senderEp)
{
	return m_udpSubSockets.mutex(_senderEp, HashArg(_senderEp));
}

bool UdpAcceptor::add(const UdpSdrEndpoint& _senderEp, UdpSubSocketShrPtr _udpSubSkt, bool _usingLock)
{
	return m_udpSubSockets.add(_senderEp, _udpSubSkt, HashArg(_senderEp), _usingLock) == 0 ? true : false;
}

UdpSubSocketShrPtr UdpAcceptor::get(const UdpSdrEndpoint& _senderEp, bool _usingLock)
{
	return m_udpSubSockets.get(_senderEp, HashArg(_senderEp), _usingLock);
}

boost::asio::ip::udp::socket* UdpAcceptor::getUdpSocket()
{
	return m_socket;
}
	
uint32 UdpAcceptor::getLifecycle()
{
	return m_lifecycle;
}
