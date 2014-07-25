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
		void doFrame(void);//1在每一个frame调用

		
		void doExecute(Event * evt = NULL);
	private:
		Ogre::SceneNode * _node;
		Ogre::Entity * _entity;
		const int _pos;
	
	};
	

		

	//同时为了配合插件体系，我们提供了相应的导演工厂类。用于系统来“生产”相应。
	class FCWallFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 