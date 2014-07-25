#include <orz/Toolkit_Base/TimerManager/ITimerManager.h>

using namespace Orz;


template<> ITimerManager* Singleton<ITimerManager>::_singleton = NULL;
