#ifndef __Orz_OrzAsio_ProtocolService_h__
#define __Orz_OrzAsio_ProtocolService_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/IOService.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
namespace Orz
{
class _OrzOrzAsioPrivate ProtocolServiceImpl;
//! ����Э��������
/*!
 *	\note �������û�ʹ�ã�����๩ϵͳ�ڲ�ʹ��
 *	\note �����û�ϣ����չOrzAsio��ͨ���̳�ʵ���Լ�������Э���ࣨ����һ������udpЭ����Զ���Ŀɿ�����Э�飬����tcp��
 */
class _OrzOrzAsioExport ProtocolService
	: public IOService
{
public:
	ProtocolService(IServiceManager* _svcMgr, uint32 _threadNum=1, bool _isIp4=true);
	virtual ~ProtocolService();

public:
	bool				isIp4					();
	virtual void		send					(ISocket* _skt, const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick);
	virtual void		halt					(ISocket* _skt, const SessionPxy& _snPxy, bool _hasHdr);

private:
	ProtocolServiceImpl*						m_impl;
};
}
#endif
