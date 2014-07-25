
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

//����������ڳ�ʼ������
void FCDirector::doEnable(void)
{


	
	


	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../OrzTowerDefense/media", "FileSystem", "General");
	using namespace Ogre;
	
	OgreGraphicsManager::getSingleton().getCamera()->setPosition(-7.6f, 281.3f, 405.f);
	OgreGraphicsManager::getSingleton().getCamera()->setOrientation(Ogre::Quaternion(0.9f, -0.4f, 0.f, 0.f));
	_camCtrl.reset(new CameraControler(OgreGraphicsManager::getSingleton().getCamera()));
	enableUpdate();



	_scene = Orz::GameFactories::getInstance().createScene("FCScene");
	getWorld()->comeIn(_scene);


}

//����֮ǰ����
void FCDirector::doDisable(void)
{

	getWorld()->goOut(_scene);
	_scene.reset();
	
	_camCtrl.reset();
	using namespace Ogre;
	//��ϵͳ�еõ�Ogre�ĳ���������
	SceneManager * sm = OgreGraphicsManager::getSingleton().getSceneManager();

	//���䳡�����
	sm->clearScene();

	
}
	

//����֮ǰ����
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

