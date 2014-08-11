#include "FCBullet.h"
#include "FCFighter.h"
#include <iostream>
using namespace Orz;

FCBullet::FCBullet(const std::string &name, const std::string & ownerName, int queryFlag, double speed, double range): Actor(name), _ownerName(ownerName),_queryFlag(queryFlag), _speed(speed), _active(false), _range(range), _query(NULL)
{
	
}

FCBullet::~FCBullet()
{
	OgreGraphicsManager::getSingleton().getSceneManager()->destroyQuery(_query);
}

void FCBullet::doEnable(void)
{
	using namespace Ogre;

	SceneManager* sm = OgreGraphicsManager::getSingleton().getSceneManager();

	FCFighter* temp = (FCFighter*)IDManager::getPointer(_ownerName, 0);

	_node = sm->getRootSceneNode()->createChildSceneNode(this->getName(), temp->getPosition());
	//std::cout<< "done here "<<temp->getPosition().x<<" "<<temp->getPosition().y<<" "<<temp->getPosition().z<<std::endl;
	_entity = sm->createEntity(this->getName(), "sphere.mesh");
	_entity->setQueryFlags(0x0);
	_node->setScale(0.1, 0.1, 0.1);
	//_node->attachObject(_entity);

		// create a particle system with 200 quota, then set its material and dimensions
	//ParticleSystem* thrusters = sm->createParticleSystem(this->getName()+"Bullets", 25);
	//thrusters ->setMaterialName("Examples/Flare");
	//thrusters ->setDefaultDimensions(8, 3);
	//thrusters->setParameter("billboard_type", "oriented_self");

	//// create two emitters for our thruster particle system
	//for (unsigned int i = 0; i < 2; i++)
	//{
	//	ParticleEmitter* emitter = thrusters ->addEmitter("Point");  // add a point emitter

	//	// set the emitter properties
	//	emitter->setAngle(Degree(180));
	//	emitter->setTimeToLive(0.1);
	//	emitter->setEmissionRate(25);
	//	emitter->setParticleVelocity(1);
	//	//emitter->setDirection(Vector3::NEGATIVE_UNIT_Z);
	//	emitter->setColour(ColourValue::White, ColourValue::Red);
	//	emitter->setPosition(Vector3(i == 0 ? -50:50, 0, 0));	
	//}
	//_node->attachObject(thrusters);
	_thrusters = sm->createParticleSystem(this->getName()+"Bullets", "bullet");
	_node->attachObject(_thrusters);

	_node->setVisible(false);

	//_thrusters->setVisible(false);
	_query = OgreGraphicsManager::getSingleton().getSceneManager()->createSphereQuery(Ogre::Sphere(), _queryFlag);
	enableUpdate();
	_logic_initiate();
}

void FCBullet::doDisable(void)
{
	_logic_shutdown();
}

void FCBullet::doFrame(void)
{
	_logic_update(WORLD_UPDATE_INTERVAL);
}

void FCBullet::doExecute(Event *evt)
{
	if(FCEvents::equal(evt, FCEvents::FIRE))
	{

		_active = true;
		_logic_process_event(EvBulletFire());
		_node->setVisible(true);
			
		//_node->attachObject(_entity);

		//_thrusters->setVisible(true);
		//std::cout << "done here"<<std::endl;
		FCFighter* temp = (FCFighter*)IDManager::getPointer(_ownerName, ACTOR);
		_node->setPosition(temp->getPosition());
		_direction = temp->getAxis()*Ogre::Vector3::UNIT_Z;
//		_node->setDirection(_direction, Ogre::Node::TS_WORLD);
		_node->setOrientation(Ogre::Quaternion(temp->getAxis().GetColumn(0), temp->getAxis().GetColumn(1), temp->getAxis().GetColumn(2)));
		
	}
}
//void FCBullet::fire(void)
//{
//	_active = true;
//	_logic_process_event(EvBulletFire());
//	_node->setVisible(true);
//	_thrusters->setVisible(true);
//	FCFighter* temp = (FCFighter*)IDManager::getPointer(_ownerName, ACTOR);
//	_node->setPosition(temp->getPosition());
//	_direction = temp->getAxis()*Ogre::Vector3::UNIT_Z;
//}

bool FCBullet::isActive(void)
{
	return _active;
}
void FCBullet::fly(void)
{

	_node->translate(Ogre::Vector3::UNIT_Z * _speed * WORLD_UPDATE_INTERVAL, Ogre::Node::TS_LOCAL);
	_distance += _speed * WORLD_UPDATE_INTERVAL;
}

bool FCBullet::isOutReach(void)
{
	return _distance > _range;
}

void FCBullet::setInactive(void)
{
	_node->setVisible(false);
	//_node->detachObject(_entity);
	//_node->detachObject(_thrusters);
	//_thrusters->setVisible(false);
	_active = false;
	_distance = 0;
}

bool FCBullet::checkHit(void)
{
	_query->setSphere(Ogre::Sphere(_node->getPosition(), 0.001f));
	Ogre::SceneQueryResult qres = _query->execute();
	if(!qres.movables.empty())
	{

		Ogre::String name = (*qres.movables.begin())->getName();
		std::cout<< name<<std::endl;
		FCFighter* attacked = (FCFighter*)IDManager::getPointer((*qres.movables.begin())->getName(), ACTOR);
		if(_ownerName != attacked->getName())
		{
			Event* evt = FCEvents::createEvent(FCEvents::DAMAGE);
			evt->setSender(this->shared_from_this());
			evt->setReceiver(attacked->shared_from_this());
			getWorld()->broadcasting(evt);
			return true;
		}
	}
	return false;
}

void FCBullet::explode(void)
{
}

const std::string & FCBulletFactory::getTypeName() const
{
	static const std::string typeName("FCBullet");
	return typeName;
}
FCBulletFactory::pointer_type FCBulletFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	std::string ownerName;
	int queryFlag = 0x0;
	if(parameter)
	{
		parameter_iterator it = parameter->find("ownerName");

		if(it!= parameter->end())
			ownerName = VariantData<std::string>::get(it->second);

		it = parameter->find("queryFlag");

		if(it!= parameter->end())
			queryFlag = VariantData<int>::get(it->second);

	}
	return pointer_type(new FCBullet(instanceName, ownerName, queryFlag));
}
