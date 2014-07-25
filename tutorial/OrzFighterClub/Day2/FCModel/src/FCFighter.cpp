#include "FCFighter.h"




using namespace Orz;
FCFighter::FCFighter(const std::string & name, int pos):Actor(name),_pos(pos),_start(false)
{
	
}
FCFighter::~FCFighter(void)
{

}

//����������ڳ�ʼ������
void FCFighter::doEnable(void)
{
	using namespace Ogre;
	//��ϵͳ�еõ�Ogre�ĳ���������
	SceneManager * sm = OgreGraphicsManager::getSingleton().getSceneManager();

	_node = sm->getRootSceneNode()->createChildSceneNode();
	_entity = sm->createEntity("FCFighter", "ogrehead.mesh");
	
	int x = _pos/16;
	int y = _pos%16;
	
	_node->attachObject(_entity);
	_node->scale(0.5f,0.5f,0.5f);
	_node->setPosition(x *32.f - 240.f, 0.f, y *32 - 240.f);


	 _start = false;
	enableUpdate();
	_logic_initiate();
}

//����֮ǰ����
void FCFighter::doDisable(void)
{
	_logic_shutdown();
}
	

Ogre::Vector3 FCFighter::getPosition(void) const
{
	return _node->getPosition();
}



void FCFighter::setPosition(const Ogre::Vector3 & pos)
{
	_node->setPosition(pos);
}


void FCFighter::doFrame(void)
{	
	if(FCKnowledge::getSingleton().mapInfo().isEnable())
	{
		_logic_update(WORLD_UPDATE_INTERVAL);
	}
	
}
	

		


const std::string & FCFighterFactory::getTypeName() const
{
	static const std::string typeName("FCFighter");
	return typeName;
}
FCFighterFactory::pointer_type FCFighterFactory::createInstance(const std::string& instanceName, parameter_type parameter)
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


	return pointer_type(new FCFighter(instanceName, pos));
}

