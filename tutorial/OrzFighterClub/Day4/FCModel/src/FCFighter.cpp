#include "FCFighter.h"
#include "FCMap.h"



using namespace Orz;
FCFighter::FCFighter(const std::string & name, int pos):FCObject(name),_pos(pos),_start(false)
{
	
}
FCFighter::~FCFighter(void)
{
	

}

//这个函数会在初始化调用
void FCFighter::doEnable(void)
{
	using namespace Ogre;
	//从系统中得到Ogre的场景管理器
	SceneManager * sm = OgreGraphicsManager::getSingleton().getSceneManager();

	_node = sm->getRootSceneNode()->createChildSceneNode();
	_entity = sm->createEntity("FCFighter", "ogrehead.mesh");
	
	int x = _pos/16;
	int y = _pos%16;
	
	_node->attachObject(_entity);
	_node->scale(0.5f,0.5f,0.5f);
	_node->setPosition(x *32.f - 240.f, 0.f, y *32 - 240.f);


	FCMap * map = FCMap::getActiveMap();
	if(map)
	{
		_pather = map->createPather();
		_pather->reset(x, y);
		_pather->solve(15, 15);
		_pather->getNext();
	}
	 _start = false;
	enableUpdate();
	_logic_initiate();
}

//销毁之前调用
void FCFighter::doDisable(void)
{
	_logic_shutdown();
}
	


Ogre::Vector3 FCFighter::getPosition(void) const
{
	return _node->getPosition();
}

bool FCFighter::nextPosition()
{
	if(_pather->isEnd())
	{
		return false;
	}
	_toWhere = _pather->getNext();
	return true;
}
Ogre::Vector3 FCFighter::getTarget(void) const
{
	return Ogre::Vector3(_toWhere.first*32.f - 240.f, _node->getPosition().y, _toWhere.second*32.f - 240.f );
}

Orz::EventHandler * FCFighter::getTargetBlocker(void) const
{
	return Orz::FCMap::getActiveMap()->getBlocker(_toWhere.first, _toWhere.second);
}
	
//FCPatherPtr FCFighter::getPather(void)
//{
//	
//}


void FCFighter::setPosition(const Ogre::Vector3 & pos)
{
	_node->setPosition(pos);
}


//销毁之前调用
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

