#ifndef __Orz_ToolkitPlus_DynShrLock_hpp_
#define __Orz_ToolkitPlus_DynShrLock_hpp_
#include <orz/Toolkit_Plus/GlobalPlus.h>
namespace Orz
{
//! 线程锁类
/*!
 *	\note 线程安全
 *	\note 可以自由的选择何种锁定方式（构造函数锁定、析构函数解锁，还是用成员函数加锁和解锁）
 example:
 \code
	#1 情况1
	boost::shared_mutex	mutex;
	{
		DynShrLock lock(mutex);	// 默认情况下，这里已经加锁，而且这种情况下选择的方式就是构造函数锁定、析构函数解锁；
								// 用户只有在lock被销毁和手动调用lock.unlock()来解锁
		lock.unlock();	// 手动调用解锁，但是这种情况下lock.lock();无效（即无法手动加锁）
	}
	// lock被销毁，同时执行解锁，但由于之前手动调用了解锁，所以这里"will nothing happen"

	#2 情况2
	boost::shared_mutex	mutex;
	{
		DynShrLock lock(mutex, DynShrLock::LT_DELAY);	// 这里并没有加锁，这种情况下选择的方式是用户需要用成员函数来手动加锁和解锁；
		lock.lock();	// 手动加锁
	}
	// lock被销毁，但是由于模式为"DynShrLock::LT_DELAY"，所以并不会自动解锁！
 \endcode
 */
class DynShrLock
	: private boost::noncopyable
{
public:
	enum LockType
	{
		LT_LOCK = 0,
		LT_TRY_LOCK, 
		LT_LOCK_SHR,
		LT_TRY_LOCK_SHR,
		LT_DELAY
	};

	ORZ_PLUS_INLINE
	DynShrLock(boost::shared_mutex& _m, DynShrLock::LockType _lockType=LT_LOCK) 
		: m_shrMutex(&_m)
		, m_lockType(_lockType)
	{
		switch (m_lockType)
		{
		case LT_LOCK:			m_shrMutex->lock();				break;
		case LT_TRY_LOCK:		m_shrMutex->try_lock();			break;
		case LT_LOCK_SHR:		m_shrMutex->lock_shared();		break;
		case LT_TRY_LOCK_SHR:	m_shrMutex->try_lock_shared();	break;
		default:		
			break;
		}
	}

	ORZ_PLUS_INLINE
	virtual ~DynShrLock()
	{
		switch (m_lockType)
		{
		case LT_LOCK:
		case LT_TRY_LOCK:		m_shrMutex->unlock();		break;
		case LT_LOCK_SHR:		
		case LT_TRY_LOCK_SHR:	m_shrMutex->unlock_shared(); break;
		default:		
			break;
		}
	}

public:
	ORZ_PLUS_INLINE
	void		lock				()
	{
		if (m_lockType == LT_DELAY)
			m_shrMutex->lock();
	}

	ORZ_PLUS_INLINE
	bool		tryLock				()
	{
		if (m_lockType == LT_DELAY)
			return m_shrMutex->try_lock();
		else
			return false;
	}

	ORZ_PLUS_INLINE
	void		unlock				()
	{
		//if (m_lockType == LT_DELAY)
		m_shrMutex->unlock();
	}

	ORZ_PLUS_INLINE
	void		lockShared			()
	{
		if (m_lockType == LT_DELAY)
			m_shrMutex->lock_shared();
	}

	ORZ_PLUS_INLINE
	bool		tryLockShared		()
	{
		if (m_lockType == LT_DELAY)
			return m_shrMutex->try_lock_shared();
		else
			return false;
	}

	ORZ_PLUS_INLINE
	void		unlockShared		()
	{
		//if (m_lockType == LT_DELAY)
		m_shrMutex->unlock_shared();
	}

private:
	mutable boost::shared_mutex*	m_shrMutex;
	const LockType					m_lockType;
};
}
#endif
