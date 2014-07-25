#ifndef __Orz_OrzAsio_SSLSocket_h__
#define __Orz_OrzAsio_SSLSocket_h__
#include <orz/OrzAsio/NetEngine/Socket.h>
#include <orz/Toolkit_Plus/Toolkit/StringPtr.hpp>
#include <orz/OrzAsio/NetEngine/ISocket.h>
namespace Orz
{
class _OrzOrzAsioExport SSLSocket
	: public ISocket
	, public boost::enable_shared_from_this<SSLSocket>
	, private boost::noncopyable
{
public:
	SSLSocket();
	virtual ~SSLSocket();

	bool				init				(ISessionShrPtr _sn, SSLService* _io, boost::asio::ssl::context& _context, ContextId _cid, IServiceManager* _svcMgr);
	void				clean				();
	bool				hasInited			();
	ssl_socket*			socket				();
	ISessionShrPtr		session				();
	SSLService*			getSSLSvc			();
	ContextId			getContextId		() const;

public:
	int32				send				(const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick);
	int32				halt				(const SessionPxy& _snPxy, bool _hasHdr);
	int32				type				() const;
	void				close				();
	bool				isClosed			() const;

public:
	void				handleAccept		(NetPort _port, IConnection* _connection, const boost::system::error_code& _error);
	void				handleHandshakeIn	(NetPort _port, IConnection* _connection, const boost::system::error_code& _error);
	void				handleConnect		(IpString _ip, NetPort _port, IConnection* _connection, void* _tag, const boost::system::error_code& _error);
	void				handleHandshakeOut	(IpString _ip, NetPort _port, IConnection* _connection, void* _tag, const boost::system::error_code& _error);

private:
	int32				recv				();
	void				handleSend			(IPacketShrPtr _pk, bool _isKick, const boost::system::error_code& _error);
	void				handleRecv			(const boost::system::error_code& _error, std::size_t _bytes_transferred);

private:
	SSLService*								m_io;
	IServiceManager*						m_svcMgr;
	ssl_socket*								m_socket;
	ContextId								m_cid;
	RecvBuffer								m_recvBuffer;
	ISessionShrPtr							m_session;
	SocketType								m_type;
	bool									m_hasInited;
};
}
#endif
