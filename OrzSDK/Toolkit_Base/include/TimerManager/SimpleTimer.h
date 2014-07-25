#ifndef __Orz_ToolkitBase_SimpleTimer_h__
#define __Orz_ToolkitBase_SimpleTimer_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/TimerManager/Timer.h>

namespace Orz{
class _OrzToolkitBasePrivate SimpleTimer:  public Timer
{
public:

	///开始定时器运行，参数分别为调用间隔、调用次数、开始时间
	virtual bool start(TimeType interval, uint32 times = 0, TimeType start = 0);

	

	///强制停止定时器运行
	virtual void stop(void);
	
	
	SimpleTimer(const TimerFunctorReturnBool & fun);
	virtual ~SimpleTimer(void);
	
	bool update(TimeType interval);
	inline void setEnable(bool enable){ _enable = enable; }

	///设置运行速率，参数将会成为一个乘数调节运行速度
	void setSpeed(float speed){_speed = speed;}

	///得到运行速率
	float getSpeed(void) const{return _speed;}



private:


	float _speed;
	TimerFunctorReturnBool _fun;
	TimeType _interval;
	uint32 _times;
	TimeType _other;


};

}
#endif
