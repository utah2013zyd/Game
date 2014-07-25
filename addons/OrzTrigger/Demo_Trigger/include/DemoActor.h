#ifndef __Orz_DemoTrigger_DemoActor_h__
#define __Orz_DemoTrigger_DemoActor_h__

#include <Ogre/Ogre.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/Framework_Base/Logic.h>

#include <orz/Toolkit_Trigger/Trigger.h>
namespace Orz
{

	//这个类继承与框架提供的Director（导演），其作用是调控整个游戏
	class DemoActor : public Actor, public TriggerCallback
	{
	public:
		DemoActor(const std::string & name = IDManager::BLANK);
		virtual ~DemoActor(void);
		
		void triggerAction(int id, Event * evt);
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//(Day2)在每一个frame调用
	private:
		
		Ogre::AnimationState* _animState;
		Ogre::AnimationState* _animState2;
		bool _move;
		boost::shared_ptr<Trigger> _trigger;
	};
	

		

	//同时为了配合插件体系，我们提供了相应的导演工厂类。用于系统来“生产”相应导演。
	class DemoActorFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 