#include <orz/Toolkit_Base/TimerManager.h>
#include <orz/Toolkit_Base/DynLibManager.h>
#include <orz/Toolkit_Base/LogManager.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/View_OIS/OISInputManager.h>

#include <orz/View/ExampleManager.h>
#include <orz/Framework_Base/FrameworkBase.h>

int main(int argc, char **argv)
{
	using namespace Orz;

	//下面通过模板元的方法，编译期绑定所有需要使用的子系统。
	SystemPtr system(new SystemList<boost::mpl::list<SimpleLogManager, SimpleTimerManager, OgreGraphicsManager, OISInputManager, PluginsManager, ExampleManager> >());

	//创建逻辑，这里使用了设计模式中的Façade
	LogicConfiger::LogicFacadePtr logic(new LogicConfiger::LogicFacade());

	//通过Builder设计模式来配置逻辑，在运行期通过插件核工厂模式来动态配置。
	LogicConfiger::ManualBuilder builder;
	builder.addPlugin("Model_Base");
	builder.addPlugin("Model");
	builder.setTheater("TheaterBase","main");
	builder.addDirector("ExampleDirector","example");
	builder.setActiveDirector("example");
	logic->building(builder);

	//游戏流程
	if(system->init())
	{
		if(logic->load())
		{
			system->run();
			logic->unload();
		}
		system->shutdown();
	}
	return 0;
}