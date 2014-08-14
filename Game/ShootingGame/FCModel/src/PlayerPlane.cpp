#include "PlayerPlane.h"
using namespace Orz;

PlayerPlane::PlayerPlane(const std::string & name, Ogre::Vector3 initPos, int queryFlag, double speedLimit, double rotateAngle):
	FCFighter(name, initPos, queryFlag, speedLimit, rotateAngle)
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
	this->getNode()->attachObject(_star);

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