


#include <iostream>
extern "C" void dllStartPlugin(void)
{
	std::cout<<"extern \"C\" void dllStartPlugin(void)"<<std::endl;
}

extern "C" void dllStopPlugin(void)
{
	std::cout<<"extern \"C\" void dllStopPlugin(void)"<<std::endl;
}



extern "C" void dllInitialisePlugin(void)
{
	
	std::cout<<"extern \"C\" void dllInitialisePlugin(void)"<<std::endl;
}
extern "C" void dllShutdownPlugin(void)
{
	
	std::cout<<"extern \"C\" void dllShutdownPlugin(void)"<<std::endl;
}
	



extern "C" void testFunction(void)
{
	
	std::cout<<"extern \"C\" void testFunction(void)"<<std::endl;
}
	
