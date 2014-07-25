#ifndef __Orz_ToolkitPlus_QueryQueue_hpp_
#define __Orz_ToolkitPlus_QueryQueue_hpp_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
class QueryQueue
{
public:
	//! 默认为同步、不指定连接
	ORZ_PLUS_INLINE
	QueryQueue()
	{
	}

	ORZ_PLUS_INLINE
	~QueryQueue()
	{
	}

public:
	//! 压入query到队列中
	ORZ_PLUS_INLINE
	void		push		(QueryShrPtr _query)
	{
		m_querys.push(_query);
	}

	//! 取得当前Query数量
	ORZ_PLUS_INLINE
	uint32		size		()
	{
		return m_querys.size();
	}

	//! 弹出query
	ORZ_PLUS_INLINE
	bool		pop			(QueryShrPtr& _q)
	{
		if (!m_querys.empty())
		{
			_q = m_querys.front();
			m_querys.pop();
			return true;
		}
		return false;
	}

private:
	std::queue<QueryShrPtr>	m_querys;
};
}
#endif
