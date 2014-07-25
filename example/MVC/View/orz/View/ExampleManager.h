#include "ExampleConfig.h"
#include <orz/Toolkit_Base/Singleton.h>
#include <orz/Framework_Base/System/SystemInterface.h>

namespace Orz
{
	class _OrzViewExport ExampleManager:public Singleton<ExampleManager>
	{
	public:
		ExampleManager(void);
		~ExampleManager(void);
		bool init(void);//可选接口，如果定义的话会被系统初始化时候调用，如果返回false，系统会停止初始化，直接退出
		void shutdown(void);//可选接口，如果定义的话会在系统关闭时候调用。
		bool update(TimeType interval);//可选接口，会在每次更新时候调用，参数是两次调用的时间间隔，返回值如果是false，系统会正常退出。

		void print(void);//这里是一个这个子系统独有的接口，用来实现系统功能。
	};

	//下面的5个宏向框架注册了这个子系统的信息。
	SYSTEMUNIT_BEGIN(ExampleManager) 
	SYSTEMUNIT_INIT 
	SYSTEMUNIT_UPDATE 
	SYSTEMUNIT_SHUTDOWN 
	SYSTEMUNIT_END(ExampleManager) 

}