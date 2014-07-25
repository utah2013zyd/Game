
#ifndef __Orz_Example4_EventData__
#define __Orz_Example4_EventData__
#include <orz/Toolkit_Base/EventSystem.h>




class MyEventHandler4_1:public Orz::EventHandler
{
public:

	virtual void doExecute(Orz::Event * evt)
	{

		using namespace Orz;
		
		if(evt->getMsg() == 1)
		{
			
			std::cout<<"MyEventHandler4_1 receive A Event, And send another Event to MyEventHandler4_2."<<std::endl;
			Event * e = EventFactory::getInstance().createEvent();
			e->setMsg(2);
			e->setSender(shared_from_this());
			
			EventHandlerPtr handler;
			try
			{
				handler = evt->getData<EventHandlerPtr>();
			}catch(...)
			{
				//todo ...
			}
			e->setReceiver(handler);
			getWorld()->broadcasting(e);
		}
		else if(evt->getMsg() == 3)
		{
			std::cout<<"MyEventHandler4_1 receive a Event"<<std::endl;
		}
	}
};
class MyEventHandler4_2:public Orz::EventHandler
{
public:

	virtual void doExecute(Orz::Event * evt)
	{
		using namespace Orz;
		if(evt->getMsg() == 2)
		{
			std::cout<<"MyEventHandler4_2 Receive A Event, And send another Event to Event's Announcer(MyEventHandler4_1)."<<std::endl;
			Event * e = EventFactory::getInstance().createEvent();
			e->setMsg(3);
			e->setSender(shared_from_this());
			e->setReceiver(evt->getSender());
			getWorld()->broadcasting(e);
		}
	}
};


void Example4_EventData(void)
{

	using namespace Orz;
	std::cout<<"=== Example4 EventData Begin ==="<<std::endl;


	
	EventWorld world;
	EventHandlerPtr handler1(new MyEventHandler4_1());
	EventHandlerPtr handler2(new MyEventHandler4_2());
	world.init();
	world.comeIn(handler1);
	world.comeIn(handler2);

	
	world.update();

	Event * evt = NULL;

	evt = EventFactory::getInstance().createEvent();
	
	evt->setMsg(1);
	evt->setReceiver(handler1);
	evt->setData<EventHandlerPtr>(handler2);
	world.broadcasting(evt);
	std::cout<<"Send A Event to MyEventHandler4_1"<<std::endl;


	
	
	world.update();
	world.update();
	world.update();
	world.update();
	world.update();
	world.update();

	world.goOut(handler1);
	world.goOut(handler2);
	world.shutdown();

	std::cout<<"=== Example4 EventData End ==="<<std::endl<<std::endl<<std::endl;
}


#endif

