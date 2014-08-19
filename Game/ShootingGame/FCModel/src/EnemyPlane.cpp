#include "EnemyPlane.h"
using namespace Orz;

EnemyPlane::EnemyPlane(const std::string & name, Ogre::Vector3 initPos, int queryFlag, double speedLimit):
	FCFighter(name, initPos, queryFlag, speedLimit)
{
	
}

EnemyPlane::~EnemyPlane(void)
{

}

void EnemyPlane::doEnable(void)
{
	FCFighter::doEnable();
}

void EnemyPlane::doDisable(void)
{
	FCFighter::doDisable();
}

void EnemyPlane::doFrame(void)
{
	FCFighter::doFrame();
}

const std::string & EnemyPlaneFactory::getTypeName() const
{
	static const std::string typeName("EnemyPlane");
	return typeName;
}
EnemyPlaneFactory::pointer_type EnemyPlaneFactory::createInstance(const std::string& instanceName, parameter_type parameter)
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
	return pointer_type(new EnemyPlane(instanceName, initPos, queryFlag, speedLimit));
}