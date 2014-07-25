#include "FCWall.h"
#include "FCMap.h"
#include "FCEvents.h"
#include "Bullets.h"
using namespace Orz;
FCWall::FCWall(const std::string & name, int x, int y):FCObject(name,x, y)
{
	
}
FCWall::~FCWall(void)
{

}

void FCWall::doExecute(Event * evt)
{
	if(evt&& FCEvents::equal(evt, FCEvents::SHOOTED)  )
	{

		Orz::Bullet::BulletInfo info = evt->getData<Orz::Bullet::BulletInfo>();
		if(this->_entity == info.getObject())
		{
			_entity->setVisible(false);
			_entity->setQueryFlags(0);
			FCMap * map = FCMap::getActiveMap();
			if(map)
			{
				map->removeBlocker(_x, _y, this);
				map->setWay(_x, _y, FCPather::WAY, 1.f);
				Orz::EventHandlerPtr handler = evt->getSender();
				if(handler)
				{
					Event * back = FCEvents::createEvent(FCEvents::DESTROYED);
					back->setSender(shared_from_this());
					back->setReceiver(handler);
					getWorld()->broadcasting(back);
				}
			
			}

		}
	}
}
//这个函数会在初始化调用
void FCWall::doEnable(void)
{
	
	
	using namespace Ogre;
	FCObject::doEnable();
	//从系统中得到Ogre的场景管理器
	SceneManager * sm = OgreGraphicsManager::getSingleton().getSceneManager();
	static int num = 0;
	_node = sm->getRootSceneNode()->createChildSceneNode();
	_entity = sm->createEntity(std::string("FCWall")+ boost::lexical_cast<std::string>(num ++), "cube.mesh");
	_entity->setMaterialName("Examples/EnvMappedRustySteel");
	//_entity->setQueryFlags(0);
	
	
	_node->attachObject(_entity);
	_node->scale(0.302f,0.302f,0.302f);
	_node->setPosition(_x *32.f - 240.f, 0.f, _y *32 - 240.f);
	


	FCMap * map = FCMap::getActiveMap();
	if(map)
	{
		map->addBlocker(_x, _y, this);
		map->setWay(_x, _y, FCPather::WAY, 10000.f);
	}



	this->enableUpdate();
}

//销毁之前调用
void FCWall::doDisable(void)
{
	FCObject::doDisable();
}
	

//销毁之前调用
void FCWall::doFrame(void)
{

	
	
	FCObject::doFrame();
	
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
	int x = 0;
	int y = 0;
	if(parameter)
	{
		parameter_iterator it = parameter->find("pos");
		if(it!= parameter->end())
		{
			pos = boost::get<int>(it->second);
			x = pos/16;
			y = pos%16;
		}
	}


	return pointer_type(new FCWall(instanceName, x, y));

}

