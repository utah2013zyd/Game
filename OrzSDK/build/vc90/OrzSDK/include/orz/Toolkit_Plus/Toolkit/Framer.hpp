#ifndef __Orz_ToolkitPlus_Framer_hpp_
#define __Orz_ToolkitPlus_Framer_hpp_
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
namespace Orz
{
class Framer
{
public:
	ORZ_PLUS_INLINE
	Framer()
		: m_time(now())
		, m_start(now())
		, m_sleepTime(0)
		, m_dt(0)
		, m_frameTime(0)
		, m_bEnable(false)
	{
	}

	ORZ_PLUS_INLINE
	Framer(ClockTime _frameTime)
		: m_time(now())
		, m_start(now())
		, m_sleepTime(0)
		, m_dt(0)
		, m_frameTime(_frameTime)
		, m_bEnable(false)
	{
	}

	ORZ_PLUS_INLINE
	~Framer()
	{
	}

public:
	ORZ_PLUS_INLINE 
	void			start			()
	{
		if (m_frameTime == 0)
			return;

		m_time = now();
		m_start = now();
		m_sleepTime = 0;
		m_dt = 0;
		m_bEnable = true;
	}

	ORZ_PLUS_INLINE 
	void			start			(ClockTime _frameTime)
	{
		m_time = now();
		m_start = now();
		m_sleepTime = 0;
		m_dt = 0;
		m_frameTime = _frameTime;
		m_bEnable = true;
	}

	//! 在需要控制frame的代码段之前调用，返回dt时间
	ORZ_PLUS_INLINE 
	ClockTime		control			()
	{
		if (!m_bEnable)
			return 0;

		m_start = now();
		m_dt = m_start - m_time;

		ClockTime et_bt = 0;

		// Framer
		if (m_dt < m_frameTime)
		{
			m_sleepTime = m_frameTime - m_dt;
			if (m_sleepTime > 0)
				boost::this_thread::sleep(boost::posix_time::microsec(m_sleepTime));

			m_time = now();
			m_dt = m_time - m_start + m_dt;
		}
		else
			m_time = now();
		
		return m_dt;
	}

private:
	ClockTime			m_time;
	ClockTime			m_start;
	ClockTime			m_sleepTime;
	ClockTime			m_dt;
	ClockTime			m_frameTime;
	bool				m_bEnable;
	timeval				m_timeout;
};

}

#endif
