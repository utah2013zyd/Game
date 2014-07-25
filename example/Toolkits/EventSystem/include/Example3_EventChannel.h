
#ifndef __Orz_Example3_EventChannel__
#define __Orz_Example3_EventChannel__
#include <orz/Toolkit_Base/EventSystem.h>



class MyEventHandler3_1:public Orz::EventHandler
{
public:
	MyEventHandler3_1()
	{
		setChannel(
			Orz::EventChannel::create()
				.addUserChannel<3>()
				.addUserChannel<8>()
				
				);
	}
	virtual void doExecute(Orz::Event * evt)
	{

		std::cout<<"MyEventHandler3_1 with channel 3 and 8"<<std::endl;
		std::cout<<"Get "<<evt->getData<std::string>()<<std::endl<<"===================="<<std::endl;
	}
};
class MyEventHandler3_2:public Orz::EventHandler
{
public:
	MyEventHandler3_2()
	{
	setChannel(
			Orz::EventChannel::create()
				.addUserChannel<2>()
				.addUserChannel<8>()
				
				);
	}

	virtual void doExecute(Orz::Event * evt)
	{
	
		
		std::cout<<"MyEventHandler3_1 with channel 2 and 8"<<std::endl;
		std::cout<<"Get "<<evt->getData<std::string>()<<std::endl<<"===================="<<std::endl;
	}
};

void Example3_EventChannel(void)
{

	using namespace Orz;
	


	
	std::cout<<"=== Example3 EventChannel Begin ==="<<std::endl;

	EventWorld world;
	EventHandlerPtr handler1(new MyEventHandler3_1());
	EventHandlerPtr handler2(new MyEventHandler3_2());
	world.init();
	world.comeIn(handler1);
	world.comeIn(handler2);

	Event * evt = NULL;

	evt = EventFactory::getInstance().createEvent();
	
	evt->setChannel(
		Orz::EventChannel::create()
				.addUserChannel<3>()
		);
	evt->setData<std::string>("Event with Channel 3");
	world.broadcasting(evt);


	
	evt = EventFactory::getInstance().createEvent();
	
	evt->setChannel(
		Orz::EventChannel::create()
				.addUserChannel<8>()
		);
	evt->setData<std::string>("Event with Channel 8");
	world.broadcasting(evt);


	evt = EventFactory::getInstance().createEvent();
	
	evt->setChannel(
		Orz::EventChannel::create()
				.addUserChannel<2>()
				.addUserChannel<3>()
		);
	evt->setData<std::string>("Event with Channel 2 and 3");
	world.broadcasting(evt);



	world.update();
	world.update();
	world.goOut(handler1);
	world.goOut(handler2);
	world.shutdown();


	
	std::cout<<"=== Example3 EventChannel End ==="<<std::endl<<std::endl<<std::endl;
}


#endif

