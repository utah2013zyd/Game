#include <EchoConnection.h>
#include <orz/OrzAsio/AsioSingleton.h>
#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <PacketBuilder.h>
#include <Packet.h>
#include <orz/OrzAsio/NetEngine/SessionDeputy.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
#include <EchoClient.h>
#include <EchoClientFreeList.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
using namespace Orz;

EchoConnection::EchoConnection(uint32 _maxInc)
	: IConnection(_maxInc)
{
	uint32 clrNum = 1;
	uint32 perNum = 1;
	if (_maxInc < 10)
	{
		clrNum = _maxInc / 2;
		perNum = _maxInc;
	}
	else if (_maxInc >= 10 && _maxInc < 100)
	{
		clrNum = _maxInc / 5;
		perNum = _maxInc + _maxInc/2;
	}
	else if (_maxInc >= 100 && _maxInc < 1000)
	{
		clrNum = _maxInc / 10;
		perNum = _maxInc / 20;
	}
	else if (_maxInc >= 1000 && _maxInc < 2000)
	{
		clrNum = _maxInc / 10;
		perNum = _maxInc / 40;
	}
	else if (_maxInc >= 2000)
	{
		clrNum = (uint32)sqrt((float64)_maxInc);
		perNum = (uint32)sqrt((float64)_maxInc);
	}

	sEClFList.set(IFreeListShrPtr(new FreeList<EchoClient>(clrNum, perNum)));
	sLogger->out(OL_INFO, " EchoConnection create success!", 0); 
}

EchoConnection::~EchoConnection()
{
	sLogger->out(OL_INFO, " EchoConnection delete success!", 0);
}

void EchoConnection::OnConnected(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc)
{
	sLogger->out(OL_INFO, " New session - snid: ", d2s(_snPxy.getSnid()).c_str(), _isInc ? "in":"out", 0);
	m_connectionNum.inc();
	uint32 cn = m_connectionNum.get();
	if (cn <= 5 || cn == 200 || cn == 400 || cn == 700 || cn == 1000 || cn == 1500 || cn == 2000 || cn == 2500 || cn == 3000 || cn ==3400)
	{
		printf("[+]m_connectionNum: %d!\n", cn);
	}

	EchoClientShrPtr ecl = allocEchoClient();
	ecl->clean();
	ecl->init(sSvcMgr(ServiceManager), _snPxy);
	ecl->setup(1);
	_snDy.setClient(ecl);
}

void EchoConnection::OnReceived(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk)
{
	sLogger->out(OL_INFO, " Session - snid: ", d2s(_snPxy.getSnid()).c_str(), " - recv data!", 0);
	IClientShrPtr cl = _snDy.getClient();
	if (!cl)
		return;
	cl->processPacket(_pk);
}

void EchoConnection::OnSended(const SessionPxy& _snPxy, SessionDeputy& _snDy)
{
	sLogger->out(OL_INFO, " Session - snid: ", d2s(_snPxy.getSnid()).c_str(), " - send data!", 0);
}

void EchoConnection::OnDisconnected(const SessionPxy& _snPxy, SessionDeputy& _snDy)
{
	sLogger->out(OL_INFO, " Session disconnected - snid: ", d2s(_snPxy.getSnid()).c_str(), 0);
	_snDy.getClient()->logoff();
	_snDy.clearClient();
	m_connectionNum.dec();
	uint32 cn = m_connectionNum.get();
	if (cn <= 5 || cn == 200 || cn == 400 || cn == 700 || cn == 1000 || cn == 1500 || cn == 2000 || cn == 2500 || cn == 3000 || cn ==3400)
	{
		printf("[-]m_connectionNum: %d!\n", cn);
	}
}
