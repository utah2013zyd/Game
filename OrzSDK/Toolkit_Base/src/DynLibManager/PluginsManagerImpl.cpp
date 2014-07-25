

#include "DynLibManager/PluginsManagerImpl.h"
#include <orz/Toolkit_Base/DynLibManager/DynLib.h>
#include <fstream>
#include <orz/Toolkit_Base/Exception.h>
#include "DynLibManager/DynLibImpl.h"

using namespace Orz;

typedef void (*DLL_START_PLUGIN)(void);
typedef void (*DLL_INIT_PLUGIN)(void);
typedef void (*DLL_STOP_PLUGIN)(void);


PluginsManagerImpl::PluginsManagerImpl(void)//:_dynLibManager(new DynLibManager())
{

}
PluginsManagerImpl::~PluginsManagerImpl(void)
{

}


bool PluginsManagerImpl::init(void)
{
	DynLibList::iterator i;
	for (i = _pluginLibs.begin(); i != _pluginLibs.end(); ++i)
	{
		// Call plugin initialise (optional)
		DLL_INIT_PLUGIN pFunc = (DLL_INIT_PLUGIN)(*i)->getSymbol("dllInitialisePlugin");
		if (pFunc)
		{
			pFunc();
		}

	}
	_isInitialised = true;
	return true;
}


void PluginsManagerImpl::shutdown(void)
{

	DynLibList::reverse_iterator i;

	// NB Shutdown plugins in reverse order to enforce dependencies
	for (i = _pluginLibs.rbegin(); i != _pluginLibs.rend(); ++i)
	{
		// Call plugin shutdown (optional)
		DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)(*i)->getSymbol("dllShutdownPlugin");
		if (pFunc)
		{
			pFunc();
		}

	}
	clear();
}

void PluginsManagerImpl::clear()
{

	DynLibList::reverse_iterator i;

    // NB Unload plugins in reverse order to enforce dependencies
    for (i = _pluginLibs.rbegin(); i != _pluginLibs.rend(); ++i)
    {
        // Call plugin shutdown
		DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)(*i)->getSymbol("dllStopPlugin");
		if(pFunc)
            pFunc();
        // Unload library & destroy
		(*i)->unload();
		//DynLibManager::getSingleton().unload(*i);

    }

    _pluginLibs.clear();
}


void PluginsManagerImpl::loadPluginFromFile(const std::string & fileName)
{
	std::ifstream file;
	file.open(fileName.c_str());
	if(!file.is_open())
	{
		throw ORZ_EXCEPTION(Exception::ERR_FILE_NOT_FOUND);
	}
	std::string pluginName;
	while(!file.eof())
	{
		file >>pluginName;
		loadPlugin(pluginName);
	}
	file.close();
}
void PluginsManagerImpl::loadPlugin(const std::string & pluginName)
{

	// Load plugin library
	DynLibPtr lib(new DynLib(pluginName));
	lib->load();
	// Store for later unload
	_pluginLibs.push_back(lib);

	// Call startup function
	DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)lib->getSymbol("dllStartPlugin");

	if (!pFunc)
	{
		//todo
		return; 

	}
	pFunc();

	if (_isInitialised)
	{
		// initialise too
		DLL_INIT_PLUGIN pFunc = (DLL_INIT_PLUGIN)lib->getSymbol("dllInitialisePlugin");
		if (pFunc)
		{
			pFunc();
		}
	}

}


void PluginsManagerImpl::unloadPlugin(const std::string & pluginName)
{
	DynLibList::iterator i;

    for (i = _pluginLibs.begin(); i != _pluginLibs.end(); ++i)
    {
		if ((*i)->getName() == pluginName)
		{
			// Call plugin shutdown
			DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)(*i)->getSymbol("dllStopPlugin");

			pFunc();
			// Unload library (destroyed by DynLibManager)
			(*i)->unload();
			//DynLibManager::getSingleton().unload(*i);
			_pluginLibs.erase(i);
			return;
		}

    }
}
