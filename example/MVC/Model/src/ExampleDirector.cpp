#include "ExampleDirector.h"

using namespace Orz;

ExampleDirector::ExampleDirector(const std::string & name):Director(name)
{

}
ExampleDirector::~ExampleDirector(void)
{

}
void ExampleDirector::doEnable(void)
{
}
void ExampleDirector::doDisable(void)
{

}
	


const std::string & ExampleDirectorFactory::getTypeName() const
{
	static std::string type("ExampleDirector");
	return type;
}
ExampleDirectorFactory::pointer_type ExampleDirectorFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	return pointer_type(new ExampleDirector());
}
