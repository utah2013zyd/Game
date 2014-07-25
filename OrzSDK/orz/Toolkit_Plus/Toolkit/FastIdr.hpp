#ifndef __Orz_ToolkitPlus_FastIdr_hpp_
#define __Orz_ToolkitPlus_FastIdr_hpp_
#include <orz/Toolkit_Plus/Toolkit/DynArray.hpp>
#include <orz/Toolkit_Plus/Toolkit/NumDef.hpp>
#include <orz/Toolkit_Plus/Toolkit/DynShrLock.hpp>
namespace Orz
{
//! ��Ч������Id������
/*!
 *	\note �̰߳�ȫ
 *	\note ����C����+����ָ�뷽ʽ�����Һͷ���Id������ͻ���ʱ����O(1)�����߳��£�����Ҫ����������ʱ�������ͬʱ����ͻ���Id���߳���������������������
 *	\note ����ͨ��ָ��������Ѿ��������Id��������ܿ�������save��loadϵͳ����Ҫ������Id��Ϣ�Ķ���
 example:
 \code
	#1 ���1
	FastIdr idr(1000, 1);			// ����һ�����1000���ɷ���Id����СIdΪ1��Id������
	Number id1 = idr.generate();	// id1 == 1
	Number id2 = idr.generate();	// id2 == 2
	Number id3 = idr.generate();	// id3 == 3
	idr.release(id1);				// id1 �����գ���1���id���Ա����·���
	idr.release(id3);				// id3 �����գ���1���id���Ա����·���
	Number id4 = idr.generate();	// id4 == 1
	Number id5 = idr.generate();	// id4 == 3

	#2 ���2
	std::vector<Number> saves;
	saves.push_back(2);
	saves.push_back(4);

	FastIdr idr(1000, saves, 1);	// ����һ�����1000���ɷ���Id����СIdΪ1��Id��������ͬʱIdֵΪ2��4��20�ģ������Ϊ�Ѿ������䣬�����Ժ�����ʱ���Զ���������ЩId
	Number id1 = idr.generate();	// id1 == 1
	Number id2 = idr.generate();	// id2 == 3	����2
	Number id3 = idr.generate();	// id3 == 5	
	Number id4 = idr.generate();	// id4 == 6	����4
	idr.release(2);					// ����IdֵΪ2��Id����ʱ��2���Ա����·���
	Number id5 = idr.generate();	// id5 == 2
	idr.generate(7);				// Ҳ���������������������ʱ�ڱ��ָ����IdΪ�Ѿ�������
	Number id6 = idr.generate();	// id6 == 8  7������

	std::vector<Number> saves2;
	saves.push_back(9);
	saves.push_back(10);
	idr.generate(saves2);			// �����ָ��һ��Id������Ѿ�����
	Number id7 = idr.generate();	// id7 == 11  9������
	Number id8 = idr.generate();	// id8 == 12  10������
 \endcode
 */
class FastIdr
	: private boost::noncopyable
{
	//! ���ڱ�ʶId��Ϣ
	struct Id
	{
		Id() : isUsing(false), prev(0), next(0) {}

		Number		i;
		bool		isUsing;
		Id*			prev;
		Id*			next;
	};

	FastIdr();

public:
	ORZ_PLUS_INLINE
	FastIdr(Number _id_num, Number _first=1)
		: m_first(_first)
		, m_currSize(0)
		, m_curr(0)
		, m_max(_id_num == 0 ? 1 : _id_num)
		, m_ids(_id_num == 0 ? 1 : _id_num)
		, m_headAdr((PtrIntType)&m_ids[0])
		, m_idSize(sizeof(Id))
		, m_endAdr((PtrIntType)(m_headAdr + (m_max - 1)*m_idSize))
	{
		setLast(m_first);
		m_ids[0].i = 0;
		m_ids[m_max - 1].i = m_max - 1;
	}

	ORZ_PLUS_INLINE
	FastIdr(Number _id_num, const std::vector<Number>& _gends, Number _first=0)
		: m_first(_first)
		, m_currSize(0)
		, m_curr(0)
		, m_max(_id_num == 0 ? 1 : _id_num)
		, m_ids(_id_num == 0 ? 1 : _id_num)
		, m_headAdr((PtrIntType)&m_ids[0])
		, m_idSize(sizeof(Id))
		, m_endAdr((PtrIntType)(m_headAdr + (m_max - 1)*m_idSize))
	{
		setLast(m_first);
		m_ids[0].i = 0;
		m_ids[m_max - 1].i = m_max - 1;
		generate(_gends);
	}

	ORZ_PLUS_INLINE
	virtual ~FastIdr()
	{
	}

private:
	//!
	ORZ_PLUS_INLINE
	void					setLast			(Number _first)
	{
		m_last = m_first + m_max - 1;
	}

	ORZ_PLUS_INLINE
	bool					isFullSt			()
	{
		if (m_currSize >= m_max)
			return true;
		else
			return false;
	}

	//! ++m_curr
	ORZ_PLUS_INLINE
	void					pushCurr			()
	{
		if (m_curr < 0 || m_curr >= m_max)
			return;

		if (m_ids[m_curr].next == 0)
		{
			++m_curr;
			if (m_curr != m_max)
			{
				m_ids[m_curr].i = m_curr;	
			}
		}
		else
		{	
			m_curr = getIndex(m_ids[m_curr].next);
		}
	}

	ORZ_PLUS_INLINE
	Number					getIndex			(Id* _ptr)
	{
		if ((PtrIntType)_ptr < m_headAdr || (PtrIntType)_ptr > m_endAdr)
			return m_max;

		return ((PtrIntType)_ptr - m_headAdr)/m_idSize;
	}

	ORZ_PLUS_INLINE 
	bool					generateSt			(Number _id)
	{
		if (_id < m_first || _id > m_last)
			return false;

		_id = _id - m_first;

		if (m_ids[_id].isUsing)
			return false;

		Id* prev = 0;
		Id* next = 0;

		if (m_ids[_id].prev != 0)
			prev = m_ids[_id].prev;
		else
		{
			if (_id != 0)
			{
				prev = &m_ids[_id - 1];
			}
		}
		if (m_ids[_id].next != 0)
			next = m_ids[_id].next;
		else
		{
			if (_id != m_max - 1)
			{
				next = &m_ids[_id + 1];
			}
		}
		
		m_ids[_id].isUsing = true;
		m_ids[_id].prev = 0;
		m_ids[_id].next = 0;
		if (prev)
		{
			prev->next = next;
		}
		if (next)
		{
			next->prev = prev;
		}

		if (m_curr == _id)
		{
			pushCurr();
		}
		++m_currSize;

		return true;
	}

public:
	ORZ_PLUS_INLINE
	void					generate			(const std::vector<Number>& _gends)
	{
		DynShrLock lock(m_shrMutex);
		for(std::vector<Number>::const_iterator itr=_gends.begin(); itr!=_gends.end(); ++itr)
		{
			generateSt(*itr);
		}
	}

	ORZ_PLUS_INLINE 
	Number					generate			()
	{
		DynShrLock lock(m_shrMutex);
		if (isFullSt())
		{
			//throw OrzAsioException(OrzAsioException::EXP_INVALID_STATE, ThrowAddress(__FILE__, __LINE__));
			return m_last+1;
		}
		else
		{
			m_ids[m_curr].isUsing = true;
			Id* pop = &m_ids[m_curr];

			pushCurr();
			++m_currSize;
			pop->prev = 0;
			pop->next = 0;

			return getIndex(pop) + m_first;
		}
	}

	ORZ_PLUS_INLINE 
	bool					generate			(Number _id)
	{
		DynShrLock lock(m_shrMutex);
		if (_id < m_first || _id > m_last)
			return false;

		_id = _id - m_first;

		if (m_ids[_id].isUsing)
			return false;

		Id* prev = 0;
		Id* next = 0;

		if (m_ids[_id].prev != 0)
			prev = m_ids[_id].prev;
		else
		{
			if (_id != 0)
			{
				prev = &m_ids[_id - 1];
			}
		}
		if (m_ids[_id].next != 0)
			next = m_ids[_id].next;
		else
		{
			if (_id != m_max - 1)
			{
				next = &m_ids[_id + 1];
			}
		}
		
		m_ids[_id].isUsing = true;
		m_ids[_id].prev = 0;
		m_ids[_id].next = 0;
		if (prev)
		{
			prev->next = next;
		}
		if (next)
		{
			next->prev = prev;
		}

		if (m_curr == _id)
		{
			pushCurr();
		}
		++m_currSize;

		return true;
	}

	ORZ_PLUS_INLINE 
	int32					release				(Number _id)
	{
		DynShrLock lock(m_shrMutex);
		if (_id < m_first || _id > m_last)
			return 1;

		_id = _id - m_first;

		if (!m_ids[_id].isUsing)
			return 1;

		if (!isFullSt())
		{
			m_ids[_id].next = &m_ids[m_curr];
			m_ids[m_curr].prev = &m_ids[_id];
		}
			
		m_ids[_id].isUsing = false;
		m_curr = _id;
		--m_currSize;
		return 0;
	}

	ORZ_PLUS_INLINE 
	bool					isFull				()
	{
		DynShrLock lock(m_shrMutex);
		if (m_currSize >= m_max)
			return true;
		else
			return false;
	}

	ORZ_PLUS_INLINE 
	Number					errObj				()
	{
		DynShrLock lock(m_shrMutex);
		return m_last+1;
	}

	ORZ_PLUS_INLINE
	Number					first				()
	{
		DynShrLock lock(m_shrMutex);
		return m_first;
	}

	ORZ_PLUS_INLINE
	Number					last				()
	{
		DynShrLock lock(m_shrMutex);
		return m_last;
	}
	
	//!
	ORZ_PLUS_INLINE 
	Number					maxSize				()
	{
		DynShrLock lock(m_shrMutex);
		return m_max;
	}

	ORZ_PLUS_INLINE 
	Number					size				()
	{
		DynShrLock lock(m_shrMutex);
		return m_currSize;
	}

private:
	Number					m_first;
	Number					m_last;
	Number					m_currSize;
	Number					m_curr;
	Number					m_max;
	DynArray<Id>			m_ids;
	PtrIntType				m_headAdr;
	PtrIntType				m_idSize;
	PtrIntType				m_endAdr;
	boost::shared_mutex		m_shrMutex;
};

}

#endif
