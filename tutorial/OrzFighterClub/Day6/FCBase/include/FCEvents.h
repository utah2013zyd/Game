#ifndef __Orz_FCBase_FCEvents_h__
#define __Orz_FCBase_FCEvents_h__


#include "FCBaseConfig.h"
#include "FCGraph.h"
#include "FCMapInfo.h"
#include "FCPather.h"
#include <orz/Toolkit_Base/EventSystem.h>
namespace Orz
{

DEF_EVENT_BEGIN_WITH_EXPORT(_FCBaseExport, FCEvents)
DEF_EVENT(FIGHTING)
DEF_EVENT(SHOOTED)//��������
DEF_EVENT(DESTROYED)//���屻�ݻ�
DEF_EVENT_END(FCEvents)

}
#endif