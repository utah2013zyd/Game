#ifndef __Orz_OrzAsio_ISession_h__
#define __Orz_OrzAsio_ISession_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/StringPtr.hpp>
namespace Orz
{
//! Session接口类
/*!
 *	\note 不建议用户使用，这个类供系统内部使用
 *	\note 但是其中一个接口相对有用：getProtocolService
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

	//! 取得ProtocolService类
	/*!
	 *	\note 通过ProtocolService类，用户可以自己向OrzAsio内部的事件队列push自己的函数，保证线程安全和函数的调用顺序
	 example:
		IOEngine* io = this->getProtocolService()->ioEngine();
		io->post(boost::bind(&Xxxx::xxfun, xxx));
	 */
	virtual ProtocolService*			getProtocolService		() = 0;
};
}
#endif
