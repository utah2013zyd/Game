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
//! ���������
/*!
 *	\note �̰߳�ȫ
 */
class _OrzOrzAsioExport ServiceManager
	: public IServiceManager
	, private boost::noncopyable
{
public:
	//! ���캯��
	/*!
	 *	\param[in] _tcpAcptrSvcNum TCP�����ߵ�����
	 *	\param[in] _udpAcptrSvcNum UDP�����ߵ�����
	 *	\param[in] _sslAcptrSvcNum SSL�����ߵ�����
	 *	\param[in] _tcpSvcNum TCPЭ�������������
	 *	\param[in] _udpSvcNum UDPЭ������������
	 *	\param[in] _sslSvcNum SSL��ȫ���ӷ����������
	 *	\param[in] _tcpAcptrThreadNum TCP�����ߵ��ڲ��߳�����
	 *	\param[in] _udpAcptrThreadNum UDP�����ߵ��ڲ��߳�����
	 *	\param[in] _sslAcptrThreadNum SSL�����ߵ��ڲ��߳�����
	 *	\param[in] _contextBase SSL��ȫ���ӵ�HashMap��hash����
	 *	\param[in] _perClrTcpSktNum TCP socket����ش�С����֮һ
	 *	\param[in] _tcpSktClrNum TCP socket����ش�С����֮��
	 *	\param[in] _perClrUdpSubSktNum UDP sub socket����ش�С����֮һ
	 *	\param[in] _udpSubSktClrNum UDP sub socket����ش�С����֮��
	 *	\param[in] _perClrUdpSktNum UDP socket����ش�С����֮һ
	 *	\param[in] _udpSktClrNum UDP socket����ش�С����֮��
	 *	\param[in] _pktParser ���������������û������Ҫ�Զ�����������ʵ�֣��Ͱ��Լ���һ�����������󴫵ݵ����Ĭ��ֵ�Ļ���ϵͳ���Զ�ѡ��OrzAsio�ṩ��һ��ʵ��
	 *	\param[in] _packetFreeList �û��Զ����Packet����ض��������Ĭ��ֵ��ϵͳʹ��OrzAsio�ṩ�ķ��ģ��
	 *	\param[in] _isIp4 �Ƿ���IP4ϵͳ������������ж�ΪIP6ϵͳ
	 *	\note OrzAsio�Ĺ�����ʽ�������ģ�ÿ������Э�鶼�н�����+����������������TCPЭ��������TCP����������ĳ���˿ڣ�Ȼ��accept�Ժ󣬽����Session�͸�������������\n
	 *		�������ʹ��������ǵ������̣߳������̰߳�ȫ������ʹ���̰߳�ȫ�ö������������ݣ��������ڲ�����ʹ���̵߳ģ����������ڲ����ǵ��̣߳����ǣ�\n
	 *		ÿ��Э����ͬʱ�������ж���������ʹ�����������TCP������1�����˿�8000��8001��TCP������2�����˿�7500��TCP������1~10�ȴ�������������������������Ǵ���\n
	 *		�û����Ը�������ǰ9������������ÿ��Э��Ľ������ʹ��������������Ա���Ӧ�Լ�ϵͳ�ĸ�������
	 *	\note ����ǰ9������������Ϊ0��ϵͳ������Ƿ�Ϊ0���ж��û���ʹ�ú���Э�����һ��ʹ�á�����m_tcpAcptrSvcNum > 0 && m_tcpSvcNum > 0���ʾ�û�Ҫʹ��TCPЭ�飬��֮���ǲ�ʹ��
	 *	\note ����ش�С����֮һ * ����ش�С����֮�� == �����������Ŀɷ���Ķ�������
	 *	\note _contextBase�����SSL��ȫ���ӵĲ�����������趨SSL������Ҫ��ר�����ݽṹSSLContext�Ĺ�������hash������һ����Ϊ10���㹻�ˣ���Ҫ����ϵͳ����������
	 *	\note �Ѹ�����ص�Ԥ��������������ĺ��ʣ�����ʹ�ö�����������ڴ棬�����ڴ���Ƭ��\n
	 *		�����ʹ��Ԥ���䷽ʽ����������ʹ���ڴ��ַ��ƫ��������Ѱ�������еĶ����Сһ�����ڴ�����������������ʽ����\n
	 *		����Ч�ʷǳ��ߣ���Ĭ�ϵ�new��delete����10������
	 *	\note _pktParser + _packetFreeList ����Ͽ������û�ʹ���Լ���������ģ��
	 example:
	 \code
		ServiceManager svcMgr(1,0,0, 1,0,0, 2,0,0,0, 10,100,1,1,1,1);	// ֻʹ��TCPЭ�飬��������ڲ��߳�Ϊ2����1����������1����������
																		// ͬʱtcp socket����ش�СΪ10x100==1000��
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
	//! ����ServerManager
	/*!
	 *	\note �û�����ֱ��ʹ��
	 */
	void				run						();

	//! �ر�ServerManager
	/*!
	 *	\note �û�����ֱ��ʹ��
	 */
	void				shutdown				();

	//! ����SSLContext���� (SSL��ȫ�������)
	/*!
	 *	\param[in] _m Ҫ���õ�ssl��ģʽ�����boost::asio�ĵ�
	 *	\param[in] _passwd ��Ҫʹ�õ�ssl�����е�У�����룬��������Ǻͽ���Ҫ�������Կ�ļ����Ǻϵ�
	 *	\return �����õ�SSLContext����
	 */
	SSLContextShrPtr	createContext			(boost::asio::ssl::context_base::method _m, const char* _passwd="test");

	//! ע��һ��SSLContext���� (SSL��ȫ�������)
	/*!
	 *	\note �����Ҫ��һЩ����Ҫ���������õĹ��̱����ڵ����������֮ǰ��
	 *	\param[in] _context SSLContext����
	 *	\param[in] _sktClrNum ���SSLContextר�õ�ssl socket����ش�С����֮һ
	 *	\param[in] _perClrNum ���SSLContextר�õ�ssl socket����ش�С����֮��
	 *	\return 0 �Ļ�ʧ�ܣ���0�ɹ� 
	 */
	ContextId			registerContext			(SSLContextShrPtr _context, uint32 _sktClrNum, uint32 _perClrNum);

	//! ����ָ���Ķ˿� (TCP���)
	/*!
	 *	\param[in] _port �����Ķ˿�(1~65535)���Ƽ�ʹ��7000���ϵ�
	 *	\param[in] _connection ���Ӷ�����Ҫ�û��̳���IConnection����IConnection
	 *	\param[in] _syn ͬʱ���ö��ٸ�accept�����ڵȴ���һ����Ϊ1����
	 *	\param[in] _delayTime �����ǰ��listen������Ϊĳ��ԭ���޷�����ִ�У����ͨ����ʱ����ѭ���ȴ�������������ǵȴ���ʱ��������λ�룬���Ϊ���򲻻�ȴ�
	 *	\return 0 �ɹ� ��0 ʧ��
	 */
	int32				listen					(NetPort _port, IConnection* _connection, uint32 _syn=1, uint32 _delayTime=5);

	//! ���ӵ�ָ��IP�Ͷ˿ڵĽ��� (TCP���)
	/*!
	 *	\param[in] _ip IP��ַ
	 *	\param[in] _port �Է��ļ����˿�
	 *	\param[in] _connection ���Ӷ�����Ҫ�û��̳���IConnection����IConnection
	 *	\param[in] _tag ���Դ��ݸ�_connection�Ĳ������û����Խ���Ҫ���ݵĶ����ָ��ŵ����Ȼ�����ӽ����󣬿���ͨ��SessionDeputy::getTag()����ȡ��
	 *	\param[in] _delayTime �����ǰ��listen������Ϊĳ��ԭ���޷�����ִ�У����ͨ����ʱ����ѭ���ȴ�������������ǵȴ���ʱ��������λ�룬���Ϊ���򲻻�ȴ�
	 *	\return 0 �ɹ� ��0 ʧ��
	 */
	int32				connect					(IpString _ip, NetPort _port, IConnection* _connection, void* _tag=0, uint32 _delayTime=5);

	//! SSL��ȫ���ӵļ������� (SSL��ȫ�������)
	/*!
	 *	\param[in] _port �����Ķ˿�(1~65535)���Ƽ�ʹ��7000���ϵ�
	 *	\param[in] _connection ���Ӷ�����Ҫ�û��̳���IConnection����IConnection
	 *	\param[in] _contextId ע���SSLContext�����Id
	 *	\param[in] _syn ͬʱ���ö��ٸ�accept�����ڵȴ���һ����Ϊ1����
	 *	\param[in] _delayTime �����ǰ��listen������Ϊĳ��ԭ���޷�����ִ�У����ͨ����ʱ����ѭ���ȴ�������������ǵȴ���ʱ��������λ�룬���Ϊ���򲻻�ȴ�
	 *	\return 0 �ɹ� ��0 ʧ��
	 */
	int32				sslListen				(NetPort _port, IConnection* _connection, ContextId _contextId, uint32 _syn=1, uint32 _delayTime=5);

	//! ���ӵ�ָ��IP�Ͷ˿ڵĽ��� (SSL��ȫ�������)
	/*!
	 *	\param[in] _ip IP��ַ
	 *	\param[in] _port �Է��ļ����˿�
	 *	\param[in] _connection ���Ӷ�����Ҫ�û��̳���IConnection����IConnection
	 *	\param[in] _contextId ע���SSLContext�����Id
	 *	\param[in] _tag ���Դ��ݸ�_connection�Ĳ������û����Խ���Ҫ���ݵĶ����ָ��ŵ����Ȼ�����ӽ����󣬿���ͨ��SessionDeputy::getTag()����ȡ��
	 *	\param[in] _delayTime �����ǰ��listen������Ϊĳ��ԭ���޷�����ִ�У����ͨ����ʱ����ѭ���ȴ�������������ǵȴ���ʱ��������λ�룬���Ϊ���򲻻�ȴ�
	 *	\return 0 �ɹ� ��0 ʧ�� 
	 */
	int32				sslConnect				(IpString _ip, NetPort _port, IConnection* _connection, ContextId _contextId, void* _tag=0, uint32 _delayTime=5);

	//! ���������� (TCP/UDP/SSL)
	/*!
	 *	\param[in] _snPxy Session��Id
	 *	\param[in] _pk Ҫ���͵�����������IPacket
	 *	\param[in] _isKick �Ƿ��ڷ�����Ϻ��ж��������
	 */
	void				send					(const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick=false);

	//! �ж����� (TCP/UDP/SSL)
	/*!
	 *	\param[in] _snPxy Session��Id
	 *	\param[in] _hasHdr �Ƿ����жϺ���ϵͳ����IConnection::OnDisconnected����
	 */
	void				kick					(const SessionPxy& _snPxy, bool _hasHdr=false);

	//! ����������������� (UDP���)
	/*!
	 *	\param[in] _port ׼��recvfrom�Ķ˿�
	 *	\param[in] _connection ���Ӷ�����Ҫ�û��̳���IConnection����IConnection
	 *	\param[in] _lifecycle ���udp����Ҫ�����೤ʱ��Ȼ���жϣ���λ��
	 *	\param[in] _syn ͬʱ���ö��ٸ�recvfrom�����ڵȴ���һ����Ϊ1����
	 *	\param[in] _delayTime �����ǰ��listen������Ϊĳ��ԭ���޷�����ִ�У����ͨ����ʱ����ѭ���ȴ�������������ǵȴ���ʱ��������λ�룬���Ϊ���򲻻�ȴ�
	 *	\return 0 �ɹ� ��0 ʧ�� 
	 *	\note ���������������ϵͳ�ڲ�����һ��udp��session���ӣ���������ǰ���ʱ���������������ڵģ�ʱ�䵽�ˣ�session�ͻ��Զ���ֹ�����ӶϿ�����Զ���ǲ���õ��Ͽ�����Ϣ��
	 */
	int32				recvfrom				(NetPort _port, IConnection* _connection, uint32 _lifecycle=30, uint32 _syn=1, uint32 _delayTime=5);

	//! �������ݵ�ָ��ip�Ͷ˿ڵĽ��� (UDP���)
	/*!
	 *	\param[in] _ip IP��ַ
	 *	\param[in] _port �Է�recvfrom�Ķ˿�
	 *	\param[in] _pk Ҫ���͵�����������IPacket
	 *	\param[in] _lifecycle ���udp����Ҫ�����೤ʱ��Ȼ���жϣ���λ��
	 *	\param[in] _tag ���Դ��ݸ�_connection�Ĳ������û����Խ���Ҫ���ݵĶ����ָ��ŵ����Ȼ�����ӽ����󣬿���ͨ��SessionDeputy::getTag()����ȡ��
	 *	\param[in] _delayTime �����ǰ��listen������Ϊĳ��ԭ���޷�����ִ�У����ͨ����ʱ����ѭ���ȴ�������������ǵȴ���ʱ��������λ�룬���Ϊ���򲻻�ȴ�
	 *	\return 0 �ɹ� ��0 ʧ�� 
	 *	\note ���������������ϵͳ�ڲ�����һ��udp��session���ӣ���������ǰ���ʱ���������������ڵģ�ʱ�䵽�ˣ�session�ͻ��Զ���ֹ�����ӶϿ�����Զ���ǲ���õ��Ͽ�����Ϣ��
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
