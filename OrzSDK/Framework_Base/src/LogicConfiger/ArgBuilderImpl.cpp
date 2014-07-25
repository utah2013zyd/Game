#include "LogicConfiger/ArgBuilderImpl.h"
#include <orz/Toolkit_Base/DynLibManager/PluginsManager.h>
#include <orz/Toolkit_Base/TimerManager.h>
#include <orz/Toolkit_Base/LogManager.h>
#include "Logic/TheaterImpl.h"

#include "LogicConfiger/LogicFacadeImpl.h"
using namespace Orz;

using namespace LogicConfiger;

const std::string PLUGIN("-p");
const std::string DIRECTOR("-d");
const std::string THEATER("-t");
const std::string ACTIVE("-a");



ArgBuilderImpl::ArgBuilderImpl(int argc, char **argv):
_argc(argc),
_argv(argv)
{
	
}

bool ArgBuilderImpl::write(Data & data) const
{
	for(int i =1; i< _argc; ++i)
	{
		std::string str(_argv[i]);

		
		if(str == THEATER)
		{
			std::string ttype( _argv[++i]);
			std::string tname( _argv[++i]);
			ILogManager::getSingleton().utf8(ttype);
			ILogManager::getSingleton().utf8(tname);
			data.setTheater(ttype, tname);
		}
		else if(str == PLUGIN)
		{
			data.addPlugin(std::string(_argv[++i]));
		}
		else if(str == DIRECTOR)
		{

			std::string dtype(_argv[++i]);
			std::string dname(_argv[++i]);
			data.addDirector(dtype, dname);
		}
		else if(str == ACTIVE)
		{
			data.setActiveDirector(std::string(_argv[++i]));
		}

	}
	return true;

}
ArgBuilderImpl::~ArgBuilderImpl()
{
	
}

