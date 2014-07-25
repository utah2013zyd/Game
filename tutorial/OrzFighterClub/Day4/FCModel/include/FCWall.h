#ifndef __Orz_FC_FCWall_h__
#define __Orz_FC_FCWall_h__
#include "FCConfig.h"
#include "FCObject.h"

namespace Orz
{

	class FCWall : public FCObject
	{
	public:
		FCWall(const std::string & name = IDManager::BLANK, int pos = 0);
		virtual ~FCWall(void);
		
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//1��ÿһ��frame����

		
		void doExecute(Event * evt = NULL);
	private:
		Ogre::SceneNode * _node;
		Ogre::Entity * _entity;
		const int _pos;
	
	};
	

		

	//ͬʱΪ����ϲ����ϵ�������ṩ����Ӧ�ĵ��ݹ����ࡣ����ϵͳ������������Ӧ��
	class FCWallFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 