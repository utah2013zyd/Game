#ifndef __Orz_OrzAsio_Session_h__
#define __Orz_OrzAsio_Session_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/DynArray.hpp>
#include <orz/Toolkit_Plus/Toolkit/Rander.hpp>
#include <orz/OrzAsio/NetEngine/NetData.h>
#include <orz/OrzAsio/NetEngine/IPacket.h>
#include <orz/OrzAsio/NetEngine/IPacketParser.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
#include <orz/OrzAsio/NetEngine/ISession.h>
#include <orz/OrzAsio/NetEngine/ISocket.h>
#include <orz/OrzAsio/NetEngine/IConnection.h>
#include <NetEngine/IConnectionImpl.h>
#include <orz/OrzAsio/NetEngine/NetData.h>
#include <orz/OrzAsio/NetEngine/NetInfoManager.h>
#include <NetEngine/NetInfoManagerImpl.h>
#include <orz/OrzAsio/NetEngine/SessionDeputy.h>
#include <NetEngine/SessionDeputyImpl.h>
#include <orz/OrzAsio/NetEngine/IServiceManager.h>
#include <orz/OrzAsio/NetEngine/ProtocolService.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
namespace Orz 
{
#define SESSION_VALID_CODE_RANGE	1000000000

class _OrzOrzAsioPrivate Session
	: public ISession
	, private boost::noncopyable
{
public:
	ORZ_PLUS_INLINE
	Session(SessionId _snid, NetInfoManager* _netInfoMgr, uint32 _vseed, IPacketParserShrPtr _pktParser)
		: m_snid(_snid)
		, m_error(false)
		, m_netInfoMgr(_netInfoMgr)
		, m_vseed(_vseed)
		, m_min(m_vseed * SESSION_VALID_CODE_RANGE + 1)
		, m_max((m_vseed + 1) * SESSION_VALID_CODE_RANGE)
		, m_currVcode(_vseed)
		, m_rander(new Rander((uint32)m_vseed))
		, m_socket(0)
		, m_pSvc(0)
		, m_parser(_pktParser)
		, m_hasInited(false)
	{
		m_currVcode = (ValidCode)m_rander->generate(0, SESSION_VALID_CODE_RANGE);
	}

	ORZ_PLUS_INLINE
	virtual ~Session()
	{
		if (m_rander)
			delete m_rander;
	}

	ORZ_PLUS_INLINE
	bool					init					(ProtocolService* _pSvc, ISocket* _skt)
	{
		if (m_hasInited || !_pSvc || !_skt)
		{
			sLogger->out(OL_DEBUG, "[Error]", __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "m_hasInited || !_pSvc || !_skt", 0);
			return false;
		}

		m_pSvc = _pSvc;
		m_socket = _skt;
		m_clearHdr = boost::bind(&Session::clearHandler, this);
		m_snPxy.m_snid = m_snid;
		m_snPxy.m_sn = this;
		m_snPxy.m_vcode = m_currVcode;
		m_snPxy.m_socketType = m_socket->type();
		m_hasInited = true;
		return true;
	}

public:
	ORZ_PLUS_INLINE
	int32					handleIn				(bool _error, const NetData& _nd)
	{
		if (_error) 
			return 1;
		
		if (_nd.connection)
			_nd.connection->impl()->setIncome(true);
		return begin(_error, _nd);
	}

	ORZ_PLUS_INLINE
	int32					handleOut				(bool _error, const NetData& _nd)
	{
		if (_error) 
			return 1;
		
		if (_nd.connection)
			_nd.connection->impl()->setIncome(false);
		return begin(_error, _nd);
	}

	ORZ_PLUS_INLINE
	void					handleRead				(const SessionPxy& _snPxy, bool _error, StringPtr _buff)
	{
		if (m_error) 
			return;

		if (!checkVcode(_snPxy) || !isValid()) 
			return;

		if (!_error && isValid())
		{
			m_readBuffs.clear();
			m_parser->parse(_buff.ptr(), _buff.size(), m_readBuffs);

			if (m_readBuffs.empty()) 
				return;

			// create and send CompletionHandler
			for (std::vector<IPacketShrPtr>::iterator itr=m_readBuffs.begin(); itr!=m_readBuffs.end(); ++itr)
			{
				m_snDy.impl().invokeRecvHdr(m_snPxy, m_snDy, *itr);
			}
		}
		else if (isValid())
			kill();
	}

	ORZ_PLUS_INLINE
	void					handleWrite				(const SessionPxy& _snPxy, bool _error, IPacketShrPtr _sended, bool _isKick)
	{
		if (m_error) 
			return;

		if (!checkVcode(_snPxy) || !isValid()) 
			return;

		if (_error && isValid())
			kill();
		else if (_isKick && isValid())
			killWithoutHdr();
		else if (isValid())
		{
			m_snDy.impl().invokeSendHdr(m_snPxy, m_snDy);
		}
	}

	ORZ_PLUS_INLINE
	void					handleEnd				(const SessionPxy& _snPxy, bool _error, bool _hasHdr)
	{
		if (m_error)
			return;

		if (!checkVcode(_snPxy) || !isValid()) 
			return;

		if (_error) 
			return;

		if (_hasHdr) 
			kill();
		else 
			killWithoutHdr();
	}

	ORZ_PLUS_INLINE
	bool					isValid					() const
	{
		if (m_hasInited && m_nd.isValid())
			return true;
		else
			return false;
	}

	ORZ_PLUS_INLINE
	const SessionPxy&		getSessionPxy			() const
	{
		return m_snPxy;
	}

	ORZ_PLUS_INLINE
	const NetData&			getNetData				() const
	{
		return m_nd;
	}

	ORZ_PLUS_INLINE
	ISocket*				getSocket				()
	{
		return m_socket;
	}

	ORZ_PLUS_INLINE
	ProtocolService*		getProtocolService		()
	{
		return m_pSvc;
	}

	ORZ_PLUS_INLINE
	void					clearHandler			()
	{
		m_parser->clear();
		if (m_netInfoMgr && m_nd.connection)
		{
			if (m_nd.connection->impl()->isIncome())
				m_netInfoMgr->impl()->decreaseIn(m_nd.port);
			else
				m_netInfoMgr->impl()->decreaseOut(m_nd.port);
		}
		m_nd.clear();
		m_snDy.impl().clear();
	}

private:
	ORZ_PLUS_INLINE
	int32					begin					(bool _error, const NetData& _nd)
	{
		if (m_nd.isValid())
			return 1;

		if (_error)
		{
			refuse();
			return 1;
		}

		if (_nd.connection && 
			m_netInfoMgr->getInConnectionNum(_nd.port) >= _nd.connection->impl()->getMaxInc())
		{
 			refuse();
			return 1;
		}

		m_error = false;

		// update valid code
		updateVcode();

		m_nd = _nd;
		// set session proxy
		m_snDy.impl().setConnection(m_nd.connection);
		m_snDy.impl().setTag(m_nd.tag);
		setHdrsVcode();

		m_netInfoMgr->impl()->increaseIn(m_nd.port);
		m_snDy.impl().invokeInitHdr(m_snPxy, m_snDy, _nd.connection->impl()->isIncome());
		return 0;
	}

	ORZ_PLUS_INLINE
	int32					kill					()
	{
		if (m_error || !isValid())
			return 1;

		m_snDy.impl().invokeErrorHdr(m_snPxy, m_snDy, m_clearHdr);
		m_error = true;
		updateVcode();
		setHdrsVcode();

		return 0;
	}

	ORZ_PLUS_INLINE
	int32					killWithoutHdr		()
	{
		if (m_error || !isValid())
			return 1;

		m_error = true;
		updateVcode();
		setHdrsVcode();

		m_clearHdr();
		return 0;
	}

	ORZ_PLUS_INLINE
	void					refuse					()
	{
		if (m_error || !isValid())
			return;

		m_error = true;
		m_parser->clear();
		m_nd.clear();
	}

	ORZ_PLUS_INLINE
	void					updateVcode				()
	{
		++m_currVcode;
		if (m_currVcode >= m_max)
			m_currVcode = m_min;
	}

	ORZ_PLUS_INLINE
	void					setHdrsVcode			()
	{
		m_snPxy.setVcode(m_currVcode);
	}

	ORZ_PLUS_INLINE
	bool					checkVcode				(const SessionPxy& _snPxy)
	{
		if (m_currVcode == 0 || !(m_snPxy == _snPxy))
			return false;

		return true;
	}

private:
	SessionId										m_snid;
	SessionPxy										m_snPxy;
	bool											m_error;
	NetInfoManager*									m_netInfoMgr;

	// valid seed
	ValidCode										m_vseed;
	ValidCode										m_min;
	ValidCode										m_max;
	ValidCode										m_currVcode;

	Rander*											m_rander;
	SessionDeputy									m_snDy;
	NetData											m_nd;
	ISocket*										m_socket;
	ProtocolService*								m_pSvc;

	std::vector<IPacketShrPtr>						m_readBuffs;
	IPacketParserShrPtr								m_parser;

	boost::function<void ()>						m_clearHdr;
	bool											m_hasInited;
};
}
#endif
