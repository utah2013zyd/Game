
#include "FCDirector.h"
#include "CameraController.h"
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
	
	FCKnowledge::getSingleton().setDirectorName(this->getName());
	//Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../OrzTowerDefense/media", "FileSystem", "General");
	using namespace Ogre;
	//OgreGraphicsManager::getSingleton().getCamera()->setPosition(200.f, 70.f, 200.f);
	//OgreGraphicsManager::getSingleton().getCamera()->setOrientation(Ogre::Quaternion(0.86f, -0.5f, 0.f, 0.f));
	_camCtrl.reset(new CameraController(OgreGraphicsManager::getSingleton().getCamera()));
	//OgreGraphicsManager::getSingleton().getSceneManager()->createSceneNode()->attachObject(OgreGraphicsManager::getSingleton().getCamera());
	enableUpdate();



	_scene = Orz::GameFactories::getInstance().createScene("FCScene");
	getWorld()->comeIn(_scene);

	//CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
	//CEGUIManager::getSingleton().getSystem()->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
	//CEGUI::FontManager::getSingleton().createFont("bluehighway-12.font");
	//CEGUIManager::getSingleton().getSystem()->setDefaultFont((CEGUI::utf8*) "BlueHighway-12");
	_logic_initiate();


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

	_logic_shutdown();

	
}
	

//����֮ǰ����
void FCDirector::doFrame(void)
{
	
	_camCtrl->update();
	_logic_update(WORLD_UPDATE_INTERVAL);
}
	
void FCDirector::doExecute(Orz::Event *evt)
{
	if(FCEvents::equal(evt, FCEvents::WIN))
		_logic_process_event(EvEnimiesExecuted());
	else if(FCEvents::equal(evt, FCEvents::LOSE))
		_logic_process_event(EvPlayerExecuted());
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

