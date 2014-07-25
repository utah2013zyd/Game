#include <orz/Framework_Base/Logic/Entity.h>

using namespace Orz;


Entity::Entity(const std::string & name, IDManager::group_type group):
IDManager(name, group),
EventHandler()
{
	
}
Entity::~Entity(void)
{
	
}

