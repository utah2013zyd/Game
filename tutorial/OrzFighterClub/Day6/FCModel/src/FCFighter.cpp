#include "FCFighter.h"
#include "FCMap.h"
#include "FCEvents.h"
#include "FighterBase.h"


using namespace Orz;

const std::string & FCFighterFactory::getTypeName() const
{
	static const std::string typeName("FCFighter");
	return typeName;
}
FCFighterFactory::pointer_type FCFighterFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{

	int pos = 0;
	int x = 0;
	int y = 0;
	int id = 0;
	if(parameter)
	{
		parameter_iterator it = parameter->find("pos");
		if(it!= parameter->end())
		{
			pos = boost::get<int>(it->second);
			x = pos/16;
			y = pos%16;
		}

		it = parameter->find("id");
		if(it!= parameter->end())
		{
			id = boost::get<int>(it->second);
		}
	}


	return pointer_type(new FighterBase(instanceName, x, y, id));
}


