#ifndef __Orz_FrameworkBase_LogicConfiger_LogicFacadeImpl_h__
#define __Orz_FrameworkBase_LogicConfiger_LogicFacadeImpl_h__

#include <orz/Framework_Base/FrameworkBaseConfig.h>
#include "LogicConfiger/Data.h"

#include <orz/Framework_Base/Logic/GameFactories.h>
#include <orz/Framework_Base/Logic/Theater.h>
#include <orz/Toolkit_Base/DynLibManager/PluginsManager.h>
#include <orz/Toolkit_Base/TimerManager.h>
#include <orz/Framework_Base/LogicConfiger/Builder.h>
#include <orz/Toolkit_Base/EventSystem/EventWorld.h>
namespace Orz
{
namespace LogicConfiger
{







class _OrzToolkitBasePrivate LogicFacadeImpl
{

public:
	LogicFacadeImpl(void)
	{
	}
	~LogicFacadeImpl(void)
	{
	
	}

	bool unload(void)
	{



		_timer->stop();

		_world->goOut(_theater);
		_world->shutdown();
		

		_timer.reset();
		_theater.reset();
		_world.reset();


		_data.unloadPlugins();

		return true;
	}
	bool load(void)
	{

		_data.loadPlugins();


		_world = EventWorldPtr(new EventWorld());
		_world->init();
		_timer = ITimerManager::getSingleton().createTimer(boost::bind(&LogicFacadeImpl::update, this));
		_timer->start(WORLD_UPDATE_INTERVAL);

		
		_theater = _data.createTheater();
		

		_world->comeIn(_theater);

		return true;
	}
	bool building(const Builder & builder)
	{
		return builder.write(_data);
	}

	inline bool update()
	{
		_world->update();
		return true;
	}

private:
	
	Data _data;
	TheaterPtr _theater;
	EventWorldPtr _world;
	TimerPtr _timer;
	/**/
};
}

}

#endif