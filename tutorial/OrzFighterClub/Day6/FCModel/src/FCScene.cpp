
#include "FCScene.h"
#include "XMLFighterLoader.h"
using namespace Orz;
FCScene::FCScene(const std::string & name):Scene(name),_map(new FCMap(16, 16))
{
}
FCScene::~FCScene(void)
{

}

//这个函数会在初始化调用
void FCScene::doEnable(void)
{
	
	_map->active();
	using namespace Ogre;
	//从系统中得到Ogre的场景管理器
	SceneManager * sm = OgreGraphicsManager::getSingleton().getSceneManager();

	//把其场景清空
	sm->clearScene();
	
	
	Plane plane;
	plane.normal = Vector3::UNIT_Y;
	plane.d = 100;
	FCKnowledge::getSingleton().mapInfo().setGround(-plane.d);
	FCKnowledge::getSingleton().mapInfo().enable();
	MeshManager::getSingleton().createPlane("Myplane",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		512, 512,20,20,true,1,60,60,Vector3::UNIT_Z);
	Ogre::Entity* pPlaneEnt = sm->createEntity( "plane", "Myplane" );
	pPlaneEnt->setMaterialName("Examples/Rockwall");
	pPlaneEnt->setQueryFlags(0);
	sm->getRootSceneNode()->createChildSceneNode()->attachObject(pPlaneEnt);

	//设置环境光
	sm->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	//创建天空盒
	//sm->setSkyBox(true, "Examples/SpaceSkyBox", 50 );

	// 创建一个光源
	Light* l = sm->createLight("MainLight");

	//设置光源位置
	l->setPosition(20,80,50);


	//下面代码随机 放置砖块
	NameValueList par;
	for(int i = 1; i< 16 *16; ++i)
	{
		if(rand()%2 == 0)
		{
			par["pos"] = i;
			ActorPtr wall = Orz::GameFactories::getInstance().createActor("FCWall",IDManager::BLANK, &par );
			getWorld()->comeIn(wall);
			_walls.push_back(wall);
		}
	}

	//在这里我们通过XML文件FighterClub.xml得到两个格斗者的名字，然后创建他们 并给他们位置和ID
	XMLFighterLoader loader;
	if(loader.load("FighterClub.xml"))
	{
		{
			par["pos"] = 10;
			par["id"] = 0;
			ActorPtr fighter = Orz::GameFactories::getInstance().createActor(loader.getFighter1(),IDManager::BLANK, &par );
			getWorld()->comeIn(fighter);
			_enemies.push_back(fighter);
			
		}
		{
			par["pos"] = 20;
			
			par["id"] = 1;
			ActorPtr fighter = Orz::GameFactories::getInstance().createActor(loader.getFighter2(),IDManager::BLANK, &par );
			getWorld()->comeIn(fighter);
		_enemies.push_back(fighter);
		}
	}
	else//如果XML读取失败 那么就采用默认的 "FCFighter"
	{
		for(int i =0; i<2; ++i)
		{
			
			par["id"] = i;
			par["pos"] = i*10;
			ActorPtr fighter = Orz::GameFactories::getInstance().createActor("FCFighter",IDManager::BLANK, &par );
			getWorld()->comeIn(fighter);
			_enemies.push_back(fighter);
		}
	}
}

//销毁之前调用
void FCScene::doDisable(void)
{

	
	FCKnowledge::getSingleton().mapInfo().disable();
	for(WallList::iterator it = _walls.begin(); it != _walls.end(); ++it)
	{
		getWorld()->goOut(*it);
	}
	_walls.clear();
	for(EnemiesList::iterator  it = _enemies.begin(); it != _enemies.end(); ++it)
	{
		getWorld()->goOut(*it);
	}

	
	_enemies.clear();
	_map->inactive();
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

