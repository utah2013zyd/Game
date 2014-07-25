
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <orz/Toolkit_Base/TimerManager/AsioTimerManager.h>
#include <orz/Toolkit_Base/TimerManager/Timer.h>
#include "TimerManager/AsioTimer.h"

using namespace Orz;


bool next(void)
{
	return true;
}




	
AsioTimerManager::AsioTimerManager(void):_io(new boost::asio::io_service())
{
	_timer = createTimer(next);
	_timer->start(0);
}
AsioTimerManager::~AsioTimerManager(void)
{
	AsioTimer::Iterator it;
	for(it = AsioTimer::begin(); it !=AsioTimer::end(); ++it)
	{
		(*it)->close();
	}
	_timer->stop();
	_timer.reset();

}
	
bool AsioTimerManager::update(TimeType interval)
{

	_io->run_one();
	return true;
}

TimerPtr AsioTimerManager::createTimer(const TimerFunctorReturnBool & fun)
{
	boost::shared_ptr<AsioTimer>  ptr(new AsioTimer(fun, *_io));
	return ptr;
}