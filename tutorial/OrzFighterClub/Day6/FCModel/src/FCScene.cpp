
#include "FCScene.h"
#include "XMLFighterLoader.h"
using namespace Orz;
FCScene::FCScene(const std::string & name):Scene(name),_map(new FCMap(16, 16))
{
}
FCScene::~FCScene(void)
{

}

//����������ڳ�ʼ������
void FCScene::doEnable(void)
{
	
	_map->active();
	using namespace Ogre;
	//��ϵͳ�еõ�Ogre�ĳ���������
	SceneManager * sm = OgreGraphicsManager::getSingleton().getSceneManager();

	//���䳡�����
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

	//���û�����
	sm->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	//������պ�
	//sm->setSkyBox(true, "Examples/SpaceSkyBox", 50 );

	// ����һ����Դ
	Light* l = sm->createLight("MainLight");

	//���ù�Դλ��
	l->setPosition(20,80,50);


	//���������� ����ש��
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

	//����������ͨ��XML�ļ�FighterClub.xml�õ��������ߵ����֣�Ȼ�󴴽����� ��������λ�ú�ID
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
	else//���XML��ȡʧ�� ��ô�Ͳ���Ĭ�ϵ� "FCFighter"
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

//����֮ǰ����
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

