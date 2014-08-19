#include "FCConfig.h"
#include "FCFighter.h"
#include "FCFighterLogic.h"
#include <orz/Toolkit_Base/FSMLogic.h>

#pragma once

namespace Orz
{
	class EnemyPlane: public FCFighter
	{
	public:
		EnemyPlane(const std::string & name = IDManager::BLANK, Ogre::Vector3 initPos = Ogre::Vector3(0.0, 0.0, 0.0), int queryFlag = 0x0, double speedLimit = 50);
		virtual ~EnemyPlane(void);

		void doEnable(void);
		void doDisable(void);
		void doFrame(void);							//called every frame

	};

	class EnemyPlaneFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};
}