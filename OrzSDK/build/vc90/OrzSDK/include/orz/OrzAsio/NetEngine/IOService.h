#ifndef __Orz_OrzAsio_IOService_h__
#define __Orz_OrzAsio_IOService_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
class _OrzOrzAsioPrivate IOServiceImpl;
//! IOService
/*!
 *	\note �������û�ʹ�ã�����๩ϵͳ�ڲ�ʹ��
 *	\note �����û�ϣ����չOrzAsio��ͨ���̳�ʵ���Լ���IOService
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
