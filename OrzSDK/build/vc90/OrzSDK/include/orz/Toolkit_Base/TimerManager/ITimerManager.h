
#ifndef __Orz_ToolkitBase_ITimerManager_h__
#define __Orz_ToolkitBase_ITimerManager_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/Singleton.h>
namespace Orz{

class _OrzToolkitBaseExport ITimerManager: public Singleton<ITimerManager>
{
private:
	/*class TimerFunNoReturn2ReturnBool
	{
	public:
		inline TimerFunNoReturn2ReturnBool(const TimerFunNoReturn & fun):_fun(fun){}
		inline bool operator()(void){_fun();return true;}
	private:
		TimerFunNoReturn _fun;
	};*/
public:
	///更新（传入）两次调用的时间间隔（单位：秒），用于触发定时器事件。
	virtual bool update(TimeType interval) = 0;
	///创建一个定时器，参数是一个boost::function仿函数对象，返回值需要布尔型数据。
	virtual TimerPtr createTimer(const TimerFunctorReturnBool & fun) = 0;
	/*inline TimerPtr createTimerWithNoReturnFun(const TimerFunNoReturn & fun)
	{
		return createTimer(static_cast<TimerFunctorReturnBool>(TimerFunNoReturn2ReturnBool(fun)));
	}*/

	virtual ~ITimerManager(void){}
};
}

#endif