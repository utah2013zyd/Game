#ifndef __Orz_FCBase_FCMapBlock_h__
#define __Orz_FCBase_FCMapBlock_h__


#include "FCBaseConfig.h"
#include "FCMapInfo.h"
#include "FCPather.h"
#include <orz/Toolkit_Base/Singleton.h>
namespace Orz
{
class _FCBasePrivate FCMapBlock
{


public:
	
	FCMapBlock(void):
	  _blocker(NULL),
	  _type(FCPather::WAY),
	  _value(1.0f)
	{
	
	}
	~FCMapBlock(void)
	{
	
	}

	void setValue(float value = 1.0f)
	{
		_value = value;
	}
	float getValue(void) const
	{
		return _value;
	}

	void addBlocker(EventHandler * blocker)
	{
		_blocker = blocker;
	}
	
	void removeBlocker(EventHandler * blocker)
	{
		_blocker = NULL;
	}

	void setWay(FCPather::Type type, float value = 1.0f)
	{
		_type = type;
		_value = value;
	}
	EventHandler * getBlocker(void) const
	{
		return _blocker;
	}


	void setType(FCPather::Type type)
	{
		_type = type;
	}
	FCPather::Type getType(void) const
	{
		return _type;
	}
private:
	EventHandler * _blocker;
	FCPather::Type _type;
	float _value;
};


}

#endif