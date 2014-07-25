#include <orz/Toolkit_Base/Exception.h>
#include "IDManager/IDManagerImpl.h"
using namespace Orz;


IDManagerImpl::GroupMap IDManagerImpl::_maps;
IDManagerImpl::PointerMap IDManagerImpl::_id2pointer;
IDManager::id_type IDManagerImpl::_count_id = 1;

IDManagerImpl::~IDManagerImpl(void)
{
	PointerMap::iterator it = _id2pointer.find(_id);
	assert(it != _id2pointer.end());
	
	_id2pointer.erase(it);

	GroupMap::iterator groupIter = _maps.find(_group);
	assert(groupIter != _maps.end());
	groupIter->second->erase(_name);
		
}

void IDManagerImpl::registerPointer(IDManager * manger)
{
	_id2pointer.insert(std::make_pair(_id, manger));
}

IDManager::id_type IDManagerImpl::registerID(const std::string & name , IDManager::group_type group)
{

	 
	GroupMap::iterator groupIter = _maps.find(group);
	if(groupIter == _maps.end())
	{
		std::pair<GroupMap::iterator, bool> ret = _maps.insert(std::make_pair(group, boost::shared_ptr<IDMap>(new IDMap())));
		assert(ret.second);
		groupIter = ret.first;
	}
	
	assert(groupIter != _maps.end());

	if(&name == &IDManager::BLANK)
	{
		_name = std::string("@") + boost::lexical_cast<std::string>(_count_id);
	}
	else
	{
		if(name.empty())
		{
			throw ORZ_EXCEPTION(Exception::ERR_INVALID_PARAMS)<<errno_info_text("you can't use a empty string for name");
		}
		else if(name[0] == '@')
		{
			
			throw ORZ_EXCEPTION(Exception::ERR_INVALID_PARAMS)<<errno_info_text("you can't use a \"@\" head string for name");
		}
		_name = name;	
	}


	IDMap::iterator it = groupIter->second->find(_name);

	if(it != groupIter->second->end())
	{
		throw ORZ_EXCEPTION(Exception::ERR_DUPLICATE_ITEM);//Exception()<<errno_info(1);
	}
	
	groupIter->second->insert(std::make_pair(_name, _count_id));
	++_count_id;
	return _count_id - 1;

}


///通过name和类型得到ID
IDManager::id_type IDManagerImpl::getTargetID(const std::string & name, IDManager::group_type group)
{

	GroupMap::iterator groupIter = _maps.find(group);
	if(groupIter == _maps.end())
	{	

		throw ORZ_EXCEPTION(Exception::ERR_ITEM_NOT_FOUND) <<errno_info_text("can't find group");
		//throw ( Orz::ExceptionFactory::create(Orz::ExceptionCodeType<Exception::ERR_ITEM_NOT_FOUND>() )<<errno_info_text("can't find type"));
	}


	IDManagerImpl::IDMap::iterator idIter = groupIter->second->find(name);

	if(idIter == groupIter->second->end())
	{
		throw ORZ_EXCEPTION(Exception::ERR_ITEM_NOT_FOUND)<<errno_info_text("can't find name");
	}

	return idIter->second;
}


///通过ID得到指针
IDManager * IDManagerImpl::getPointer(IDManager::id_type id)
{

	PointerMap::iterator it = _id2pointer.find(id);
	if(it == _id2pointer.end())
	{
		throw ORZ_EXCEPTION(Exception::ERR_ITEM_NOT_FOUND)<<errno_info_text("can't fint id");
	}
	
	return it->second;
}
