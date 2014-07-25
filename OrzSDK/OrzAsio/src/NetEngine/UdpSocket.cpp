#include <orz/OrzAsio/NetEngine/UdpSocket.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/OrzAsio/NetEngine/UdpService.h>
#include <orz/OrzAsio/NetEngine/IOEngine.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
using namespace Orz;

UdpSocket::UdpSocket()
	: m_io(0)
	, m_socket(0)
	, m_hasInited(false)
{
}

UdpSocket::UdpSocket(UdpService* _io, NetPort _port, bool _isIp4)
	: m_io(_io)
	, m_socket(new boost::asio::ip::udp::socket(m_io->ioEngine().ioService(), 
			   boost::asio::ip::udp::endpoint(_isIp4 ? boost::asio::ip::udp::v4() : boost::asio::ip::udp::v6(), _port)))
	, m_hasInited(true)
{
}

UdpSocket::~UdpSocket()
{
	if (m_socket)
		delete m_socket;
}

bool UdpSocket::init(UdpService* _io, NetPort _port, bool _isIp4)
{
	if (m_hasInited || !_io)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "m_hasInited || !_io", 0);
		return false;
	}

	m_io = _io;
	m_socket = new boost::asio::ip::udp::socket(m_io->ioEngine().ioService(), 
			   boost::asio::ip::udp::endpoint(_isIp4 ? boost::asio::ip::udp::v4() : boost::asio::ip::udp::v6(), _port));
	m_hasInited = true;
	return true;
}

void UdpSocket::clean()
{
	m_io = 0;
	if (m_socket)
	{
		delete m_socket;
		m_socket = 0;
	}
}

boost::asio::ip::udp::socket* UdpSocket::socket()
{
	return m_socket;
}

UdpService* UdpSocket::getUdpSvc()
{
	return m_io;
}

bool UdpSocket::hasInited() const
{
	return m_hasInited;
}
