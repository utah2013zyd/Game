#include "FCWall.h"
using namespace Orz;
FCWall::FCWall(const std::string & name, int pos):Actor(name),_pos(pos)
{
	std::cout<<_pos<<std::endl;
}
FCWall::~FCWall(void)
{

}

//����������ڳ�ʼ������
void FCWall::doEnable(void)
{
	

	using namespace Ogre;
	//��ϵͳ�еõ�Ogre�ĳ���������
	SceneManager * sm = OgreGraphicsManager::getSingleton().getSceneManager();
	
	_node = sm->getRootSceneNode()->createChildSceneNode();
	_entity = sm->createEntity(std::string("FCWall")+ boost::lexical_cast<std::string>(_pos), "cube.mesh");
	_entity->setMaterialName("Examples/EnvMappedRustySteel");
	
	int x = _pos/16;
	int y = _pos%16;
	
	_node->attachObject(_entity);
	_node->scale(0.302f,0.302f,0.302f);
	_node->setPosition(y *32.f - 240.f, 0.f, x *32 - 240.f);
	


	//FCMap * map = FCMap::getActiveMap();
	/*if(map)
	{
		map->addBlocker(x, y, this);
		map->setWay(x, y, FCPather::WAY, 10000.f);
	}*/



	this->enableUpdate();
}

//����֮ǰ����
void FCWall::doDisable(void)
{
	
}
	

//����֮ǰ����
void FCWall::doFrame(void)
{

	

	
	if(FCKnowledge::getSingleton().mapInfo().isEnable())
	{
		float d = FCKnowledge::getSingleton().mapInfo().getGround();
		Ogre::Vector3 pos = _node->getPosition();
		if(pos.y > d + 0.31f)
		{
			pos.y -= 0.8f;
		}
		else
		{
			pos.y = d;
			disableUpdate();
			
		}

		_node->setPosition(pos);
	}

}
	

		


const std::string & FCWallFactory::getTypeName() const
{
	static const std::string typeName("FCWall");
	return typeName;
}
FCWallFactory::pointer_type FCWallFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{

	int pos = 0;
	if(parameter)
	{
		parameter_iterator it = parameter->find("pos");
		if(it!= parameter->end())
		{
			pos = boost::get<int>(it->second);
		}
	}
	return pointer_type(new FCWall(instanceName, pos));
}

