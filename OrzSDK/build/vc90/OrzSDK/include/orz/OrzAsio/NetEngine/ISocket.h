#ifndef __Orz_OrzAsio_ISocket_h__
#define __Orz_OrzAsio_ISocket_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/StringPtr.hpp>
namespace Orz
{
//! Socket接口类
/*!
 *	\note 不建议用户使用，这个类供系统内部使用
 */
class ISocket
{
public:
	ISocket() {}
	virtual ~ISocket() {}

public:
	//! send 
	/*!
	 *	\note send data to another net system. if UDP must: "out -> send -> halt" == sendto
	 *	\param[in] _pk the data that will send
	 *	\param[in] _handler the handler to deal with send result(first "bool" is show whether failed; second means "isKick")
	 *	\return 0 success, 1 failed
	 */
	virtual int32				send					(const SessionPxy& _snPxy, const IPacketShrPtr _pk, bool _isKick) = 0;

	//! halt 
	/*!
	 *	\note stop relationship with another net system
	 *	\param[in] _handler the handler to deal with stop
	 *	\return 0 success, 1 failed
	 */
	virtual int32				halt					(const SessionPxy& _snPxy, bool _hasHdr) = 0;

	//! type
	/*!
	 *	get the socket type(user self defined)
	 */
	virtual int32				type					() const = 0;

	//! close
	/*!
	 *	\note close the socket
	 */
	virtual void				close					() = 0;

	//! isClosed
	/*!
	 *	\note check if the socket is closed
	 *	\return true closed, false not close
	 */
	virtual bool				isClosed				() const = 0;
};
}
#endif
