#include "EnemyController.h"
#include <iostream>
using namespace Orz;

EnemyController::EnemyController(const std::string &name, const std::string &fighterName): Actor(name),
	_enemyName(fighterName)
{
}

EnemyController::~EnemyController(void)
{
}

void EnemyController::doEnable(void)
{
	enableUpdate();
	_logic_initiate();
}

void EnemyController::doDisable(void)
{
		_logic_shutdown();
}

void EnemyController::doFrame(void)
{
	//std::cout<< "done frame" <<std::endl;

	_logic_update(WORLD_UPDATE_INTERVAL);
}

void EnemyController::doExecute(Orz::Event *evt)
{
}

void EnemyController::approach(void)
{
	EnemyPlane* enemy = static_cast<EnemyPlane*>(IDManager::getPointer(_enemyName, ACTOR));
	Ogre::Vector3 temp = FCKnowledge::getSingleton().getPlayerPosition() - enemy->getPosition();
	Ogre::Vector3 direction = temp * enemy->getAxis();
	//std::cout<<direction.x<<" "<<direction.y<<" "<<direction.z<<std::endl;
	if(direction.angleBetween(Ogre::Vector3::NEGATIVE_UNIT_Z) >= Ogre::Radian(Ogre::Degree(1)))
	{
		Ogre::Quaternion test = direction.getRotationTo(Ogre::Vector3::NEGATIVE_UNIT_Z);
		double angle = enemy->getRotateAngle();
		
		double yawNum = test.getYaw().valueDegrees()/(angle*WORLD_UPDATE_INTERVAL);
		yawNum = Ogre::Math::Clamp(yawNum, -1.0, 1.0);
		enemy->yaw(yawNum);

		double pitchNum = test.getPitch().valueDegrees()/(angle*WORLD_UPDATE_INTERVAL);
		pitchNum = Ogre::Math::Clamp(pitchNum, -1.0, 1.0);
		enemy->pitch(pitchNum);
		
		double rollNum = test.getRoll().valueDegrees()/(angle*WORLD_UPDATE_INTERVAL);
		rollNum = Ogre::Math::Clamp(rollNum, -1.0, 1.0);
		enemy->roll(rollNum);
		
	}
	else
	{
		enemy->yaw(0);
		enemy->pitch(0);
		enemy->roll(0);
	}
}

bool EnemyController::withinReach(void)
{
	EnemyPlane* enemy = static_cast<EnemyPlane*>(IDManager::getPointer(_enemyName, ACTOR));
	double distance = enemy->getPosition().distance(FCKnowledge::getSingleton().getPlayerPosition());
	
	return distance < 100.0;
}

void EnemyController::fire(void)
{
	EnemyPlane* enemy = static_cast<EnemyPlane*>(IDManager::getPointer(_enemyName, ACTOR));
	enemy->fire();
	//std::cout << enemy->getName()<<std::endl;
}

const std::string & EnemyControllerFactory::getTypeName() const
{
	static const std::string typeName("EnemyController");
	return typeName;
}
EnemyControllerFactory::pointer_type EnemyControllerFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	std::string fighterName;
	if(parameter)
	{
		parameter_iterator it = parameter->find("fighterName");

		if(it!= parameter->end())
		{
			fighterName = VariantData<std::string>::get(it->second);
		}
	}
	return pointer_type(new EnemyController(instanceName, fighterName));
}