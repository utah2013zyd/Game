#ifndef __Orz_ActOGServer_Idr_h__
#define __Orz_ActOGServer_Idr_h__
#include <orz/Tookit_Plus/Tookit/FastIdrCluster.hpp>
namespace Orz
{
class Idr
{
public:
	FORCEINLINE
	Idr(uint32 _idrNum, uint32 _perIdNum) 
		: m_idr(_idrNum, _perIdNum)
		, m_idrNum(m_idr.idr_num())
		, m_perIdNum(m_idr.perIdNum())
	{
	}

	FORCEINLINE
	~Idr() {}

public:
	FORCEINLINE
	uint32					generate				()
	{
		ClrId clrId = m_idr.generate();
		if (clrId.isValid())
			return clrId.clr*m_perIdNum + clrId.id;
		else
			return m_idrNum*m_perIdNum;
	}

	FORCEINLINE
	void					release					(uint32 _id)
	{
		ClrId clrId;
		clrId.clr = _id / m_perIdNum;
		clrId.id = _id % m_perIdNum;
		clrId.valid = true;
		m_idr.release(clrId);
	}

	FORCEINLINE
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
