#ifndef __Orz_ToolkitScript_ScriptInterface_h__
#define __Orz_ToolkitScript_ScriptInterface_h__
#include "ToolkitScriptConfig.h"
#include <orz/Toolkit_Base/EventSystem.h>
#include <orz/Toolkit_Base/LogManager.h>
#include <orz/Framework_Base/Logic.h>
namespace Orz
{
class _OrzToolkitScriptExport ScriptInterface : public EventHandler
{
	typedef std::map<std::string, ActorPtr> ActorMap;
public:
	ScriptInterface(void);

	~ScriptInterface(void);
	void createActor(const std::string & type, const std::string& name);
	void enableActor(const std::string& name);
	void disableActor(const std::string& name);
	void destroyActor(const std::string& name);

	
	//void createScene(const std::string & type, const std::string& name);
	//void enableScene(const std::string& name);
	//void disableScene(const std::string& name);
	//void destroyScene(const std::string& name);
private:
	ActorMap _actors;
	//std::map<std::string, Scene *> _scenes;
};


typedef boost::shared_ptr<ScriptInterface> ScriptInterfacePtr;

}

#endif