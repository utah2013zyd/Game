#ifndef __Orz_FCBase_FCEvents_h__
#define __Orz_FCBase_FCEvents_h__


#include "FCBaseConfig.h"

#include <orz/Toolkit_Base/EventSystem.h>
namespace Orz
{

DEF_EVENT_BEGIN_WITH_EXPORT(_FCBaseExport, FCEvents)

DEF_EVENT(FIRE)
DEF_EVENT(DAMAGE)
DEF_EVENT(CRUSH)

DEF_EVENT(WIN)
DEF_EVENT(LOSE)

DEF_EVENT_END(FCEvents)

}
#endif