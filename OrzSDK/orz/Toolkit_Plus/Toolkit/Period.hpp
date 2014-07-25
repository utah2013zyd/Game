#ifndef __Orz_ToolkitPlus_Period_hpp_
#define __Orz_ToolkitPlus_Period_hpp_
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
namespace Orz
{
// 时间单位：微秒。1秒 = 1,000,000 微秒
class Period
{
public:
	ORZ_PLUS_INLINE
	Period()
		: m_durationTime(0)
		, m_isStart(false)
		, m_startTime(now())
		, m_hasLoopHdr(false)
	{
	}

	ORZ_PLUS_INLINE
	Period(const ClockTime _durationTime)
		: m_durationTime(_durationTime)
		, m_isStart(false)
		, m_startTime(now())
		, m_hasLoopHdr(false)
	{
	}

	ORZ_PLUS_INLINE
	~Period()
	{
	}

public:
	//!
	ORZ_PLUS_INLINE 
	void				setDuration			(const ClockTime _durationTime)
	{
		m_durationTime = _durationTime;
	}

	//!
	ORZ_PLUS_INLINE 
	int32				start					
	(
		// arg1: 总的定时时间; arg2: 已经经过的时间; arg3: dt(last invoke - now)
		const boost::function<void (ClockTime, ClockTime, ClockTime)>& _loopHandler, 
		// arg1: 已经经过的时间; arg2: dt(last invoke - now); arg3: 是否正常结束（false表示提前结束，因为某种原因中断）
		const boost::function<void (ClockTime, ClockTime, bool)>& _finishHandler 
	)
	{
		m_hasLoopHdr = true;
		m_loopHandler = _loopHandler;
		m_finishHandler = _finishHandler;
		m_startTime = now();
		m_currentTime = m_startTime;
		m_isStart = true;
		return 0;
	}

	//!
	ORZ_PLUS_INLINE 
	void				start					
	(
		// arg1: 已经经过的时间; arg2: dt(last invoke - now); arg3: 是否正常结束（false表示提前结束，因为某种原因中断）
		const boost::function<void (ClockTime, ClockTime, bool)>& _finishHandler
	)
	{
		m_hasLoopHdr = false;
		m_finishHandler = _finishHandler;
		m_startTime = now();
		m_currentTime = m_startTime;
		m_isStart = true;
	}

	//!
	ORZ_PLUS_INLINE 
	void				run						()
	{
		
		ClockTime currTime = now();
		ClockTime pastTime = currTime - m_startTime;
		ClockTime dt = currTime - m_currentTime;
		m_currentTime = currTime;

		if (m_isStart)
		{
			if (m_hasLoopHdr)
				m_loopHandler(m_durationTime, pastTime, dt);

			if (m_isStart && (pastTime >= m_durationTime || fabs((float64)(m_durationTime - pastTime)) < (float64)dt / 2.0f))
			{
				m_isStart = false;
				m_finishHandler(pastTime, dt, false);
			}
		}
	}

	//!
	ORZ_PLUS_INLINE 
	void				stop					()
	{
		if (m_isStart)
		{
			m_isStart = false;
			ClockTime currTime = now();
			ClockTime pastTime = currTime - m_startTime;
			ClockTime dt = currTime - m_currentTime;
			m_currentTime = currTime;
			m_finishHandler(pastTime, dt, true);
		}
	}

	//!
	ORZ_PLUS_INLINE 
	bool				isStart					() const
	{
		return m_isStart;
	}

public:
	ClockTime										m_durationTime;
	bool											m_isStart;
	ClockTime										m_startTime;
	ClockTime										m_currentTime;

	bool											m_hasLoopHdr;
	boost::function<void (ClockTime, ClockTime, ClockTime)>		
													m_loopHandler;
	boost::function<void (ClockTime, ClockTime, bool)>		
													m_finishHandler;
};

}

#endif
