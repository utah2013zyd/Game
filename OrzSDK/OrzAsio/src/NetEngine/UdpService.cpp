#include <orz/OrzAsio/NetEngine/UdpService.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/OrzAsio/NetEngine/UdpSubSktFreeList.h>
#include <orz/OrzAsio/NetEngine/UdpSubSocket.h>
#include <orz/OrzAsio/NetEngine/IOEngine.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
#include <orz/OrzAsio/NetEngine/UdpAcceptor.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
using namespace Orz;

UdpService::UdpService(IServiceManager* _svcMgr, uint32 _threadNum, bool _isIp4)
	: ProtocolService(_svcMgr, _threadNum, _isIp4)
{
}

UdpService::~UdpService()
{
}

void UdpService::send(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick)
{
	if (!_skt || !_pk)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_skt || !_pk", 0);
		return;
	}

	IOService::ioEngine().post(boost::bind(&UdpService::processSend, this, _skt, _snPxy, _pk, _isKick));
}

void UdpService::halt(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr)
{
	if (!_skt)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_skt", 0);
		return;
	}

	IOService::ioEngine().post(boost::bind(&UdpService::processHalt, this, _skt, _snPxy, _hasHdr));
}

void UdpService::sendto(UdpSocketShrPtr _udpSocket, NetPort _port, IpString _ip, 
						const IPacketShrPtr _pk, IConnection* _connection, void* _tag, uint32 _lifecycle)
{
	if (!_udpSocket || !_pk || !_connection)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_udpSocket || !_pk || !_connection", 0);
		return;
	}

	IOService::ioEngine().post(boost::bind(&UdpService::processSendto, this, _udpSocket, _port, _ip, _pk, _connection, _tag, _lifecycle));
}

void UdpService::accepted(UdpAcceptorService* _udpAcptrSvc, UdpAcceptorShrPtr _acptr, UdpSubSocketShrPtr _udpSubSkt, 
						  IConnection* _connection, const boost::system::error_code& _error)
{
	if (!_udpAcptrSvc || !_acptr || !_udpSubSkt || !_connection)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_udpAcptrSvc || !_acptr || !_udpSubSkt || !_connection", 0);
		return;
	}

	IOService::ioEngine().post(boost::bind(&UdpService::processAccepted, this, _udpAcptrSvc, _acptr, _udpSubSkt, _connection, _acptr->getLifecycle(), _error));
}

void UdpService::sended(UdpSubSocketShrPtr _skt, UdpSubSocket::Type _type, const SessionPxy& _snPxy, const IPacketShrPtr _pk, 
						bool _isKick, const boost::system::error_code& _error, std::size_t _bytes_transferred)
{
	IOService::ioEngine().post(boost::bind(&UdpService::processSended, this, _skt, _type, _snPxy, _pk, _isKick, _error, _bytes_transferred));
}

void UdpService::recved(UdpSubSocketShrPtr _udpSubSkt, const SessionPxy& _snPxy, StringPtr _buffer, const boost::system::error_code& _error)
{
	if (!_udpSubSkt)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_udpSubSkt", 0);
		return;
	}

	RecvBuffer recvedBuffer;
	memcpy(recvedBuffer.buffer, _buffer.ptr(), _buffer.size());
	recvedBuffer.recvedSize = _buffer.size();
	IOService::ioEngine().post(boost::bind(&UdpService::processRecved, this, _udpSubSkt, _snPxy, recvedBuffer, _error));
}

void UdpService::processSend(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick)
{
	_skt->send(_snPxy, _pk, _isKick);
}

void UdpService::processHalt(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr)
{
	_skt->halt(_snPxy, _hasHdr);
}

void UdpService::processSendto(UdpSocketShrPtr _udpSocket, NetPort _port, IpString _ip, 
							   const IPacketShrPtr _pk, IConnection* _connection, void* _tag, uint32 _lifecycle)
{
	UdpSubSocketShrPtr udpSubSkt = allocUdpSubSkt();
	if (udpSubSkt && udpSubSkt->hasInited())
		udpSubSkt->handleConnect(_udpSocket, _port, _ip, _pk, _connection, _tag, _lifecycle);
}

void UdpService::processAccepted(UdpAcceptorService* _udpAcptrSvc, UdpAcceptorShrPtr _acptr, UdpSubSocketShrPtr _udpSubSkt, 
								 IConnection* _connection, uint32 _lifecycle, const boost::system::error_code& _error)
{
	_udpSubSkt->handleAccept(_udpAcptrSvc, _acptr, _connection, _lifecycle, _error);
}

void UdpService::processSended(UdpSubSocketShrPtr _udpSubSkt, UdpSubSocket::Type _type, SessionPxy _snPxy, const IPacketShrPtr _pk, 
							   bool _isKick, const boost::system::error_code& _error, std::size_t _bytes_transferred)
{
	_udpSubSkt->handleSend(_type, _snPxy, _pk, _isKick, _error, _bytes_transferred);
}

void UdpService::processRecved(UdpSubSocketShrPtr _udpSubSkt, const SessionPxy& _snPxy, RecvBuffer _buffer, const boost::system::error_code& _error)
{
	_udpSubSkt->handleRecv(_snPxy, _buffer, _error);
}
