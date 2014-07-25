#ifndef __Orz_Test_OrzAR_h__
#define __Orz_Test_OrzAR_h__
#include "config.h"
#include <iostream>

#include <orz/Framework_Base/FrameworkBase.h>
#include <orz/Framework_Base/System.h>
#include <orz/Toolkit_Base/TimerManager.h>
#include <orz/Toolkit_Base/DynLibManager.h>
#include <orz/Toolkit_Base/LogManager.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/View_OGRE3D/CEGUIManager.h>
#include <orz/View_OIS/OISInputManager.h>
#include <orz/View_Fmod/FmodSoundManager.h>
#include <orz/View_OgreNewt/OgreNewtManager.h>
#include <orz/Toolkit_Base/Singleton.h>
#include "OgreNewt.h"
#include "OgreNewt_CollisionPrimitives.h"
#include <Ogre.h>

class Box :public 
	OgreNewt::ConvexCollision
{
public:
	//! constructor
	/*!
	Create a 'blank' box collision object.  Can be used for CollisionSerializer::importCollision
	\param world pointer to the OgreNewt::World
	*/
	Box(const OgreNewt::World* world): ConvexCollision( world )
	{}

	Box(const  OgreNewt::World* world, const Ogre::Vector3& size,  int id,
		const Ogre::Quaternion& orient = Ogre::Quaternion::IDENTITY, const Ogre::Vector3& pos = Ogre::Vector3::ZERO ) : ConvexCollision( world )
	{
		float matrix[16];

		OgreNewt::Converters::QuatPosToMatrix( orient, pos, &matrix[0] );

		m_col = NewtonCreateBox( m_world->getNewtonWorld(), (float)size.x, (float)size.y, (float)size.z, id, &matrix[0] );
	}


	//! constructor
	/*!
	\param world pointer to OgreNewt::World
	\param size vector representing width, height, depth
	\param id an id for this shape, can be received with collision->getUserId()
	\param orient orientation offset of the primitive
	\param pos position offset of the primitive
	*/

};
class OgreNewtRunning
{
public:


	enum BodType { BT_BASIC, BT_CONVEYOR };

	const OgreNewt::MaterialID* mMatDefault;
	const OgreNewt::MaterialID* mMatConveyor;
	OgreNewt::MaterialPair* mMatPairDefaultConveyor;


public:

	OgreNewtRunning()
	{
		//	
		using namespace Ogre;

		Ogre::SceneManager * mSceneMgr = Orz::OgreGraphicsManager::getSingleton().getSceneManager();
		mSceneMgr->clearScene();


		OgreNewt::World * m_World = Orz::OgreNewtManager::getSingleton().getNewtWorld();
		m_World->setWorldSize(Ogre::Vector3(-500.0,-500,-500.0),Ogre::Vector3(500.0,500.0,500.0));
		m_World->setFrictionModel(OgreNewt::World::FM_ADAPTIVE);
		m_World->setSolverModel(OgreNewt::World::SM_EXACT);
		//m_World->setMinimumFrameRate(0.1f);

		mMatDefault = m_World->getDefaultMaterialID();
		mMatConveyor = new OgreNewt::MaterialID( m_World );

		// sky box.
		//mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox");


		//createEvent();


		// shadows on!
		mSceneMgr->setShadowTechnique( Ogre::SHADOWTYPE_STENCIL_ADDITIVE );


		// this will be a static object that we can throw objects at.  we'll use a simple cylinder primitive.
		// first I load the visual mesh that represents it.  I have some simple primitive shaped .mesh files in
		// the "primitives" directory to make this simple... all of them have a basic size of "1" so that they
		// can easily be scaled to fit any size primitive.

		// make a simple rope.
		Ogre::Vector3 size(3,1.5,1.5);
		Ogre::Vector3 pos(0,50,0);
		Ogre::Quaternion orient = Ogre::Quaternion::IDENTITY;

		// loop through, making bodies and connecting them.
		OgreNewt::Body* parent = NULL;
		OgreNewt::Body* child = NULL;

		for (int x=0;x<5;x++)
		{
			// make the next box.
			child = makeSimpleBox(size, pos, orient);

		}


		// floor object!
		Ogre::Entity* floor1;
		Ogre::SceneNode* floornode1;
		floor1 = mSceneMgr->createEntity("Floor1", "simple_terrain.mesh" );
		floornode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "FloorNode1" );
		floornode1->attachObject( floor1 );
		floor1->setMaterialName( "Simple/BeachStones" );
		floor1->setCastShadows( false );

