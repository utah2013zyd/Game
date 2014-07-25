#ifndef __Orz_OrzAsio_ISession_h__
#define __Orz_OrzAsio_ISession_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/StringPtr.hpp>
namespace Orz
{
//! Session�ӿ���
/*!
 *	\note �������û�ʹ�ã�����๩ϵͳ�ڲ�ʹ��
 *	\note ��������һ���ӿ�������ã�getProtocolService
 */
class ISession
{
public:
	ISession() {}
	virtual ~ISession() {}

public:
	virtual bool						init					(ProtocolService* _pSvc, ISocket* _skt) = 0;
	virtual int32						handleIn				(bool _error, const NetData& _nd) = 0;
	virtual int32						handleOut				(bool _error, const NetData& _nd) = 0;
	virtual void						handleRead				(const SessionPxy& _snPxy, bool _error, StringPtr _buff) = 0;
	virtual void						handleWrite				(const SessionPxy& _snPxy, bool _error, IPacketShrPtr _sended, bool _isKick) = 0;
	virtual void						handleEnd				(const SessionPxy& _snPxy, bool _error, bool _hasHdr) = 0;
	virtual bool						isValid					() const = 0;
	virtual const SessionPxy&			getSessionPxy			() const = 0;
	virtual const NetData&				getNetData				() const = 0;
	virtual ISocket*					getSocket				() = 0;

	//! ȡ��ProtocolService��
	/*!
	 *	\note ͨ��ProtocolService�࣬�û������Լ���OrzAsio�ڲ����¼�����push�Լ��ĺ�������֤�̰߳�ȫ�ͺ����ĵ���˳��
	 example:
		IOEngine* io = this->getProtocolService()->ioEngine();
		io->post(boost::bind(&Xxxx::xxfun, xxx));
	 */
	virtual ProtocolService*			getProtocolService		() = 0;
};
}
#endif
