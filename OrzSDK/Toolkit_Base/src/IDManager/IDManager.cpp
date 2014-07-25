#include <orz/Toolkit_Base/IDManager/IDManager.h>
#include "IDManager/IDManagerImpl.h"
using namespace Orz;

const std::string IDManager::BLANK = "No Name";

IDManager::IDManager(const std::string & name, group_type group):_impl(new IDManagerImpl(name, group))
{
	_impl->registerPointer(this);
}
	
/////拷贝构造函数
//IDManager::IDManager(const IDManager & manager):_impl(new IDManagerImpl(BLANK, manager.getType()))
//{
//
//}


//void IDManager::swap(IDManager& manager)
//{
//	_impl.swap(manager._impl);
//}

IDManager::id_type IDManager::getID(void) const
{
	return _impl->getID();
}


const std::string & IDManager::getName(void) const
{
	return _impl->getName();
}


IDManager::group_type IDManager::getGroup(void) const
{
	return _impl->getGroup();
}
IDManager::~IDManager(void)
{
}


///通过name和类型得到id_type
IDManager::id_type IDManager::getTargetID(const std::string & name, IDManager::group_type group)
{
	return IDManagerImpl::getTargetID(name, group);
}

///通过id_type得到指针
IDManager * IDManager::getPointer(IDManager::id_type id)
{
	return IDManagerImpl::getPointer(id);
}

