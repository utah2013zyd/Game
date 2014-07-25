#ifndef __Orz_FCBase_FCMapInfo_h__
#define __Orz_FCBase_FCMapInfo_h__


#include "FCBaseConfig.h"

namespace Orz
{

class _FCBaseExport FCMapInfo
{
public:
	FCMapInfo(void);
	virtual ~FCMapInfo(void);
	bool isEnable(void) const;
	void enable(void);
	void disable(void);
	
	float getGround(void) const;
	void setGround(float ground);
private:
	bool _enable;
	float _ground;
};


}
#endif