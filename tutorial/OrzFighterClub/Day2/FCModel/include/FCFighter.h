#ifndef __Orz_FC_FCFighter_h__
#define __Orz_FC_FCFighter_h__
#include "FCConfig.h"
#include "FCFighterLogic.h"
#include <orz/Toolkit_Base/FSMLogic.h>
namespace Orz
{

	class FCFighter : public Actor, private FSMLogic<FCFighter, FCFighterLogic>
	{
	public:
		FCFighter(const std::string & name = IDManager::BLANK, int pos = 0);
		virtual ~FCFighter(void);
		
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//1��ÿһ��frame����

		Ogre::Vector3 getPosition(void) const;
		
		void setPosition(const Ogre::Vector3 & pos);
		
	private:
		Ogre::SceneNode * _node;
		Ogre::Entity * _entity;
		int _pos;
		bool _start;
		bool _run; 
	
	};
	

		

	//ͬʱΪ����ϲ����ϵ�������ṩ����Ӧ�ĵ��ݹ����ࡣ����ϵͳ������������Ӧ��
	class FCFighterFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 