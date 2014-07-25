#include <orz/Toolkit_Script/ScriptInterface.h>
using namespace Orz;

ScriptInterface::ScriptInterface(void)
{
	
}

ScriptInterface::~ScriptInterface(void)
{
	
}	 
void ScriptInterface::createActor(const std::string & type, const std::string& name)
{
	
	Orz::ActorPtr actor = Orz::GameFactories::getInstance().createActor(type, name);
	
	_actors.insert(std::make_pair(name,actor));
	if(Orz::ILogManager::getSingletonPtr())
	{
		Orz::ILogManager::getSingleton().utf8("ScriptInterface: We create Actor, type is "+type+", name is "+name+".");
	}
}
void ScriptInterface::enableActor(const std::string& name)
{


	std::map<std::string, ActorPtr>::iterator it = _actors.find(name);
	if(it != _actors.end())
	{
		getWorld()->comeIn(it->second);
		if(Orz::ILogManager::getSingletonPtr())
		{
			Orz::ILogManager::getSingleton().utf8("ScriptInterface: We enable Actor, name is "+name+".");
		}
	}
}
void ScriptInterface::disableActor(const std::string& name)
{
	
	ActorMap::iterator it = _actors.find(name);
	if(it != _actors.end())
	{
		getWorld()->goOut(it->second);
		if(Orz::ILogManager::getSingletonPtr())
		{
			Orz::ILogManager::getSingleton().utf8("ScriptInterface: We disable Actor, name is "+name+".");
		}
	}
}
void ScriptInterface::destroyActor(const std::string& name)
{
	

	ActorMap::iterator it = _actors.find(name);
	if(it != _actors.end())
	{
		if(it->second->isInWorld())
			getWorld()->goOut(it->second);
		_actors.erase(it);

		if(Orz::ILogManager::getSingletonPtr())
		{
			Orz::ILogManager::getSingleton().utf8("ScriptInterface: We destroy Actor, name is "+name+".");
		}
	}
	
}


//	void createActor(const std::string & type, const std::string& name);
//	void enableActor(const std::string& name);
//	void disableActor(const std::string& name);
//	void destroyActor(const std::string& name);
//
//	
//	//void createScene(const std::string & type, const std::string& name);
//	//void enableScene(const std::string& name);
//	//void disableScene(const std::string& name);
//	//void destroyScene(const std::string& name);
//private:
//	std::map<std::string, Actor *> _actors;

//void ScriptInterface::createScene(const std::string & type, const std::string& name);
//void ScriptInterface::enableScene(const std::string& name);
//void ScriptInterface::disableScene(const std::string& name);
//void ScriptInterface::destroyScene(const std::string& name);
//private:
//	std::map<std::string, Actor *> _actors;
//	std::map<std::string, Scene *> _scenes;
//};