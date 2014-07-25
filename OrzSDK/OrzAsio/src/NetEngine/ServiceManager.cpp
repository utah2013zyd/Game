#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <orz/Toolkit_Plus/Toolkit/NumChooser.hpp>
#include <orz/Toolkit_Plus/Toolkit/ResCluster.hpp>
#include <orz/OrzAsio/NetEngine/NetData.h>
#include <orz/OrzAsio/NetEngine/TcpAcceptorService.h>
#include <orz/OrzAsio/NetEngine/SSLAcceptorService.h>
#include <orz/OrzAsio/NetEngine/UdpAcceptorService.h>
#include <orz/OrzAsio/NetEngine/TcpService.h>
#include <orz/OrzAsio/NetEngine/SSLService.h>
#include <orz/OrzAsio/NetEngine/UdpService.h>
#include <orz/OrzAsio/NetEngine/TcpSocket.h>
#include <orz/OrzAsio/NetEngine/SSLSocket.h>
#include <orz/OrzAsio/NetEngine/UdpSubSocket.h>
#include <orz/OrzAsio/NetEngine/UdpSubSktFreeList.h>
#include <orz/OrzAsio/NetEngine/TcpSktFreeList.h>
#include <orz/OrzAsio/NetEngine/SSLSktFreeList.h>
#include <orz/OrzAsio/NetEngine/UdpSktFreeList.h>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/OrzAsio/NetEngine/IPacketParser.h>
#include <NetEngine/Session.h>
#include <orz/OrzAsio/NetEngine/UdpSocket.h>
#include <orz/OrzAsio/NetEngine/PacketParser.h>
#include <orz/OrzAsio/NetEngine/PacketFreeList.h>
#include <orz/OrzAsio/NetEngine/Packet.hpp>
#include <orz/OrzAsio/NetEngine/IOEngine.h>
#include <orz/OrzAsio/NetEngine/SSLContext.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
using namespace Orz;

