#ifndef __Orz_ToolkitPlus_PeriodicTimer_hpp_
#define __Orz_ToolkitPlus_PeriodicTimer_hpp_
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Toolkit/ITimer.hpp>
#include <orz/Toolkit_Plus/Toolkit/Framer.hpp>
#include <orz/Toolkit_Plus/Agent/Agent.h>
namespace Orz
{
//! �û�����ֱ��ʹ�ã���PTimerMgr
class PeriodicTimer
	: public ITimer
{
public:
	ORZ_PLUS_INLINE
	PeriodicTimer(TmrId _id)
		: ITimer(_id)
		, m_startTime(now())
		, m_loop(0)
		, m_lock(m_mutex, DynShrLock::LT_DELAY)
	{
	}

	ORZ_PLUS_INLINE
	~PeriodicTimer()
	{
		m_lock.unlock();
	}

public:
	ORZ_PLUS_INLINE
	void		start			(uint32 _time, PtrIntType _data, const boost::function<void (TmrId, PtrIntType)>& _hdr, int32 _loop)
	{
		if (!m_lock.tryLock())
		{
			return;
		}

		m_time = _time*1000;
		m_data = _data;
		m_hdr = _hdr;
		m_loop = _loop;
		m_startTime = now();
		m_currentTime = m_startTime;
	}

	ORZ_PLUS_INLINE 
	void		run				()
	{
		if (m_loop == 0)
		{
			m_lock.unlock();
			return;
		}

		uint32 currTime = now();
		uint32 pastTime = currTime - m_startTime;
		uint32 dt = currTime - m_currentTime;
		m_currentTime = currTime;

		if ((pastTime >= m_time || fabs((float64)(m_time - pastTime)) < (float64)dt / 2.0f))
		{
			m_hdr(ITimer::getId(), m_data);
			if (m_loop > 0)
			{
				--m_loop;
			}

			if (m_loop == 0)
			{
				m_lock.unlock();
				return;
			}
			m_startTime = now();
			m_currentTime = m_startTime;
		}
	}

	ORZ_PLUS_INLINE
	void		stop			()
	{
		m_loop = 0;
	}

private:
	uint32										m_time;
	uint32										m_startTime;
	uint32										m_currentTime;
	PtrIntType									m_data;
	boost::function<void (TmrId, PtrIntType)>	m_hdr;
	int32										m_loop;
	boost::shared_mutex							m_mutex;
	DynShrLock									m_lock;
};

//! �û�����ֱ��ʹ�ã���PTimerMgr
class PTThread
	: public ITimerThread
{
public:
	PTThread(uint32 _dt, ThreadShrMap<TmrId, ITimer*>& _timers)
		: ITimerThread(_timers)
		, m_dt(_dt)
		, m_isRunning(false)
	{
	}

	~PTThread()
	{
	}

public:
	bool		start			()
	{
		if (m_agent.start() != 0)
		{
			return false;
		}
		m_framer.start(m_dt);
		m_isRunning = true;
		m_agent.post(boost::bind(&PTThread::run, this));
		return true;
	}

	void		run				()
	{
		while (m_isRunning)
		{
			m_framer.control();
			DynShrLock lock(ITimerThread::m_timers.mutex());
			std::map<TmrId, ITimer*>& m = ITimerThread::m_timers.map();
			std::map<TmrId, ITimer*>::iterator itr = m.begin();
			for (; itr!=m.end(); ++itr)
			{
				if (itr->second)
				{
					itr->second->run();
				}
			}
		}
	}

	void		stop			()
	{
		m_isRunning = false;
		m_agent.stop();
		m_agent.interrupt();
	}

	void		join			()
	{
		m_agent.join();
	}

private:
	uint32				m_dt;
	Agent				m_agent;
	bool				m_isRunning;
	Framer				m_framer;
};

//! periodic timer�Ĺ�����
/*!
 *	\note �̰߳�ȫ�����ڶ��߳��µļ�ʱ������
 *	\note ���Ը���ϵͳ��������ָ�����ڲ��̵߳����������ǲ�ͬ�߳����еĲ�ͬTimer����ص������ڲ��������ͬ�����ݣ�\n
 *		����ڹ�����Ǹ����ݣ������̰߳�ȫ�ģ���Ҫ�û����д���
 *	\note �̳���ITimerMgr
 *	\note ���ȸ���DeadlineTimer������Ӧ������ʱ���ĵ���Դ�϶�
 example:
 \code
	void timerHdr(TmrId _tid, int32 _data)
	{
	}

	PTimerMgr tmrMgr(10, 10, 100, 50000);	// ������һ��10���̣߳�������10x100��Timer��ͬʱ��ʱ���Ϊ50ms��PTimerMgr
	tmrMgr.boost();
	TmrId tid = tmrMgr.createTimer();

	tmrMgr.startTimer(tid, 1000, 1, boost::bind(&timerHdr, tid, 1), 5);	// ��ʼһ��idΪtid��1�롢��������1���ݵ�ѭ��5�ε�timer
	...

	tmrMgr.shutdown();		// �������һ���̻߳���ͬһ���߳̾���
 \endcode
 */
class PTimerMgr
	: public ITimerMgr
{
public:
	ORZ_PLUS_INLINE
	PTimerMgr(uint32 _threadPoolSize, uint32 _maxTimerClr, uint32 _maxTimerPer, uint32 _dt)
		: ITimerMgr(_threadPoolSize, _maxTimerClr, _maxTimerPer)
	{
		std::vector<ITimerThread*> tmrThreads;
		for (uint32 i=0; i<_threadPoolSize; ++i)
		{
			tmrThreads.push_back(new PTThread(_dt, ITimerMgr::m_timers.threadMap(i)));
		}
		ITimerMgr::initialize(tmrThreads);
	}

	ORZ_PLUS_INLINE
	~PTimerMgr()
	{
		uint32 size = ITimerMgr::m_timers.arraySize();
		for (uint32 i=0; i<size; ++i)
		{
			ThreadShrMap<TmrId, ITimer*>& tMap = ITimerMgr::m_timers.threadMap(i);
			DynShrLock lock(tMap.mutex());
			std::map<TmrId, ITimer*>& m = tMap.map();
			std::map<TmrId, ITimer*>::iterator itr = m.begin();
			for (; itr!=m.end(); ++itr)
			{
				if (itr->second)
				{
					delete itr->second;
					itr->second = 0;
				}
			}
			m.clear();
		}

		for (uint32 i=0; i<ITimerMgr::m_tmrThreads.size(); ++i)
		{
			ITimerThread* ptr = ITimerMgr::m_tmrThreads[i];
			if (ptr)
			{
				delete ptr;
				ITimerMgr::m_tmrThreads[i] = 0;
			}
		}
	}

public:
	ORZ_PLUS_INLINE
	TmrId		createTimer			()
	{
		TmrId tid = ITimerMgr::m_idr.generate();
		if (m_timers.add(tid, new PeriodicTimer(tid), tid) != 0)
		{
			return TmrId();
		}
		return tid;
	}
};
}
#endif
