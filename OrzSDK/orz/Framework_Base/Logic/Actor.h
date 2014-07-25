#ifndef __Orz_FrameworkBase_Actor_h__
#define __Orz_FrameworkBase_Actor_h__
#include <orz/Framework_Base/FrameworkBaseConfig.h>

#include <orz/Framework_Base/Logic/Entity.h>

namespace Orz{

class _OrzFrameworkBaseExport  Actor:public Entity
{
public:
	Actor(const std::string & name);
	virtual ~Actor(void);
};

}
#endif