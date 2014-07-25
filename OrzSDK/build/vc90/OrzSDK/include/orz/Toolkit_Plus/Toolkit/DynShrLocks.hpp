#ifndef __Orz_ToolkitPlus_DynShrLocks_hpp_
#define __Orz_ToolkitPlus_DynShrLocks_hpp_
#include <orz/Toolkit_Plus/Toolkit/DynArray.hpp>
namespace Orz
{
//! �̶߳�������
/*!
 *	\note �̰߳�ȫ
 *	\note �������ɵ�ѡ�����������ʽ�����캯���������������������������ó�Ա���������ͽ�����
 *	\note ͬʱ�����ͽ��������
 example:
 \code
	#1 ���1
	boost::shared_mutex	mutex1;
	boost::shared_mutex	mutex2;
	{
		DynShrLocks locks(mutex1, mutex2);	// Ĭ������£������Ѿ��������������������ѡ��ķ�ʽ���ǹ��캯����������������������
											// �û�ֻ����lock�����ٺ��ֶ�����lock.unlock()������
		locks.unlock();	// �ֶ����ý������������������lock.lock();��Ч�����޷��ֶ�������
	}
	// locks�����٣�ͬʱִ�н�����������֮ǰ�ֶ������˽�������������"will nothing happen"

	#2 ���2
	boost::shared_mutex	mutex1;
	boost::shared_mutex	mutex2;
	{
		DynShrLocks locks(mutex1, mutex2, DynShrLock::LT_DELAY);	// ���ﲢû�м��������������ѡ��ķ�ʽ���û���Ҫ�ó�Ա�������ֶ������ͽ�����
		locks.lock();	// �ֶ�����
	}
	// locks�����٣���������ģʽΪ"DynShrLocks::LT_DELAY"�����Բ������Զ�������
 \endcode
 */
class DynShrLocks
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
	DynShrLocks(boost::shared_mutex& _m, DynShrLocks::LockType _lockType=LT_LOCK) 
		: m_shrMutexs(1)
		, m_lockType(_lockType)
	{
		m_shrMutexs[0] = &_m;
		initLock();
	}

	ORZ_PLUS_INLINE
	DynShrLocks(boost::shared_mutex& _m1, boost::shared_mutex& _m2, DynShrLocks::LockType _lockType=LT_LOCK) 
		: m_shrMutexs(2)
		, m_lockType(_lockType)
	{
		m_shrMutexs[0] = &_m1;
		m_shrMutexs[1] = &_m2;
		initLock();
	}

	ORZ_PLUS_INLINE
	DynShrLocks(boost::shared_mutex& _m1, boost::shared_mutex& _m2, boost::shared_mutex& _m3, 
				DynShrLocks::LockType _lockType=LT_LOCK) 
		: m_shrMutexs(3)
		, m_lockType(_lockType)
	{
		m_shrMutexs[0] = &_m1;
		m_shrMutexs[1] = &_m2;
		m_shrMutexs[2] = &_m3;
		initLock();
	}

	ORZ_PLUS_INLINE
	DynShrLocks(boost::shared_mutex& _m1, boost::shared_mutex& _m2, boost::shared_mutex& _m3, 
				boost::shared_mutex& _m4, DynShrLocks::LockType _lockType=LT_LOCK) 
		: m_shrMutexs(4)
		, m_lockType(_lockType)
	{
		m_shrMutexs[0] = &_m1;
		m_shrMutexs[1] = &_m2;
		m_shrMutexs[2] = &_m3;
		m_shrMutexs[3] = &_m4;
		initLock();
	}

	ORZ_PLUS_INLINE
	DynShrLocks(boost::shared_mutex& _m1, boost::shared_mutex& _m2, boost::shared_mutex& _m3, 
				boost::shared_mutex& _m4, boost::shared_mutex& _m5, DynShrLocks::LockType _lockType=LT_LOCK) 
		: m_shrMutexs(5)
		, m_lockType(_lockType)
	{
		m_shrMutexs[0] = &_m1;
		m_shrMutexs[1] = &_m2;
		m_shrMutexs[2] = &_m3;
		m_shrMutexs[3] = &_m4;
		m_shrMutexs[4] = &_m5;
		initLock();
	}

	ORZ_PLUS_INLINE
	DynShrLocks(boost::shared_mutex& _m1, boost::shared_mutex& _m2, boost::shared_mutex& _m3, 
				boost::shared_mutex& _m4, boost::shared_mutex& _m5, boost::shared_mutex& _m6, 
				DynShrLocks::LockType _lockType=LT_LOCK) 
		: m_shrMutexs(6)
		, m_lockType(_lockType)
	{
		m_shrMutexs[0] = &_m1;
		m_shrMutexs[1] = &_m2;
		m_shrMutexs[2] = &_m3;
		m_shrMutexs[3] = &_m4;
		m_shrMutexs[4] = &_m5;
		m_shrMutexs[5] = &_m6;
		initLock();
	}

