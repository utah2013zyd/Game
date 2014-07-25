//from http://www.cnblogs.com/WuErPIng/archive/2005/09/21/241407.html  (吴尔平's Blog)
#ifndef __Orz_ToolkitBase_Clock_h__
#define __Orz_ToolkitBase_Clock_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>


namespace Orz
{
template <class T = float, int MAX = 0>
class Clock
{
public:
	inline Clock(void): _startTime(boost::posix_time::microsec_clock::local_time()),_maxTime(0,0,MAX,0)
	{
	}

	inline ~Clock(void)
	{
	}

	///时间归零
	inline void restart(void)
	{
		_startTime = boost::posix_time::microsec_clock::local_time();
	}

	///返回逝去的时间
	inline T elapsed(void)
	{
		boost::posix_time::time_duration d = boost::posix_time::microsec_clock::local_time() - _startTime;
		while(d >= _maxTime)
		{
			_startTime += _maxTime;
			d = boost::posix_time::microsec_clock::local_time() - _startTime;
		}
		T result = static_cast<T>(d.ticks());
		return result /= static_cast<T>(d.ticks_per_second());
	}

	///得到elapsed()方法返回的最大值
	inline T elapsed_max(void) const   
	{
		return static_cast<T>(MAX);
	}

	/// 得到elapsed()方法返回的最小值
	inline T elapsed_min(void) const 
	{
		return static_cast<T>(0);
	}


private:
    boost::posix_time::ptime _startTime;
    boost::posix_time::time_duration _maxTime;
};


template<class T>
class Clock<T, 0>
{
public:
	inline Clock(void): _startTime(boost::posix_time::microsec_clock::local_time())
	{
	}

	inline ~Clock(void)
	{
	}

	inline void restart(void)
	{
		_startTime = boost::posix_time::microsec_clock::local_time();
	}

	inline T elapsed(void)
	{
		boost::posix_time::time_duration d = boost::posix_time::microsec_clock::local_time() - _startTime;
		T result = static_cast<T>(d.ticks());
		return result /= static_cast<T>(d.ticks_per_second());
	}

	/// 得到elapsed()方法返回的最大值
	inline T elapsed_max(void) const  
	{
		return static_cast<T>(0);
	}

	/// 得到elapsed()方法返回的最小值
	inline T elapsed_min(void) const 
	{
		return static_cast<T>(0);
	}


private:
    boost::posix_time::ptime _startTime;
};



}
#endif
