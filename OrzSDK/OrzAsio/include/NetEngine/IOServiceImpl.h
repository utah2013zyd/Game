#ifndef __Orz_OrzAsio_IOServiceImpl_h__
#define __Orz_OrzAsio_IOServiceImpl_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/IOEngine.h>
#include <orz/OrzAsio/NetEngine/IServiceManager.h>
namespace Orz 
{
class _OrzOrzAsioPrivate IOServiceImpl
{
public:
	ORZ_PLUS_INLINE
	IOServiceImpl(IServiceManager* _svcMgr, uint32 _threadNum)
		: m_svcMgr(_svcMgr)
		, m_io(_threadNum == 0 ? 1 : _threadNum)
	{
	}

	ORZ_PLUS_INLINE
	~IOServiceImpl()
	{
	}

public:
	ORZ_PLUS_INLINE
	void				boost					()
	{
		m_io.boost();
	}

	ORZ_PLUS_INLINE
	void				shutdown				()
	{
		m_io.shutdown();
	}

	ORZ_PLUS_INLINE
	void				join					()
	{
		m_io.join();
	}

	ORZ_PLUS_INLINE
	IOEngine&			ioEngine				()
	{
		return m_io;
	}

	ORZ_PLUS_INLINE
	IServiceManager*	svcMgr					()
	{
		return m_svcMgr;
	}

private:
	IServiceManager*							m_svcMgr;
	IOEngine									m_io;
};
}
#endif