ServiceManager::ServiceManager(uint32 _tcpAcptrSvcNum, uint32 _udpAcptrSvcNum, uint32 _sslAcptrSvcNum, 
							   uint32 _tcpSvcNum, uint32 _udpSvcNum, uint32 _sslSvcNum, 
							   uint32 _tcpAcptrThreadNum, uint32 _udpAcptrThreadNum, uint32 _sslAcptrThreadNum, uint32 _contextBase, 
							   uint32 _perClrTcpSktNum, uint32 _tcpSktClrNum, uint32 _perClrUdpSubSktNum, uint32 _udpSubSktClrNum, 
							   uint32 _perClrUdpSktNum, uint32 _udpSktClrNum, IFreeListShrPtr _packetFreeList, 
							   IPacketParserShrPtr _pktParser, bool _isIp4)
	: m_tcpAcptrSvcNum(_tcpAcptrSvcNum)
	, m_udpAcptrSvcNum(_udpAcptrSvcNum)
	, m_sslAcptrSvcNum(_sslAcptrSvcNum)
	, m_tcpSvcNum(_tcpSvcNum)
	, m_udpSvcNum(_udpSvcNum)
	, m_sslSvcNum(_sslSvcNum)
	, m_tcpAcptrThreadNum(_tcpAcptrThreadNum == 0 ? 1 : _tcpAcptrThreadNum)
	, m_udpAcptrThreadNum(_udpAcptrThreadNum == 0 ? 1 : _udpAcptrThreadNum)
	, m_sslAcptrThreadNum(_sslAcptrThreadNum == 0 ? 1 : _sslAcptrThreadNum)
	, m_perClrTcpSktNum(_perClrTcpSktNum == 0 ? 1 : _perClrTcpSktNum)
	, m_tcpSktClrNum(_tcpSktClrNum == 0 ? 1 : _tcpSktClrNum)
	, m_perClrUdpSubSktNum(_perClrUdpSubSktNum == 0 ? 1 : _perClrUdpSubSktNum)
	, m_udpSubSktClrNum(_udpSubSktClrNum == 0 ? 1 : _udpSubSktClrNum)
	, m_perClrUdpSktNum(_perClrUdpSktNum == 0 ? 1 : _perClrUdpSktNum)
	, m_udpSktClrNum(_udpSktClrNum == 0 ? 1 : _udpSktClrNum)

	, m_usingTcp(m_tcpAcptrSvcNum > 0 && m_tcpSvcNum > 0)
	, m_usingUdp(m_udpAcptrSvcNum > 0 && m_udpSvcNum > 0)
	, m_usingSSL(m_sslAcptrSvcNum > 0 && m_sslSvcNum > 0)

	, m_contexts(_contextBase)
	, m_netInfoMgr((m_tcpSvcNum+m_udpSvcNum+_contextBase)/3+1)

	, m_tcpAcptrSvcs(m_tcpAcptrSvcNum, NumChooser(0, (m_tcpAcptrSvcNum == 0 ? 1 : m_tcpAcptrSvcNum) - 1, 24249, false))
	, m_sslAcptrSvcs(m_sslAcptrSvcNum, NumChooser(0, (m_sslAcptrSvcNum == 0 ? 1 : m_sslAcptrSvcNum) - 1, 21139, false))
	, m_udpAcptrSvcs(m_udpAcptrSvcNum, NumChooser(0, (m_udpAcptrSvcNum == 0 ? 1 : m_udpAcptrSvcNum) - 1, 23449, false))
	, m_tcpSvcs(m_tcpSvcNum, NumChooser(0, (m_tcpSvcNum == 0 ? 1 : m_tcpSvcNum) - 1, 14249, false))
	, m_sslSvcs(m_sslSvcNum, NumChooser(0, (m_sslSvcNum == 0 ? 1 : m_sslSvcNum) - 1, 1222, false))
	, m_udpSvcs(m_udpSvcNum, NumChooser(0, (m_udpSvcNum == 0 ? 1 : m_udpSvcNum) - 1, 54249, false))
	, m_isIp4(_isIp4)
	, m_currMaxSnIndex(0)
{
	if (_pktParser)
	{
		m_pktParser = _pktParser;
	}
	else
	{
		m_pktParser = IPacketParserShrPtr(new PacketParser);
	}

	m_contexts.setHash();

	for (uint32 i=0; i<m_tcpAcptrSvcs.size(); ++i)
	{
		m_tcpAcptrSvcs[i] = new TcpAcceptorService(this, m_tcpAcptrThreadNum, m_isIp4);
	}
	for (uint32 i=0; i<m_sslAcptrSvcs.size(); ++i)
	{
		m_sslAcptrSvcs[i] = new SSLAcceptorService(this, m_sslAcptrThreadNum, m_isIp4);
	}
	for (uint32 i=0; i<m_udpAcptrSvcs.size(); ++i)
	{
		m_udpAcptrSvcs[i] = new UdpAcceptorService(this, m_udpAcptrThreadNum, m_isIp4);
	}
	for (uint32 i=0; i<m_tcpSvcs.size(); ++i)
	{
		m_tcpSvcs[i] = new TcpService(this, m_isIp4);
	}
	for (uint32 i=0; i<m_sslSvcs.size(); ++i)
	{
		m_sslSvcs[i] = new SSLService(this, m_isIp4);
	}
	for (uint32 i=0; i<m_udpSvcs.size(); ++i)
	{
		m_udpSvcs[i] = new UdpService(this, m_isIp4);
	}

	// create and init tcp socket free list
	IFreeListShrPtr tcpSktFl(new FreeList<TcpSocket>(m_tcpSktClrNum, m_perClrTcpSktNum));
	uint32 maxTcpNum = m_tcpSktClrNum*m_perClrTcpSktNum;
	for (uint32 ix=0, i=0; (i<maxTcpNum && m_usingTcp); ++i)
	{
		ix = (m_currMaxSnIndex+i) % m_tcpSvcs.size();
		TcpSocket* tcpSkt = (TcpSocket*)tcpSktFl->get(i);
		if (tcpSkt)
			tcpSkt->init(ISessionShrPtr(new Session(m_currMaxSnIndex+i, &m_netInfoMgr, m_currMaxSnIndex+i, 
													m_pktParser->create())), m_tcpSvcs[ix], this);
	}
	sTcpSktFList.set(tcpSktFl);
	if (m_usingTcp)
	{
		m_currMaxSnIndex += maxTcpNum;
	}

	// create and init udp sub socket free list
	IFreeListShrPtr udpSubSktFl(new FreeList<UdpSubSocket>(m_udpSubSktClrNum, m_perClrUdpSubSktNum));
	uint32 maxUdpSubNum = m_udpSubSktClrNum*m_perClrUdpSubSktNum;
	for (uint32 ix=0, i=0; (i<maxUdpSubNum && m_usingUdp); ++i)
	{
		ix = (m_currMaxSnIndex+i) % m_udpSvcs.size();
		UdpSubSocket* udpSubSkt = (UdpSubSocket*)udpSubSktFl->get(i);
		if (udpSubSkt)
			udpSubSkt->init(ISessionShrPtr(new Session(m_currMaxSnIndex+i, &m_netInfoMgr, m_currMaxSnIndex+i, 
													   m_pktParser->create())), m_udpSvcs[ix], this, m_isIp4);
	}
	sUdpSubSktFList.set(udpSubSktFl);
	if (m_usingUdp)
	{
		m_currMaxSnIndex += maxUdpSubNum;
	}

	// create and init udp socket free list
	IFreeListShrPtr udpSktFl(new FreeList<UdpSocket>(m_udpSktClrNum, m_perClrUdpSktNum));
	uint32 maxUdpNum = m_udpSktClrNum*m_perClrUdpSktNum;
	for (uint32 ix=0, i=0; (i<maxUdpNum && m_usingUdp); ++ix, ++i)
	{
		if (ix >= m_udpSvcs.size())
			ix = 0;
		UdpSocket* udpSkt = (UdpSocket*)udpSktFl->get(i);
		if (udpSkt)
			udpSkt->init(m_udpSvcs[ix], 0, m_isIp4);
	}
	sUdpSktFList.set(udpSktFl);

	if (_packetFreeList)
	{
		sPkFList.set(_packetFreeList);
	}
	else
	{
		sPkFList.set(IFreeListShrPtr(new FreeList<NetPacket>(10, 100)));
	}
}