	ORZ_PLUS_INLINE
	DynShrLocks(boost::shared_mutex& _m1, boost::shared_mutex& _m2, boost::shared_mutex& _m3, 
				boost::shared_mutex& _m4, boost::shared_mutex& _m5, boost::shared_mutex& _m6, 
				boost::shared_mutex& _m7, DynShrLocks::LockType _lockType=LT_LOCK) 
		: m_shrMutexs(7)
		, m_lockType(_lockType)
	{
		m_shrMutexs[0] = &_m1;
		m_shrMutexs[1] = &_m2;
		m_shrMutexs[2] = &_m3;
		m_shrMutexs[3] = &_m4;
		m_shrMutexs[4] = &_m5;
		m_shrMutexs[5] = &_m6;
		m_shrMutexs[6] = &_m7;
		initLock();
	}

	ORZ_PLUS_INLINE
	DynShrLocks(boost::shared_mutex& _m1, boost::shared_mutex& _m2, boost::shared_mutex& _m3, 
				boost::shared_mutex& _m4, boost::shared_mutex& _m5, boost::shared_mutex& _m6, 
				boost::shared_mutex& _m7, boost::shared_mutex& _m8, DynShrLocks::LockType _lockType=LT_LOCK) 
		: m_shrMutexs(8)
		, m_lockType(_lockType)
	{
		m_shrMutexs[0] = &_m1;
		m_shrMutexs[1] = &_m2;
		m_shrMutexs[2] = &_m3;
		m_shrMutexs[3] = &_m4;
		m_shrMutexs[4] = &_m5;
		m_shrMutexs[5] = &_m6;
		m_shrMutexs[6] = &_m7;
		m_shrMutexs[7] = &_m8;
		initLock();
	}

	ORZ_PLUS_INLINE
	DynShrLocks(boost::shared_mutex& _m1, boost::shared_mutex& _m2, boost::shared_mutex& _m3, 
				boost::shared_mutex& _m4, boost::shared_mutex& _m5, boost::shared_mutex& _m6, 
				boost::shared_mutex& _m7, boost::shared_mutex& _m8, boost::shared_mutex& _m9, 
				DynShrLocks::LockType _lockType=LT_LOCK) 
		: m_shrMutexs(9)
		, m_lockType(_lockType)
	{
		m_shrMutexs[0] = &_m1;
		m_shrMutexs[1] = &_m2;
		m_shrMutexs[2] = &_m3;
		m_shrMutexs[3] = &_m4;
		m_shrMutexs[4] = &_m5;
		m_shrMutexs[5] = &_m6;
		m_shrMutexs[6] = &_m7;
		m_shrMutexs[7] = &_m8;
		m_shrMutexs[8] = &_m9;
		initLock();
	}

	ORZ_PLUS_INLINE
	DynShrLocks(boost::shared_mutex& _m1, boost::shared_mutex& _m2, boost::shared_mutex& _m3, 
				boost::shared_mutex& _m4, boost::shared_mutex& _m5, boost::shared_mutex& _m6, 
				boost::shared_mutex& _m7, boost::shared_mutex& _m8, boost::shared_mutex& _m9, 
				boost::shared_mutex& _m10, DynShrLocks::LockType _lockType=LT_LOCK) 
		: m_shrMutexs(10)
		, m_lockType(_lockType)
	{
		m_shrMutexs[0] = &_m1;
		m_shrMutexs[1] = &_m2;
		m_shrMutexs[2] = &_m3;
		m_shrMutexs[3] = &_m4;
		m_shrMutexs[4] = &_m5;
		m_shrMutexs[5] = &_m6;
		m_shrMutexs[6] = &_m7;
		m_shrMutexs[7] = &_m8;
		m_shrMutexs[8] = &_m9;
		m_shrMutexs[9] = &_m10;
		initLock();
	}

	ORZ_PLUS_INLINE
	DynShrLocks(const std::vector<boost::shared_mutex*>& _ms, DynShrLocks::LockType _lockType=LT_LOCK) 
		: m_shrMutexs(_ms.size())
		, m_lockType(_lockType)
	{
		std::vector<boost::shared_mutex*>::const_iterator itr = _ms.begin();
		for (uint32 i=0; itr!=_ms.end(); ++itr, ++i)
		{
			m_shrMutexs[i] = *itr;
		}
		initLock();
	}

