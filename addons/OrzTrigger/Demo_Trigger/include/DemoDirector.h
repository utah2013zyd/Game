#ifndef __Orz_DemoTrigger_DemoDirector_h__
#define __Orz_DemoTrigger_DemoDirector_h__

#include <Ogre/Ogre.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/Framework_Base/Logic.h>
namespace Orz
{

	//这个类继承与框架提供的Director（导演），其作用是调控整个游戏
	class DemoDirector : public Director
	{
	public:
		DemoDirector(const std::string & name = IDManager::BLANK);
		virtual ~DemoDirector(void);
		
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//(Day2)在每一个frame调用
	private:
		ActorPtr _actor;
	};
	

		

	//同时为了配合插件体系，我们提供了相应的导演工厂类。用于系统来“生产”相应导演。
	class DemoDirectorFactory: public IFactory<Director>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 