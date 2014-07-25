#ifndef __Orz_OrzAsio_ProtocolService_h__
#define __Orz_OrzAsio_ProtocolService_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/IOService.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
namespace Orz
{
class _OrzOrzAsioPrivate ProtocolServiceImpl;
//! 网络协议服务基类
/*!
 *	\note 不建议用户使用，这个类供系统内部使用
 *	\note 除非用户希望扩展OrzAsio，通过继承实现自己的网络协议类（例如一个基于udp协议的自定义的可靠传输协议，类似tcp）
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
