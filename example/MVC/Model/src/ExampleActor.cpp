#include "ExampleActor.h"

using namespace Orz;

ExampleActor::ExampleActor(const std::string & name):Actor(name)
{

}
ExampleActor::~ExampleActor(void)
{

}
void ExampleActor::doEnable(void)
{
}
void ExampleActor::doDisable(void)
{

}
	


const std::string & ExampleActorFactory::getTypeName() const
{

	static std::string type("ExampleActor");
	return type;
}
ExampleActorFactory::pointer_type ExampleActorFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	return pointer_type(new ExampleActor());
}
