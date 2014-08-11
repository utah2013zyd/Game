
#include "FCScene.h"
#include "FCDirector.h"

using namespace Orz;
FCScene::FCScene(const std::string & name):Scene(name)
{
}
FCScene::~FCScene(void)
{

}

//这个函数会在初始化调用
void FCScene::doEnable(void)
{
	
	using namespace Ogre;
//	using namespace OgreNewt;
	//从系统中得到Ogre的场景管理器
	SceneManager * sm = OgreGraphicsManager::getSingleton().getSceneManager();
	
	
	//把其场景清空
	sm->clearScene();
	_numEnemies = 5;
	//sm->setWorldGeometry("terrain.cfg");
	

	Plane plane;
	plane.normal = Vector3::UNIT_Y;
	plane.d = 100;
	
	FCKnowledge::getSingleton().mapInfo().setGround(-plane.d);
	FCKnowledge::getSingleton().mapInfo().enable();

	FCKnowledge::getSingleton().setSceneName(this->getName());
	//MeshManager::getSingleton().createPlane("Myplane",
	//	ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
	//	1500,1500,20,20,true,1,60,60,Vector3::UNIT_Z);
	//Ogre::Entity* pPlaneEnt = sm->createEntity( "plane", "Myplane" );
	//pPlaneEnt->setMaterialName("Examples/Rockwall");
	//sm->getRootSceneNode()->createChildSceneNode()->attachObject(pPlaneEnt);

	//设置环境光
	sm->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
	sm->setSkyBox(true, "Examples/SpaceSkyBox");
	// 创建一个光源
	Light* l = sm->createLight("MainLight");
//	OgreGraphicsManager::getSingleton().getCamera()->lookAt(Vector3(0,0,0));

	//设置光源位置
	l->setPosition(20,80,50);

	NameValueList par;
	par["initPos"] = Ogre::Vector3(200, 50, 270);
	par["queryFlag"] = 0x2;
	par["speedLimit"] = 50.0;
	_player = Orz::GameFactories::getInstance().createActor("FCFighter", "player", &par);
	getWorld()->comeIn(_player);

	for(int i = 0; i < _numEnemies; i++)
	{

		par["initPos"] = Ogre::Vector3(Ogre::Math::RangeRandom(0, 1000), Ogre::Math::RangeRandom(0, 200), Ogre::Math::RangeRandom(0, 1000));
		par["queryFlag"] = 0x8;
		par["speedLimit"] = 10.0;
		ActorPtr enemy = GameFactories::getInstance().createActor("FCFighter", "enemy"+boost::lexical_cast<std::string>(i), &par);
		getWorld()->comeIn(enemy);
		_enemies.push_back(enemy);

		//std::cout<< "done here"<< std::endl;
		NameValueList ctrlPar;
		ctrlPar["fighterName"] = "enemy"+boost::lexical_cast<std::string>(i);
		//std::cout<<"done parameter"<<std::endl;
		ActorPtr enemyCtrl = GameFactories::getInstance().createActor("EnemyController", "enemyCtrl"+boost::lexical_cast<std::string>(i), &ctrlPar);
		//std::cout<<"done create"<<std::endl;
		getWorld()->comeIn(enemyCtrl);
		_enemiesCtrl.push_back(enemyCtrl);
	}

	NameValueList playerCtrlPar;
	playerCtrlPar["playerName"] = std::string("player");

	_playerCtrl = GameFactories::getInstance().createActor("PlayerController", "playerCtrl", &playerCtrlPar);
	getWorld()->comeIn(_playerCtrl);
	enableUpdate();


	//_camNode = ((FCFighter*)_player.get())->getNode()->createChildSceneNode();
	_camNode = OgreGraphicsManager::getSingleton().getSceneManager()->createSceneNode();
	OgreGraphicsManager::getSingleton().getCamera()->setNearClipDistance(0.5);
	_camNode->attachObject(OgreGraphicsManager::getSingleton().getCamera());
	
	FCFighter* fighter = (FCFighter*)_player.get();
	_star = sm->createParticleSystem("camStar", "star2");
	_star->fastForward(6);
	fighter->getNode()->attachObject(_star);
	
	//Ogre::CompositorManager::getSingleton().addCompositor(OgreGraphicsManager::getSingleton().getViewport(), "Radial Blur", -1);
	//Ogre::CompositorManager::getSingleton().setCompositorEnabled(OgreGraphicsManager::getSingleton().getViewport(), "Radial Blur", true);



}

//销毁之前调用
void FCScene::doDisable(void)
{
	FCKnowledge::getSingleton().mapInfo().disable();
	EnemiesList::iterator it;
	getWorld()->goOut(_player);

	getWorld()->goOut(_playerCtrl);
	for(it = _enemies.begin(); it != _enemies.end(); ++it)
	{
		getWorld()->goOut(*it);
	}
	_enemies.clear();

	for(it = _enemiesCtrl.begin(); it != _enemiesCtrl.end(); ++it)
	{
		getWorld()->goOut(*it);
	}
	_enemiesCtrl.clear();
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(OgreGraphicsManager::getSingleton().getViewport(), "Radial Blur", false);
	Ogre::CompositorManager::getSingleton().removeAll();
//	OgreNewtManager::getSingleton().getNewtWorld()->destroyAllBodies();
}
	
void FCScene::doFrame(void)
{
	FCFighter* fighter = (FCFighter*)_player.get();
	_camNode->setPosition(fighter->getPosition() + fighter->getAxis()*Ogre::Vector3(-0.5, 3, -1));
	//_camNode->setDirection(fighter->getAxis()*Ogre::Vector3::UNIT_Z, Ogre::Node::TS_WORLD);
	_camNode->setOrientation(fighter->getNode()->getOrientation()*Ogre::Quaternion(0, 0, 1, 0));
	FCKnowledge::getSingleton().setPlayerPosition(fighter->getPosition());
}
void FCScene::doExecute(Event *evt)
{
	if(FCEvents::equal(evt, FCEvents::CRUSH))
	{
		FCFighter* fighter = (FCFighter*)evt->getSender().get();
		getWorld()->goOut(evt->getSender());
		
		if(fighter->getID() == _player.get()->getID())
		{
			FCDirector* director = (FCDirector* )IDManager::getPointer(FCKnowledge::getSingleton().getDirectorName(), DIRECTOR);
			Event* evt = FCEvents::createEvent(FCEvents::LOSE);
			evt->setSender(this->shared_from_this());
			evt->setReceiver(director->shared_from_this());
			getWorld()->broadcasting(evt);
		}
		else
		{
			_numEnemies--;
			if(_numEnemies == 0)
			{
				FCDirector* director = (FCDirector* )IDManager::getPointer(FCKnowledge::getSingleton().getDirectorName(), DIRECTOR);
				Event* evt = FCEvents::createEvent(FCEvents::WIN);
				evt->setSender(this->shared_from_this());
				evt->setReceiver(director->shared_from_this());
				getWorld()->broadcasting(evt);
			}
		}
	}
	
}

		


const std::string & FCSceneFactory::getTypeName() const
{
	static const std::string typeName("FCScene");
	return typeName;
}
FCSceneFactory::pointer_type FCSceneFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	return pointer_type(new FCScene(instanceName));
}

