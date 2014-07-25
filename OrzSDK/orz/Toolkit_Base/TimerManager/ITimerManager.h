
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
	///���£����룩���ε��õ�ʱ��������λ���룩�����ڴ�����ʱ���¼���
	virtual bool update(TimeType interval) = 0;
	///����һ����ʱ����������һ��boost::function�º������󣬷���ֵ��Ҫ���������ݡ�
	virtual TimerPtr createTimer(const TimerFunctorReturnBool & fun) = 0;
	/*inline TimerPtr createTimerWithNoReturnFun(const TimerFunNoReturn & fun)
	{
		return createTimer(static_cast<TimerFunctorReturnBool>(TimerFunNoReturn2ReturnBool(fun)));
	}*/

	virtual ~ITimerManager(void){}
};
}

#endif