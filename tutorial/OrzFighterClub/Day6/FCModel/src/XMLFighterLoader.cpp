
#include "XMLFighterLoader.h"

#include <tinyxml/tinyxml.h>

using namespace Orz;


const std::string XMLFighterLoader::FIGHTER1("Fighter1");
const std::string XMLFighterLoader::FIGHTER2("Fighter2");
const std::string XMLFighterLoader::FIGHTER("Fighter");
const std::string XMLFighterLoader::NAME("name");
XMLFighterLoader::XMLFighterLoader()
{

}
XMLFighterLoader::~XMLFighterLoader(void)
{

}

bool XMLFighterLoader::load(const std::string & fileName)
{
	TiXmlDocument document(fileName.c_str());
	document.LoadFile();
	
		
	TiXmlElement* root = document.FirstChildElement(FIGHTER);
	if (root)
	{	

		for(TiXmlElement*  element = root->FirstChildElement();  element; element = element->NextSiblingElement(  ) ) 
		{ 
			const std::string  * name = element->Attribute(NAME);
			if(name)
			{
				if(element->ValueStr() == FIGHTER1)
					_fighter1 = *name;
				else if(element->ValueStr() == FIGHTER2)
					_fighter2 = *name;
				
			}
		}
	
	}
	if(_fighter1.empty() || _fighter2.empty())
		return false;
	return true;
}
const std::string & XMLFighterLoader::getFighter1(void) const
{
	return _fighter1;
}
const std::string & XMLFighterLoader::getFighter2(void) const
{
	return _fighter2;
}
	
		