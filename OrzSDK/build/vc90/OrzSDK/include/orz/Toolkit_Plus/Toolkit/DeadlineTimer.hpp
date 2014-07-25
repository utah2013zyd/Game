#ifndef __Orz_ToolkitPlus_DeadlineTimer_hpp_
#define __Orz_ToolkitPlus_DeadlineTimer_hpp_
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Toolkit/ITimer.hpp>
#include <orz/Toolkit_Plus/Agent/Agent.h>
namespace Orz
{
//! �û�����ֱ��ʹ�ã���DLTimerMgr
class DeadlineTimer
	: public ITimer
{
public:
	ORZ_PLUS_INLINE
	DeadlineTimer(TmrId _id, boost::asio::io_service& _io)
		: ITimer(_id)
		, m_timer(_io)
		, m_loop(0)
		, m_lock(m_mutex, DynShrLock::LT_DELAY)
	{
	}

	ORZ_PLUS_INLINE
	~DeadlineTimer()
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

		m_time = _time;
		m_data = _data;
		m_hdr = _hdr;
		m_loop = _loop;
		m_timer.expires_from_now(boost::posix_time::millisec(m_time));
		m_timer.async_wait(boost::bind(&DeadlineTimer::handler, this));
	}

	void		run				()
	{
	}

	ORZ_PLUS_INLINE
	void		handler			()
	{
		if (m_loop == 0)
		{
			m_lock.unlock();
			return;
		}

		m_hdr(ITimer::getId(), m_data);
		if (m_loop > 0)
		{
			--m_loop;	
		}

		if (m_loop > 0 || m_loop < 0)
		{
			m_timer.expires_at(m_timer.expires_at() + boost::posix_time::millisec(m_time));
			m_timer.async_wait(boost::bind(&DeadlineTimer::handler, this));
		}
		else
		{
			m_lock.unlock();
		}
	}

	ORZ_PLUS_INLINE
	void		stop			()
	{
		m_loop = 0;
	}

private:
	uint32										m_time;
	boost::asio::deadline_timer					m_timer;
	PtrIntType									m_data;
	boost::function<void (TmrId, PtrIntType)>	m_hdr;
	int32										m_loop;
	boost::shared_mutex							m_mutex;
	DynShrLock									m_lock;
};

//! �û�����ֱ��ʹ�ã���DLTimerMgr
class DLTThread
	: public ITimerThread
{
public:
	ORZ_PLUS_INLINE
	DLTThread(boost::asio::io_service& _io, ThreadShrMap<TmrId, ITimer*>& _timers)
		: ITimerThread(_timers)
		, m_workSvc(_io)
		, m_work(new boost::asio::io_service::work(m_workSvc))
	{
	}

	ORZ_PLUS_INLINE
	~DLTThread()
	{
	}

public:
	ORZ_PLUS_INLINE
	bool		start			()
	{
		if (m_workThread)
			return false;

		ThreadShrPtr tptr(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_workSvc)));
		m_workThread = tptr;
		return true;
	}

	ORZ_PLUS_INLINE
	void		stop			()
	{
		m_workSvc.stop();
	}

	ORZ_PLUS_INLINE
	void		join			()
	{
		m_workSvc.join();
	}

private:
	boost::asio::io_service&							m_workSvc;
	boost::scoped_ptr<boost::asio::io_service::work>	m_work;
	ThreadShrPtr										m_workThread;
};

//! deadline timer�Ĺ�����
/*!
 *	\note �̰߳�ȫ�����ڶ��߳��µļ�ʱ������
 *	\note ���Ը���ϵͳ��������ָ�����ڲ��̵߳����������ǲ�ͬ�߳����еĲ�ͬTimer����ص������ڲ��������ͬ�����ݣ�\n
 *		����ڹ�����Ǹ����ݣ������̰߳�ȫ�ģ���Ҫ�û����д���
 *	\note �̳���ITimerMgr
 \code
 example:
	void timerHdr(TmrId _tid, int32 _data)
	{
	}

	DLTimerMgr tmrMgr(10, 10, 100);
	tmrMgr.boost();
	TmrId tid = tmrMgr.createTimer();

	tmrMgr.startTimer(tid, 1000, 1, boost::bind(&timerHdr, tid, 1), 5);	// ��ʼһ��idΪtid��1�롢��������1���ݵ�ѭ��5�ε�timer
	...

	tmrMgr.shutdown();		// �������һ���̻߳���ͬһ���߳̾���
 \endcode
 */
class DLTimerMgr
	: public ITimerMgr
{
public:
	ORZ_PLUS_INLINE
	DLTimerMgr(uint32 _threadPoolSize, uint32 _maxTimerClr, uint32 _maxTimerPer)
		: ITimerMgr(_threadPoolSize, _maxTimerClr, _maxTimerPer)
		, m_ios(_threadPoolSize)
	{
		std::vector<ITimerThread*> tmrThreads;
		for (uint32 i=0; i<_threadPoolSize; ++i)
		{
			tmrThreads.push_back(new DLTThread(m_ios[i], ITimerMgr::m_timers.threadMap(i)));
		}
		ITimerMgr::initialize(tmrThreads);
	}

	ORZ_PLUS_INLINE
	~DLTimerMgr()
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
		if (!tid.isValid())
		{
			return TmrId();
		}

		uint32 i = ITimerMgr::m_tmrThreads.selectId();
		if (m_timers.add(tid, new DeadlineTimer(tid, m_ios[i]), tid) != 0)
		{
			return TmrId();
		}
		return tid;
	}

private:
	DynArray<boost::asio::io_service>	m_ios;
};
}
#endif
