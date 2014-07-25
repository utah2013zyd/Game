#include "TutorialDirector.h"

#include <orz/Toolkit_Base/Global.h>
#include <orz/Framework_Base/System.h>
#include <orz/Toolkit_Base/TimerManager.h>
#include <orz/Toolkit_Base/DynLibManager.h>
#include <orz/Toolkit_Base/LogManager.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/View_OGRE3D/CEGUIManager.h>
#include <orz/View_OIS/OISInputManager.h>
#include <orz/View_Fmod/FmodSoundManager.h>


//Tutorial.cpp 省略一些代码

int main(int argc, char **argv)
{
    using namespace Orz;
    //创建一个导演工厂
    TutorialDirectorFactory * director_factory = new Orz::TutorialDirectorFactory();
    //把导演工厂注册到游戏工厂中
    GameFactories::getInstance().addDirectorFactory(director_factory); 

    //配置所有子系统，这里面分别是：日志管理器、计时器（调度器）管理器、Ogre图形管理器、OIS输入管理器、Fmod音频管理器、插件管理器
    SystemPtr system(new SystemList<boost::mpl::list<SimpleLogManager, SimpleTimerManager, OgreGraphicsManager, OISInputManager, FmodSoundManager, PluginsManager> >());

    //创建一个逻辑
    LogicConfiger::LogicFacadePtr logic(new LogicConfiger::LogicFacade());
 
    //通过手动定制地方法，在游戏中配置逻辑
    LogicConfiger::ManualBuilder  builder;
    //载入插件“Model_base”，这里面提供了“TheaterBase”类型大厅的定义。
    builder.addPlugin("Model_Base");
    //创建一个“TheaterBase”类型大厅，命名为"main"。
    builder.setTheater("TheaterBase","main");
    //创建一个刚才我们定义的导演类型“TutorialDirector”，并把实例命名为"Tutorial"。
    builder.addDirector("TutorialDirector","Tutorial");
    //把刚才的导演设置为“激活”，同一时间只能有一个导演被激活。
    builder.setActiveDirector("Tutorial");

    //通过上面信息来构造逻辑。
    logic->building(builder);


    if(system->init())//初始化系统
    {
        if(logic->load())//载入逻辑
        {
            system->run();//运行系统
            logic->unload();//卸载逻辑
        }
        system->shutdown();//关闭系统
    }

    //在最后别忘了把之前的导演工厂给注销掉，然后释放了。
    GameFactories::getInstance().removeDirectorFactory(director_factory); 
    delete director_factory;
}
