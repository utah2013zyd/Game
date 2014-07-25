#ifndef __Orz_OrzAsio_IConnectionImpl_h__
#define __Orz_OrzAsio_IConnectionImpl_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz 
{
class _OrzOrzAsioPrivate IConnectionImpl
{
public:
	ORZ_PLUS_INLINE
	IConnectionImpl()
		: m_incMaxNum(1)
		, m_bIsInc(true)
	{
	}

	ORZ_PLUS_INLINE
	IConnectionImpl(const IConnectionImpl& _other)
		: m_incMaxNum(_other.m_incMaxNum)
		, m_bIsInc(_other.m_bIsInc)
	{
	}

	ORZ_PLUS_INLINE
	~IConnectionImpl()
	{
	}

	IConnectionImpl& operator=(const IConnectionImpl& _other)
	{
		if (this == &_other)
		{
			return *this;
		}

		m_incMaxNum = _other.m_incMaxNum;
		m_bIsInc = _other.m_bIsInc;
		return *this;
	}

public:
	ORZ_PLUS_INLINE
	void							setMaxInc				(uint32 _incNum)
	{
		m_incMaxNum = _incNum;
	}

	ORZ_PLUS_INLINE
	uint32							getMaxInc				() const
	{
		return m_incMaxNum;
	}

	ORZ_PLUS_INLINE
	void							setIncome				(bool _isInc=true)
	{
		m_bIsInc = _isInc;
	}

	ORZ_PLUS_INLINE
	bool							isIncome				() const
	{
		return m_bIsInc;
	}

private:
	uint32							m_incMaxNum;
	bool							m_bIsInc;
};
}
#endif
