
#include "TutorialDirector.h"

using namespace Orz;
TutorialDirector::TutorialDirector(const std::string & name):Director(name)
{
}
TutorialDirector::~TutorialDirector(void)
{

}

//这个函数会在初始化调用
void TutorialDirector::doEnable(void)
{
	using namespace Ogre;

	//从系统中得到Ogre的场景管理器
	SceneManager * sm = OgreGraphicsManager::getSingleton().getSceneManager();

	//设置环境光
	sm->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	//sm->setWorldGeometry("terrain.cfg");

	sm->setSkyDome(true, "Examples/CloudySky", 5, 8);
	////创建天空盒
	//sm->setSkyBox(true, "Examples/SpaceSkyBox", 50 );

	// 创建一个光源
	Light* l = sm->createLight("MainLight");
	

	//设置光源位置
	l->setPosition(20,80,50);

}

//销毁之前调用
void TutorialDirector::doDisable(void)
{
	
}
	

		


const std::string & TutorialDirectorFactory::getTypeName() const
{
	static const std::string typeName("TutorialDirector");
	return typeName;
}
TutorialDirectorFactory::pointer_type TutorialDirectorFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	return pointer_type(new TutorialDirector(instanceName));
}

