
#include "DemoActor.h"

#include "OgreRangeTriggerConditions.h"

using namespace Orz;
DemoActor::DemoActor(const std::string & name):Actor(name),_animState(NULL),_move(true)
{
}
DemoActor::~DemoActor(void)
{

}

void DemoActor::triggerAction(int id, Event * evt)
{
	std::cout<<id<<std::endl;
	
	_animState2->setEnabled(true);
	_move = false;
}
//这个函数会在初始化调用
void DemoActor::doEnable(void)
{
	Ogre::SceneManager * mSceneMgr = OgreGraphicsManager::getSingleton().getSceneManager();
    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    // Create a skydome
    mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    // Accept default settings: point light, white diffuse, just set position
    // NB I could attach the light to a SceneNode if I wanted it to move automatically with
    //  other objects, but I don't
    l->setPosition(20,80,50);

	Ogre::Entity *ent;

    // Define a floor plane mesh
    Ogre::Plane p;
    p.normal =  Ogre::Vector3::UNIT_Y;
    p.d = 200;
    Ogre::MeshManager::getSingleton().createPlane(
        "FloorPlane",  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
        p, 200000, 200000, 20, 20, true, 1, 50, 50,  Ogre::Vector3::UNIT_Z);

    // Create an entity (the floor)
    ent = mSceneMgr->createEntity("floor", "FloorPlane");
    ent->setMaterialName("Examples/RustySteel");
    // Attach to child of root node, better for culling (otherwise bounds are the combination of the 2)
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);

    // Add a head, give it it's own node
    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    ent = mSceneMgr->createEntity("head", "ogrehead.mesh");
    headNode->attachObject(ent);

    // Make sure the camera track this node
    OgreGraphicsManager::getSingleton().getCamera()->setAutoTracking(true, headNode);

    // Create the camera node & attach camera
	Ogre:: SceneNode* camNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    camNode->attachObject(OgreGraphicsManager::getSingleton().getCamera());

    // set up spline animation of node
	Ogre::Animation* anim = mSceneMgr->createAnimation("CameraTrack", 10);
    // Spline it for nice curves
    anim->setInterpolationMode( Ogre::Animation::IM_SPLINE);
    // Create a track to animate the camera's node
    Ogre::NodeAnimationTrack* track = anim->createNodeTrack(0, headNode);
    // Setup keyframes
    Ogre::TransformKeyFrame* key = track->createNodeKeyFrame(0); // startposition
	
    key->setTranslate( Ogre::Vector3(-500,0,100));
    key = track->createNodeKeyFrame(5);
    key->setTranslate( Ogre::Vector3(500,0,100));
    key = track->createNodeKeyFrame(10);
    key->setTranslate( Ogre::Vector3(-500,0,100));
    // Create a new animation state to track this
   _animState = mSceneMgr->createAnimationState("CameraTrack");
   _animState->setEnabled(true);


	anim = mSceneMgr->createAnimation("up", 2);
    // Spline it for nice curves
    anim->setInterpolationMode( Ogre::Animation::IM_SPLINE);
    // Create a track to animate the camera's node
    track = anim->createNodeTrack(0, headNode);
    // Setup keyframes
    key = track->createNodeKeyFrame(0); // startposition
	
    key->setTranslate( Ogre::Vector3(0,0,0));
    key = track->createNodeKeyFrame(1);
    key->setTranslate( Ogre::Vector3(0,50,0));
    key = track->createNodeKeyFrame(2);
    key->setTranslate( Ogre::Vector3(0,0,0));
    // Create a new animation state to track this
    _animState2 = mSceneMgr->createAnimationState("up");
	_animState2->setLoop(false);
	_animState2->setEnabled(false);

    // Put in a bit of fog for the hell of it
    mSceneMgr->setFog( Ogre::FOG_EXP,  Ogre::ColourValue::White, 0.0002);



	enableUpdate();
}

//销毁之前调用
void DemoActor::doDisable(void)
{

}
void DemoActor::doFrame(void)
{
	//_trigger->update();
	if(_move)
	{
		_animState->addTime(0.015f);
	}else
	{
		_animState2->addTime(0.015f);
		if(_animState2->hasEnded())
		{
			_move = true;
			_animState2->setTimePosition(0.f);
			
			_animState2->setEnabled(false);

		}
	}
}

		


const std::string & DemoActorFactory::getTypeName() const
{
	static const std::string typeName("DemoActor");	
	return typeName;
}
DemoActorFactory::pointer_type DemoActorFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	return pointer_type(new DemoActor(instanceName));
}