ServiceManager::~ServiceManager()
{
	uint32 maxTcpNum = m_tcpSktClrNum*m_perClrTcpSktNum;
	for (uint32 i=0; (i<maxTcpNum && m_usingTcp); ++i)
	{
		TcpSocket* tcpSkt = (TcpSocket*)sTcpSktFList.get()->get(i);
		if (tcpSkt)
		{
			tcpSkt->close();
			tcpSkt->clean();
		}
	}

	HashMapShrPtr hm = sSSLSktFList.getFls();
	if (hm)
	{
		uint32 size = hm->arraySize();
		for (uint32 i=0; i<size; ++i)
		{
			hm->lockWrite((ContextId)i, (ContextId)i);
			std::map<ContextId, IFreeListShrPtr>& hmMap = hm->map(i);
			for (std::map<ContextId, IFreeListShrPtr>::iterator itr=hmMap.begin(); itr!=hmMap.end(); ++itr)
			{
				uint32 maxSSLSktNum = itr->second->getClr() * itr->second->getPer();
				for (uint32 j=0; (j<maxSSLSktNum && m_usingSSL); ++j)
				{
					SSLSocket* sslSkt = (SSLSocket*)itr->second->get(j);
					if (sslSkt)
					{
						sslSkt->socket()->lowest_layer().close();
						sslSkt->clean();
					}
				}
			}
			hm->unlockWrite((ContextId)i, (ContextId)i);
		}
	}
	hm = HashMapShrPtr();

	uint32 maxUdpSubNum = m_udpSubSktClrNum*m_perClrUdpSubSktNum;
	for (uint32 i=0; (i<maxUdpSubNum && m_usingUdp); ++i)
	{
		UdpSubSocket* udpSubSkt = (UdpSubSocket*)sUdpSubSktFList.get()->get(i);
		if (udpSubSkt)
		{
			udpSubSkt->close();
			udpSubSkt->clean();
		}
	}
	uint32 maxUdpNum = m_udpSktClrNum*m_perClrUdpSktNum;
	for (uint32 i=0; (i<maxUdpNum && m_usingUdp); ++i)
	{
		UdpSocket* udpSkt = (UdpSocket*)sUdpSktFList.get()->get(i);
		if (udpSkt)
		{
			udpSkt->socket()->close();
			udpSkt->clean();
		}
	}

	m_contexts.clear();

	for (uint32 i=0; i<m_tcpAcptrSvcs.size(); ++i)
	{
		if (m_tcpAcptrSvcs[i])
			delete m_tcpAcptrSvcs[i];
	}
	for (uint32 i=0; i<m_sslAcptrSvcs.size(); ++i)
	{
		if (m_sslAcptrSvcs[i])
			delete m_sslAcptrSvcs[i];
	}
	for (uint32 i=0; i<m_udpAcptrSvcs.size(); ++i)
	{
		if (m_udpAcptrSvcs[i])
			delete m_udpAcptrSvcs[i];
	}
	for (uint32 i=0; i<m_tcpSvcs.size(); ++i)
	{
		if (m_tcpSvcs[i])
			delete m_tcpSvcs[i];
	}
	for (uint32 i=0; i<m_sslSvcs.size(); ++i)
	{
		if (m_sslSvcs[i])
			delete m_sslSvcs[i];
	}
	for (uint32 i=0; i<m_udpSvcs.size(); ++i)
	{
		if (m_udpSvcs[i])
			delete m_udpSvcs[i];
	}
	
}

