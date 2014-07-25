#ifndef __Orz_DemoTrigger_DemoDirector_h__
#define __Orz_DemoTrigger_DemoDirector_h__

#include <Ogre/Ogre.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/Framework_Base/Logic.h>
namespace Orz
{

	//�����̳������ṩ��Director�����ݣ����������ǵ���������Ϸ
	class DemoDirector : public Director
	{
	public:
		DemoDirector(const std::string & name = IDManager::BLANK);
		virtual ~DemoDirector(void);
		
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//(Day2)��ÿһ��frame����
	private:
		ActorPtr _actor;
	};
	

		

	//ͬʱΪ����ϲ����ϵ�������ṩ����Ӧ�ĵ��ݹ����ࡣ����ϵͳ������������Ӧ���ݡ�
	class DemoDirectorFactory: public IFactory<Director>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 