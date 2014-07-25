#include "FCFighter.h"
#include "FCBullet.h"
#include "FCBulletLogic.h"
#include "FCScene.h"
#include <iostream>

using namespace Orz;
FCFighter::FCFighter(const std::string & name, Ogre::Vector3 initPos, int queryFlag, double speedLimit, double rotateAngle):Actor(name),
	_rotateVec(Ogre::Vector3(0.0, 0.0, 0.0)), _power(1.0), _initPos(initPos), _queryFlag(queryFlag), _speedLimit(speedLimit), _rotateAngle(rotateAngle), _lifePoint(100.0)
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

	_node = sm->getRootSceneNode()->createChildSceneNode(this->getName(), _initPos);
	_entity = sm->createEntity(this->getName(), "Razor.mesh");
	_entity->setQueryFlags(_queryFlag);
	_node->attachObject(_entity);
	_node->setScale(0.1, 0.1, 0.1);
	_query = OgreGraphicsManager::getSingleton().getSceneManager()->createSphereQuery(Ogre::Sphere(), 0x8);
	// create a particle system with 200 quota, then set its material and dimensions
	ParticleSystem* thrusters = sm->createParticleSystem(this->getName()+"Tails", 25);
	thrusters ->setMaterialName("Examples/Flare");
	thrusters ->setDefaultDimensions(8, 3);

	// create two emitters for our thruster particle system
	for (unsigned int i = 0; i < 2; i++)
	{
		ParticleEmitter* emitter = thrusters ->addEmitter("Point");  // add a point emitter

		// set the emitter properties
		emitter->setAngle(Degree(3));
		emitter->setTimeToLive(0.1);
		emitter->setEmissionRate(25);
		emitter->setParticleVelocity(25);
		emitter->setDirection(Vector3::NEGATIVE_UNIT_Z);
		emitter->setColour(ColourValue::White, ColourValue::Red);
		emitter->setPosition(Vector3(i == 0 ? 5.7 : -18, 0, -70));	
	}
	_node->attachObject(thrusters);


	for(int i = 0; i < 10; i++)
	{
		NameValueList par;
		par["ownerName"] = this->getName();
		if(_queryFlag == 0x2)
			par["queryFlag"] = 0x8;
		else if(_queryFlag == 0x8)
			par["queryFlag"] = 0x2;

		ActorPtr bullet = Orz::GameFactories::getInstance().createActor("FCBullet", getName()+"bullet"+boost::lexical_cast<std::string>(i), &par);
		getWorld()->comeIn(bullet);
		_bullets.push_back(bullet);
	}
	enableUpdate();
	_logic_initiate();
	
}

//销毁之前调用
void FCFighter::doDisable(void)
{
	std::cout << "distroying ..." << std::endl;
	if(!_bullets.empty())
	{
		BulletsList::iterator it;
		for(it = _bullets.begin(); it != _bullets.end(); ++it)
		{
			getWorld()->goOut(*it);
		}
		_bullets.clear();
	}
	_logic_shutdown();
}
	

void FCFighter::doFrame(void)
{	

	//if(FCKnowledge::getSingleton().mapInfo().isEnable())
	//{
	//	float d = FCKnowledge::getSingleton().mapInfo().getGround();
	//	Ogre::Vector3 pos = _node->getPosition();
	//	if(pos.y > d + 0.31f)
	//	{
	//		pos.y -= 0.3f;
	//	}
	//	else
	//		pos.y = d;
	//	pos.x += 0.1f;
	//	pos.z += 0.1f;
	//	_node->setPosition(pos);
	//}
	_logic_update(WORLD_UPDATE_INTERVAL);
}
	
void FCFighter::doExecute(Event* evt)
{
	if(FCEvents::equal(evt, FCEvents::DAMAGE))
	{
		_lifePoint -= 30.0;
		if(_lifePoint <= 0.0)
			_logic_process_event(EvFighterCrush());
	}
	if(FCEvents::equal(evt, FCEvents::FIRE))
		fire();
}

void FCFighter::roll(double z)
{

	_rotateVec.z = z;
}

void FCFighter::yaw(double y)
{
	//std::cout<< "test done"<<std::endl;
	_rotateVec.y = y;
}

