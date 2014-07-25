#ifndef __Orz_ToolkitPlus_ITimer_hpp_
#define __Orz_ToolkitPlus_ITimer_hpp_
#include <orz/Toolkit_Plus/Toolkit/Agency.hpp>
#include <orz/Toolkit_Plus/Toolkit/FastIdrCluster.hpp>
#include <orz/Toolkit_Plus/Toolkit/HashMap.hpp>
namespace Orz
{
//! timer的接口类
/*!
 *	\note 允许用户自行继承来实现自己的Timer，
 *	\note DeadlineTimer和PeriodicTimer均继承自此类
 */
class ITimer
{
public:
	ORZ_PLUS_INLINE
	ITimer(TmrId _id) 
		: m_id(_id)
	{
	}

	ORZ_PLUS_INLINE
	virtual ~ITimer()
	{
	}

public:
	virtual 
	void		start			(uint32 _time, PtrIntType _data, const boost::function<void (TmrId, PtrIntType)>& _hdr, int32 _loop)
	{
	}

	virtual
	void		run				()
	{
	}

	virtual 
	void		stop			()
	{
	}

protected:
	ORZ_PLUS_INLINE
	TmrId		getId			()
	{
		return m_id;
	}

private:
	TmrId				m_id;
};

//! 运行timer的线程接口类
/*!
 *	\note 允许用户自行继承来实现自己的运行timer的线程类
 *	\note DLTThread和PTThread均继承自此类
 *	\note 一个ITimerThread负责运行一组Timer
 */
class ITimerThread
{
public:
	ORZ_PLUS_INLINE
	ITimerThread(ThreadShrMap<TmrId, ITimer*>& _timers)
		: m_timers(_timers)
	{
	}

	ORZ_PLUS_INLINE
	virtual ~ITimerThread() 
	{
	}

public:
	virtual 
	bool		start			()
	{
		return false;
	}

	virtual 
	void		stop			()
	{
	}

	virtual 
	void		join			()
	{
	}

protected:
	ThreadShrMap<TmrId, ITimer*>&		m_timers;
};

//! timer的管理接口类
/*!
 *	\note 允许用户自行继承来实现自己的运行timer的管理类
 *	\note DLTimerMgr和PTimerMgr均继承自此类
 */
class ITimerMgr
{
public:
	ITimerMgr(uint32 _threadPoolSize, uint32 _maxTimerClr, uint32 _maxTimerPer)
		: m_tmrThreads(_threadPoolSize == 0 ? 1 : _threadPoolSize, NumChooser(0, (_threadPoolSize == 0 ? 1 : _threadPoolSize) - 1, (uint32)&_maxTimerPer, false))
		, m_timers(_threadPoolSize == 0 ? 1 : _threadPoolSize)
		, m_idr(_maxTimerClr, _maxTimerPer)
	{
		m_timers.setHash();
	}

	virtual ~ITimerMgr()
	{
	}

protected:
	void		initialize			(std::vector<ITimerThread*>& _timerThreadPool)
	{
		for (uint32 i=0; i<_timerThreadPool.size(); ++i)
		{
			m_tmrThreads[i] = _timerThreadPool[i];
		}
	}

public:
	ORZ_PLUS_INLINE
	bool		boost				()
	{
		for (uint32 i=0; i<m_tmrThreads.size(); ++i)
		{
			if (!m_tmrThreads[i]->start())
			{	
				return false;	
			}
		}
		return true;
	}

	ORZ_PLUS_INLINE
	void		shutdown			()
	{
		for (uint32 i=0; i<m_tmrThreads.size(); ++i)
		{
			m_tmrThreads[i]->stop();
		}
	}

	ORZ_PLUS_INLINE
	void		join				()
	{
		for (uint32 i=0; i<m_tmrThreads.size(); ++i)
		{
			m_tmrThreads[i]->join();
		}
	}

public:
	virtual
	TmrId		createTimer			()
	{
		return TmrId();
	}

	ORZ_PLUS_INLINE
	void		deleteTimer			(TmrId _id)
	{
		DynShrLock lock(m_timers.mutex(_id, _id));
		ITimer* tmr = m_timers.get(_id, _id, false);
		if (tmr)
		{
			delete tmr;
		}

		m_timers.erase(_id, _id, false);
		m_idr.release(_id);
	}

	ORZ_PLUS_INLINE
	void		startTimer			(TmrId _id, uint32 _time, PtrIntType _data, const boost::function<void (TmrId, PtrIntType)>& _callback, int32 _loop)
	{
		DynShrLock lock(m_timers.mutex(_id, _id));
		ITimer* tmr = m_timers.get(_id, _id, false);
		if (!tmr)
		{
			return;
		}

		tmr->start(_time, _data, _callback, _loop);
	}

	ORZ_PLUS_INLINE
	void		stopTimer			(TmrId _id)
	{
		DynShrLock lock(m_timers.mutex(_id, _id));
		ITimer* tmr = m_timers.get(_id, _id, false);
		if (!tmr)
		{
			return;
		}

		tmr->stop();
	}

protected:
	Agency<ITimerThread*>			m_tmrThreads;
	HashShrMap<TmrId, ITimer*>		m_timers;
	FastIdrCluster					m_idr;
};
}
#endif
