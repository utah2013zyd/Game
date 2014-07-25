#ifndef __Orz_FC_FCFighter_h__
#define __Orz_FC_FCFighter_h__
#include "FCConfig.h"
#include "FCMovable.h"
#include "Bullets.h"
#include <orz/Toolkit_Base/FSMLogic.h>
namespace Orz
{


	//同时为了配合插件体系，我们提供了相应的导演工厂类。用于系统来“生产”相应。
	class FCFighterFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 