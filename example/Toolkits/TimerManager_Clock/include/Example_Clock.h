#include <orz/Toolkit_Base/Clock.h>
using namespace Orz;



void Example_Clock(void )
{

	
	std::cout<<"Example Clock Begin"<<std::endl;

	Clock<> clock;
	clock.restart();
	for(float time = 0; time < 1.f; time = clock.elapsed())
	{
		std::cout<<time<<std::endl;
	}

	std::cout<<"Example Clock End"<<std::endl;
}