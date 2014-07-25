

#ifndef __Orz_Example2_EventSpace__
#define __Orz_Example2_EventSpace__
#include <orz/Toolkit_Base/EventSystem.h>
#include "MyEventSpace.h"



class MyEventHandler2:public Orz::EventHandler
{
public:
	/**
	当接收到消息时候调用，返回值用于提供给派生类了解执行结果
	**/
	virtual void doExecute(Orz::Event * evt)
	{
		using namespace Orz;
		if(MyEventSpace1::in(evt))
		{
			std::cout<<"the Event in MyEventSpace1 "<<std::endl;
			switch(MyEventSpace1::getMsg(evt))
			{
			case MyEventSpace1::MyEventA:
				std::cout<<"and the Event is MyEventSpace1::MyEventA"<<std::endl;
				break;
			}

		}else 
		{
			EventSpace * space = EventFactory::getInstance().findEventSpace(evt);
			if(space != NULL)
			{
				std::cout<<"the Event in "<<space->getName()<<std::endl;

				if(space->equal(evt, MyEventSpace2::MyEvent3))
				{
					std::cout<<"and the Event is MyEventSpace2::MyEvent3"<<std::endl;
				}
			}
		}
	}
};




void Example2_EventSpace(void)
{
	using namespace Orz;


	std::cout<<"=== Example2 EventSpace Begin ==="<<std::endl;

	EventWorld world;


	EventHandlerPtr handler(new MyEventHandler2());
	world.init();
	world.comeIn(handler);
	Event * evt1 = MyEventSpace1::createEvent(MyEventSpace1::MyEventA);
	world.broadcasting(evt1);

	Event * evt2 = Orz::EventFactory::getInstance().getEventSpace("MyEventSpace2")->createEvent(3/*即MyEvent3*/);
	world.broadcasting(evt2);

	world.update();
	world.update();
	world.goOut(handler);
	world.shutdown();

	std::cout<<"=== Example2 EventSpace End ==="<<std::endl<<std::endl<<std::endl;


}


#endif

