#ifndef __Orz_FrameworkBase_Entity_h__
#define __Orz_FrameworkBase_Entity_h__
#include <orz/Framework_Base/FrameworkBaseConfig.h>

#include <orz/Toolkit_Base/EventSystem/EventHandler.h>
#include <orz/Toolkit_Base/IDManager/IDManager.h>
namespace Orz{

class _OrzFrameworkBaseExport Entity: public IDManager, public EventHandler
{
protected:
	enum Group
	{
		ACTOR,
		DIRECTOR,
		SCENE,
		THEATER,
		ENTITY,
		OTHER
	};
public:
	Entity(const std::string & name, IDManager::group_type group = ENTITY);
	virtual ~Entity(void);
};

}
#endif