void ServiceManager::run()
{
	for (uint32 i=0; i<m_tcpAcptrSvcs.size(); ++i)
	{
		if (m_tcpAcptrSvcs[i])
			m_tcpAcptrSvcs[i]->boost();
	}
	for (uint32 i=0; i<m_sslAcptrSvcs.size(); ++i)
	{
		if (m_sslAcptrSvcs[i])
			m_sslAcptrSvcs[i]->boost();
	}
	for (uint32 i=0; i<m_udpAcptrSvcs.size(); ++i)
	{
		if (m_udpAcptrSvcs[i])
			m_udpAcptrSvcs[i]->boost();
	}
	for (uint32 i=0; i<m_tcpSvcs.size(); ++i)
	{
		if (m_tcpSvcs[i])
			m_tcpSvcs[i]->boost();
	}
	for (uint32 i=0; i<m_sslSvcs.size(); ++i)
	{
		if (m_sslSvcs[i])
			m_sslSvcs[i]->boost();
	}
	for (uint32 i=0; i<m_udpSvcs.size(); ++i)
	{
		if (m_udpSvcs[i])
			m_udpSvcs[i]->boost();
	}


	for (uint32 i=0; i<m_udpSvcs.size(); ++i)
	{
		if (m_udpSvcs[i])
			m_udpSvcs[i]->join();
	}
	for (uint32 i=0; i<m_tcpSvcs.size(); ++i)
	{
		if (m_tcpSvcs[i])
			m_tcpSvcs[i]->join();
	}
	for (uint32 i=0; i<m_sslSvcs.size(); ++i)
	{
		if (m_sslSvcs[i])
			m_sslSvcs[i]->join();
	}
	for (uint32 i=0; i<m_udpAcptrSvcs.size(); ++i)
	{
		if (m_udpAcptrSvcs[i])
			m_udpAcptrSvcs[i]->join();
	}
	for (uint32 i=0; i<m_sslAcptrSvcs.size(); ++i)
	{
		if (m_sslAcptrSvcs[i])
			m_sslAcptrSvcs[i]->join();
	}
	for (uint32 i=0; i<m_tcpAcptrSvcs.size(); ++i)
	{
		if (m_tcpAcptrSvcs[i])
			m_tcpAcptrSvcs[i]->join();
	}
}

