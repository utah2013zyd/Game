
#include <orz/Framework_Base/System/SystemInterface.h>
#include "System/SystemInterfaceImpl.h"

using namespace Orz;
template<> SystemInterface* Singleton<SystemInterface>::_singleton = NULL;
SystemInterface::SystemInterface(void):_impl(new SystemInterfaceImpl()),_init(false)
{

}
SystemInterface::~SystemInterface(void)
{

}
	
void SystemInterface::run(void)
{

	reset();
	while(running())
	{
		if(!update(getInterval()))
		{
			break;
		}
	}

}
void SystemInterface::exit(void)
{
	_impl->exit();
}

void SystemInterface::_setParame(const std::string & key, const Variant & value)
{
	_impl->setParame(key, value);
}

void SystemInterface::debugParame(void) const
{
	_impl->debugParame();
}
bool SystemInterface::hasParame(const std::string & key) const
{
	return _impl->hasParame(key);
}

bool SystemInterface::running(void) const
{
	return _impl->running();
}
TimeType SystemInterface::getInterval(void)
{
	return _impl->getInterval();
}
void SystemInterface::reset(void)
{
	_impl->reset();
}
const Variant & SystemInterface::_getParame(const std::string & key) const
{
	return _impl->getParame(key);
}

bool SystemInterface::isInit(void){return _init;}
bool SystemInterface::init(void){_init = true; return true;}
void SystemInterface::shutdown(void){_init = false; }
bool SystemInterface::update(TimeType){return true;}


