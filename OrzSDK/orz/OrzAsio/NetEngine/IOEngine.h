#ifndef __Orz_OrzAsio_IOEngine_h__
#define __Orz_OrzAsio_IOEngine_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/DynArray.hpp>
namespace Orz
{
//! IOEngine
/*!
 *	\note 不建议用户使用，这个类供系统内部使用
 *	\note 但是其中一个接口相对有用：post，见ISession
 */
class _OrzOrzAsioExport IOEngine
	: public boost::enable_shared_from_this<IOEngine>
	, private boost::noncopyable
{
public:
	explicit IOEngine(uint32 _threadPoolSize);
	virtual ~IOEngine();

public:
	void						boost					();
	boost::asio::io_service&	ioService				();

	template <typename Handler>
	ORZ_PLUS_INLINE
	void						post					(const Handler& _handler)
	{
		m_ioSvc.post(_handler);
	}

	void						shutdown				();
	void						join					();

private:
	typedef boost::scoped_ptr<boost::asio::io_service::work> work_ptr;

	boost::asio::io_service							m_ioSvc;
	boost::asio::io_service::work					m_work;

	DynArray<ThreadShrPtr>							m_threadPool;
};
}
#endif
