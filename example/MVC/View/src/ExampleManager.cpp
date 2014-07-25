#include <orz/View/ExampleManager.h>
using namespace Orz;




template<> ExampleManager* Singleton<ExampleManager>::_singleton = NULL;


ExampleManager::ExampleManager(void)
{

}
ExampleManager::~ExampleManager(void)
{

}
bool ExampleManager::init(void)
{
	return true;
}
void ExampleManager::shutdown(void)
{

}
bool ExampleManager::update(TimeType interval)
{
	return true;
}
void ExampleManager::print(void)
{
	std::cout<<"I'm ExampleManager!"<<std::endl;
}
