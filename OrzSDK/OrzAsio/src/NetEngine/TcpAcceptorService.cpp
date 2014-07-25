#include <orz/OrzAsio/NetEngine/TcpAcceptorService.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/OrzAsio/NetEngine/TcpSktFreeList.h>
#include <orz/OrzAsio/NetEngine/TcpSocket.h>
#include <orz/OrzAsio/NetEngine/IOEngine.h>
#include <orz/OrzAsio/NetEngine/TcpService.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
using namespace Orz;

TcpAcceptorService::TcpAcceptorService(IServiceManager* _svcMgr, uint32 _threadNum, bool _isIp4)
	: IOService(_svcMgr, _threadNum)
	, m_isIp4(_isIp4)
{
}

TcpAcceptorService::~TcpAcceptorService()
{
}

TcpAcceptorShrPtr TcpAcceptorService::createAcceptor(NetPort _port)
{
	boost::asio::ip::tcp::endpoint endpoint(m_isIp4 ? boost::asio::ip::tcp::v4() : boost::asio::ip::tcp::v6(), _port);

	// create new acceptor
	TcpAcceptorShrPtr tcpAcptr(new boost::asio::ip::tcp::acceptor(IOService::ioEngine().ioService()));

	boost::system::error_code er;

	tcpAcptr->open(endpoint.protocol(), er);
	if (er)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "acceptor::open failed!", 0);
		return TcpAcceptorShrPtr();
	}
	tcpAcptr->set_option(boost::asio::socket_base::reuse_address(true));

	tcpAcptr->bind(endpoint, er);
	if (er)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "acceptor::bind failed!", 0);
		return TcpAcceptorShrPtr();
	}

	tcpAcptr->listen(boost::asio::socket_base::max_connections, er);
	if (er)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "acceptor::listen failed!", 0);
		return TcpAcceptorShrPtr();
	}

	// config the new acceptor
	tcpAcptr->set_option(boost::asio::ip::tcp::no_delay(true));
	tcpAcptr->set_option(boost::asio::socket_base::keep_alive(true));
	tcpAcptr->set_option(boost::asio::socket_base::enable_connection_aborted(true));
	tcpAcptr->set_option(boost::asio::socket_base::receive_buffer_size(RECV_BUFFER_SIZE));
	tcpAcptr->set_option(boost::asio::socket_base::send_buffer_size(SEND_BUFFER_SIZE));

	return tcpAcptr;
}

int32 TcpAcceptorService::accept(TcpAcceptorShrPtr _acptr, IConnection* _connection, uint32 _syn, uint32 _delayTime)
{
	if (!_acptr || !_connection || _syn == 0)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_acptr || !_connection || _syn == 0", 0);
		return 1;
	}

	for (uint32 i=0; i<_syn; ++i)
	{
		TcpSocketShrPtr tcpSkt = allocTcpSkt();
		if (tcpSkt && tcpSkt->socket())
		{
			_acptr->async_accept(*(tcpSkt->socket()), boost::bind(&TcpAcceptorService::handleAccept, this, _acptr, 
																  tcpSkt, _connection, boost::asio::placeholders::error, _delayTime));
		}
		else if (_delayTime > 0)
		{
			delayAccept(DlTimerShrPtr(new boost::asio::deadline_timer(IOService::ioEngine().ioService())), _acptr, _connection, _delayTime);
		}
	}
	return 0;
}

void TcpAcceptorService::handleAccept(TcpAcceptorShrPtr _acptr, TcpSocketShrPtr _tcpSkt, IConnection* _connection, 
									  const boost::system::error_code& _error, uint32 _delayTime)
{
	if (!_acptr || !_tcpSkt || !_connection)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_acptr || !_sslSkt || !_connection == 0", 0);
		return;
	}

	TcpService* tcpSvc = _tcpSkt->getTcpSvc();
	if (!tcpSvc)
		return;

	tcpSvc->accepted(_tcpSkt, _acptr->local_endpoint().port(), _connection, _error);
	accept(_acptr, _connection, 1, _delayTime);
}

void TcpAcceptorService::delayAccept(DlTimerShrPtr _tmr, TcpAcceptorShrPtr _acptr, IConnection* _connection, uint32 _delayTime)
{
	_tmr->expires_from_now(boost::posix_time::seconds(_delayTime));
	_tmr->async_wait(boost::bind(&TcpAcceptorService::checkAccept, this, _tmr, _acptr, _connection, _delayTime));
}

void TcpAcceptorService::checkAccept(DlTimerShrPtr _tmr, TcpAcceptorShrPtr _acptr, IConnection* _connection, uint32 _delayTime)
{
	TcpSocketShrPtr tcpSkt = allocTcpSkt();
	if (tcpSkt && tcpSkt->socket())
	{	
		_acptr->async_accept(*(tcpSkt->socket()), boost::bind(&TcpAcceptorService::handleAccept, this, _acptr, tcpSkt, 
															  _connection, boost::asio::placeholders::error, _delayTime));
	}
	else
	{
		delayAccept(_tmr, _acptr, _connection, _delayTime);
	}
}
