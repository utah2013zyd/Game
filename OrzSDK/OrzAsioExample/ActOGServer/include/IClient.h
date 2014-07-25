#ifndef __Orz_ActOGServer_IClient_h__
#define __Orz_ActOGServer_IClient_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
#include <packet/packet.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
using namespace Orz;

class IClient
{
public:
	//! packet common error code
	enum err_pkt
	{
		ep_normal = 0,				// 正常
		ep_type_err,				// 类型错误
		ep_arg_num_err,				// 参数数量不对
		ep_total
	};

	ORZ_PLUS_INLINE
	IClient(ServiceManager* _svcMgr, const SessionPxy& _snPxy)
		: m_svcMgr(_svcMgr)
		, m_sn(_snPxy)
	{
	}

	ORZ_PLUS_INLINE
	virtual ~IClient()
	{
	}

	//-----------------------------------------------------------------------------------------
	// logoff 
	virtual ORZ_PLUS_INLINE
	void						logoff							()
	{
	}

public:
	//!
	virtual ORZ_PLUS_INLINE
	int32						process_packet					(IPacketShrPtr _pk)
	{
		if (check_packet(_pk) != 0)
			return 1;
		else
			return 0;
	}

	//! check packet 返回error code 0 合格 >1 error code
	ORZ_PLUS_INLINE
	err_pkt						check_packet					(IPacketShrPtr _pk)
	{
		if (_pk->getMsg() < M_NULL || _pk->getMsg() >= M_TOTAL)
			return ep_type_err;
		
		return ep_normal;
	}

	ORZ_PLUS_INLINE
	ServiceManager*				svcmgr							()
	{
		return m_svcMgr;
	}

	ORZ_PLUS_INLINE
	const SessionPxy&			sn								()
	{
		return m_sn;
	}

protected:
	mutable boost::shared_mutex		m_shr_mutex;

private:
	ServiceManager*					m_svcMgr;
	SessionPxy						m_sn;
};
#endif
