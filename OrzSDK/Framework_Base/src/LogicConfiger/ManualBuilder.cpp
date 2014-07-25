#include <orz/Framework_Base/LogicConfiger/ManualBuilder.h>
#include "LogicConfiger/Data.h"
using namespace Orz;
using namespace LogicConfiger;
ManualBuilder::ManualBuilder(void)
{

}
ManualBuilder::~ManualBuilder(void)
{
	
}
bool ManualBuilder::write(Data & data) const
{
	data.setTheater(_theater.first, _theater.second);

	for(std::vector<std::string>::const_iterator it = _plugins.begin(); it != _plugins.end(); ++it)
	{
		data.addPlugin(*it);
	}


	
	for(std::vector<std::pair<std::string, std::string> >::const_iterator it = _directors.begin(); it != _directors.end(); ++it)
	{
		data.addDirector(it->first, it->second);
	}

	data.setActiveDirector(_activeDirector);


	return true;
}
void ManualBuilder::setActiveDirector(const std::string & active)
{
	_activeDirector = active;
}

void ManualBuilder::setTheater(const std::string & type, const std::string & name)
{
	_theater.first = type;
	_theater.second = name;
}

void ManualBuilder::addDirector(const std::string & type, const std::string & name)
{
	_directors.push_back(std::make_pair(type, name));
}
void ManualBuilder::addPlugin(const std::string & name)
{
	_plugins.push_back(name);
}