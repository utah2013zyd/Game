#ifndef __Orz_OrzAsio_ServiceManager_h__
#define __Orz_OrzAsio_ServiceManager_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
#include <orz/Toolkit_Plus/Toolkit/Agency.hpp>
#include <orz/Toolkit_Plus/Toolkit/HashMap.hpp>
#include <orz/OrzAsio/NetEngine/IServiceManager.h>
#include <orz/OrzAsio/NetEngine/NetInfoManager.h>
namespace Orz
{
//! 服务管理者
/*!
 *	\note 线程安全
 */
class _OrzOrzAsioExport ServiceManager
	: public IServiceManager
	, private boost::noncopyable
{
public:
	//! 构造函数
	/*!
	 *	\param[in] _tcpAcptrSvcNum TCP接收者的数量
	 *	\param[in] _udpAcptrSvcNum UDP接收者的数量
	 *	\param[in] _sslAcptrSvcNum SSL接收者的数量
	 *	\param[in] _tcpSvcNum TCP协议服务对象的数量
	 *	\param[in] _udpSvcNum UDP协议服务对象数量
	 *	\param[in] _sslSvcNum SSL安全连接服务对象数量
	 *	\param[in] _tcpAcptrThreadNum TCP接收者的内部线程数量
	 *	\param[in] _udpAcptrThreadNum UDP接收者的内部线程数量
	 *	\param[in] _sslAcptrThreadNum SSL接收者的内部线程数量
	 *	\param[in] _contextBase SSL安全连接的HashMap的hash数量
	 *	\param[in] _perClrTcpSktNum TCP socket对象池大小参数之一
	 *	\param[in] _tcpSktClrNum TCP socket对象池大小参数之二
	 *	\param[in] _perClrUdpSubSktNum UDP sub socket对象池大小参数之一
	 *	\param[in] _udpSubSktClrNum UDP sub socket对象池大小参数之二
	 *	\param[in] _perClrUdpSktNum UDP socket对象池大小参数之一
	 *	\param[in] _udpSktClrNum UDP socket对象池大小参数之二
	 *	\param[in] _pktParser 网络封包解析对象，用户如果想要自定义网络封包的实现，就把自己的一个网络封包对象传递到这里；默认值的话，系统会自动选择OrzAsio提供的一套实现
	 *	\param[in] _packetFreeList 用户自定义的Packet对象池对象，如果是默认值则系统使用OrzAsio提供的封包模块
	 *	\param[in] _isIp4 是否是IP4系统，如果不是则被判定为IP6系统
	 *	\note OrzAsio的工作方式是这样的：每个网络协议都有接收器+处理器合作处理，用TCP协议来讲，TCP接收器监听某个端口，然后accept以后，将这个Session送给处理器来处理。\n
	 *		接收器和处理器都是单独的线程，并且线程安全，对外使用线程安全得队列来传递数据，接收器内部可以使多线程的，而处理器内部则是单线程，但是，\n
	 *		每个协议在同时都可以有多个接收器和处理器，例如TCP接收器1监听端口8000和8001，TCP接收器2监听端口7500，TCP处理器1~10等待这两个接收器分配任务给它们处理。\n
	 *		用户可以根据上述前9个参数来设置每种协议的接收器和处理器的数量，以便适应自己系统的负载需求。
	 *	\note 上述前9个参数均可以为0，系统会根据是否为0来判断用户想使用何种协议或者一起使用。例如m_tcpAcptrSvcNum > 0 && m_tcpSvcNum > 0则表示用户要使用TCP协议，反之就是不使用
	 *	\note 对象池大小参数之一 * 对象池大小参数之二 == 这个对象池最大的可分配的对象数量
	 *	\note _contextBase是针对SSL安全连接的参数，其可以设定SSL连接需要的专用数据结构SSLContext的管理器的hash数量（一般设为10就足够了，主要根据系统负载来定）
	 *	\note 把各对象池的预分配对象的数量设的合适，尽量使用对象池来分配内存，避免内存碎片；\n
	 *		对象池使用预分配方式，查找则是使用内存地址的偏移量来找寻对象（所有的对象大小一样，内存又是连续的数组形式），\n
	 *		所以效率非常高，比默认的new和delete快了10倍以上
	 *	\note _pktParser + _packetFreeList 相配合可以让用户使用自己的网络封包模块
	 example:
	 \code
		ServiceManager svcMgr(1,0,0, 1,0,0, 2,0,0,0, 10,100,1,1,1,1);	// 只使用TCP协议，其接收器内部线程为2条，1个接收器和1个处理器，
																		// 同时tcp socket对象池大小为10x100==1000个
	 \endcode
	 */
	ServiceManager(uint32 _tcpAcptrSvcNum, uint32 _udpAcptrSvcNum, uint32 _sslAcptrSvcNum, 
				   uint32 _tcpSvcNum, uint32 _udpSvcNum, uint32 _sslSvcNum, 
				   uint32 _tcpAcptrThreadNum, uint32 _udpAcptrThreadNum, uint32 _sslAcptrThreadNum, uint32 _contextBase, 
				   uint32 _perClrTcpSktNum, uint32 _tcpSktClrNum, uint32 _perClrUdpSubSktNum, uint32 _udpSubSktClrNum, 
				   uint32 _perClrUdpSktNum, uint32 _udpSktClrNum, IFreeListShrPtr _packetFreeList=IFreeListShrPtr(), 
				   IPacketParserShrPtr _pktParser=IPacketParserShrPtr(), bool _isIp4=true);
	virtual ~ServiceManager();

public:
	//! 运行ServerManager
	/*!
	 *	\note 用户无需直接使用
	 */
	void				run						();

	//! 关闭ServerManager
	/*!
	 *	\note 用户无需直接使用
	 */
	void				shutdown				();

	//! 创建SSLContext对象 (SSL安全连接相关)
	/*!
	 *	\param[in] _m 要设置的ssl的模式，详见boost::asio文档
	 *	\param[in] _passwd 需要使用的ssl连接中的校验密码，这个密码是和将来要检验的密钥文件相吻合的
	 *	\return 创建好的SSLContext对象
	 */
	SSLContextShrPtr	createContext			(boost::asio::ssl::context_base::method _m, const char* _passwd="test");

	//! 注册一个SSLContext对象 (SSL安全连接相关)
	/*!
	 *	\note 如果需要有一些设置要做，则设置的过程必须在调用这个方法之前做
	 *	\param[in] _context SSLContext对象
	 *	\param[in] _sktClrNum 这个SSLContext专用的ssl socket对象池大小参数之一
	 *	\param[in] _perClrNum 这个SSLContext专用的ssl socket对象池大小参数之二
	 *	\return 0 的话失败，非0成功 
	 */
	ContextId			registerContext			(SSLContextShrPtr _context, uint32 _sktClrNum, uint32 _perClrNum);

	//! 监听指定的端口 (TCP相关)
	/*!
	 *	\param[in] _port 监听的端口(1~65535)，推荐使用7000以上的
	 *	\param[in] _connection 连接对象，需要用户继承自IConnection。见IConnection
	 *	\param[in] _syn 同时设置多少个accept处理在等待，一般设为1即可
	 *	\param[in] _delayTime 如果当前的listen请求因为某种原因无法立刻执行，则会通过计时器来循环等待，这个参数就是等待的时间间隔，单位秒，如果为负则不会等待
	 *	\return 0 成功 非0 失败
	 */
	int32				listen					(NetPort _port, IConnection* _connection, uint32 _syn=1, uint32 _delayTime=5);

	//! 连接到指定IP和端口的进程 (TCP相关)
	/*!
	 *	\param[in] _ip IP地址
	 *	\param[in] _port 对方的监听端口
	 *	\param[in] _connection 连接对象，需要用户继承自IConnection。见IConnection
	 *	\param[in] _tag 可以传递给_connection的参数，用户可以将需要传递的对象的指针放到这里，然后当连接建立后，可以通过SessionDeputy::getTag()来获取到
	 *	\param[in] _delayTime 如果当前的listen请求因为某种原因无法立刻执行，则会通过计时器来循环等待，这个参数就是等待的时间间隔，单位秒，如果为负则不会等待
	 *	\return 0 成功 非0 失败
	 */
	int32				connect					(IpString _ip, NetPort _port, IConnection* _connection, void* _tag=0, uint32 _delayTime=5);

	//! SSL安全连接的监听方法 (SSL安全连接相关)
	/*!
	 *	\param[in] _port 监听的端口(1~65535)，推荐使用7000以上的
	 *	\param[in] _connection 连接对象，需要用户继承自IConnection。见IConnection
	 *	\param[in] _contextId 注册的SSLContext对象的Id
	 *	\param[in] _syn 同时设置多少个accept处理在等待，一般设为1即可
	 *	\param[in] _delayTime 如果当前的listen请求因为某种原因无法立刻执行，则会通过计时器来循环等待，这个参数就是等待的时间间隔，单位秒，如果为负则不会等待
	 *	\return 0 成功 非0 失败
	 */
	int32				sslListen				(NetPort _port, IConnection* _connection, ContextId _contextId, uint32 _syn=1, uint32 _delayTime=5);

	//! 连接到指定IP和端口的进程 (SSL安全连接相关)
	/*!
	 *	\param[in] _ip IP地址
	 *	\param[in] _port 对方的监听端口
	 *	\param[in] _connection 连接对象，需要用户继承自IConnection。见IConnection
	 *	\param[in] _contextId 注册的SSLContext对象的Id
	 *	\param[in] _tag 可以传递给_connection的参数，用户可以将需要传递的对象的指针放到这里，然后当连接建立后，可以通过SessionDeputy::getTag()来获取到
	 *	\param[in] _delayTime 如果当前的listen请求因为某种原因无法立刻执行，则会通过计时器来循环等待，这个参数就是等待的时间间隔，单位秒，如果为负则不会等待
	 *	\return 0 成功 非0 失败 
	 */
	int32				sslConnect				(IpString _ip, NetPort _port, IConnection* _connection, ContextId _contextId, void* _tag=0, uint32 _delayTime=5);

	//! 发送网络封包 (TCP/UDP/SSL)
	/*!
	 *	\param[in] _snPxy Session的Id
	 *	\param[in] _pk 要发送的网络封包，见IPacket
	 *	\param[in] _isKick 是否在发送完毕后中断这个连接
	 */
	void				send					(const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick=false);

	//! 中断连接 (TCP/UDP/SSL)
	/*!
	 *	\param[in] _snPxy Session的Id
	 *	\param[in] _hasHdr 是否在中断后让系统调用IConnection::OnDisconnected方法
	 */
	void				kick					(const SessionPxy& _snPxy, bool _hasHdr=false);

	//! 接受来自外面的数据 (UDP相关)
	/*!
	 *	\param[in] _port 准备recvfrom的端口
	 *	\param[in] _connection 连接对象，需要用户继承自IConnection。见IConnection
	 *	\param[in] _lifecycle 这个udp连接要持续多长时间然后中断，单位秒
	 *	\param[in] _syn 同时设置多少个recvfrom处理在等待，一般设为1即可
	 *	\param[in] _delayTime 如果当前的listen请求因为某种原因无法立刻执行，则会通过计时器来循环等待，这个参数就是等待的时间间隔，单位秒，如果为负则不会等待
	 *	\return 0 成功 非0 失败 
	 *	\note 调用这个方法会在系统内部建立一个udp的session连接，这个连接是按照时间来控制生命周期的，时间到了，session就会自动终止，连接断开，但远端是不会得到断开的消息的
	 */
	int32				recvfrom				(NetPort _port, IConnection* _connection, uint32 _lifecycle=30, uint32 _syn=1, uint32 _delayTime=5);

	//! 发送数据到指定ip和端口的进程 (UDP相关)
	/*!
	 *	\param[in] _ip IP地址
	 *	\param[in] _port 对方recvfrom的端口
	 *	\param[in] _pk 要发送的网络封包，见IPacket
	 *	\param[in] _lifecycle 这个udp连接要持续多长时间然后中断，单位秒
	 *	\param[in] _tag 可以传递给_connection的参数，用户可以将需要传递的对象的指针放到这里，然后当连接建立后，可以通过SessionDeputy::getTag()来获取到
	 *	\param[in] _delayTime 如果当前的listen请求因为某种原因无法立刻执行，则会通过计时器来循环等待，这个参数就是等待的时间间隔，单位秒，如果为负则不会等待
	 *	\return 0 成功 非0 失败 
	 *	\note 调用这个方法会在系统内部建立一个udp的session连接，这个连接是按照时间来控制生命周期的，时间到了，session就会自动终止，连接断开，但远端是不会得到断开的消息的
	 */
	int32				sendto					(IpString _ip, NetPort _port, IPacketShrPtr _pk, IConnection* _connection, void* _tag=0, uint32 _lifecycle=30);

private:
	uint32										m_tcpAcptrSvcNum;
	uint32										m_udpAcptrSvcNum;
	uint32										m_sslAcptrSvcNum;
	uint32										m_tcpSvcNum;
	uint32										m_udpSvcNum;
	uint32										m_sslSvcNum;
	uint32										m_tcpAcptrThreadNum;
	uint32										m_udpAcptrThreadNum;
	uint32										m_sslAcptrThreadNum;
	uint32										m_perClrTcpSktNum;
	uint32										m_tcpSktClrNum;
	uint32										m_perClrUdpSubSktNum;
	uint32										m_udpSubSktClrNum;
	uint32										m_perClrUdpSktNum;
	uint32										m_udpSktClrNum;

	bool										m_usingTcp;
	bool										m_usingUdp;
	bool										m_usingSSL;

	HashShrMap<ContextId, SSLContextShrPtr, ContextId>	m_contexts;
	NetInfoManager								m_netInfoMgr;
	IPacketParserShrPtr							m_pktParser;

	Agency<TcpAcceptorService*>					m_tcpAcptrSvcs;
	Agency<SSLAcceptorService*>					m_sslAcptrSvcs;
	Agency<UdpAcceptorService*>					m_udpAcptrSvcs;
	Agency<TcpService*>							m_tcpSvcs;
	Agency<SSLService*>							m_sslSvcs;
	Agency<UdpService*>							m_udpSvcs;
	bool										m_isIp4;
	uint32										m_currMaxSnIndex;
};
}
#endif
