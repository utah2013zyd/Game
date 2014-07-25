#ifndef __Orz_ActOGServer_Idr_h__
#define __Orz_ActOGServer_Idr_h__
#include <orz/Toolkit_Plus/Toolkit/FastIdrCluster.hpp>
namespace Orz
{
class Idr
{
public:
	ORZ_PLUS_INLINE
	Idr(uint32 _idrNum, uint32 _perIdNum) 
		: m_idr(_idrNum, _perIdNum)
		, m_idrNum(m_idr.idr_num())
		, m_perIdNum(m_idr.perIdNum())
	{
	}

	ORZ_PLUS_INLINE
	~Idr() {}

public:
	ORZ_PLUS_INLINE
	uint32					generate				()
	{
		ClrId clrId = m_idr.generate();
		if (clrId.isValid())
			return clrId.clr/**m_perIdNum*/ + clrId.id*m_idrNum;
		else
			return m_idrNum*m_perIdNum;
	}

	ORZ_PLUS_INLINE
	void					release					(uint32 _id)
	{
		ClrId clrId;
		//clrId.clr = _id / m_perIdNum;
		//clrId.id = _id % m_perIdNum;
		clrId.clr = _id % m_idrNum;
		clrId.id = _id / m_idrNum;
		clrId.valid = true;
		m_idr.release(clrId);
	}

	ORZ_PLUS_INLINE
	uint32					getMax					() const
	{
		return m_idrNum*m_perIdNum;
	}

private:
	FastIdrCluster				m_idr;
	uint32						m_idrNum;
	uint32						m_perIdNum;
};
}
#endif
