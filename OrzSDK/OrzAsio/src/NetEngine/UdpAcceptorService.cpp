#include <orz/OrzAsio/NetEngine/UdpAcceptorService.h>
#include <orz/OrzAsio/NetEngine/UdpSubSktFreeList.h>
#include <orz/OrzAsio/NetEngine/UdpAcceptor.h>
#include <orz/OrzAsio/NetEngine/UdpSocket.h>
#include <orz/OrzAsio/NetEngine/IPacket.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
#include <orz/OrzAsio/NetEngine/UdpService.h>
#include <orz/OrzAsio/NetEngine/ISession.h>
#include <orz/OrzAsio/NetEngine/IOEngine.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
using namespace Orz;

UdpAcceptorService::UdpAcceptorService(IServiceManager* _svcMgr, uint32 _threadNum, bool _isIp4)
	: IOService(_svcMgr, _threadNum)
	, m_isIp4(_isIp4)
{
}

UdpAcceptorService::~UdpAcceptorService()
{
}

UdpAcceptorShrPtr UdpAcceptorService::createAcceptor(NetPort _port, uint32 _lifecycle)
{
	return UdpAcceptorShrPtr(new UdpAcceptor(IOService::ioEngine(), _port, _lifecycle, m_isIp4));
}

void UdpAcceptorService::accept(UdpAcceptorShrPtr _acptr, IConnection* _connection, uint32 _syn, uint32 _delayTime)
{
	if (!_acptr || !_connection || _syn == 0)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_acptr || !_connection || _syn == 0", 0);
		return;
	}

	for (uint32 i=0; i<_syn; ++i)
	{
		UdpSubSocketShrPtr udpSubSkt = allocUdpSubSkt();
		if (udpSubSkt && udpSubSkt->hasInited())
		{
			_acptr->getUdpSocket()->async_receive_from(boost::asio::buffer(udpSubSkt->m_recvBuffer.buffer, udpSubSkt->m_recvBuffer.max), 
													   udpSubSkt->m_senderEp.m_senderEp, boost::bind(&UdpAcceptorService::handleAccept, this, _acptr, udpSubSkt, 
													   _connection, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, _delayTime));
		}
		else if (_delayTime > 0)
		{
			delayAccept(DlTimerShrPtr(new boost::asio::deadline_timer(IOService::ioEngine().ioService())), _acptr, _connection, _delayTime);
		}
	}
}

void UdpAcceptorService::send(UdpAcceptorShrPtr _acptr, UdpService* _udpSvc, UdpSubSocketShrPtr _skt, boost::asio::ip::udp::endpoint& _ep, 
							   UdpSubSocket::Type _type, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick)
{
	if (!_acptr || !_udpSvc || !_skt || !_pk)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_acptr || !_udpSvc || !_skt || !_pk", 0);
		return;
	}
	
	_acptr->getUdpSocket()->async_send_to(boost::asio::buffer(_pk->raw(), _pk->size()), _ep, 
										  boost::bind(&UdpAcceptorService::handleSend, this, _udpSvc, _skt, _type, _snPxy, _pk, 
										  _isKick, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void UdpAcceptorService::handleAccept(UdpAcceptorShrPtr _acptr, UdpSubSocketShrPtr _udpSubSkt, IConnection* _connection, 
									  const boost::system::error_code& _error, std::size_t _bytes_transferred, uint32 _delayTime)
{
	if (!_acptr || !_udpSubSkt || !_connection)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_acptr || !_udpSubSkt || !_connection == 0", 0);
		return;
	}

	DynShrLock lock(_acptr->mutex(_udpSubSkt->m_senderEp));
	_udpSubSkt->m_recvBuffer.recvedSize = _bytes_transferred;
	UdpService* udpSvc = _udpSubSkt->getUdpSvc();
	if (udpSvc)
	{
		if (_acptr->add(_udpSubSkt->m_senderEp, _udpSubSkt))
			udpSvc->accepted(this, _acptr, _udpSubSkt, _connection, _error);
		else
		{
			UdpSubSocketShrPtr udpSubSkt = _acptr->get(_udpSubSkt->m_senderEp);
			if (udpSubSkt)
				udpSvc->recved(udpSubSkt, udpSubSkt->session()->getSessionPxy(), StringPtr(_udpSubSkt->m_recvBuffer.buffer, _udpSubSkt->m_recvBuffer.recvedSize), _error);
		}
	}

	accept(_acptr, _connection, 1, _delayTime);
}

void UdpAcceptorService::handleSend(UdpService* _udpSvc, UdpSubSocketShrPtr _skt, UdpSubSocket::Type _type, SessionPxy _snPxy, 
									 const IPacketShrPtr _pk, bool _isKick, const boost::system::error_code& _error, std::size_t _bytes_transferred)
{
	_udpSvc->sended(_skt, _type, _snPxy, _pk, _isKick, _error, _bytes_transferred);
}

void UdpAcceptorService::delayAccept(DlTimerShrPtr _tmr, UdpAcceptorShrPtr _acptr, IConnection* _connection, uint32 _delayTime)
{
	_tmr->expires_from_now(boost::posix_time::seconds(_delayTime));
	_tmr->async_wait(boost::bind(&UdpAcceptorService::checkAccept, this, _tmr, _acptr, _connection, _delayTime));
}

void UdpAcceptorService::checkAccept(DlTimerShrPtr _tmr, UdpAcceptorShrPtr _acptr, IConnection* _connection, uint32 _delayTime)
{
	UdpSubSocketShrPtr udpSubSkt = allocUdpSubSkt();
	if (udpSubSkt && udpSubSkt->hasInited())
	{
		_acptr->getUdpSocket()->async_receive_from(boost::asio::buffer(udpSubSkt->m_recvBuffer.buffer, udpSubSkt->m_recvBuffer.max), 
												   udpSubSkt->m_senderEp.m_senderEp, boost::bind(&UdpAcceptorService::handleAccept, this, _acptr, udpSubSkt, 
												   _connection, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, _delayTime));
	}
	else
	{
		delayAccept(_tmr, _acptr, _connection, _delayTime);
	}
}
