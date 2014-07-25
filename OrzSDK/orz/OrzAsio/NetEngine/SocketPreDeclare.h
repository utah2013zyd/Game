#ifndef __Orz_OrzAsio_SocketPreDeclare_H_
#define __Orz_OrzAsio_SocketPreDeclare_H_
#include <orz/OrzAsio/OrzAsioPreDeclare.h>
namespace Orz
{
// socket
	class _OrzOrzAsioExport TcpSocket;
	typedef boost::shared_ptr<TcpSocket> TcpSocketShrPtr;

	class _OrzOrzAsioExport UdpAcceptor;
	typedef boost::shared_ptr<UdpAcceptor> UdpAcceptorShrPtr;

	class _OrzOrzAsioExport UdpSubSocket;
	typedef boost::shared_ptr<UdpSubSocket> UdpSubSocketShrPtr;

	class _OrzOrzAsioExport UdpSocket;
	typedef boost::shared_ptr<UdpSocket> UdpSocketShrPtr;

	typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;
	class _OrzOrzAsioExport SSLSocket;
	typedef boost::shared_ptr<SSLSocket> SSLSocketShrPtr;

	class SSLContext;
	typedef boost::shared_ptr<SSLContext> SSLContextShrPtr;

	class _OrzOrzAsioExport ISocket;
	//template class _OrzOrzAsioExport boost::shared_ptr<ISocket>;
	typedef boost::shared_ptr<ISocket> ISocketShrPtr;

	#define		RECV_BUFFER_SIZE		256000
	#define		SEND_BUFFER_SIZE		256000
	#define		RECV_ONCE_SIZE			512

	enum SocketType
	{
		SKTP_NULL = 0,
		SKTP_TCP,
		SKTP_UDP,
		SKTP_SSL,
		SKTP_TOTAL
	};
}
#endif
