#ifndef __Orz_DemoTrigger_DemoActor_h__
#define __Orz_DemoTrigger_DemoActor_h__

#include <Ogre/Ogre.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/Framework_Base/Logic.h>

#include <orz/Toolkit_Trigger/Trigger.h>
namespace Orz
{

	//�����̳������ṩ��Director�����ݣ����������ǵ���������Ϸ
	class DemoActor : public Actor, public TriggerCallback
	{
	public:
		DemoActor(const std::string & name = IDManager::BLANK);
		virtual ~DemoActor(void);
		
		void triggerAction(int id, Event * evt);
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//(Day2)��ÿһ��frame����
	private:
		
		Ogre::AnimationState* _animState;
		Ogre::AnimationState* _animState2;
		bool _move;
		boost::shared_ptr<Trigger> _trigger;
	};
	

		

	//ͬʱΪ����ϲ����ϵ�������ṩ����Ӧ�ĵ��ݹ����ࡣ����ϵͳ������������Ӧ���ݡ�
	class DemoActorFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 