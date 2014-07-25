#ifndef __Orz_Test_Config_h__
#define __Orz_Test_Config_h__

#define BOOST_TEST_MODULE Test_Toolkit_Base
#include <boost/test/unit_test.hpp>

#include <orz/Framework_Base/System.h>
#include <orz/Toolkit_Base/TimerManager.h>
#include <orz/Toolkit_Base/DynLibManager.h>
#include <orz/Toolkit_Base/LogManager.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/View_OGRE3D/CEGUIManager.h>
#include <orz/View_OIS/OISInputManager.h>
#include <orz/View_Fmod/FmodSoundManager.h>

#include <orz/View_OgreNewt/OgreNewtManager.h>



namespace Orz
{
//
//SYSTEMUNIT_BEGIN(LogManager)
//SYSTEMUNIT_END(LogManager)
//
//template<>
//class SystemFactory<LogManager>
//{
//public:
//	static boost::shared_ptr<LogManager> createInstance(void)
//	{
//		return boost::shared_ptr<LogManager>(new LogManager());
//	}
//};
//
//
//
//SYSTEMUNIT_BEGIN(TimerManager)
//SYSTEMUNIT_UPDATE
//SYSTEMUNIT_END(TimerManager)
//
//template<>
//class SystemFactory<TimerManager>
//{
//public:
//	static boost::shared_ptr<TimerManager> createInstance(void)
//	{
//		return boost::shared_ptr<TimerManager>(new TimerManager());
//	}
//};
//
//
//SYSTEMUNIT_BEGIN(OgreGraphicsManager)
//SYSTEMUNIT_INIT
//SYSTEMUNIT_UPDATE
//SYSTEMUNIT_SHUTDOWN
//SYSTEMUNIT_END(OgreGraphicsManager)
//
//template<>
//class SystemFactory<OgreGraphicsManager>
//{
//public:
//	static boost::shared_ptr<OgreGraphicsManager> createInstance(void)
//	{
//		return boost::shared_ptr<OgreGraphicsManager>(new OgreGraphicsManager());
//	}
//};
//
//
//
//
//
//
//SYSTEMUNIT_BEGIN(IInputManager)
//SYSTEMUNIT_INIT
//SYSTEMUNIT_UPDATE
//SYSTEMUNIT_SHUTDOWN
//SYSTEMUNIT_END(IInputManager)
//
//template<>
//class SystemFactory<IInputManager>
//{
//public:
//	static boost::shared_ptr<IInputManager> createInstance(void)
//	{
//		return boost::shared_ptr<IInputManager>(new InputManager());
//	}
//};
//
//
//
//
//SYSTEMUNIT_BEGIN(OgreNewtManager)
//SYSTEMUNIT_INIT
//SYSTEMUNIT_UPDATE
//SYSTEMUNIT_SHUTDOWN
//SYSTEMUNIT_END(OgreNewtManager)
//
//template<>
//class SystemFactory<OgreNewtManager>
//{
//public:
//	static boost::shared_ptr<OgreNewtManager> createInstance(void)
//	{
//		return boost::shared_ptr<OgreNewtManager>(new OgreNewtManager());
//	}
//};
}
#endif