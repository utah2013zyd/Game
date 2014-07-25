#include "TimerManager/SimpleTimer.h"
using namespace Orz;


bool SimpleTimer::start(TimeType interval, uint32 times, TimeType start)
{
	
	_interval = interval;
	_times = times;
	_other = -start + interval;
	setEnable(true);
	return true;

}

void SimpleTimer::stop(void)
{
	setEnable(false);
}


SimpleTimer::SimpleTimer( const TimerFunctorReturnBool & fun)
	: _fun(fun)
	, _interval(0.f)
	, _times(0)
	, _other(0.f)
	,_speed(1.f)
{
}


bool SimpleTimer::update(TimeType interval)
{
	interval *= _speed;

	TimeType timePass = interval + _other;
	while(timePass > _interval)
	{
		timePass -= _interval;
		if(!(_fun()))
			return false;

		if(_times)
		{
			--_times;
			if(!_times)
			{
				return false;
			}
		}
	}
	_other = timePass;
	return true;
}

SimpleTimer::~SimpleTimer()
{
}

