#ifndef __Orz_OrzAsio_UdpAcceptor_h__
#define __Orz_OrzAsio_UdpAcceptor_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
#include <orz/Toolkit_Plus/Toolkit/HashMap.hpp>
#include <orz/Toolkit_Plus/Toolkit/DynShrLock.hpp>
#include <orz/OrzAsio/NetEngine/UdpSdrEndpoint.h>
namespace Orz
{
class _OrzOrzAsioExport UdpAcceptor
{
	struct HashArg
	{
		HashArg(const UdpSdrEndpoint& _senderEp) : port((uint32)_senderEp.m_senderEp.port()) {}
		HashArg(uint32 _port) : port(_port) {}

		Number operator%(uint32 _base) const
		{
			return (Number)(port % _base);
		}

		uint32 port;
	};
public:
	UdpAcceptor(IOEngine& _io, NetPort _port, uint32 _lifecycle, bool _isIp4=true);
	~UdpAcceptor();

public:
	boost::shared_mutex&			mutex				(const UdpSdrEndpoint& _senderEp);
	bool							add					(const UdpSdrEndpoint& _senderEp, UdpSubSocketShrPtr _udpSubSkt, bool _usingLock=false);
	UdpSubSocketShrPtr				get					(const UdpSdrEndpoint& _senderEp, bool _usingLock=false);
	boost::asio::ip::udp::socket*	getUdpSocket		();
	uint32							getLifecycle		();

private:
	HashShrMap<UdpSdrEndpoint, UdpSubSocketShrPtr, HashArg, UdpSdrEndpointLess>	m_udpSubSockets;
	boost::asio::ip::udp::socket*												m_socket;
	uint32																		m_lifecycle;
};
}
#endif
