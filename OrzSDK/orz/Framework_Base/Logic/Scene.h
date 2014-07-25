#ifndef __Orz_FrameworkBase_Scene_h__
#define __Orz_FrameworkBase_Scene_h__
#include <orz/Framework_Base/FrameworkBaseConfig.h>

#include <orz/Framework_Base/Logic/Entity.h>

namespace Orz{

class _OrzFrameworkBaseExport Scene:public Entity
{
public:
	Scene(const std::string & name);
	virtual ~Scene(void);
};

}
#endif