void FCFighter::pitch(double x)
{
	_rotateVec.x = x;
}

void FCFighter::forward(void)
{
	_node->yaw(Ogre::Degree(_rotateVec.y* _rotateAngle * WORLD_UPDATE_INTERVAL));
	_node->pitch(Ogre::Degree(_rotateVec.x * _rotateAngle * WORLD_UPDATE_INTERVAL));
	_node->roll(Ogre::Degree(_rotateVec.z * _rotateAngle * WORLD_UPDATE_INTERVAL));
	
	_node->translate(_speedLimit * _power * Ogre::Vector3::UNIT_Z * WORLD_UPDATE_INTERVAL, Ogre::Node::TS_LOCAL);
}

void FCFighter::fire(void)
{
	BulletsList::iterator iter;
	for(iter = _bullets.begin(); iter != _bullets.end(); iter++)
	{
		
		if(!((FCBullet*)iter->get())->isActive())
			break;
	}
	if(iter != _bullets.end() && !((FCBullet*)iter->get())->isActive())
	{	
		Event* evt = FCEvents::createEvent(FCEvents::FIRE);
		evt->setSender(this->shared_from_this());
		evt->setReceiver(((FCBullet*)iter->get())->shared_from_this());
		getWorld()->broadcasting(evt);
	}
}
void FCFighter::crush(void)
{
		//_node->setVisible(false);
		_entity->setQueryFlags(0x0);

		BulletsList::iterator it;
		for(it = _bullets.begin(); it != _bullets.end(); ++it)
		{
			((FCBullet*)it->get())->setInactive();
			getWorld()->goOut(*it);
		}
		_bullets.clear();
		
		Ogre::SceneManager* sm =  OgreGraphicsManager::getSingleton().getSceneManager();
		Ogre::ParticleSystem* explosion = sm->createParticleSystem("explosion"+this->getName(), "explosion");
		Ogre::ParticleSystem* explosion2 = sm->createParticleSystem("explosion2"+this->getName(), "explosion2");
		_node->attachObject(explosion);
		_node->attachObject(explosion2);



}

void FCFighter::disappear()
{
	_node->detachAllObjects();
	std::string sceneName = FCKnowledge::getSingleton().getSceneName();
	FCScene* scene = (FCScene*)IDManager::getPointer(sceneName, SCENE);
	Event* evt = FCEvents::createEvent(FCEvents::CRUSH);
	evt->setSender(this->shared_from_this());
	evt->setReceiver(scene->shared_from_this());
	getWorld()->broadcasting(evt);


}

bool FCFighter::checkHit(void)
{
	_query->setSphere(Ogre::Sphere(_node->getPosition(), 0.001f));
	Ogre::SceneQueryResult qres = _query->execute();
	if(!qres.movables.empty())
	{

		Ogre::String name = (*qres.movables.begin())->getName();
		//std::cout<< name<<std::endl;
		FCFighter* attacked = (FCFighter*)IDManager::getPointer((*qres.movables.begin())->getName(), ACTOR);
		if(this != attacked)
		{
			Event* evt = FCEvents::createEvent(FCEvents::CRUSH);
			evt->setSender(this->shared_from_this());
			evt->setReceiver(attacked->shared_from_this());
			getWorld()->broadcasting(evt);
			_logic_process_event(EvFighterCrush());	
			return true;
		}
	}
	return false;
}
const std::string & FCFighterFactory::getTypeName() const
{
	static const std::string typeName("FCFighter");
	return typeName;
}
FCFighterFactory::pointer_type FCFighterFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	Ogre::Vector3 initPos = Ogre::Vector3(0.0, 0.0, 0.0);
	int queryFlag = 0x0;
	double speedLimit = 0;
	if(parameter)
	{
		parameter_iterator it = parameter->find("initPos");
		if(it!= parameter->end())
			initPos = VariantData<Ogre::Vector3>::get(it->second);

		it = parameter->find("queryFlag");
		if(it != parameter->end())
			queryFlag = VariantData<int>::get(it->second);

		it = parameter->find("speedLimit");
		if(it != parameter->end())
			speedLimit = VariantData<double>::get(it->second);
	}
	return pointer_type(new FCFighter(instanceName, initPos, queryFlag, speedLimit));
}