		//Ogre::Vector3 siz(100.0, 10.0, 100.0);
		/*OgreNewt::CollisionPtr col1 = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::TreeCollision( m_World, floor1, true, 0 ));
		OgreNewt::Body* bod1 = new OgreNewt::Body( m_World, col1  );*/
		OgreNewt::ConvexCollisionPtr col1 = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull( m_World, floor1,  0 ));
		OgreNewt::Body* bod1 = new OgreNewt::Body( m_World, col1 );

		Ogre::Vector3 inertia1, offset1;
		col1->calculateInertialMatrix(inertia1, offset1);
		col1.reset();

		bod1->setMassMatrix( 100.0, 100.0*inertia1 );
		bod1->attachNode( floornode1 );
		bod1->setPositionOrientation( Ogre::Vector3(0.0,0,0.0), Ogre::Quaternion::IDENTITY );

		
		//make a light
		Ogre::Light* light;

		light = mSceneMgr->createLight( "Light1" );
		light->setType( Ogre::Light::LT_POINT );
		light->setPosition( Ogre::Vector3(0.0, 100.0, 100.0) );


		OgreNewt::Debugger& debug(m_World->getDebugger());
		debug.init(mSceneMgr);
		debug.showDebugInformation();
		//debug.startRaycastRecording();
		//debug.clearRaycastsRecorded();

	}

	OgreNewt::Body* makeSimpleBox( Ogre::Vector3& size, Ogre::Vector3& pos, Ogre::Quaternion& orient )
	{

		Ogre::SceneManager * mSceneMgr = Orz::OgreGraphicsManager::getSingleton().getSceneManager();
		OgreNewt::World * m_World = Orz::OgreNewtManager::getSingleton().getNewtWorld();

		Ogre::Entity* box1;
		Ogre::SceneNode* box1node;
		static int mEntityCount = 0;

		box1 = mSceneMgr->createEntity( "Entity"+Ogre::StringConverter::toString(mEntityCount++), "box.mesh" );
		box1node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		box1node->attachObject( box1 );
		box1node->setScale( size );

		OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box( m_World, size, 0 ));
		OgreNewt::Body* bod = new OgreNewt::Body( m_World, col );


		// base mass on the size of the object.
		Ogre::Real mass = size.x * size.y * size.z * 2.5;

		// calculate the inertia based on box formula and mass
		Ogre::Vector3 inertia, offset;
		col->calculateInertialMatrix(inertia, offset);
#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
		delete col;
#endif

		bod->attachNode( box1node );
		bod->setMassMatrix( mass, mass*inertia );
		bod->setCenterOfMass(offset);
		bod->setStandardForceCallback();

		box1->setMaterialName( "Simple/BumpyMetal" );


		bod->setPositionOrientation( pos, orient );

		return bod;
	}



	~OgreNewtRunning(void)
	{

		OgreNewt::World * m_World = Orz::OgreNewtManager::getSingleton().getNewtWorld();

		OgreNewt::Debugger& debug(m_World->getDebugger());
		debug.hideDebugInformation();
		//debug.clearRaycastsRecorded();
		//debug.stopRaycastRecording();
	
		Ogre::SceneManager * mSceneMgr = Orz::OgreGraphicsManager::getSingleton().getSceneManager();
		mSceneMgr->clearScene();

	}
	bool update(void)
	{

		OgreNewt::World * m_World = Orz::OgreNewtManager::getSingleton().getNewtWorld();		
		OgreNewt::Debugger& debug(m_World->getDebugger());
		debug.showDebugInformation();
		//debug.startRaycastRecording();

		//debug.clearRaycastsRecorded();
	
		return true;
	}

private:


};


BOOST_AUTO_TEST_SUITE( OrzAR )

struct F {
	F() : i( 0 ) {
		using namespace Orz;
		system.reset(new SystemList<boost::mpl::list<SimpleLogManager, SimpleTimerManager, OgreGraphicsManager, OgreNewtManager, OISInputManager> >());
		BOOST_TEST_MESSAGE( "setup fixture" ); 
	}
	~F()         { BOOST_TEST_MESSAGE( "teardown fixture" ); }


	Orz::SystemPtr system;
	int i;
};
//#if TRACKER_SINGLE

BOOST_FIXTURE_TEST_CASE(simple, F)
{


	Orz::SystemInterface::getSingleton().setParame<bool>("w32_mouse",true);
	Orz::SystemInterface::getSingleton().setParame<float>("physics_speed", 1.0f);
	if(system->init())
	{
		boost::shared_ptr<OgreNewtRunning>  runing(new OgreNewtRunning());

		Orz::TimerPtr timer = Orz::ITimerManager::getSingleton().createTimer(boost::bind(&OgreNewtRunning::update, runing.get()));
		timer->start(WORLD_UPDATE_INTERVAL);

		system->run();



		runing.reset();

		system->shutdown();
	}





}
//#endif
#if TRACKER_MULTI


BOOST_AUTO_TEST_CASE( Multie )
{

	Orz::ArTrackerMultiPtr at = Orz::TrackerFactory::createTrackerMulti();
	if(at->init())
	{
		at->execute();
	}

}

#endif
//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END()

#endif