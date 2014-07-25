#include "ModelBaseConfig.h"
#include "TheaterBase.h"
#include "DirectorBase.h"

using namespace Orz;
TheaterBase::TheaterBase(const std::string & name):Theater(name)
{
	std::cout<<"TheaterBase(const std::string & name = IDManager::BLANK):Theater(name)"<<std::endl;
}
TheaterBase::~TheaterBase(void)
{
	
}
void TheaterBase::doActiveDirector(DirectorPtr director)
{
	if(_activeDirector)
	{
		if(isInWorld())
			getWorld()->goOut(_activeDirector);
	}

	_activeDirector = director;

	if(isInWorld())
	{
		getWorld()->comeIn(_activeDirector);
	}
	
}

void TheaterBase::doEnable(void)
{
	if(_activeDirector)
	{
		getWorld()->comeIn(_activeDirector);
	}
	else
	{
		doActiveDirector(DirectorPtr(new DirectorBase()));
	}
}


void TheaterBase::doDisable(void)
{
	if(_activeDirector)
	{
		getWorld()->goOut(_activeDirector);
	}
}
	
	
const std::string & TheaterBaseFactory::getTypeName() const
{
	static const std::string typeName("TheaterBase");	
	return typeName;
}
TheaterBaseFactory::pointer_type TheaterBaseFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	return pointer_type(new TheaterBase(instanceName));
}
	
