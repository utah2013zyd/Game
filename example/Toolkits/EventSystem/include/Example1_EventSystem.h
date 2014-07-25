#ifndef __Orz_Example1_EventSystem__
#define __Orz_Example1_EventSystem__
#include <orz/Toolkit_Base/EventSystem.h>


class MyEventHandler:public Orz::EventHandler
{
public:
	/**
	当接收到消息时候调用，返回值用于提供给派生类了解执行结果
	**/
	virtual void doExecute(Orz::Event * evt)
	{
		std::cout<<"bool MyEventHander::doExecute(Orz::Event * evt)"<<std::endl;
		
	}
	
	/**当就进入EventWorld管理之后调用，返回值代表时候成功**/
	virtual void doEnable(void)
	{
		std::cout<<"bool MyEventHander::doEnable(void)"<<std::endl;

	}

	/**当离开EventWorld管理之后调用**/
	virtual void doDisable(void)
	{
		std::cout<<"void MyEventHander::doDisable(void)"<<std::endl;
	}

	/**当通过enableUpdate方法开启之后，会在每一次EventWorld更新之后调用，通过disableUpdate方法关闭
		当返回false时候，系统退出。
	**/
	virtual void doFrame(void)
	{
		std::cout<<"bool MyEventHander::doFrame(void)"<<std::endl;

	}
};

void Example1_EventSystem(void)
{
	using namespace Orz;
	


	
	std::cout<<"=== Example1 EventSystem Begin ==="<<std::endl;

	EventWorld world;//创建消息世界（主管理器）
	EventHandlerPtr handler(new MyEventHandler());//消息处理器，注意这里需要智能指针。
	world.init();//初始化
	world.comeIn(handler);//消息处理器进入
	handler->enableUpdate();//打开更新，否则将不会调用doFrame
	Event * evt = EventFactory::getInstance().createEvent();//创建一个消息
	evt->setMsg(1981);//设置消息标示
	evt->setData<int>(1112);//设置数据（任意类型）
	
	world.broadcasting(evt);//广播消息
	world.update();//更新世界
	world.update();
	world.update();
	world.goOut(handler);//消息管理器退出
	world.shutdown();//关闭世界


	
	std::cout<<"=== Example1 EventSystem End ==="<<std::endl<<std::endl<<std::endl;
}


#endif