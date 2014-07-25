#ifndef __Orz_FC_FCFighter_h__
#define __Orz_FC_FCFighter_h__
#include "FCConfig.h"
#include "FCMovable.h"
#include "Bullets.h"
#include <orz/Toolkit_Base/FSMLogic.h>
namespace Orz
{


	//ͬʱΪ����ϲ����ϵ�������ṩ����Ӧ�ĵ��ݹ����ࡣ����ϵͳ������������Ӧ��
	class FCFighterFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 