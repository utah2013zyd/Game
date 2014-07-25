
#ifndef __Orz_Example5_EventWorldUpdate__
#define __Orz_Example5_EventWorldUpdate__
#include <orz/Toolkit_Base/EventSystem.h>
#include <orz/Toolkit_Base/Clock.h>

class MyEventHandler5:public Orz::EventHandler
{
public:
	
	virtual void doExecute(Orz::Event * evt)
	{
		using namespace Orz;
		if(evt->getData<int>())
			std::cout<<"Update "<<evt->getData<int>()<<" times."<<std::endl;
		else 
			std::cout<<"Update "<<evt->getData<int>()<<" time."<<std::endl;


		Event * e = EventFactory::getInstance().createEvent();
		e->setSender(shared_from_this());
		e->setData<int>(evt->getData<int>() +1);
		getWorld()->broadcasting(e);
	}

	
};


void Example5_EventWorldUpdate(void)
{

	using namespace Orz;


	
	std::cout<<"=== Example5 EventWorldUpdate Begin ==="<<std::endl;
	
	EventWorld world;
	EventHandlerPtr handler(new MyEventHandler5());

	world.init();
	world.comeIn(handler);

		
	Event * evt = EventFactory::getInstance().createEvent();
	evt->setData<int>(0);
	world.broadcasting(evt);



	Clock<> clock;
	
	float time = 0.f;
	const float interval = 0.5f;
	float passTime = 0.f;
	while(true)
	{
		time = clock.elapsed();
		if(time >10.f)
			break;
		
		

		while(passTime+interval <time)
		{

			passTime+=interval;
			world.update();
		}


	}


	


	world.goOut(handler);
	world.shutdown();
	

	std::cout<<"=== Example5 EventWorldUpdate End ==="<<std::endl<<std::endl<<std::endl;
}


#endif

