#include <EchoConnection.h>
#include <orz/OrzAsio/AsioSingleton.h>
#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <PacketBuilder.h>
#include <Packet.h>
#include <orz/OrzAsio/NetEngine/SessionDeputy.h>
#include <orz/OrzAsio/NetEngine/ISession.h>
#include <orz/OrzAsio/NetEngine/ISocket.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
using namespace Orz;

EchoConnection::EchoConnection(std::string& _outIp, uint16 _port, uint32 _maxInc)
	: IConnection(_maxInc)
	, m_outIp(_outIp)
	, m_port(_port)
	, m_usingUdp(false)
	, m_udpConnection(false)
	, m_old(now())
{
}

EchoConnection::~EchoConnection()
{
	m_agent.join();
}

void EchoConnection::OnConnected(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc)
{
	sLogger->out(OL_INFO, "New session - snid: [", d2s(_snPxy.getSnid()).c_str(), "].", 0);
	if (_snPxy.getSocketType() == SKTP_TCP)
	{
		m_snPxy = _snPxy;
		// using thread to get user's input
		if (m_agent.start() != 0)
			return;
		m_agent.post(boost::bind(&EchoConnection::inputEchoStr, this));
	}
	else if (_snPxy.getSocketType() == SKTP_UDP)
	{
		m_udpSnPxy = _snPxy;
		m_udpConnection = true;
	}
}

void EchoConnection::OnReceived(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk)
{
	int32 err = 0;
	StringPtr str = _pk->readStr(err);
	if (!err)
	{	
		if (_snPxy.getSocketType() == SKTP_TCP)
			sLogger->out(OL_INFO, "[TCP] recv time - send time: ", d2s(now() - m_old).c_str(), 0);
		else
			sLogger->out(OL_INFO, "[UDP] recv time - send time: ", d2s(now() - m_old).c_str(), 0);
	}
}

void EchoConnection::OnSended(const SessionPxy& _snPxy, SessionDeputy& _snDy)
{
}

void EchoConnection::OnDisconnected(const SessionPxy& _snPxy, SessionDeputy& _snDy)
{
	sLogger->out(OL_INFO, "Session disconnected - snid: [", d2s(_snPxy.getSnid()).c_str(), "].", 0);
	if (_snPxy.getSocketType() == SKTP_TCP)
	{
		m_agent.stop();
		sAsioMgr->shutdown();
	}
	else
		m_udpConnection = false;
}

void EchoConnection::shutdown()
{
	m_agent.stop();
	sAsioMgr->shutdown();
}

void EchoConnection::write(IPacketShrPtr _pk)
{
	m_old = now();
	sSvcMgr(ServiceManager)->send(m_snPxy, _pk);
}

void EchoConnection::writeto(IPacketShrPtr _pk)
{
	m_old = now();
	if (!m_udpConnection)
		sSvcMgr(ServiceManager)->sendto(m_outIp.c_str(), m_port, _pk, this, 0, 5);
	else
		sSvcMgr(ServiceManager)->send(m_udpSnPxy, _pk);
}

void EchoConnection::inputEchoStr()
{
	char line[NET_PACKET_MAX_SIZE + 1];
    while (std::cin.getline(line, NET_PACKET_MAX_SIZE + 1))
    {
		using namespace std; // For strlen and memcpy.
		if (strcmp(line, "quit") == 0 || 
			strcmp(line, "bye") == 0 || 
			strcmp(line, "esc") == 0)
		{
			m_agent.stop();
			sAsioMgr->shutdown();
			return;
		}
		if (strcmp(line, "udp") == 0)
		{
			m_usingUdp = true;
			printPtr("EchoTest");
			continue;
		}
		if (strcmp(line, "!udp") == 0)
		{
			m_usingUdp = false;
			printPtr("EchoTest");
			continue;
		}
		IPacketShrPtr pk = allocPacket();
		pk->put(line, NET_PACKET_MAX_SIZE);
		if (!m_usingUdp)
			write(pk);
		else
			writeto(pk);
    }
}

void EchoConnection::printPtr(const char* _title)
{
	std::cout << "[" << _title << "]# " << std::flush;
}
