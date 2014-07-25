#include <orz/OrzAsio/NetEngine/SSLService.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/OrzAsio/NetEngine/SSLSktFreeList.h>
#include <orz/OrzAsio/NetEngine/SSLSocket.h>
#include <orz/OrzAsio/NetEngine/IOEngine.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
using namespace Orz;

SSLService::SSLService(IServiceManager* _svcMgr, bool _isIp4)
	: ProtocolService(_svcMgr, 1, _isIp4)
{
}

SSLService::~SSLService()
{
}

void SSLService::send(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick)
{
	if (!_skt || !_pk)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_skt || !_pk", 0);
		return;
	}

	IOService::ioEngine().post(boost::bind(&SSLService::processSend, this, _skt, _snPxy, _pk, _isKick));
}

void SSLService::halt(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr)
{
	if (!_skt)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_skt", 0);
		return;
	}

	IOService::ioEngine().post(boost::bind(&SSLService::processHalt, this, _skt, _snPxy, _hasHdr));
}

void SSLService::connect(IpString _ip, NetPort _port, IConnection* _connection, ContextId _contextId, void* _tag, uint32 _delayTime)
{
	IOService::ioEngine().post(boost::bind(&SSLService::processConnect, this, _ip, _port, _connection, _contextId, _tag, _delayTime));
}

void SSLService::accepted(SSLSocketShrPtr _sslSkt, NetPort _port, IConnection* _connection, const boost::system::error_code& _error)
{
	IOService::ioEngine().post(boost::bind(&SSLService::processAccepted, this, _sslSkt, _port, _connection, _error));
}

void SSLService::processSend(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick)
{
	_skt->send(_snPxy, _pk, _isKick);
}

void SSLService::processHalt(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr)
{
	_skt->halt(_snPxy, _hasHdr);
}

void SSLService::processConnect(IpString _ip, NetPort _port, IConnection* _connection, ContextId _contextId, void* _tag, uint32 _delayTime)
{
	SSLSocketShrPtr sslSkt = allocSSLSkt(_contextId);
	if (sslSkt && sslSkt->hasInited() && sslSkt->socket())
	{
		boost::asio::ip::tcp::resolver resolver(IOService::ioEngine().ioService());
		boost::asio::ip::tcp::resolver::query query(_ip.c_str(), d2s(_port).c_str());
		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
		boost::asio::ip::tcp::endpoint endpoint = *iterator;

		sslSkt->socket()->lowest_layer().async_connect(endpoint, boost::bind(&SSLService::handleConnect, this, sslSkt, _ip, _port, 
													   _connection, _tag, boost::asio::placeholders::error, ++iterator));
	}
	else if (_delayTime > 0)
	{
		delayConnect(DlTimerShrPtr(new boost::asio::deadline_timer(IOService::ioEngine().ioService())), _ip, _port, _connection, _contextId, _tag, _delayTime);
	}
}

void SSLService::processAccepted(SSLSocketShrPtr _sslSkt, NetPort _port, IConnection* _connection, const boost::system::error_code& _error)
{
	_sslSkt->handleAccept(_port, _connection, _error);
}

void SSLService::handleConnect(SSLSocketShrPtr _sslSkt, IpString _ip, NetPort _port, IConnection* _connection, void* _tag, 
							   const boost::system::error_code& _error, boost::asio::ip::tcp::resolver::iterator _itr)
{
	if (!_error)
    {
		_sslSkt->handleConnect(_ip, _port, _connection, _tag, _error);
	}
	else if (_itr != boost::asio::ip::tcp::resolver::iterator())
	{
		boost::asio::ip::tcp::endpoint endpoint = *_itr;
		_sslSkt->socket()->lowest_layer().async_connect(endpoint, boost::bind(&SSLService::handleConnect, this, _sslSkt, _ip, _port, _connection, _tag, 
																			  boost::asio::placeholders::error, ++_itr));
	}
}

void SSLService::delayConnect(DlTimerShrPtr _tmr, IpString _ip, NetPort _port, IConnection* _connection, ContextId _contextId, void* _tag, uint32 _delayTime)
{
	_tmr->expires_from_now(boost::posix_time::seconds(_delayTime));
	_tmr->async_wait(boost::bind(&SSLService::checkConnect, this, _tmr, _ip, _port, _connection, _contextId, _tag, _delayTime));
}

void SSLService::checkConnect(DlTimerShrPtr _tmr, IpString _ip, NetPort _port, IConnection* _connection, ContextId _contextId, void* _tag, uint32 _delayTime)
{
	SSLSocketShrPtr sslSkt = allocSSLSkt(_contextId);
	if (sslSkt && sslSkt->hasInited() && sslSkt->socket())
	{
		boost::asio::ip::tcp::resolver resolver(IOService::ioEngine().ioService());
		boost::asio::ip::tcp::resolver::query query(_ip.c_str(), d2s(_port).c_str());
		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
		boost::asio::ip::tcp::endpoint endpoint = *iterator;

		sslSkt->socket()->lowest_layer().async_connect(endpoint, boost::bind(&SSLService::handleConnect, this, sslSkt, _ip, _port, 
													   _connection, _tag, boost::asio::placeholders::error, ++iterator));
	}
	else
	{
		delayConnect(_tmr, _ip, _port, _connection, _contextId, _tag, _delayTime);
	}
}
