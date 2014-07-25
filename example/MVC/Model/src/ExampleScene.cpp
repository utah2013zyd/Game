#include "ExampleScene.h"

using namespace Orz;

ExampleScene::ExampleScene(const std::string & name):Scene(name)
{

}
ExampleScene::~ExampleScene(void)
{

}
void ExampleScene::doEnable(void)
{
}
void ExampleScene::doDisable(void)
{

}
	


const std::string & ExampleSceneFactory::getTypeName() const
{
	static std::string type("ExampleScene");
	return type;
}
ExampleSceneFactory::pointer_type ExampleSceneFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	return pointer_type(new ExampleScene());
}
