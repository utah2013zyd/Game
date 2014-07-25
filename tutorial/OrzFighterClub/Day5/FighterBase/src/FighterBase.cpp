#include "FighterBase.h"
#include "FCMap.h"
#include "FCEvents.h"
#include "FCKnowledge.h"

using namespace Orz;
FighterBase::FighterBase(const std::string & name, int x, int y, int id):FCMovable(name, x, y)/*,_pos(pos)*/,_start(false), _fired(false),_fireTime(0.f)//,_bulletManager(sm, 20)
, _moved(false), _moveTime (1.f), _moveAllTime(1.f), _fighterID(id)
{
	
}
FighterBase::~FighterBase(void)
{
	

}

FighterBase::ActionReturnPtr FighterBase::getAction(const ACTION action, const DIRECTOR director, float power) const
{
	return ActionReturnPtr(new ActionReturn(action, director, power, _fighterID));
}

//这个函数会在初始化调用
void FighterBase::doEnable(void)
{
	Orz::FCMovable::doEnable();
	using namespace Ogre;
	static int num = 0;
	//从系统中得到Ogre的场景管理器
	SceneManager * sm = OgreGraphicsManager::getSingleton().getSceneManager();
	int query = 1<<num;
	_bulletManager.reset(new BulletManager(sm, 20,  ~query));
	_node = sm->getRootSceneNode()->createChildSceneNode();
	_entity = sm->createEntity("FighterBase"+ Ogre::StringConverter::toString(num++), "ogrehead.mesh");
	_entity->setQueryFlags(query);
	_node->attachObject(_entity);
	_node->scale(0.5f,0.5f,0.5f);
	resetPosition();//_node->setPosition(_x *32.f - 240.f, 0.f, _y *32 - 240.f);


	patherSolve(15, 15);
	 _start = false;
	enableUpdate();
	_logic_initiate();


	FCMap * map = FCMap::getActiveMap();
	if(map)
	{
		map->setFighter(_fighterID, this);
	
	}
	
	_power = 50.f;
	_maxPower = 100.f;
	_fired = false;
	_fireTime = 0.f;

	_health = 500.f;
	
	_moved = false;
	_moveTime = 1.f;
	_moveAllTime = 1.f;
}

//销毁之前调用
void FighterBase::doDisable(void)
{
	
	FCMap * map = FCMap::getActiveMap();
	if(map)
	{
		
		
		map->setFighter(_fighterID, NULL);
		
	}
	_bulletManager.reset();

	_logic_shutdown();
	//_logicShutdown();
}
	


Ogre::Vector3 FighterBase::getPosition(void) const
{
	return _node->getPosition();
}

bool FighterBase::nextPosition()
{
	if(_pather->isEnd())
	{
		return false;
	}
	_toWhere = _pather->getNext();
	return true;
}
Ogre::Vector3 FighterBase::getTarget(void) const
{
	return Ogre::Vector3(_toWhere.first*32.f - 240.f, _node->getPosition().y, _toWhere.second*32.f - 240.f );
}

Orz::EventHandler * FighterBase::getTargetBlocker(void) const
{
	return Orz::FCMap::getActiveMap()->getBlocker(_toWhere.first, _toWhere.second);
}

FighterBase::ActionReturnPtr FighterBase::thinking(int power)
{
	
	
	switch(rand()%9)
	{
	case 0:
		return getAction(Move, East, power);
		break;
	case 1:
		return getAction(Move, South, power);
		break;
	case 2:
		return getAction(Move, West, power);
		break;
	case 3:
		return getAction(Move, North, power);
		break;
	case 4:
		return getAction(Fire, East, power);
		break;
	case 5:
		return getAction(Fire, South, power);
		break;
	case 6:
		return getAction(Fire, West, power);
		break;
	case 7:
		return getAction(Fire, North, power);
		break;
	case 8:
		return getAction(Sleep, East, power);
		break;
	
	}
	return getAction(Move, North, power);
}

	

void FighterBase::move(DIRECTOR dir, float speed,  bool isSleep)
{
	
	_moved = true;
	if(!isSleep)
	{
		int x = _x;
		int y = _y;

		if(speed <10.f)
		{
			speed = 10.f;
		}

		if(speed >= _power)
		{
			
			speed = _power;
			_power = 0.f;
		}
		else  
		{
			_power -= speed;
		}


		switch(dir)
		{
		case East:
			x+=1;
			break;
		case South:
			y-=1;
			break;
		case West:
			x-=1;
			break;
		case North:
			y+=1;		
			break;
		}

		
		if(canMove(x, y))
		{
			_moveAllTime = _moveTime = 100.f/(speed+50.f);
			_toWhere.first = x;
			_toWhere.second = y;
		}
		else
		{
			_moveTime = 0.1f;
			_toWhere.first = _x;
			_toWhere.second = _y;
			
		}
	}
	else
	{
		_moveTime = 0.3f;
		_toWhere.first = _x;
		_toWhere.second = _y;
	}

}

