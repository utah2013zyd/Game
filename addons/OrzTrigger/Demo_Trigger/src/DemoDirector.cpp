
#include "DemoDirector.h"

#include <orz/Toolkit_Base/EventSystem.h>
using namespace Orz;
DemoDirector::DemoDirector(const std::string & name):Director(name)
{
}
DemoDirector::~DemoDirector(void)
{

}

//����������ڳ�ʼ������
void DemoDirector::doEnable(void)
{
	using namespace Ogre;
	_actor =  GameFactories::getInstance().createActor("DemoActor");
	getWorld()->comeIn(_actor);
	//enableUpdate();
}

//����֮ǰ����
void DemoDirector::doDisable(void)
{
	getWorld()->goOut(_actor);
}
void DemoDirector::doFrame(void)
{


}

		


const std::string & DemoDirectorFactory::getTypeName() const
{
	static const std::string typeName("DemoDirector");	
	return typeName;
}
DemoDirectorFactory::pointer_type DemoDirectorFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	return pointer_type(new DemoDirector(instanceName));
}

