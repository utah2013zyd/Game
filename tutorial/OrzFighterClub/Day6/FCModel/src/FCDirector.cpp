
#include "FCDirector.h"
#include "CameraControler.h"
using namespace Orz;
FCDirector::FCDirector(const std::string & name):Director(name)
{
	_knowledge.reset(new FCKnowledge());
}
FCDirector::~FCDirector(void)
{
	_knowledge.reset();
}

//这个函数会在初始化调用
void FCDirector::doEnable(void)
{

	using namespace Ogre;

	
	OgreGraphicsManager::getSingleton().getCamera()->setPosition(-7.6f, 281.3f, 405.f);
	OgreGraphicsManager::getSingleton().getCamera()->setOrientation(Ogre::Quaternion(0.9f, -0.4f, 0.f, 0.f));


	//我们在这里写一个ASDW摄像机控制器，和Ogre Demo里面一样
	_camCtrl.reset(new CameraControler(OgreGraphicsManager::getSingleton().getCamera()));
	
	//让程序每逻辑frame更新
	enableUpdate();

	//创建一个场景
	_scene = Orz::GameFactories::getInstance().createScene("FCScene");
	
	//放入世界（初始化，接受消息）
	getWorld()->comeIn(_scene);


}

//销毁之前调用
void FCDirector::doDisable(void)
{

	//关闭 停止接受消息
	getWorld()->goOut(_scene);
	//相应的释放
	_scene.reset();
	_camCtrl.reset();
	using namespace Ogre;
	//从系统中得到Ogre的场景管理器
	SceneManager * sm = OgreGraphicsManager::getSingleton().getSceneManager();

	//把其场景清空
	sm->clearScene();

	
}
	

//销毁之前调用
void FCDirector::doFrame(void)
{
	_camCtrl->update();
}
	

		


const std::string & FCDirectorFactory::getTypeName() const
{
	static const std::string typeName("FCDirector");
	return typeName;
}
FCDirectorFactory::pointer_type FCDirectorFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	return pointer_type(new FCDirector(instanceName));
}

