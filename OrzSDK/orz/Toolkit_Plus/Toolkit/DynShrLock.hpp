#ifndef __Orz_ToolkitPlus_DynShrLock_hpp_
#define __Orz_ToolkitPlus_DynShrLock_hpp_
#include <orz/Toolkit_Plus/GlobalPlus.h>
namespace Orz
{
//! �߳�����
/*!
 *	\note �̰߳�ȫ
 *	\note �������ɵ�ѡ�����������ʽ�����캯���������������������������ó�Ա���������ͽ�����
 example:
 \code
	#1 ���1
	boost::shared_mutex	mutex;
	{
		DynShrLock lock(mutex);	// Ĭ������£������Ѿ��������������������ѡ��ķ�ʽ���ǹ��캯����������������������
								// �û�ֻ����lock�����ٺ��ֶ�����lock.unlock()������
		lock.unlock();	// �ֶ����ý������������������lock.lock();��Ч�����޷��ֶ�������
	}
	// lock�����٣�ͬʱִ�н�����������֮ǰ�ֶ������˽�������������"will nothing happen"

	#2 ���2
	boost::shared_mutex	mutex;
	{
		DynShrLock lock(mutex, DynShrLock::LT_DELAY);	// ���ﲢû�м��������������ѡ��ķ�ʽ���û���Ҫ�ó�Ա�������ֶ������ͽ�����
		lock.lock();	// �ֶ�����
	}
	// lock�����٣���������ģʽΪ"DynShrLock::LT_DELAY"�����Բ������Զ�������
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
