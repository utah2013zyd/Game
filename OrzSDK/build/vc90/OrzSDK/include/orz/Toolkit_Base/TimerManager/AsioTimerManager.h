
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
	
	///���£����룩���ε��õ�ʱ��������λ���룩�����ڴ�����ʱ���¼���
	bool update(TimeType interval);

	///����һ����ʱ����������һ��boost::function�º������󣬷���ֵ��Ҫ���������ݡ�
	TimerPtr createTimer(const TimerFunctorReturnBool & fun);
private:
	boost::scoped_ptr<boost::asio::io_service> _io;
	boost::shared_ptr<Timer> _timer;
	
};



}



#endif
