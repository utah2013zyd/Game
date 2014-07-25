#ifndef __Orz_OrzAsio_PreDeclare_H_
#define __Orz_OrzAsio_PreDeclare_H_
#include <orz/Toolkit_Plus/ToolkitPlusPreDeclare.h>

class IClient;
typedef boost::shared_ptr<IClient> IClientShrPtr;

namespace Orz
{	
// id def
	typedef uint16					NetPort;
	typedef uint32					ProcessorId;
	typedef uint32					SessionId;
	typedef uint32					ServiceId;
	typedef uint64					ValidCode;
	typedef PtrIntType				ContextId;

	#define MAX_PER_CLR_SN_NUM		100000
	#define MAX_SN_CLR_NUM			100

// Packet
	#define NET_PACKET_MAX_SIZE		1400	// less than default MTU
	template <uint32 SIZE> class Packet;
	typedef Packet<NET_PACKET_MAX_SIZE> NetPacket;

	#define SESSION_TAG_MAX_SIZE	32
	typedef Packet<SESSION_TAG_MAX_SIZE> SessionTag;

	#define TIMER_ARG_MAX_SIZE		32
	typedef Packet<SESSION_TAG_MAX_SIZE> TimerArg;

	struct DateSys;
	class IPacket;
	typedef boost::shared_ptr<IPacket> IPacketShrPtr;

	class IPacketParser;
	typedef boost::shared_ptr<IPacketParser> IPacketParserShrPtr;

// Net
	class AsioManager;
	class IOEngine;
	class TcpAcceptorService;
	class TcpService;
	class UdpAcceptorService;
	class UdpService;
	class SSLAcceptorService;
	class SSLService;
	class ProtocolService;
	class IOService;
	class IServiceManager;
	class ServiceManager;

	#define	IP_STR_SIZE		128
	template <uint32 SIZE> class MemString;
	typedef MemString<IP_STR_SIZE+1> IpString;

	class IConnection;
	class Session;
	class ISession;
	typedef boost::shared_ptr<ISession> ISessionShrPtr;

	class SessionPxy;
	struct NetData;
	class NetInfoManager;
	class NetInfoManagerSt;
	class SessionDeputy;

// boost
	typedef boost::shared_ptr<boost::asio::ip::tcp::acceptor> TcpAcceptorShrPtr;
	typedef boost::shared_ptr<boost::asio::deadline_timer> DlTimerShrPtr;
}

#endif
