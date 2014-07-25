
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
	//_actor =  GameFactories::getInstance().createActor("DemoActor");
	//getWorld()->comeIn(_actor);
	_jsBinding.reset(new JSBinding());
	_scriptInterface.reset(new ScriptInterface());
	
	//enableUpdate();
}

//����֮ǰ����
void DemoDirector::doDisable(void)
{
	_jsBinding.reset();
	//getWorld()->goOut(_actor);
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

