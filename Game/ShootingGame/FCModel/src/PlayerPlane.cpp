#include "PlayerPlane.h"
using namespace Orz;

PlayerPlane::PlayerPlane(const std::string & name, Ogre::Vector3 initPos, int queryFlag, double speedLimit):
FCFighter(name, initPos, queryFlag, speedLimit), _rotateAcceleration(3), _rotateDeacceleration(2), 
_rotateSpeed(Ogre::Quaternion::IDENTITY), _rotateLastSpeed(Ogre::Quaternion::IDENTITY),
_rotProgress(0), _rotFactor(0), _rotating(false)
{
	
}

PlayerPlane::~PlayerPlane(void)
{

}

void PlayerPlane::doEnable(void)
{
	FCFighter::doEnable();

	//create star effects
	Ogre::SceneManager * sm = OgreGraphicsManager::getSingleton().getSceneManager();
	_star = sm->createParticleSystem("camStar", "star2");
	_star->fastForward(6);
	_node->attachObject(_star);

	//attach camera node
	_camNode = OgreGraphicsManager::getSingleton().getSceneManager()->createSceneNode();
	OgreGraphicsManager::getSingleton().getCamera()->setNearClipDistance(0.5);
	_camNode->attachObject(OgreGraphicsManager::getSingleton().getCamera());

	//add radial blur effect
	//Ogre::CompositorManager::getSingleton().addCompositor(OgreGraphicsManager::getSingleton().getViewport(), "Radial Blur", -1);
	//Ogre::CompositorManager::getSingleton().setCompositorEnabled(OgreGraphicsManager::getSingleton().getViewport(), "Radial Blur", true);


}

void PlayerPlane::doDisable(void)
{
	FCFighter::doDisable();

	//destroy radial blur
	//Ogre::CompositorManager::getSingleton().setCompositorEnabled(OgreGraphicsManager::getSingleton().getViewport(), "Radial Blur", false);
	//Ogre::CompositorManager::getSingleton().removeAll();
}

void PlayerPlane::doFrame(void)
{
	FCFighter::doFrame();

	_camNode->setPosition(this->getPosition() + this->getAxis()*Ogre::Vector3(0, 3, -2));
	_camNode->setOrientation(this->getNode()->getOrientation()*Ogre::Quaternion(0, 0, 1, 0));
	FCKnowledge::getSingleton().setPlayerPosition(this->getPosition());
}

void PlayerPlane::forward(void)
{
	if(_rotateVec != Ogre::Vector3::ZERO)
	{
		_rotateSpeed = _rotateSpeed * Ogre::Quaternion(Ogre::Radian(Ogre::Degree(_rotateVec.z * _rotateAcceleration * WORLD_UPDATE_INTERVAL)), Ogre::Vector3::UNIT_Z);
		_rotateSpeed = _rotateSpeed * Ogre::Quaternion(Ogre::Radian(Ogre::Degree(_rotateVec.y * _rotateAcceleration * WORLD_UPDATE_INTERVAL)), Ogre::Vector3::UNIT_Y);
		_rotateSpeed = _rotateSpeed * Ogre::Quaternion(Ogre::Radian(Ogre::Degree(_rotateVec.x * _rotateAcceleration * WORLD_UPDATE_INTERVAL)), Ogre::Vector3::UNIT_X);
		Ogre::Degree angle;
		Ogre::Vector3 axis;
		_rotateSpeed.ToAngleAxis(angle, axis);
		angle = Ogre::Math::Clamp(angle, -_rotateLimit*WORLD_UPDATE_INTERVAL, _rotateLimit*WORLD_UPDATE_INTERVAL);
		_rotFactor = _rotateDeacceleration.valueDegrees()*WORLD_UPDATE_INTERVAL / angle.valueDegrees();
		_rotating  = true;
		_rotProgress = 0;

		//std::cout << angle << " " << _rotateLimit*WORLD_UPDATE_INTERVAL << std::endl;
		_rotateSpeed.FromAngleAxis(angle, axis);
		_rotateLastSpeed = _rotateSpeed;
		_node->rotate(_rotateSpeed);
	}
	else
	{
		if(_rotating)
		{
			_rotProgress += _rotFactor;	
			if(_rotProgress > 1)
			{
				_rotating =false;
				_rotProgress = 0;
			}
			else
			{
				_rotateSpeed = Ogre::Quaternion::Slerp(_rotProgress, _rotateLastSpeed, Ogre::Quaternion::IDENTITY, true);
				_node->rotate(_rotateSpeed);
			}
		}

	}

	if(_speed > _power * _speedLimit)
		_speed -= _acceleration * _speedLimit;
	else if(_speed < _power * _speedLimit)
		_speed += _acceleration * _speedLimit;
	_node->translate(_speed * Ogre::Vector3::UNIT_Z * WORLD_UPDATE_INTERVAL, Ogre::Node::TS_LOCAL);
}

const std::string & PlayerPlaneFactory::getTypeName() const
{
	static const std::string typeName("PlayerPlane");
	return typeName;
}
PlayerPlaneFactory::pointer_type PlayerPlaneFactory::createInstance(const std::string& instanceName, parameter_type parameter)
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
	return pointer_type(new PlayerPlane(instanceName, initPos, queryFlag, speedLimit));
}