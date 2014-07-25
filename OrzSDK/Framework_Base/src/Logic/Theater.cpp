#include <orz/Framework_Base/Logic/Theater.h>
#include "Logic/TheaterImpl.h"
using namespace Orz;


Theater::Theater(const std::string & name):
Entity(name, Entity::THEATER),
_impl(new TheaterImpl())
{

}

Theater::~Theater(void)
{
	
}

void Theater::addDirector(const std::string & type, const std::string & name, NameValueList * parameter)
{
	_impl->addDirector(type, name, parameter);
	doAddDirector(type, name, parameter);
}
void Theater::activeDirector(const std::string & name)
{
	doActiveDirector(_impl->activeDirector(name));
}

void Theater::doAddDirector(const std::string & type, const std::string & name, NameValueList * parameter)
{
	
}
void Theater::doActiveDirector(DirectorPtr director)
{

}
