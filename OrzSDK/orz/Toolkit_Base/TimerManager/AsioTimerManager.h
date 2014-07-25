
#ifndef __Orz_ToolkitBase_AsioTimerManager_h__
#define __Orz_ToolkitBase_AsioTimerManager_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/Singleton.h>
#include <orz/Toolkit_Base/TimerManager/ITimerManager.h>
namespace boost
{
	namespace asio
	{
		class io_service;
	}
}

namespace Orz{
class AsioTimer;
class _OrzToolkitBaseExport AsioTimerManager : public ITimerManager
{
	
public:
	~AsioTimerManager(void);
	AsioTimerManager(void);
	
	///更新（传入）两次调用的时间间隔（单位：秒），用于触发定时器事件。
	bool update(TimeType interval);

	///创建一个定时器，参数是一个boost::function仿函数对象，返回值需要布尔型数据。
	TimerPtr createTimer(const TimerFunctorReturnBool & fun);
private:
	boost::scoped_ptr<boost::asio::io_service> _io;
	boost::shared_ptr<Timer> _timer;
	
};



}



#endif
