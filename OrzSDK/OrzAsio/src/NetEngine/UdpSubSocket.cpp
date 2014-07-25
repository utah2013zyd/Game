#include <orz/OrzAsio/NetEngine/UdpSubSocket.h>
#include <orz/OrzAsio/NetEngine/UdpSocket.h>
#include <orz/OrzAsio/NetEngine/Packet.hpp>
#include <orz/Toolkit_Plus/Toolkit/MemString.hpp>
#include <orz/OrzAsio/NetEngine/NetData.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
#include <orz/OrzAsio/NetEngine/ISession.h>
#include <orz/OrzAsio/NetEngine/UdpSubSktFreeList.h>
#include <orz/OrzAsio/NetEngine/IServiceManager.h>
#include <orz/OrzAsio/NetEngine/UdpService.h>
#include <orz/OrzAsio/NetEngine/IOEngine.h>
#include <orz/OrzAsio/NetEngine/UdpAcceptorService.h>
#include <orz/OrzAsio/NetEngine/UdpAcceptor.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
using namespace Orz;

UdpSubSocket::UdpSubSocket()
	: m_io(0)
	, m_svcMgr(0)
	, m_udpAcptrSvc(0)
	, m_sktType(SKTP_UDP)
	, m_lifecycle(30)
	, m_port(0)
	, m_isClosed(false)
	, m_hasNetAction(false)
	, m_timer(0)
	, m_hasInited(false)
{
}

UdpSubSocket::~UdpSubSocket()
{
	if (m_timer)
		delete m_timer;
}

bool UdpSubSocket::init(ISessionShrPtr _sn, UdpService* _io, IServiceManager* _svcMgr, bool _isIp4)
{
	if (m_hasInited || !_sn || !_io || !_svcMgr)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "m_hasInited || !_sn || !_io || !_svcMgr", 0);
		return false;
	}

	m_io = _io;
	m_svcMgr = _svcMgr;
	m_session = _sn;
	m_session->init(m_io, this);
	m_timer = new boost::asio::deadline_timer(m_io->ioEngine().ioService());
	m_hasInited = true;
	return true;
}

void UdpSubSocket::clean()
{
	m_io = 0;
	m_svcMgr = 0;
	m_acptr = UdpAcceptorShrPtr();
	m_udpSkt = UdpSocketShrPtr();
	m_session = ISessionShrPtr();
	if (m_timer)
	{
		delete m_timer;
		m_timer = 0;
	}
}

ISessionShrPtr UdpSubSocket::session()
{
	return m_session;
}

UdpService* UdpSubSocket::getUdpSvc()
{
	return m_io;
}

bool UdpSubSocket::hasInited()
{
	return m_hasInited;
}

int32 UdpSubSocket::send(const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick)
{
	if (_snPxy == m_session->getSessionPxy())
	{
		if (m_type == TYPE_SEND)
		{
			const NetData& nd = m_session->getNetData();
			boost::asio::ip::address adrs(boost::asio::ip::address::from_string(nd.ip.c_str()));
			boost::asio::ip::udp::endpoint peerPoint(adrs, nd.port);
			m_udpSkt->socket()->async_send_to(boost::asio::buffer(_pk->raw(), _pk->size()), peerPoint, 
											  boost::bind(&UdpSubSocket::handleSendto, shared_from_this(), _pk, 
											  _isKick, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			return 0;
		}
		else if (m_type == TYPE_RECV)
		{
			m_udpAcptrSvc->send(m_acptr, m_io, shared_from_this(), m_senderEp.m_senderEp, m_type, _snPxy, _pk, _isKick);
			return 0;
		}
		else
			return 1;
	}
	else
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "_snPxy != m_session->getSessionPxy()", 0);
		return 1;
	}
}

int32 UdpSubSocket::halt(const SessionPxy& _snPxy, bool _hasHdr)
{
	if (_snPxy == m_session->getSessionPxy())
	{
		m_session->handleEnd(_snPxy, false, _hasHdr);
		return 0;
	}
	else
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "_snPxy != m_session->getSessionPxy()", 0);
		return 1;
	}
}

int32 UdpSubSocket::type() const
{
	return (int32)m_sktType;
}

void UdpSubSocket::close()
{
	m_isClosed = true;
	m_hasNetAction = false;
	m_udpAcptrSvc = 0;
	m_acptr = UdpAcceptorShrPtr();
	if (m_timer)
		m_timer->cancel();
}

bool UdpSubSocket::isClosed() const
{
	return m_isClosed;
}