void FighterBase::sendFightingEvent(FighterBase::ActionReturnPtr arp)
{
	
		Event * evt = FCEvents::createEvent(FCEvents::FIGHTING);
		
		if(evt)
		{
			evt->setSender(shared_from_this());
			evt->setData(arp);
			
			getWorld()->broadcasting(evt);
		}
		
	
	
	
}
void FighterBase::updateMove(void)
{
	if(_moved)
	{
		if(_moveTime >0.f)
		{
			_moveTime -= WORLD_UPDATE_INTERVAL;
			Ogre::Vector3 from = XY2Position(_x, _y);
			Ogre::Vector3 to =  XY2Position(_toWhere.first, _toWhere.second);
			this->setPosition((to- from) * (_moveAllTime - _moveTime) / _moveAllTime + from );
		}else
		{
			_moved = false;
			_logic_process_event(ActionOverEvt());
			_x =_toWhere.first;
			_y = _toWhere.second;
			resetPosition();
		}
	}
}
//FCPatherPtr FCFighter::getPather(void)
//{
//	
//}


void FighterBase::addPower(float power)
{
	float newPower = power + _power;
	if(newPower<=_maxPower && newPower >=0.f)
		_power = newPower;
}
void FighterBase::setMaxPower(float power)
{
	_maxPower = power;
}


float FighterBase::getMaxPower(void) const
{
	return _maxPower;
}

float FighterBase::getThePower(void) const
{
	return _power;
}

void FighterBase::doExecute(Event * evt)
{
	if(FCEvents::equal(evt, FCEvents::SHOOTED)  )
	{
		Orz::Bullet::BulletInfo info = evt->getData<Orz::Bullet::BulletInfo>();
		if(this->_entity == info.getObject())
		{
			_health -= info.getPower();
			if(_health <=0.f)
			{
				disableUpdate();
				_entity->setVisible(false);
				sendFightingEvent(this->getAction(Die));
			}else
			{
				sendFightingEvent(this->getAction(Hurt));
			}
			
		}
	}else if(FCEvents::equal(evt, FCEvents::DESTROYED))
	{
		setMaxPower(getMaxPower() + 3.f);
		
	}else if(FCEvents::equal(evt, FCEvents::FIGHTING))
	{

		ActionReturnPtr ap  = evt->getData<ActionReturnPtr>();
		radar(ap);
		
	}


}
void FighterBase::setPosition(const Ogre::Vector3 & pos)
{
	_node->setPosition(pos);
}

void FighterBase::fire(DIRECTOR dir, float power)
{

	
	if(power <10.f)
	{
		power = 10.f;
	}

	if(power >= _power)
	{
		
		power = _power;
		_power = 0.f;
	}
	else  
	{
		_power -= power;
	}

	_fireTime = 0.25;
	_fired = true;
	Ogre::Vector3 speed(0.f, 0.f, 0.f);
	switch(dir)
	{
	case East:
		speed.x=5.f *power/100.f;
		break;
	case South:
		
		speed.z=5.f *power/100.f;
		break;
	case West:
		
		speed.x=-5.f *power/100.f;
		break;
	case North:
		
		speed.z=-5.f *power/100.f;
		break;
	}

	_bulletManager->fire(_node->getPosition(), speed, power);

}

void FighterBase::updateFire(void)
{
	_bulletManager->update();
	while(Bullet* bullet = _bulletManager->touch())
	{
		Event * evt = FCEvents::createEvent(FCEvents::SHOOTED);
		evt->setSender(shared_from_this());
		evt->setData(bullet->getInfo(_fighterID));
		//evt->setChannel()
		getWorld()->broadcasting(evt);
	}

	if(_fired)
	{
		if(_fireTime >0.f)
		{
			_fireTime -= WORLD_UPDATE_INTERVAL;
		}else
		{
			_fired = false;
			_logic_process_event(ActionOverEvt());
		}
	}
}
//销毁之前调用
void FighterBase::doFrame(void)
{	
	FCMovable::doFrame();
	updateFire();
	updateMove();
	if(FCKnowledge::getSingleton().mapInfo().isEnable())
	{
	
		_logic_update(WORLD_UPDATE_INTERVAL);
	}
	
}



void FighterBase::resetPosition(void)
{

	setPosition(XY2Position(_x, _y));
}
		


Ogre::Vector3 FighterBase::XY2Position(int x, int y)
{
	
	return Ogre::Vector3(x *32.f - 240.f, _node->getPosition().y, y *32 - 240.f);
}

		



bool FighterBase::canMove(int x, int y)
{

	if(!FCMovable::canMove(x, y))
		return false;


	FCMap * map = FCMap::getActiveMap();
	if(map)
	{
		if(map->hasFighter(!_fighterID, x, y))
			return false;
	}

	return true;
}