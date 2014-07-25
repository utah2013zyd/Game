#ifndef __Orz_FrameworkBase_Director_h__
#define __Orz_FrameworkBase_Director_h__
#include <orz/Framework_Base/FrameworkBaseConfig.h>

#include <orz/Framework_Base/Logic/Entity.h>

namespace Orz{

class _OrzFrameworkBaseExport Director:public Entity
{
public:

	Director(const std::string & name);
	virtual ~Director(void);
};

}
#endif