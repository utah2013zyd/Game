#ifndef __Orz_OrzAsio_SessionDeputyImpl_h__
#define __Orz_OrzAsio_SessionDeputyImpl_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
#include <orz/OrzAsio/NetEngine/IConnection.h>
namespace Orz 
{
class _OrzOrzAsioPrivate SessionDeputyImpl
{
public:
	ORZ_PLUS_INLINE
	SessionDeputyImpl()
		: m_isValid(false)
		, m_tag(0)
	{
	}

	ORZ_PLUS_INLINE
	~SessionDeputyImpl()
	{
	}

public:
	ORZ_PLUS_INLINE
	void					setClient				(IClientShrPtr _client)
	{
		m_client = _client;
		if (m_client)
			m_isValid = true;
		else
			m_isValid = false;
	}

	ORZ_PLUS_INLINE
	IClientShrPtr			getClient				() const
	{
		return m_client;
	}

	ORZ_PLUS_INLINE
	void					clearClient				()
	{
		m_client = IClientShrPtr();
		m_isValid = false;
	}

	ORZ_PLUS_INLINE
	void					setTag					(void* _tag)
	{
		m_tag = _tag;
	}

	ORZ_PLUS_INLINE
	void*					getTag					()
	{
		return m_tag;
	}

	ORZ_PLUS_INLINE
	void					clearTag				()
	{
		m_tag = 0;
	}

	ORZ_PLUS_INLINE
	void					setSnPxy				(const SessionPxy& _snPxy)
	{
		m_snPxy = _snPxy;
	}

	ORZ_PLUS_INLINE
	const SessionPxy&		getSnPxy				() const
	{
		return m_snPxy;
	}

	ORZ_PLUS_INLINE
	bool					isValid					() const
	{
		return m_isValid;
	}

	ORZ_PLUS_INLINE
	void					clear					()
	{
		m_connection = 0;
		m_client.reset();
		m_isValid = false;
		m_snPxy = SessionPxy();
		m_tag = 0;
	}

	ORZ_PLUS_INLINE
	void					setConnection			(IConnection* _connection)
	{
		m_connection = _connection;
	}

	ORZ_PLUS_INLINE
	void					invokeInitHdr			(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc)
	{
		if (m_connection)
			m_connection->OnConnected(_snPxy, _snDy, _isInc);
	}

	ORZ_PLUS_INLINE
	void					invokeRecvHdr			(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk)
	{
		if (m_connection)
			m_connection->OnReceived(_snPxy, _snDy, _pk);
	}

	ORZ_PLUS_INLINE
	void					invokeSendHdr			(const SessionPxy& _snPxy, SessionDeputy& _snDy)
	{
		if (m_connection)
			m_connection->OnSended(_snPxy, _snDy);
	}

	ORZ_PLUS_INLINE
	void					invokeErrorHdr			(const SessionPxy& _snPxy, SessionDeputy& _snDy, boost::function<void ()>& _snClearHdr)
	{
		if (m_connection)
			m_connection->OnDisconnected(_snPxy, _snDy);
		_snClearHdr();
	}

private:
	IConnection*									m_connection;
	IClientShrPtr									m_client;
	SessionPxy										m_snPxy;
	bool											m_isValid;
	void*											m_tag;
};
}
#endif
