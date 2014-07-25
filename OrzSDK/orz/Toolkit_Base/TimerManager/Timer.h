
#ifndef __Orz_ToolkitBase_Timer_h__
#define __Orz_ToolkitBase_Timer_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>


namespace Orz{
class _OrzToolkitBaseExport Timer:  private boost::noncopyable
{
public:
	Timer(void):_enable(false){}
	///开始定时器运行，参数分别为调用间隔、调用次数、开始时间
	virtual bool start(TimeType interval, uint32 times = 0, TimeType start = 0) = 0;

	
	///强制停止定时器运行
	virtual void stop(void) = 0;
	
	///设置运行速率，参数将会成为一个乘数调节运行速度
	virtual void setSpeed(float speed) = 0;

	///得到运行速率
	virtual float getSpeed(void) const  = 0;

	///检查是否在运行
	inline bool isEnable(void) const{return _enable;}

	virtual ~Timer(void){}
protected:
	bool _enable;

};

}
#endif
