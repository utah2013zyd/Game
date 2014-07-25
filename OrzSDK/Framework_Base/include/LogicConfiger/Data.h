
#ifndef __Orz_FrameworkBase_LogicConfiger_Data_h__
#define __Orz_FrameworkBase_LogicConfiger_Data_h__

#include <orz/Toolkit_Base/TimerManager.h>
#include <orz/Toolkit_Base/PluginsManager.h>
#include <orz/Framework_Base/Logic/GameFactories.h>
#include <orz/Framework_Base/Logic/Theater.h>
#include <orz/Framework_Base/Logic/Director.h>
#include "Logic/GameFactoriesImpl.h"
#include "Logic/TheaterImpl.h"
namespace Orz
{
	namespace LogicConfiger
	{
		class _OrzToolkitBasePrivate Data
		{
			/**/
		public:
			void addPlugin(const std::string & name)
			{
				_plugins.push_back(name);
			}
			void loadPlugins(void)
			{
				for(std::vector<std::string>::iterator it = _plugins.begin(); it != _plugins.end(); ++it)
				{
					PluginsManager::getSingleton().loadPlugin(*it);
				}
			}
			void unloadPlugins(void)
			{
				for(std::vector<std::string>::iterator it = _plugins.begin(); it != _plugins.end(); ++it)
				{
					PluginsManager::getSingleton().unloadPlugin(*it);
				}
			}

			TheaterPtr createTheater(void)
			{
				TheaterPtr theater = GameFactories::getInstance().createTheater(_theaterType, _theaterName);

				DirectorTypeNameList::iterator it;
				for(it = _directorTypeName.begin(); it != _directorTypeName.end(); ++it)
				{
					theater->addDirector(it->first, it->second, NULL);
				}



				if(!_activeDirector.empty())
					theater->activeDirector(_activeDirector);

				return theater;
			}
			void setTheater(const std::string & type, const std::string & name)
			{
				_theaterType = type;
				_theaterName = name;
			}
			void addDirector(const std::string & type, const std::string & name)
			{
				_directorTypeName.push_back(std::make_pair(type, name));
			}
			void setActiveDirector(const std::string & active)
			{
				_activeDirector = active;
			}


		private:
			typedef std::vector<std::pair<std::string, std::string> >  DirectorTypeNameList;
			std::vector<std::string> _plugins;
			std::string _activeDirector;
			std::string _theaterType;
			std::string _theaterName;
			DirectorTypeNameList _directorTypeName;

		};
	}
}

#endif