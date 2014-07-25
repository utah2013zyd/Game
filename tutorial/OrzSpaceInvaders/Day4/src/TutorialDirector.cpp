
#include "TutorialDirector.h"
using namespace Orz;
TutorialDirector::TutorialDirector(const std::string & name):Director(name)
,_plane()//(Day2)加入战斗机智能指针初始化（空）
,_enemy()
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

	//把其场景清空
	sm->clearScene();

	
	
	//设置环境光
	sm->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	//创建天空盒
	sm->setSkyBox(true, "Examples/SpaceSkyBox", 50 );

	// 创建一个光源
	Light* l = sm->createLight("MainLight");
	

	//设置光源位置 1
	l->setPosition(20,80,1000);


	//(Day2)从系统中得到Ogre的摄像机
	Camera * camera = OgreGraphicsManager::getSingleton().getCamera();

	//(Day2)设置摄像机位置
	camera->setPosition(0.0f, 0.0f, 1000.0f);

	//(Day2)设置摄像机朝向
	camera->lookAt(0.0f, 0.0f, 0.0f);

	//(Day2)构造飞机类对象
	_plane.reset(new BattlePlane(sm));

	//(Day2)打开每frame更新
	enableUpdate();
	
	//(Day3)构造敌人对象
	_enemy.reset(new EnemyPlanes(sm));
}

//销毁之前调用
void TutorialDirector::doDisable(void)
{
	
	//(Day3)销毁敌人
	_enemy.reset(static_cast<EnemyPlanes*>(NULL));

	//(Day2)销毁飞机
	_plane.reset(static_cast<BattlePlane*>(NULL));
}
void TutorialDirector::doFrame(void)
{


	//(Day2)每fream更新战斗机状态（这里的frame是定帧的，每一帧间隔0.015s）
	_plane->update();
	//(Day3)每fream更新敌人状态
	_enemy->update();	
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

