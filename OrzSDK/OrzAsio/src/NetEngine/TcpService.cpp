#include <orz/OrzAsio/NetEngine/TcpService.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/OrzAsio/NetEngine/TcpSktFreeList.h>
#include <orz/OrzAsio/NetEngine/TcpSocket.h>
#include <orz/OrzAsio/NetEngine/IOEngine.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
using namespace Orz;

TcpService::TcpService(IServiceManager* _svcMgr, bool _isIp4)
	: ProtocolService(_svcMgr, 1, _isIp4)
{
}

TcpService::~TcpService()
{
}

void TcpService::send(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick)
{
	if (!_skt || !_pk)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_skt || !_pk", 0);
		return;
	}

	IOService::ioEngine().post(boost::bind(&TcpService::processSend, this, _skt, _snPxy, _pk, _isKick));
}

void TcpService::halt(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr)
{
	if (!_skt)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_skt", 0);
		return;
	}

	IOService::ioEngine().post(boost::bind(&TcpService::processHalt, this, _skt, _snPxy, _hasHdr));
}

void TcpService::connect(IpString _ip, NetPort _port, IConnection* _connection, void* _tag, uint32 _delayTime)
{
	IOService::ioEngine().post(boost::bind(&TcpService::processConnect, this, _ip, _port, _connection, _tag, _delayTime));
}

void TcpService::accepted(TcpSocketShrPtr _tcpSkt, NetPort _port, IConnection* _connection, const boost::system::error_code& _error)
{
	IOService::ioEngine().post(boost::bind(&TcpService::processAccepted, this, _tcpSkt, _port, _connection, _error));
}

void TcpService::processSend(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick)
{
	_skt->send(_snPxy, _pk, _isKick);
}

void TcpService::processHalt(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr)
{
	_skt->halt(_snPxy, _hasHdr);
}

void TcpService::processConnect(IpString _ip, NetPort _port, IConnection* _connection, void* _tag, uint32 _delayTime)
{
	TcpSocketShrPtr tcpSkt = allocTcpSkt();
	if (tcpSkt && tcpSkt->hasInited() && tcpSkt->socket())
	{
		boost::asio::ip::tcp::resolver resolver(IOService::ioEngine().ioService());
		boost::asio::ip::tcp::resolver::query query(_ip.c_str(), d2s(_port).c_str());
		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
		boost::asio::ip::tcp::endpoint endpoint = *iterator;

		tcpSkt->socket()->async_connect(endpoint, boost::bind(&TcpService::handleConnect, this, tcpSkt, _ip, _port, 
										_connection, _tag, boost::asio::placeholders::error, ++iterator));
	}
	else if (_delayTime > 0)
	{
		delayConnect(DlTimerShrPtr(new boost::asio::deadline_timer(IOService::ioEngine().ioService())), _ip, _port, _connection, _tag, _delayTime);
	}
}

void TcpService::processAccepted(TcpSocketShrPtr _tcpSkt, NetPort _port, IConnection* _connection, const boost::system::error_code& _error)
{
	_tcpSkt->handleAccept(_port, _connection, _error);
}

void TcpService::handleConnect(TcpSocketShrPtr _tcpSkt, IpString _ip, NetPort _port, IConnection* _connection, void* _tag, 
							   const boost::system::error_code& _error, boost::asio::ip::tcp::resolver::iterator _itr)
{
	if (!_error)
    {
		_tcpSkt->handleConnect(_ip, _port, _connection, _tag, _error);
	}
	else if (_itr != boost::asio::ip::tcp::resolver::iterator())
	{
		boost::asio::ip::tcp::endpoint endpoint = *_itr;
		_tcpSkt->socket()->async_connect(endpoint, boost::bind(&TcpService::handleConnect, this, _tcpSkt, _ip, _port, _connection, _tag,
															   boost::asio::placeholders::error, ++_itr));
	}
}

void TcpService::delayConnect(DlTimerShrPtr _tmr, IpString _ip, NetPort _port, IConnection* _connection, void* _tag, uint32 _delayTime)
{
	_tmr->expires_from_now(boost::posix_time::seconds(_delayTime));
	_tmr->async_wait(boost::bind(&TcpService::checkConnect, this, _tmr, _ip, _port, _connection, _tag, _delayTime));
}

void TcpService::checkConnect(DlTimerShrPtr _tmr, IpString _ip, NetPort _port, IConnection* _connection, void* _tag, uint32 _delayTime)
{
	TcpSocketShrPtr tcpSkt = allocTcpSkt();
	if (tcpSkt && tcpSkt->hasInited() && tcpSkt->socket())
	{
		boost::asio::ip::tcp::resolver resolver(IOService::ioEngine().ioService());
		boost::asio::ip::tcp::resolver::query query(_ip.c_str(), d2s(_port).c_str());
		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
		boost::asio::ip::tcp::endpoint endpoint = *iterator;

		tcpSkt->socket()->async_connect(endpoint, boost::bind(&TcpService::handleConnect, this, tcpSkt, _ip, _port, 
										_connection, _tag, boost::asio::placeholders::error, ++iterator));
	}
	else
	{
		delayConnect(_tmr, _ip, _port, _connection, _tag, _delayTime);
	}
}
