#include <orz/OrzAsio/NetEngine/TcpSocket.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/OrzAsio/NetEngine/IPacket.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
#include <orz/OrzAsio/NetEngine/NetData.h>
#include <orz/OrzAsio/NetEngine/ISession.h>
#include <orz/OrzAsio/NetEngine/IServiceManager.h>
#include <orz/OrzAsio/NetEngine/TcpService.h>
#include <orz/OrzAsio/NetEngine/IOEngine.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
using namespace Orz;

TcpSocket::TcpSocket()
	: m_io(0)
	, m_svcMgr(0)
	, m_socket(0)
	, m_type(SKTP_TCP)
	, m_hasInited(false)
{
}

TcpSocket::~TcpSocket()
{
	if (m_socket)
		delete m_socket;
}

bool TcpSocket::init(ISessionShrPtr _sn, TcpService* _io, IServiceManager* _svcMgr)
{
	if (m_hasInited || !_io || !_svcMgr)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "m_hasInited || !_io || !_svcMgr", 0);
		return false;
	}

	m_io = _io;
	m_svcMgr = _svcMgr;
	m_socket = new boost::asio::ip::tcp::socket(m_io->ioEngine().ioService());
	m_session = _sn;
	m_session->init(m_io, this);
	//boost::asio::ip::tcp::socket::non_blocking_io non_blocking_io(true);
 //   m_socket->io_control(non_blocking_io);
	m_hasInited = true;
	return true;
}

void TcpSocket::clean()
{
	m_io = 0;
	m_svcMgr = 0;
	if (m_socket)
	{
		delete m_socket;
		m_socket = 0;
	}
	m_session = ISessionShrPtr();
}

bool TcpSocket::hasInited()
{
	return m_hasInited;
}

boost::asio::ip::tcp::socket* TcpSocket::socket()
{
	return m_socket;
}

ISessionShrPtr TcpSocket::session()
{
	return m_session;
}

TcpService* TcpSocket::getTcpSvc()
{
	return m_io;
}

int32 TcpSocket::send(const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick)
{
	if (!m_session || !m_socket || !m_socket->is_open())
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_session || !m_socket || !m_socket->is_open()", 0);
		return 1;
	}

	if (_snPxy == m_session->getSessionPxy())
	{
		boost::asio::async_write(*m_socket, boost::asio::buffer(_pk->raw(), _pk->size()), 
								 boost::bind(&TcpSocket::handleSend, shared_from_this(), _pk, _isKick, boost::asio::placeholders::error));
		return 0;
	}
	else
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "_snPxy != m_session->getSessionPxy()", 0);
		return 1;
	}
}

int32 TcpSocket::halt(const SessionPxy& _snPxy, bool _hasHdr)
{
	if (!m_session || !m_socket || !m_socket->is_open())
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_session || !m_socket || !m_socket->is_open()", 0);
		return 1;
	}

	if (_snPxy == m_session->getSessionPxy())
	{
		m_session->handleEnd(_snPxy, m_socket->is_open() ? false : true, _hasHdr);
		close();
		return 0;
	}
	else
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "_snPxy != m_session->getSessionPxy()", 0);
		return 1;
	}
}

int32 TcpSocket::type() const
{
	return (int32)m_type;
}

void TcpSocket::close()
{
	if (m_socket && m_hasInited)
		m_socket->close();
}

bool TcpSocket::isClosed() const
{
	if (m_socket && m_hasInited)
		return !m_socket->is_open();
	else
		return false;
}

void TcpSocket::handleAccept(NetPort _port, IConnection* _connection, const boost::system::error_code& _error)
{	
	if (!m_socket || !_connection || !m_session)
	{
		close();
		return;
	}

	if (m_session->handleIn(_error ? true : false, NetData(_port, IpString(), _connection)) == 0)
		recv();
	else
		close();
}

void TcpSocket::handleConnect(IpString _ip, NetPort _port, IConnection* _connection, void* _tag, const boost::system::error_code& _error)
{
	if (!m_session || !_connection || !m_socket)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_session || !_connection || !m_socket", 0);
		return;
	}
	
	if (_error) 
	{
		close();
		return;
	}

	if (m_session->handleOut(_error ? true : false, NetData(_port, _ip, _connection, _tag)) == 0)
		recv();
	else
		close();
}

int32 TcpSocket::recv()
{
	m_socket->async_read_some(boost::asio::buffer(m_recvBuffer.buffer, m_recvBuffer.max), 
							  boost::bind(&TcpSocket::handleRecv, shared_from_this(), 
							  boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	return 0;
}

void TcpSocket::handleSend(IPacketShrPtr _pk, bool _isKick, const boost::system::error_code& _error)
{
	if (!m_session || !m_socket)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_session || !m_socket", 0);
		return;
	}

	m_session->handleWrite(m_session->getSessionPxy(), _error ? true : false, _pk, _isKick);
	if (_error || _isKick)
		close();
}

void TcpSocket::handleRecv(const boost::system::error_code& _error, std::size_t _bytes_transferred)
{
	if (!m_session || !m_socket)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_session || !m_socket", 0);
		return;
	}

	m_session->handleRead(m_session->getSessionPxy(), _error ? true : false, StringPtr(m_recvBuffer.buffer, _bytes_transferred));
	if (!_error) 
		recv();
	if (_error)	
		close();
}
