#ifndef __Orz_EchoServer_IClient_h__
#define __Orz_EchoServer_IClient_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
#include <orz/OrzAsio/NetEngine/IPacket.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
using namespace Orz;

class IClient
{
public:
	//! packet common error code
	enum ErrPkt
	{
		EP_NORMAL = 0,				// 正常
		EP_TYPE_ERR,				// 类型错误
		EP_ARG_NUM_ERR,				// 参数数量不对
		EP_TOTAL
	};

	ORZ_PLUS_INLINE IClient() : m_bHasInited(false) {}

	ORZ_PLUS_INLINE
	IClient(ServiceManager* _svcMgr, const SessionPxy& _snPxy)
		: m_svcMgr(_svcMgr)
		, m_sn(_snPxy)
		, m_bHasInited(true)
	{
	}

	ORZ_PLUS_INLINE
	virtual ~IClient()
	{
	}

	ORZ_PLUS_INLINE
	void						init							(ServiceManager* _svcMgr, const SessionPxy& _snPxy)
	{
		if (m_bHasInited)
			return;

		m_svcMgr = _svcMgr;
		m_sn = _snPxy;
		m_bHasInited = true;
	}

	ORZ_PLUS_INLINE
	void						clear							()
	{
		m_bHasInited = false;
	}

	//-----------------------------------------------------------------------------------------
	// logoff 
	virtual ORZ_PLUS_INLINE
	void						logoff							()
	{
	}

public:
	virtual ORZ_PLUS_INLINE
	int32						processPacket					(IPacketShrPtr _pk)
	{
		if (checkPacket(_pk) != 0)
			return 1;
		else
			return 0;
	}

	virtual ORZ_PLUS_INLINE
	ErrPkt						checkPacket						(IPacketShrPtr _pk)
	{	
		return EP_NORMAL;
	}

	ORZ_PLUS_INLINE
	ServiceManager*				skp								()
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
	bool							m_bHasInited;
};
#endif
