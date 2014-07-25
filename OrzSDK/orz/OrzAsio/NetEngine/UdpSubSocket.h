#ifndef __Orz_OrzAsio_UdpSubSocket_h__
#define __Orz_OrzAsio_UdpSubSocket_h__
#include <orz/OrzAsio/NetEngine/Socket.h>
#include <orz/Toolkit_Plus/Toolkit/StringPtr.hpp>
#include <orz/OrzAsio/NetEngine/ISocket.h>
#include <orz/OrzAsio/NetEngine/UdpSdrEndpoint.h>
namespace Orz
{
class _OrzOrzAsioExport UdpSubSocket
	: public ISocket
	, public boost::enable_shared_from_this<UdpSubSocket>
	, private boost::noncopyable
{
	friend class UdpAcceptorService;

public:
	enum Type
	{
		TYPE_RECV = 0,
		TYPE_SEND,
		TYPE_TOTAL
	};

	UdpSubSocket();
	virtual ~UdpSubSocket();

	bool				init					(ISessionShrPtr _sn, UdpService* _io, IServiceManager* _svcMgr, bool _isIp4);
	void				clean					();
	ISessionShrPtr		session					();
	UdpService*			getUdpSvc				();
	bool				hasInited				();

public:
	int32				send					(const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick);
	int32				halt					(const SessionPxy& _snPxy, bool _hasHdr);
	int32				type					() const;
	void				close					();
	bool				isClosed				() const;

public:
	int32				handleAccept			(UdpAcceptorService* _udpAcptrSvc, UdpAcceptorShrPtr _acptr, IConnection* _connection, 
												 uint32 _lifecycle, const boost::system::error_code& _error);
	int32				handleConnect			(UdpSocketShrPtr _udpSocket, NetPort _port, IpString _ip, 
												 const IPacketShrPtr _pk, IConnection* _connection, void* _tag, uint32 _lifecycle);
	void				handleRecv				(const SessionPxy& _snPxy, RecvBuffer& _buffer, const boost::system::error_code& _error);
	void				handleSend				(UdpSubSocket::Type _type, const SessionPxy& _snPxy, const IPacketShrPtr _pk, 
												 bool _isKick, const boost::system::error_code& _error, std::size_t _bytes_transferred);

private:
	void				handleSendto			(IPacketShrPtr _pk, bool _isKick, const boost::system::error_code& _error, std::size_t _bytes_transferred);
	void				handleSendtoBack		(const boost::system::error_code& _error, std::size_t _bytes_transferred);
	void				checkSessionLife		(SessionPxy _snPxy, TimerArg _arg);
	void				addTimer				(uint32 _msec);

private:
	UdpService*									m_io;
	IServiceManager*							m_svcMgr;

	UdpAcceptorService*							m_udpAcptrSvc;
	UdpAcceptorShrPtr							m_acptr;
	UdpSocketShrPtr								m_udpSkt;
	RecvBuffer									m_recvBuffer;
	UdpSdrEndpoint								m_senderEp;
	ISessionShrPtr								m_session;
	
	SocketType									m_sktType;
	Type										m_type;
	uint32										m_lifecycle;
	NetPort										m_port;
	
	bool										m_isClosed;
	bool										m_hasNetAction;
	boost::asio::deadline_timer*				m_timer;
	bool										m_hasInited;
};
}
#endif
