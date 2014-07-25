#ifndef __Orz_ToolkitPlus_QueryArg_hpp_
#define __Orz_ToolkitPlus_QueryArg_hpp_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
class QueryArg
{
public:
	ORZ_PLUS_INLINE
	QueryArg()
		: m_hasCn(false)
	{
	}

	ORZ_PLUS_INLINE
	QueryArg(DBCnId _cnid)
		: m_hasCn(true)
		, m_cnid(_cnid)
	{
	}

	ORZ_PLUS_INLINE
	virtual ~QueryArg()
	{
	}

public:
	//! 是否指定了连接
	ORZ_PLUS_INLINE
	bool		hasCn		() const
	{
		return m_hasCn;
	}

	//! 返回指定的连接id，这个方法只有在hasCn返回true时才有效
	ORZ_PLUS_INLINE
	DBCnId		getCnId		() const
	{
		return m_cnid;
	}

private:
	bool					m_hasCn;
	DBCnId					m_cnid;
};
}
#endif
