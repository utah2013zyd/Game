#ifndef __Orz_FrameworkBase_TheaterImpl_h__
#define __Orz_FrameworkBase_TheaterImpl_h__
#include <orz/Framework_Base/FrameworkBaseConfig.h>
#include <orz/Framework_Base/Logic/GameFactories.h>
#include "Logic/GameFactoriesImpl.h"
namespace Orz{

class _OrzToolkitBasePrivate TheaterImpl
{

	typedef std::pair<std::string, NameValueList*> TypeParameter; 
	typedef std::map<std::string, TypeParameter> NameTypeMap;
public:


	TheaterImpl(void){}
	~TheaterImpl(void){}

	void addDirector(const std::string & type, const std::string & name, NameValueList * parameter = NULL)
	{

		if(&name == &IDManager::BLANK)
		{
			throw  ORZ_EXCEPTION(Exception::ERR_INVALID_PARAMS);
		}
		NameTypeMap::iterator it = _map.find(name);
		if(it != _map.end())
			throw  ORZ_EXCEPTION(Exception::ERR_DUPLICATE_ITEM);
		_map.insert(std::make_pair(name, std::make_pair(type, parameter)));
	}
	DirectorPtr activeDirector(const std::string & name)
	{
		NameTypeMap::iterator it = _map.find(name);
		if(it == _map.end())
			throw  ORZ_EXCEPTION(Exception::ERR_ITEM_NOT_FOUND);
		return GameFactories::getInstance().createDirector(it->second.first, it->first, it->second.second);

	}
public:
	NameTypeMap _map;
};


}
#endif


