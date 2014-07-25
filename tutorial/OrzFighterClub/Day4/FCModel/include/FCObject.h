#ifndef __Orz_FC_FCObject_h__
#define __Orz_FC_FCObject_h__
#include "FCConfig.h"
#include <orz/Toolkit_Base/FSMLogic.h>
namespace Orz
{

	class FCObject : public Actor
	{
	public:
		FCObject(const std::string & name = IDManager::BLANK);
		virtual ~FCObject(void);
		
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//1在每一个frame调用

		Ogre::Vector3 getPosition(void) const;
		bool nextPosition();
		void setPosition(const Ogre::Vector3 & pos);
		
		Ogre::Vector3 getTarget(void) const;
	private:
	
	};
	

		

}

#endif 