void ServiceManager::shutdown()
{
	for (uint32 i=0; i<m_udpSvcs.size(); ++i)
	{
		if (m_udpSvcs[i])
			m_udpSvcs[i]->shutdown();
	}
	for (uint32 i=0; i<m_sslSvcs.size(); ++i)
	{
		if (m_sslSvcs[i])
			m_sslSvcs[i]->shutdown();
	}
	for (uint32 i=0; i<m_tcpSvcs.size(); ++i)
	{
		if (m_tcpSvcs[i])
			m_tcpSvcs[i]->shutdown();
	}
	for (uint32 i=0; i<m_udpAcptrSvcs.size(); ++i)
	{
		if (m_udpAcptrSvcs[i])
			m_udpAcptrSvcs[i]->shutdown();
	}
	for (uint32 i=0; i<m_sslAcptrSvcs.size(); ++i)
	{
		if (m_sslAcptrSvcs[i])
			m_sslAcptrSvcs[i]->shutdown();
	}
	for (uint32 i=0; i<m_tcpAcptrSvcs.size(); ++i)
	{
		if (m_tcpAcptrSvcs[i])
			m_tcpAcptrSvcs[i]->shutdown();
	}
}

SSLContextShrPtr ServiceManager::createContext(boost::asio::ssl::context_base::method _m, const char* _passwd)
{
	if (!m_usingSSL)
		return SSLContextShrPtr();

	SSLAcceptorService* sslAcptrSvc = m_sslAcptrSvcs.select();
	if (!sslAcptrSvc)
		return SSLContextShrPtr();

	return SSLContextShrPtr(new SSLContext(sslAcptrSvc->ioEngine().ioService(), _m, _passwd));
}

ContextId ServiceManager::registerContext(SSLContextShrPtr _context, uint32 _sktClrNum, uint32 _perClrNum)
{
	if (!m_usingSSL)
		return 0;

	if (!_context)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!_context", 0);
		return 0;
	}

	_sktClrNum = _sktClrNum == 0 ? 1 : _sktClrNum;
	_perClrNum = _perClrNum == 0 ? 1 : _perClrNum;

	DynShrLock lock(m_contexts.mutex((ContextId)_context.get(), (ContextId)_context.get()));
	if (m_contexts.add((ContextId)_context.get(), _context, (ContextId)_context.get(), false) != 0)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "m_contexts.add != 0", 0);
		return 0;
	}
	else
	{
		// create sslSocket freelist
		IFreeListShrPtr sslSktFl(new FreeList<SSLSocket>(_sktClrNum, _perClrNum));
		sSSLSktFList.init(_sktClrNum);
		uint32 maxSSLNum = _sktClrNum*_perClrNum;
		for (uint32 ix=0, i=0; i<maxSSLNum; ++i)
		{
			ix = (m_currMaxSnIndex+i) % m_sslSvcs.size();
			SSLSocket* sslSkt = (SSLSocket*)sslSktFl->get(i);
			if (sslSkt)
				sslSkt->init(ISessionShrPtr(new Session(m_currMaxSnIndex+i, &m_netInfoMgr, m_currMaxSnIndex+i+(uint32)_context.get(), 
														m_pktParser->create())), m_sslSvcs[ix], _context->getContext(), (ContextId)_context.get(), this);
		}
		m_currMaxSnIndex += maxSSLNum;
		if (sSSLSktFList.add((ContextId)_context.get(), sslSktFl) != 0)
		{
			sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "sSSLSktFList.add != 0", 0);
			return 0;
		}
		else
			return (ContextId)_context.get();
	}
}

