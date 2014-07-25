
#include "Example_FSMLogic.h"
using namespace Orz;



namespace
{
	char GetKey()
	{
		char key;
		std::cin >> key;
		return key;
	}
}


int main(void)
{

	std::cout << "Boost.Statechart StopWatch example\n\n";
	
	std::cout << "u<CR>: Update The Logic!!!\n\n";

	std::cout << "s<CR>: Starts/Stops stop watch\n";
	std::cout << "r<CR>: Resets stop watch\n";
	std::cout << "d<CR>: Displays the elapsed time in seconds\n";
	std::cout << "e<CR>: Exits the program\n\n";
	std::cout << "You may chain commands, e.g. rs<CR> resets and starts stop watch\n\n";



	GameEntity entity;




	char key = GetKey();

	while ( key != 'e' )
	{
		switch( key )
		{
		case 'r':
			{
				entity.reset();
			}
			break;

		case 's':
			{
				entity.startStop();
			}
			break;

		case 'd':
			{
				std::cout << "Elapsed time: " << entity.getElapsedTime() << "\n";
			}
			break;
		case 'u':
			{
				entity.update(0.1f);
			}
			break;

		default:
			{
				std::cout << "Invalid key!\n";
			}
			break;
		}

		key = GetKey();
	}




	return 0;
}