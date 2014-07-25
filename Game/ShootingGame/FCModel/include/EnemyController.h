#include "FCConfig.h"
#include "FCFighter.h"
#include "EnemyControllerLogic.h"
#include <orz/Toolkit_Base/FSMLogic.h>

#pragma once
namespace Orz
{

	class EnemyController: public Actor, private FSMLogic<EnemyController, EnemyControllerLogic>
	{
	public:
		EnemyController(const std::string & name = IDManager::BLANK, const std::string & fighterName = IDManager::BLANK);
		~EnemyController(void);

		void doEnable(void);
		void doDisable(void);
		void doFrame(void);
		void doExecute(Event *evt);

		void approach(void);
		bool withinReach(void);
		void fire(void);

	private:
		std::string _enemyName;

	};

		//同时为了配合插件体系，我们提供了相应的导演工厂类。用于系统来“生产”相应。
	class EnemyControllerFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}