int32 UdpSubSocket::handleAccept(UdpAcceptorService* _udpAcptrSvc, UdpAcceptorShrPtr _acptr, IConnection* _connection, 
								 uint32 _lifecycle, const boost::system::error_code& _error)
{
	if (!m_session)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_session", 0);
		return 1;
	}

	m_udpAcptrSvc = _udpAcptrSvc;
	m_acptr = _acptr;
	m_type = TYPE_RECV;
	m_lifecycle = _lifecycle;
	m_hasNetAction = false;
	
	if (m_session->handleIn(_error ? true : false, NetData(m_acptr->getUdpSocket()->local_endpoint().port(), IpString(), _connection)) == 0)
	{
		handleRecv(m_session->getSessionPxy(), m_recvBuffer, _error);
		m_udpAcptrSvc->accept(_acptr, _connection);
		addTimer(m_lifecycle*1000);
		return 0;
	}
	else
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "m_session->handleIn != 0", 0);
		return 1;
	}
}

int32 UdpSubSocket::handleConnect(UdpSocketShrPtr _udpSocket, NetPort _port, IpString _ip, const IPacketShrPtr _pk, IConnection* _connection, void* _tag, uint32 _lifecycle)
{
	if (!m_session)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_session", 0);
		return 1;
	}

	m_udpSkt = _udpSocket;
	m_type = TYPE_SEND;
	m_lifecycle = _lifecycle;
	m_hasNetAction = false;
	
	if (m_session->handleOut(false, NetData(_port, _ip, _connection, _tag)) == 0)
	{
		send(m_session->getSessionPxy(), _pk, false);
		addTimer(m_lifecycle*1000);
		return 0;
	}
	else
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "m_session->handleOut != 0", 0);
		return 1;
	}
}

void UdpSubSocket::handleRecv(const SessionPxy& _snPxy, RecvBuffer& _buffer, const boost::system::error_code& _error)
{
	if (!m_session)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_session", 0);
		return;
	}

	if (m_session->getSessionPxy() == _snPxy)
	{
		m_hasNetAction = true;
		m_session->handleRead(_snPxy, _error ? true : false, StringPtr(_buffer.buffer, _buffer.recvedSize));
	}
	else
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "_snPxy != m_session->getSessionPxy()", 0);
}

void UdpSubSocket::handleSend(UdpSubSocket::Type _type, const SessionPxy& _snPxy, const IPacketShrPtr _pk, 
							  bool _isKick, const boost::system::error_code& _error, std::size_t _bytes_transferred)
{
	if (!m_session)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_session", 0);
		return;
	}

	if (_snPxy == m_session->getSessionPxy())
	{
		m_session->handleWrite(_snPxy, _error ? true : false, _pk, _isKick);
		if (_type == TYPE_SEND)
		{
			m_udpSkt->socket()->async_receive_from(boost::asio::buffer(m_recvBuffer.buffer, m_recvBuffer.max), 
												   m_senderEp.m_senderEp, boost::bind(&UdpSubSocket::handleSendtoBack, shared_from_this(), 
												   boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		}
		m_hasNetAction = true;
	}
	else
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "_snPxy != m_session->getSessionPxy()", 0);
}

void UdpSubSocket::handleSendto(IPacketShrPtr _pk, bool _isKick, const boost::system::error_code& _error, std::size_t _bytes_transferred)
{
	handleSend(TYPE_SEND, m_session->getSessionPxy(), _pk, _isKick, _error, _bytes_transferred);
	if (!_error)
	{
		m_udpSkt->socket()->async_receive_from(boost::asio::buffer(m_recvBuffer.buffer, m_recvBuffer.max), 
											   m_senderEp.m_senderEp, boost::bind(&UdpSubSocket::handleSendtoBack, shared_from_this(), 
											   boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
}

void UdpSubSocket::handleSendtoBack(const boost::system::error_code& _error, std::size_t _bytes_transferred)
{
	m_recvBuffer.recvedSize = _bytes_transferred;
	handleRecv(m_session->getSessionPxy(), m_recvBuffer, _error);
	if (!_error)
	{
		m_udpSkt->socket()->async_receive_from(boost::asio::buffer(m_recvBuffer.buffer, m_recvBuffer.max), 
											   m_senderEp.m_senderEp, boost::bind(&UdpSubSocket::handleSendtoBack, shared_from_this(), 
											   boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
}

void UdpSubSocket::checkSessionLife(SessionPxy _snPxy, TimerArg _arg)
{
	if (_snPxy == m_session->getSessionPxy())
	{
		if (m_hasNetAction)
		{
			_arg.reset();
			m_hasNetAction = false;
			addTimer(m_lifecycle*1000);
		}
		else
			halt(m_session->getSessionPxy(), true);
	}
	else
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "_snPxy != m_session->getSessionPxy()", 0);
}

void UdpSubSocket::addTimer(uint32 _msec)
{
	if (!m_hasInited)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!m_hasInited", 0);
		return;
	}

	m_timer->expires_from_now(boost::posix_time::millisec(_msec));
	m_timer->async_wait(boost::bind(&UdpSubSocket::checkSessionLife, shared_from_this(), SessionPxy(m_session->getSessionPxy()), TimerArg()));
}
