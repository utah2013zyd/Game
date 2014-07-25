#ifndef __Orz_ToolkitPlus_Counter_hpp__
#define __Orz_ToolkitPlus_Counter_hpp__
#include <orz/Toolkit_Plus/Toolkit/DynArray.hpp>
#include <orz/Toolkit_Plus/Toolkit/NumDef.hpp>
namespace Orz 
{
//! 计数器
/*!
 *	\note 使用了lock-free技术，线程安全，用于多线程下的计数
 *	\note 最小值为0， 最大值为32为整型的最大值（2147483647）
 example:
 \code
	Counter c;
	c.inc();				// 线程1
	c.inc();				// 线程2
	c.dec();				// 线程3
	uint32 size = c.get();	// 线程4
	// 这个时候size == 1
 \endcode
 */
class Counter
{
public:
	ORZ_PLUS_INLINE
	Counter()
		: m_count(0)
	{
	}

	ORZ_PLUS_INLINE
	~Counter()
	{
	}

public:
	ORZ_PLUS_INLINE
	bool				operator==			(const Counter& _other)
	{
		return m_count == _other.m_count;
	}

	ORZ_PLUS_INLINE
	bool				operator==			(int32 _num)
	{
		return m_count == _num;
	}

	ORZ_PLUS_INLINE
	int32				get					() const
	{
		return m_count;
	}

	ORZ_PLUS_INLINE
	void				inc					(uint32 _num=1)
	{
		while (true)
		{
			int32 old = m_count;
			if (CAS_assembly(&m_count, old, (int32)(m_count + (int32)_num)))
			{
				break;
			}
		}
	}

	ORZ_PLUS_INLINE
	void				dec					(uint32 _num=1)
	{
		while (true)
		{
			int32 old = m_count;
			if (CAS_assembly(&m_count, old, (int32)(m_count - (int32)_num)))
			{
				break;
			}
		}
	}

private:
	int32											m_count;
};

template <typename HashArg>
class HashCounter
{
public:
	ORZ_PLUS_INLINE explicit
	HashCounter(uint32 _base)
		: m_base(_base)
		, m_counters(_base)
	{
	}

	ORZ_PLUS_INLINE
	~HashCounter()
	{
	}

	ORZ_PLUS_INLINE
	void				setHash				()
	{
		m_hashHdr = boost::bind(&HashCounter<HashArg>::defaultHash, this, _1);
	}

	ORZ_PLUS_INLINE
	void				setHash				(const boost::function<Number (const HashArg&)>& _hash)
	{
		m_hashHdr = _hash;
	}

public:
	ORZ_PLUS_INLINE
	bool				operator==			(const HashCounter<HashArg>& _other)
	{
		return get() == _other.get();
	}

	ORZ_PLUS_INLINE
	bool				operator==			(int32 _num)
	{
		int32 size = 0;
		for (uint32 i=0; i<m_base; ++i)
		{
			size += m_counters[i].get();
		}

		return size == _num;
	}

	ORZ_PLUS_INLINE
	int32				get					() const
	{
		int32 count = 0;
		for (uint32 i=0; i<m_base; ++i)
		{
			count += m_counters[i].get();
		}

		return count;
	}

	ORZ_PLUS_INLINE
	int32				get					(const HashArg& _hashArg) const
	{
		return m_counters[m_hashHdr(_hashArg)].get();
	}

	ORZ_PLUS_INLINE
	void				inc					(const HashArg& _hashArg, uint32 _num=1)
	{
		m_counters[m_hashHdr(_hashArg)].inc(_num);
	}

	ORZ_PLUS_INLINE
	void				dec					(const HashArg& _hashArg, uint32 _num=1)
	{
		m_counters[m_hashHdr(_hashArg)].dec(_num);
	}

private:
	ORZ_PLUS_INLINE
	Number				defaultHash			(const HashArg& _hashArg)
	{
		return _hashArg % m_base;
	}

private:
	uint32											m_base;
	DynArray<Counter>								m_counters;
	boost::function<Number (const HashArg&)>		m_hashHdr;
};

}
#endif
