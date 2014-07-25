#ifndef __Orz_ToolkitPlus_ObjectSelector_hpp_
#define __Orz_ToolkitPlus_ObjectSelector_hpp_
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
namespace Orz
{
template <typename ObjectType> class ObjectList;
template <typename ObjectType>
class ObjectSelector
{
	friend class ObjectList<ObjectType>;
private:
	ObjectSelector();

	ORZ_PLUS_INLINE
	ObjectSelector(PtrIntType _idNum) 
		: m_max(_idNum == 0 ? 1 : _idNum)
		, m_init(0)
		, m_first(0)
		, m_currSize(0)
		, m_currMax(0)
		, m_ids(new id[_idNum == 0 ? 1 : _idNum])
	{
		setLast(m_first);
		m_emptyHead.errObj = m_max;
	}

	ORZ_PLUS_INLINE
	~ObjectSelector()
	{
		if (m_ids)
			delete [] m_ids;
	}

private:
	//!
	ORZ_PLUS_INLINE
	void					setLast			(PtrIntType _first)
	{
		for (uint32 i=1; i<m_max; ++i)
		{
			++_first;
		}
		m_last = _first;
	}

	//! 用于标识id信息
	struct id
	{
		id() : isUsing(false), next(0) {}

		PtrIntType		i;
		bool				isUsing;
		id*					next;
	};

	struct EmptyIdHead
	{
		EmptyIdHead() : now(0), last(0) {}

		ORZ_PLUS_INLINE
		bool is_empty() const
		{
			return now ? false:true;
		}

		ORZ_PLUS_INLINE
		PtrIntType pop()
		{
			if (is_empty())
				//return errObj;
				throw OrzAsioException(OrzAsioException::EXP_INVALID_STATE, ThrowAddress(__FILE__, __LINE__));

			PtrIntType rt = now->i;
			now->isUsing = true;
			if (now->next)
				now = now->next;
			else
			{
				now = 0;
				last = now;
			}
			return rt;
		}

		ORZ_PLUS_INLINE
		void	push(id* _i)
		{
			if (!_i)
				return;

			_i->isUsing = false;
			_i->next = 0;
			if (is_empty())
			{
				now = _i;
				last = now;
			}
			else
			{
				last->next = _i;
				last = _i;
			}
		}

		id*		now;
		id*		last;
		PtrIntType	errObj;
	};

public:
	ORZ_PLUS_INLINE
	PtrIntType	select				()
	{
		if (isFull())
		{
			//printf("WARNING: BEYOND THE MAX ID SIZE! The max size is: %d\n", m_max);
			throw OrzAsioException(OrzAsioException::EXP_INVALID_STATE, ThrowAddress(__FILE__, __LINE__));
			//return m_max;
		}
		else
		{
			try
			{
				++m_currSize;
				if (m_emptyHead.is_empty())
				{
					PtrIntType cm = m_currMax;
					++m_currMax;
					m_ids[cm].i = m_init;
					++m_init;
					m_ids[cm].isUsing = true;
					return m_ids[cm].i;
				}
				else
					return m_emptyHead.pop();
			}
			catch(OrzAsioException& _e)
			{
				throw OrzAsioException(OrzAsioException::EXP_INVALID_STATE, ThrowAddress(__FILE__, __LINE__), _e);
			}
			catch(...)
			{
				throw OrzAsioException(OrzAsioException::EXP_INTERNAL_ERROR, ThrowAddress(__FILE__, __LINE__));
			}
		}
	}

	ORZ_PLUS_INLINE
	int32			release				(PtrIntType _id)
	{
		if (_id < m_first || _id - m_first >= m_max)
			return 1;

		PtrIntType rf = _id - m_first;
		if (!m_ids[rf].isUsing)
			return 1;

		--m_currSize;
		m_emptyHead.push(&m_ids[rf]);
		return 0;
	}

	ORZ_PLUS_INLINE
	bool				isFull				() const
	{
		if (m_currSize >= m_max)
			return true;
		else
			return false;
	}

	ORZ_PLUS_INLINE 
	PtrIntType			errObj				()
	{
		return m_max;
	}

	ORZ_PLUS_INLINE
	PtrIntType			first				() const
	{
		return m_first;
	}

	ORZ_PLUS_INLINE
	PtrIntType			last				() const
	{
		return m_last;
	}
	
	//!
	ORZ_PLUS_INLINE 
	PtrIntType			maxSize				() const
	{
		return m_max;
	}

	ORZ_PLUS_INLINE 
	PtrIntType			size				() const
	{
		return m_currSize;
	}

private:
	PtrIntType						m_init;
	PtrIntType						m_first;
	PtrIntType						m_last;
	PtrIntType						m_currSize;
	PtrIntType						m_currMax;
	PtrIntType						m_max;
	id*								m_ids;
	EmptyIdHead						m_emptyHead;
};

}

#endif
