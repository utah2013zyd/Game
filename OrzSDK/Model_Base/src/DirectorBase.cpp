#include "ModelBaseConfig.h"
#include "DirectorBase.h"

#pragma warning(push)
#pragma warning(disable:4819)
#include <Ogre/Ogre.h>
#pragma warning(pop)
#include <orz/View_OGRE3D/OgreGraphicsManager.h>


using namespace Orz;
DirectorBase::DirectorBase(const std::string & name):Director(name)
{
	std::cout<<"TheaterBase(const std::string & name = IDManager::BLANK):Theater(name)"<<std::endl;
	
	
}
DirectorBase::~DirectorBase(void)
{
	
}


void DirectorBase::doEnable(void)
{

	Ogre::SceneManager * sm = Orz::OgreGraphicsManager::getSingleton().getSceneManager();
	Ogre::Camera * camera = Orz::OgreGraphicsManager::getSingleton().getCamera();
	// Set ambient light
	sm->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	// Create a point light
	Ogre::Light* l = sm->createLight("MainLight");
    // Accept default settings: point light, white diffuse, just set position
    // NB I could attach the light to a SceneNode if I wanted it to move automatically with
    //  other objects, but I don't
    l->setPosition(20,80,50);

	Ogre::Entity *ent = sm->createEntity("head", "ogrehead.mesh");


    // Set material loaded from Example.material
	ent->setMaterialName("Examples/EnvMappedRustySteel");

    // Add entity to the root scene node
	Ogre::SceneNode *sn =  sm->getRootSceneNode()->createChildSceneNode();
	sn->attachObject(ent);
	camera->move(Ogre::Vector3(30.0f, 30.0f, 30.0f));
	camera->lookAt(sm->getRootSceneNode()->getPosition());

}


void DirectorBase::doDisable(void)
{
	
}
	
	
const std::string & DirectorBaseFactory::getTypeName() const
{
	static const std::string typeName("TheaterBase");	
	return typeName;
}
DirectorBaseFactory::pointer_type DirectorBaseFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	return pointer_type(new DirectorBase(instanceName));
}
	
