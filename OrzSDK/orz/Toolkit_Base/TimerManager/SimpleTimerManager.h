
#ifndef __Orz_ToolkitBase_SimpleTimerManager_h__
#define __Orz_ToolkitBase_SimpleTimerManager_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/Singleton.h>
#include <orz/Toolkit_Base/TimerManager/ITimerManager.h>
namespace Orz{
class SimpleTimer;
class _OrzToolkitBaseExport SimpleTimerManager : public ITimerManager
{
	
public:
	typedef std::vector<SimpleTimer *> TimerList;

	~SimpleTimerManager(void);
	SimpleTimerManager(void);
	
	///更新（传入）两次调用的时间间隔（单位：秒），用于触发定时器事件。
	bool update(TimeType interval);

	///创建一个定时器，参数是一个boost::function仿函数对象，返回值需要布尔型数据。
	TimerPtr createTimer(const TimerFunctorReturnBool & fun);
private:
	void addList(void);
	void deleteList(void);
	TimerList _deleteList;
	TimerList _addList;
	TimerList _timerList;
};



}




//\
//SYSTEMUNIT_FACTORY(Interface, TimerManager)

#endif
