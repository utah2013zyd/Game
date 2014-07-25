#include <orz/Toolkit_Base/TimerManager.h>
using namespace Orz;


bool other_update(const std::string & text)
{
	static int times = 0;
	std::cout<<text<<" update! ("<<++times<<")"<<std::endl;
	return true;
}
bool update(void)
{	
	
	static Orz::TimerPtr timer3 = ITimerManager::getSingleton().createTimer(boost::bind(other_update, "Timer3"));
	
	static int times = 0;
	std::cout<<"Timer1 update! ("<<++times<<")"<<std::endl;
	if(times == 10)
	{
		
		timer3->start(2,3,0);
	}	
	
	return true;
}


class Foo
{
public:
	bool update(void)
	{
		static int times = 0;
		std::cout<<"Timer2 update! ("<<++times<<")"<<std::endl;
		return true;
	}
};
void Example_TimerManager(void )
{
	using namespace Orz;

	
	std::cout<<"Example TimerManager Begin"<<std::endl;
	//boost::scoped_ptr<AsioTimerManager> tm(new AsioTimerManager());
	boost::scoped_ptr<SimpleTimerManager> tm(new SimpleTimerManager());
	{

		
		
		Orz::TimerPtr timer1= ITimerManager::getSingleton().createTimer(update);
		timer1->start(1,12,0.001f);
		Foo foo;
		Orz::TimerPtr timer2 = ITimerManager::getSingleton().createTimer(boost::bind(&Foo::update, &foo));
		timer2->start(0.5,7,5);

	
		Clock<> clock;
		clock.restart();
		float lastTimer = 0;
		for(float time = 0; time < 15.f; time = clock.elapsed())
		{
			
			tm->update(time - lastTimer);
			//std::cout<<time<<std::endl;
			lastTimer = time;
		}



		
	}
	std::cout<<"Example TimerManager End"<<std::endl;
}