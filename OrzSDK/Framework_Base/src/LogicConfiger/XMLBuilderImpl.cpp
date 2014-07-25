#include "LogicConfiger/XMLBuilderImpl.h"

#include "Logic/TheaterImpl.h"
#include <tinyxml/tinyxml.h>
using namespace Orz;
using namespace LogicConfiger;
const std::string PLUGIN("plugin");
const std::string ORZ_CONFIG("OrzConfig");
const std::string NAME("name");
const std::string DIRECTOR("director");
const std::string THEATER("theater");
const std::string TYPE("type");
const std::string ACTIVE("active");

bool XMLBuilderImpl::write(Data & data) const
{
	
	TiXmlDocument document(_filename.c_str());
	document.LoadFile();
	TiXmlElement* root = document.FirstChildElement(ORZ_CONFIG);
	if (root)
	{	
		
		TiXmlElement* platform = root->FirstChildElement(ORZ_PLATFORM_STRING);
		if(platform)
		{
			for(TiXmlElement*  element = platform->FirstChildElement();  element; element = element->NextSiblingElement(  ) ) 
			{ 
				if(element->ValueStr() == PLUGIN)
				{
					if(!readPlugin(element, data))
						return false;
				}
				else if(element->ValueStr() == THEATER)
				{
					if(!readTheater(element, data))
						return false;
				}
			} 
		}
		return true;
	}
	return false;

}

XMLBuilderImpl::XMLBuilderImpl(const std::string & filename):_filename(filename)
{
	
}
XMLBuilderImpl::~XMLBuilderImpl()
{
	
}

	
bool XMLBuilderImpl::readPlugin(TiXmlElement*  element,Data & data) const
{
	const std::string  * name = element->Attribute(NAME);

	if(!name)
		return false;

	data.addPlugin(*name);
	
	return true;
}
bool XMLBuilderImpl::readTheater(TiXmlElement*  element, Data & data) const
{

	const std::string * ttype = element->Attribute(TYPE);
	const std::string * tname = element->Attribute(NAME);
	if(ttype && tname)
		data.setTheater(*ttype, *tname);

	for(TiXmlElement* node  = element->FirstChildElement();  node; node = node->NextSiblingElement() ) 
	{ 
		
		if(node->ValueStr() == DIRECTOR)
		{
			const std::string * dtype = node->Attribute(TYPE);
			const std::string * dname = node->Attribute(NAME);
			if(dtype && dname)
			{
				data.addDirector(*dtype, *dname);
			}
		
			
		}
	
	} 

	const std::string * active = element->Attribute(ACTIVE);
	if(active)
	{
		data.setActiveDirector(*active);
	}

	return true;
}