	ORZ_PLUS_INLINE
	virtual ~DynShrLocks()
	{
		uninitLock();
	}

public:
	ORZ_PLUS_INLINE
	void		lock				(int32 _index=-1)
	{
		if (m_lockType == LT_DELAY)
		{
			if (_index >= 0 && _index < m_shrMutexs.size())
			{
				m_shrMutexs[_index]->lock();
			}
			else
			{
				for (uint32 i=0; i<m_shrMutexs.size(); ++i)
				{
					m_shrMutexs[i]->lock();
				}
			}
		}
	}

	ORZ_PLUS_INLINE
	bool		tryLock				(int32 _index=-1)
	{
		if (m_lockType == LT_DELAY)
		{
			if (_index >= 0 && _index < m_shrMutexs.size())
			{
				return m_shrMutexs[_index]->try_lock();
			}
			else
			{
				for (uint32 i=0; i<m_shrMutexs.size(); ++i)
				{
					m_shrMutexs[i]->try_lock();
				}
				return true;
			}
		}
		else
		{
			return false;
		}
	}

	ORZ_PLUS_INLINE
	void		unlock				(int32 _index=-1)
	{
		if (_index >= 0 && _index < m_shrMutexs.size())
		{
			m_shrMutexs[_index]->unlock();
		}
		else
		{
			for (uint32 i=0; i<m_shrMutexs.size(); ++i)
			{
				m_shrMutexs[i]->unlock();
			}
		}
	}

	ORZ_PLUS_INLINE
	void		lockShared			(int32 _index=-1)
	{
		if (m_lockType == LT_DELAY)
		{
			if (_index >= 0 && _index < m_shrMutexs.size())
			{
				m_shrMutexs[_index]->lock_shared();
			}
			else
			{
				for (uint32 i=0; i<m_shrMutexs.size(); ++i)
				{
					m_shrMutexs[i]->lock_shared();
				}
			}
		}
	}

	ORZ_PLUS_INLINE
	bool		tryLockShared		(int32 _index=-1)
	{
		if (m_lockType == LT_DELAY)
		{
			if (_index >= 0 && _index < m_shrMutexs.size())
			{
				return m_shrMutexs[_index]->try_lock_shared();
			}
			else
			{
				for (uint32 i=0; i<m_shrMutexs.size(); ++i)
				{
					m_shrMutexs[i]->try_lock_shared();
				}
				return true;
			}
		}
		else
		{
			return false;
		}
	}

	ORZ_PLUS_INLINE
	void		unlockShared		(int32 _index=-1)
	{
		if (_index >= 0 && _index < m_shrMutexs.size())
		{
			m_shrMutexs[_index]->unlock_shared();
		}
		else
		{
			for (uint32 i=0; i<m_shrMutexs.size(); ++i)
			{
				m_shrMutexs[i]->unlock_shared();
			}
		}
	}

private:
	ORZ_PLUS_INLINE
	void		initLock			()
	{
		switch (m_lockType)
		{
		case LT_LOCK:			
			{
				for (uint32 i=0; i<m_shrMutexs.size(); ++i)
				{
					m_shrMutexs[i]->lock();				
				}
			}break;
		case LT_TRY_LOCK:		
			{
				for (uint32 i=0; i<m_shrMutexs.size(); ++i)
				{
					m_shrMutexs[i]->try_lock();				
				}
			}break;
		case LT_LOCK_SHR:		
			{
				for (uint32 i=0; i<m_shrMutexs.size(); ++i)
				{
					m_shrMutexs[i]->lock_shared();				
				}
			}break;
		case LT_TRY_LOCK_SHR:	
			{
				for (uint32 i=0; i<m_shrMutexs.size(); ++i)
				{
					m_shrMutexs[i]->try_lock_shared();				
				}
			}break;
		default:		
			break;
		}
	}

	ORZ_PLUS_INLINE
	void		uninitLock			()
	{
		switch (m_lockType)
		{
		case LT_LOCK:
		case LT_TRY_LOCK:		
			{
				for (uint32 i=0; i<m_shrMutexs.size(); ++i)
				{
					m_shrMutexs[i]->unlock();		
				}
			}break;
		case LT_LOCK_SHR:		
		case LT_TRY_LOCK_SHR:	
			{
				for (uint32 i=0; i<m_shrMutexs.size(); ++i)
				{
					m_shrMutexs[i]->unlock_shared();		
				}
			}break;
		default:		
			break;
		}
	}

private:
	mutable DynArray<boost::shared_mutex*>	m_shrMutexs;
	const LockType							m_lockType;
};
}
#endif