int32 ServiceManager::listen(NetPort _port, IConnection* _connection, uint32 _syn, uint32 _delayTime)
{
	if (!m_usingTcp)
		return 1;

	TcpAcceptorService* tcpAcptrSvc = m_tcpAcptrSvcs.select();
	if (!tcpAcptrSvc || !_connection || _port == 0)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!tcpAcptrSvc || !_connection || _port == 0", 0);
		return 1;
	}

	return tcpAcptrSvc->accept(tcpAcptrSvc->createAcceptor(_port), _connection, _syn, _delayTime);
}

int32 ServiceManager::connect(IpString _ip, NetPort _port, IConnection* _connection, void* _tag, uint32 _delayTime)
{
	if (!m_usingTcp)
		return 1;

	TcpService* tcpSvc = m_tcpSvcs.select();
	if (!tcpSvc || !_connection || _port == 0)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!tcpSvc || !_connection || _port == 0", 0);
		return 1;
	}

	tcpSvc->connect(_ip, _port, _connection, _tag, _delayTime);
	return 0;
}

int32 ServiceManager::sslListen(NetPort _port, IConnection* _connection, ContextId _contextId, uint32 _syn, uint32 _delayTime)
{
	if (!m_usingSSL)
		return 1;

	SSLAcceptorService* sslAcptrSvc = m_sslAcptrSvcs.select();
	if (!sslAcptrSvc || !_connection || _port == 0)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!sslAcptrSvc || !_connection || _port == 0", 0);
		return 1;
	}

	return sslAcptrSvc->accept(sslAcptrSvc->createAcceptor(_port), _connection, _contextId, _syn, _delayTime);
}

int32 ServiceManager::sslConnect(IpString _ip, NetPort _port, IConnection* _connection, ContextId _contextId, void* _tag, uint32 _delayTime)
{
	if (!m_usingSSL)
		return 1;

	SSLService* sslSvc = m_sslSvcs.select();
	if (!sslSvc || !_connection || _port == 0)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!sslSvc || !_connection || _port == 0", 0);
		return 1;
	}

	sslSvc->connect(_ip, _port, _connection, _contextId, _tag, _delayTime);
	return 0;
}

void ServiceManager::send(const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick)
{
	ISession* sn = _snPxy.getSession();
	if (!sn)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!sn", 0);
		return;
	}

	ProtocolService* pSvc = sn->getProtocolService();
	ISocket* skt = sn->getSocket();

	if (!pSvc || !skt)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!pSvc || !skt", 0);
		return;
	}

	pSvc->send(skt, _snPxy, _pk, _isKick);
}

void ServiceManager::kick(const SessionPxy& _snPxy, bool _hasHdr)
{
	ISession* sn = _snPxy.getSession();
	if (!sn)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!sn", 0);
		return;
	}

	ProtocolService* pSvc = sn->getProtocolService();
	ISocket* skt = sn->getSocket();

	if (!pSvc || !skt)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!pSvc || !skt", 0);
		return;
	}

	pSvc->halt(skt, _snPxy, _hasHdr);
}

int32 ServiceManager::recvfrom(NetPort _port, IConnection* _connection, uint32 _lifecycle, uint32 _syn, uint32 _delayTime)
{
	if (!m_usingUdp)
		return 1;

	UdpAcceptorService* udpAcptrSvc = m_udpAcptrSvcs.select();
	if (!udpAcptrSvc)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!sn", 0);
		return 1;
	}

	udpAcptrSvc->accept(udpAcptrSvc->createAcceptor(_port, _lifecycle), _connection, _syn, _delayTime);
	return 0;
}

int32 ServiceManager::sendto(IpString _ip, NetPort _port, IPacketShrPtr _pk, IConnection* _connection, void* _tag, uint32 _lifecycle)
{
	if (!m_usingUdp)
		return 1;

	UdpSocketShrPtr udpSkt = allocUdpSkt();
	if (!udpSkt)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!udpSkt", 0);
		return 1;
	}

	UdpService* udpSvc = udpSkt->getUdpSvc();
	if (!udpSvc)
	{
		sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "!udpSvc", 0);
		return 1;
	}

	udpSvc->sendto(udpSkt, _port, _ip, _pk, _connection, _tag, _lifecycle);
	return 0;
}
