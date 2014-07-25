#ifndef __Orz_OrzAsio_IOService_h__
#define __Orz_OrzAsio_IOService_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
class _OrzOrzAsioPrivate IOServiceImpl;
//! IOService
/*!
 *	\note 不建议用户使用，这个类供系统内部使用
 *	\note 除非用户希望扩展OrzAsio，通过继承实现自己的IOService
 */
class _OrzOrzAsioExport IOService
	: private boost::noncopyable
{
public:
	IOService(IServiceManager* _svcMgr, uint32 _threadNum=1);
	virtual ~IOService();

public:
	void				boost					();
	void				shutdown				();
	void				join					();
	IOEngine&			ioEngine				();
	IServiceManager*	svcMgr					();

private:
	IOServiceImpl*								m_impl;
};
}
#endif
