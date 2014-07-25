#ifndef __Orz_ToolkitPlus_ResCluster_hpp_
#define __Orz_ToolkitPlus_ResCluster_hpp_
#include <orz/Toolkit_Plus/Toolkit/FastIdrCluster.hpp>
namespace Orz
{
template <typename Resource>
class ResCluster;

template <typename Resource>
class ResWrap
{
	friend class ResCluster<Resource>;
public:
	ResWrap()
		: m_res(0)
	{
	}

	ResWrap(ClrId _cid, Resource* _res)
		: m_cid(_cid)
		, m_res(_res)
	{
	}

public:
	ORZ_PLUS_INLINE
	ClrId			id				()
	{
		return m_cid;
	}

	ORZ_PLUS_INLINE
	bool			isValid		() const
	{
		return m_cid.isValid();
	}

	ORZ_PLUS_INLINE
	Resource&		res				()
	{
		return *m_res;
	}

	ORZ_PLUS_INLINE
	void			clear			()
	{
		m_cid.clear();
		m_res = 0;
	}

private:
	ClrId			m_cid;
	Resource*		m_res;
};

template <typename Resource>
class ResCluster
	: private boost::noncopyable
{
	ResCluster();

public:
	ORZ_PLUS_INLINE
	ResCluster(uint32 _clrNum, uint32 _perClrNum)
		: m_clr_num(_clrNum == 0 ? 1 : _clrNum)
		, m_perClrNum(_perClrNum == 0 ? 1 : _perClrNum)
		, m_resSlr(_clrNum, _perClrNum)
		, m_resource(_clrNum == 0 ? 1 : _clrNum)
	{
		for (uint32 i=0; i<m_resource.size(); ++i)
		{
			m_resource[i].reset(m_perClrNum);
		}
	}
	ORZ_PLUS_INLINE
	virtual ~ResCluster()
	{
	}

public:
	ORZ_PLUS_INLINE
	DynArray<DynArray<Resource>	>&		all					()
	{
		return m_resource;
	}

	ORZ_PLUS_INLINE
	ResWrap<Resource>					alloc				()
	{
		ResWrap<Resource> resw;

		try
		{
			ClrId cid = m_resSlr.generate();
			if (cid.id < m_perClrNum)
			{
				resw.m_cid = cid;
				resw.m_res = &m_resource[cid.clr][cid.id];
			}
		}
		catch(OrzAsioException& _e)
		{
			throw OrzAsioException(OrzAsioException::EXP_INVALID_STATE, ThrowAddress(__FILE__, __LINE__), _e);
		}
		catch(...)
		{
			throw OrzAsioException(OrzAsioException::EXP_INTERNAL_ERROR, ThrowAddress(__FILE__, __LINE__));
		}
		return resw;
	}

	ORZ_PLUS_INLINE
	ResWrap<Resource>					alloc				(uint32 _clr)
	{
		ResWrap<Resource> resw;

		try
		{
			ClrId cid = m_resSlr.generate(_clr);
			if (cid.id < m_perClrNum)
			{
				resw.m_cid = cid;
				resw.m_res = &m_resource[cid.clr][cid.id];
			}
		}
		catch(OrzAsioException& _e)
		{
			throw OrzAsioException(OrzAsioException::EXP_INVALID_STATE, ThrowAddress(__FILE__, __LINE__), _e);
		}
		catch(...)
		{
			throw OrzAsioException(OrzAsioException::EXP_INTERNAL_ERROR, ThrowAddress(__FILE__, __LINE__));
		}
		return resw;
	}

	ORZ_PLUS_INLINE
	int32								dealloc				(ClrId _cid)
	{
		return m_resSlr.release(_cid);
	}

	ORZ_PLUS_INLINE
	Resource&							operator()			(uint32 _clrI, uint32 _resI)
	{
		try
		{
			return m_resource[_clrI][_resI];
		}
		catch(OrzAsioException& _e)
		{
			throw RangeExp(RangeExp::REXP_ARRAY, ThrowAddress(__FILE__, __LINE__), _e);
		}
		catch(...)
		{
			throw OrzAsioException(OrzAsioException::EXP_INTERNAL_ERROR, ThrowAddress(__FILE__, __LINE__));
		}
	}

	ORZ_PLUS_INLINE
	DynArray<Resource>&					operator[]			(uint32 _clrI)
	{
		try
		{
			return m_resource[_clrI];
		}
		catch(OrzAsioException& _e)
		{
			throw RangeExp(RangeExp::REXP_ARRAY, ThrowAddress(__FILE__, __LINE__), _e);
		}
		catch(...)
		{
			throw OrzAsioException(OrzAsioException::EXP_INTERNAL_ERROR, ThrowAddress(__FILE__, __LINE__));
		}
	}

	ORZ_PLUS_INLINE
	uint32								clrSize			() const
	{
		return m_resource.size();
	}

	ORZ_PLUS_INLINE
	uint32								perSize			() const
	{
		return m_resource[0].size();
	}

private:
	FastIdrCluster						m_resSlr;
	uint32								m_clr_num;
	uint32								m_perClrNum;
	DynArray<DynArray<Resource>	>		m_resource;
};

}

#endif
