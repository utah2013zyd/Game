//  (C) Copyright Gennadiy Rozental 2005-2008.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

// Boost.Test

#include "Config.h"
#include "DemoActor.h"
#include "DemoDirector.h"
#include <orz/Framework_Base/Logic.h>
int main(int argc, char **argv)
{
    using namespace Orz;
    //创建一个导演工厂
    DemoDirectorFactory * director_factory = new Orz::DemoDirectorFactory();
    DemoActorFactory * actor_factory = new Orz::DemoActorFactory();
    //把导演工厂注册到游戏工厂中
    GameFactories::getInstance().addFactory(director_factory); 
    GameFactories::getInstance().addFactory(actor_factory); 

    //配置所有子系统，这里面分别是：日志管理器、计时器（调度器）管理器、Ogre图形管理器、OIS输入管理器、Fmod音频管理器、插件管理器
    SystemPtr system(new SystemList<boost::mpl::list<SimpleLogManager, SimpleTimerManager, OgreGraphicsManager, OISInputManager, PluginsManager> >());

    //创建一个逻辑
    LogicConfiger::LogicFacadePtr logic(new LogicConfiger::LogicFacade());
 
    //通过手动定制地方法，在游戏中配置逻辑
    LogicConfiger::ManualBuilder  builder;
    //载入插件“Model_base”，这里面提供了“TheaterBase”类型大厅的定义。
    builder.addPlugin("Model_Base");
    //创建一个“TheaterBase”类型大厅，命名为"main"。
    builder.setTheater("TheaterBase","main");
    //创建一个刚才我们定义的导演类型“TutorialDirector”，并把实例命名为"Tutorial"。
    builder.addDirector("DemoDirector","Demo");
    //把刚才的导演设置为“激活”，同一时间只能有一个导演被激活。
    builder.setActiveDirector("Demo");

    //通过上面信息来构造逻辑。
    logic->building(builder);

	try
	{
    if(system->init())//初始化系统
    {
        if(logic->load())//载入逻辑
        {
            system->run();//运行系统
            logic->unload();//卸载逻辑
        }
        system->shutdown();//关闭系统
    }
	}
	catch(std::exception & e)
	{
		std::cout<<e.what()<<std::endl;
	}
    //在最后别忘了把之前的导演工厂给注销掉，然后释放了。
    GameFactories::getInstance().removeFactory(director_factory); 
    GameFactories::getInstance().removeFactory(actor_factory); 
    delete director_factory;
	delete actor_factory;
}
