#ifndef __Orz_OrzAsio_ProtocolServiceImpl_h__
#define __Orz_OrzAsio_ProtocolServiceImpl_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/IOEngine.h>
#include <orz/OrzAsio/NetEngine/IServiceManager.h>
namespace Orz 
{
class _OrzOrzAsioPrivate ProtocolServiceImpl
{
public:
	ORZ_PLUS_INLINE
	ProtocolServiceImpl(bool _isIp4)
		: m_isIp4(_isIp4)
	{
	}

	ORZ_PLUS_INLINE
	~ProtocolServiceImpl()
	{
	}

	ORZ_PLUS_INLINE
	bool				isIp4			()
	{
		return m_isIp4;
	}

private:
	bool				m_isIp4;
};
}
#endif
