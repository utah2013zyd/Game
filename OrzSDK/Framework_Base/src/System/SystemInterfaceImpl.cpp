#include "System/SystemInterfaceImpl.h"
#include <orz/Toolkit_Base/Exception.h>
#include <orz/Toolkit_Base/LogManager.h>
using namespace Orz;



SystemInterfaceImpl::SystemInterfaceImpl(void)
:_parames(),
_clock(),
_now(0),
_exit(false)
{
	
}
SystemInterfaceImpl::~SystemInterfaceImpl(void)
{

}
	
void SystemInterfaceImpl::exit(void)
{
	_exit = true;
}
bool SystemInterfaceImpl::running(void) const
{
	return !_exit;
}

void SystemInterfaceImpl::setParame(const std::string & key, const Variant & value)
{
	ParameMap::iterator it = _parames.find(key);
	if(it == _parames.end())
	{
		_parames.insert(std::make_pair(key, value));
	}
	else
	{
		it->second = value;
	}
}
void SystemInterfaceImpl::debugParame(void) const
{
	ParameMap::const_iterator it;
	for(it = _parames.begin(); it != _parames.end(); ++it)
	{
		if(ILogManager::getSingletonPtr())
			ILogManager::getSingleton().utf8(it->first);
		else
			std::cerr<<it->first<<std::endl;
	}

}
bool SystemInterfaceImpl::hasParame(const std::string & key) const
{
	ParameMap::const_iterator it = _parames.find(key);
	return (it != _parames.end());
}
const Variant & SystemInterfaceImpl::getParame(const std::string & key) const
{

	
	ParameMap::const_iterator it = _parames.find(key);
	if(it == _parames.end())
	{
		throw ORZ_EXCEPTION(Exception::ERR_ITEM_NOT_FOUND);
	}
	return it->second;
}
	
TimeType SystemInterfaceImpl::getInterval(void)
{
	TimeType temp = _clock.elapsed();
	if(_now > temp)
	{
		_now -= _clock.elapsed_max();
	}
	TimeType interval = temp - _now;
	_now = temp;
	return interval;
}
void SystemInterfaceImpl::reset(void)
{
	_exit = false;
	_clock.restart();
	_now =_clock.elapsed();
}

