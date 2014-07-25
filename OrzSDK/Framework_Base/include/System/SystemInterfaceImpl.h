#ifndef __Orz_FrameworkBase_SystemInterfaceImpl_h__
#define __Orz_FrameworkBase_SystemInterfaceImpl_h__
#include <orz/Framework_Base/FrameworkBaseConfig.h>
#include <orz/Toolkit_Base/Clock.h>
namespace Orz{

class _OrzToolkitBasePrivate SystemInterfaceImpl
{

public:
	SystemInterfaceImpl(void);
	~SystemInterfaceImpl(void);
	
	void exit(void);

	void setParame(const std::string & key, const Variant & value);
	bool hasParame(const std::string & key) const;
	const Variant & getParame(const std::string & key) const;
	void debugParame(void) const;
	bool running(void) const;
	TimeType getInterval(void);
	void reset(void);
	

private:
	typedef std::map<std::string, Variant> ParameMap; 
	ParameMap _parames;
	Clock<TimeType, 1> _clock;
	TimeType _now;
	bool _exit;

};
}


#endif