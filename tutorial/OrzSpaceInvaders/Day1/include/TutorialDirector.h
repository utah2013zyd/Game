#ifndef __Orz_Tutorial_TutorialDirector_h__
#define __Orz_Tutorial_TutorialDirector_h__

#include <orz/Framework_Base/Logic.h>

#include <orz/Framework_Base/FrameworkBase.h>


#include <Ogre/Ogre.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>

namespace Orz
{

	//这个类继承与框架提供的Director（导演），其作用是调控整个游戏
	class TutorialDirector : public Director
	{
	public:
		TutorialDirector(const std::string & name = IDManager::BLANK);
		virtual ~TutorialDirector(void);
		
		void doEnable(void);
		void doDisable(void);
	};

	//同时为了配合插件体系，我们提供了相应的导演工厂类。用于系统来“生产”相应导演。
	class TutorialDirectorFactory: public IFactory<Director>
	{
	public:
		virtual const std::string & getTypeName() const;
		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 