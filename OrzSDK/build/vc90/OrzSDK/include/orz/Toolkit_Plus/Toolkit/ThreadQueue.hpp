#ifndef __Orz_ToolkitPlus_ThreadQueue_hpp__
#define __Orz_ToolkitPlus_ThreadQueue_hpp__
#include <orz/Toolkit_Plus/Toolkit/ObjectList.hpp>
#include <orz/Toolkit_Plus/Toolkit/DynShrLock.hpp>
namespace Orz
{
/** linked-list style queue
 */
template<typename T>
class ThreadQueue
{
	struct Node
	{
		T		element;
		Node*	next;
	};

	ThreadQueue();

public:
	ORZ_PLUS_INLINE
	ThreadQueue(PtrIntType _maxNodeNum, int32 _cwaitTime = 3000)
		: m_maxNodeNum(_maxNodeNum == 0 ? 1 : _maxNodeNum)
		, m_nodes(m_maxNodeNum)
		, m_cwaitTime(_cwaitTime<0 ? 30000 : _cwaitTime)
		, last(0)
		, first(0)
		, m_bCondOver(false)
		, m_bCondInfinitude(_cwaitTime<0 ? true : false)
	{
	}

	ORZ_PLUS_INLINE
	~ThreadQueue()
	{
		stopPopCond();
	}

public:
	ORZ_PLUS_INLINE
	void notifyAll()
	{
		m_cond.notify_all();
	}

	ORZ_PLUS_INLINE
	void notifyOne()
	{
		m_cond.notify_one();
	}

	//! Í£Ö¹condition£¬±äpopÎªpopNoWait
	ORZ_PLUS_INLINE
	void stopPopCond(bool _usingLock=true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);

		if (m_bCondOver)
			return;

		m_bCondOver = true;
		m_cond.notify_all();
		
	}

	ORZ_PLUS_INLINE
	void activePopCond(bool _usingLock=true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);

		if (!m_bCondOver)
			return;
		m_bCondOver = false;
		m_cond.notify_all();
		
	}

	ORZ_PLUS_INLINE
	void clear(bool _usingLock=true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		// clear any elements
		while(last != 0)
			pop(false);
	}

	ORZ_PLUS_INLINE
	void push(const T& _elem, bool _usingLock=true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);

		Node * n = 0;
		n = m_nodes.select();
		if (!n)
			return;
		
		if(last)
			last->next = n;
		else
			first = n;

		last = n;
		n->next = 0;
		n->element = _elem;

		m_cond.notify_all();
	}

	ORZ_PLUS_INLINE
	bool tryPush(const T& _elem, bool _usingLock=true)
	{
		DynShrLock lock(m_shrMutex, DynShrLock::LT_DELAY);
		if (!_usingLock || !lock.tryLock())
			return false;

		Node * n = 0;
		n = m_nodes.select();
		if (!n)
		{
			lock.unlock();
			return false;
		}
		if(last)
			last->next = n;
		else
			first = n;

		last = n;
		n->next = 0;
		n->element = _elem;

		m_cond.notify_all();
		lock.unlock();
		return true;
	}

	ORZ_PLUS_INLINE
	void popNoWait(bool _usingLock=true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);

		if(first == 0)
			return;
        
		Node* td = first;
		first = td->next;
		if(!first)
			last = 0;

		m_nodes.release(td);
		return;
	}

	ORZ_PLUS_INLINE
	void pop(bool _usingLock=true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);

		if(first == 0)
			return;
        
		Node* td = first;
		first = td->next;
		if(!first)
			last = 0;

		m_nodes.release(td);
		return;
	}

	ORZ_PLUS_INLINE
	bool popNoWait(T& _t, bool _usingLock=true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		
		if(first == 0)
			return false;
        
		_t = first->element;
		Node* td = first;
		first = td->next;
		if(!first)
			last = 0;

		if (m_nodes.release(td) != 0)
			return false;

		return true;
	}

	ORZ_PLUS_INLINE
	bool pop(T& _t, bool _usingLock=true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		while (first == 0 && !m_bCondOver)
		{
			m_cond.timed_wait(lock, m_cwaitTime);
			if (!m_bCondInfinitude)
			{
				break;
			}
		}

		if (first == 0)
		{
			m_cond.notify_all();
			return false;
		}
        
		_t = first->element;
		Node* td = first;
		first = td->next;
		if(!first)
			last = 0;

		if (m_nodes.release(td) != 0)
		{
			m_cond.notify_all();
			return false;
		}

		m_cond.notify_all();
		return true;
	}

	ORZ_PLUS_INLINE
	bool tryPop(T& _t, bool _usingLock=true)
	{	
		DynShrLock lock(m_shrMutex, DynShrLock::LT_DELAY);
		if (!_usingLock || !lock.tryLock())
			return false;

		if(first == 0)
		{
			lock.unlock();
			return false;
		}
        
		_t = first->element;
		Node* td = first;
		first = td->next;
		if(!first)
			last = 0;

		if (m_nodes.release(td) != 0)
		{
			lock.unlock();
			return false;
		}
		lock.unlock();
		return true;
	}

	ORZ_PLUS_INLINE
	T& front(bool _usingLock=true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		if (!first)
			throw InvalidParamsExp(InvalidParamsExp::IPEXP_NULL_PTR, ThrowAddress(__FILE__, __LINE__));
		else
			return first->element;
	}

	ORZ_PLUS_INLINE
	T& back(bool _usingLock=true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		if (!last)
			throw InvalidParamsExp(InvalidParamsExp::IPEXP_NULL_PTR, ThrowAddress(__FILE__, __LINE__));
		else
			return last->element;
	}

	ORZ_PLUS_INLINE
	bool front(T& _elem, bool _usingLock=true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		if (!first) return false;
		_elem = first->element;
		return true;
	}

	ORZ_PLUS_INLINE
	bool back(T& _elem, bool _usingLock=true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		if (!last) return false;
		_elem = last->element;
		return true;
	}

	ORZ_PLUS_INLINE
	bool empty(bool _usingLock=true) const
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		return first == 0;
	}

	ORZ_PLUS_INLINE
	void lockRead()
	{
		m_shrMutex.lock_shared();
	}

	ORZ_PLUS_INLINE
	void unlockRead()
	{
		m_shrMutex.unlock_shared();
	}

	ORZ_PLUS_INLINE
	boost::shared_mutex& mutex(bool _usingLock=true) const
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		return m_shrMutex;
	}

	ORZ_PLUS_INLINE
	void lockWrite()
	{
		m_shrMutex.lock();
	}

	ORZ_PLUS_INLINE
	void unlockWrite()
	{
		m_shrMutex.unlock();
	}

private:
	PtrIntType							m_maxNodeNum;
	ObjectList<Node>						m_nodes;
	mutable boost::shared_mutex				m_shrMutex;
	mutable boost::condition_variable_any	m_cond;
	boost::posix_time::millisec				m_cwaitTime;
	Node*									last;
	Node*									first;
	bool									m_bCondOver;
	bool									m_bCondInfinitude;
};

}

#endif
