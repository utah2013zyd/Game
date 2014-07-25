#include <orz/Toolkit_Base/TimerManager/SimpleTimerManager.h>
#include <orz/Toolkit_Base/TimerManager/Timer.h>
#include "TimerManager/SimpleTimer.h"


namespace Orz
{
	class TimerDeleter
	{
	public:
		TimerDeleter(SimpleTimerManager::TimerList & deleteList):_deleteList(deleteList)
		{
		}


		virtual ~TimerDeleter()
		{
		}

		void operator()(SimpleTimer * timer)
		{
			if(SimpleTimerManager::getSingletonPtr())
			{
				_deleteList.push_back(timer);
			}
			else
			{
				delete timer;
			}
		}
	private:
		SimpleTimerManager::TimerList & _deleteList;
	};
}
 
using namespace Orz;



void SimpleTimerManager::addList(void)
{
	TimerList::iterator it;
	for(it = _addList.begin(); it != _addList.end(); ++it)
	{
		_timerList.push_back(*it);
	}
	_addList.clear();
}


void SimpleTimerManager::deleteList(void)
{
	
	TimerList::iterator it;
	for(it = _deleteList.begin(); it != _deleteList.end(); ++it)
	{
		_timerList.erase(std::remove(_timerList.begin(), _timerList.end(), *it), _timerList.end());
		delete(*it);
	}
	_deleteList.clear();
}

SimpleTimerManager::~SimpleTimerManager(void)
{
	deleteList();
}
SimpleTimerManager::SimpleTimerManager(void)
{
}


TimerPtr SimpleTimerManager::createTimer(const TimerFunctorReturnBool & fun)
{
	
	boost::shared_ptr<SimpleTimer>  ptr(new SimpleTimer(fun), TimerDeleter(_deleteList));
	_addList.push_back(ptr.get());
	return ptr;
}

bool SimpleTimerManager::update(TimeType interval)
{
	
	addList();
	deleteList();

	TimerList::iterator it;
	for(it = _timerList.begin(); it != _timerList.end(); ++it)
	{
		if((*it)->isEnable())
		{
			bool b = (*it)->update(interval);
			(*it)->setEnable(b);
		}
	}
	return true;
}

